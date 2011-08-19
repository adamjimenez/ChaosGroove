// Game (.Cpp)
// -----------

// Chaos Groove V0.9!
// ------------------

// You are free to use and modify this source code however you see fit, as long as this game or any modified
// version is not sold in any way. This game must always be freely available!
// Richard Phipps, 13/04/08


// This is the main file which contains the overall inner game loops.

#include <ptk.h>
#include <vector>
#include <list>
#include <sstream>
#include "KPTK.h"
#include "time.h"

using namespace std;

#include "bridge.hpp"
#include "game.hpp"
#include "log.hpp"
#include "timer.hpp"
#include "scene.hpp"
#include "config.hpp"
#include "board.hpp"
#include "piece.hpp"

#include "net.hpp"
#include "chat.hpp"
#include "wizard.hpp"
#include "panel.hpp"
#include "spell.hpp"
#include "ai.hpp"
#include "effect.hpp"
#include "options.hpp"
#include "sound.hpp"

struct game_t game;
struct screen_t screen;

int WINAPI WinMain(IN HINSTANCE hInstance, 
									 IN HINSTANCE hPrevInstance, 
									 IN LPSTR lpCmdLine, 
									 IN int nShowCmd )
{

	// Do first time setup stuff, quitting on error.
	if (!first_time_setup()) return 0;

	game.exit = false;

  start:

	if (game.exit) goto end_game;

	// We always start with the title screen.
  game.stage = GAME_MENUS;//GAME_INGAME;

	// Ingame Loop
	// -----------

	if (game.stage == GAME_MENUS)
	{
   menu_loop();
   if (game.exit) goto end_game;
	}

	game.stage = GAME_INGAME;

	if (game.stage == GAME_INGAME)
	{
	 ingame_loop();
	}

	if (!game.exit) goto start;

  end_game:

	log("Exiting Game..");
  do_exit_cleanup();

	return 0;
}

bool setup_display(void)
{
 char file[MAX_STRING], text[MAX_STRING];

 log("");
 log("Loading in display data from display.ini file");

 sprintf(text, "\\text_files\\display.ini");
 sprintf(file, "%s", KMiscTools::makeFilePath(text));

 if (set_config_file_new(CONFIG_DISPLAY, file, false))
 {
  screen.w = GetConfigInt(CONFIG_DISPLAY, "DISPLAY", "WIDTH", 1280);
  screen.h = GetConfigInt(CONFIG_DISPLAY, "DISPLAY", "HEIGHT", 1024);

  screen.dx = GetConfigYes(CONFIG_DISPLAY, "DISPLAY", "DX", false);
  screen.windowed = GetConfigYes(CONFIG_DISPLAY, "DISPLAY", "WINDOWED", false);
 }
 else
 {
  log("Couldn't find or open display.ini file. Using default settings instead..");
  screen.w = 1280;
	screen.h = 1024;
	screen.dx = false;
	screen.windowed = false;
 }

 log("");
 log("Display information found :");
 log("Width : %d, Height : %d, DirectX : %d, Windowed : %d", screen.w, screen.h, screen.dx, screen.windowed);
 log("");
 
 if (GetConfigYes(CONFIG_DISPLAY, "DISPLAY", "USE_DESKTOP_RESOLUTION", false))
 {
	log("* USE_DESKTOP_RESOLUTION = YES : Using desktop resolution of %d x %d, instead of user settings..", screen.desktop_w, screen.desktop_h);
	screen.w = screen.desktop_w;
	screen.h = screen.desktop_h;
 }

 log_no_cr("Attempting to open a screen with these settings..");
 return CreateGameScreen(screen.w, screen.h, screen.windowed, screen.dx);
}

bool first_time_setup(void)
{
 // Setup logfile.
 open_log( KMiscTools::makeFilePath("logfile.txt") );
 log_header();

 // Setup screen.

 get_desktop_resolution(&screen.desktop_w, &screen.desktop_h);
 log("Desktop resolution : %d x %d", screen.desktop_w, screen.desktop_h);

 log_no_cr("Opening Screen...");
 if (!setup_display())
 {
  log("  Failed!");
	MessageBox("Screen Error", "Can't open up a screenmode!");

	// Fatal error.
	close_log();
	return false;
 }
 log("  Successful.");
 log("");

 KInput::hidePointer( ) ;

 // Setup timer.
 start_timer();
 log("Started timer.");
  
 // Seed random number generator from time.
 log("Random Seed: %d", SeedFromTime());
 //Seed(1185702357);
 //Seed(5632);

 // Load options.
 load_options();

 // Load all texture images.
 if (!load_all_images()) return false;

 // Now setup general variables..
 game.release_version = true;
 game.AI_debug = false;
 game.debug_to_chat = false;//true;
 game.show_fps = false;

 // Call setup functions.
 setup_fonts();

 // Game setup:
 load_all_pieces();
 setup_board();
 AI_setup();
 load_spells();

 // Sound setup
 setup_sound_system();

 // Centre mouse.
 mouse.x = 640;
 mouse.y = 480;

 // Net setup.
 log("Setting up network code..");
 
 setup_users();
 setup_chat_lines();

 log("Setup completed!");
 return true;
}

bool setup_fonts(void)
{
 int c;

 log_no_cr("Loading gfx fonts.. ");

 // Blank Font tables..
 for (c = 0 ; c < 255 ; c++)
 {
  largefontTablePtr[c]._c = 0;
	smallfontTablePtr[c]._c = 0;
 }

 // Large
 add_font_letters(largefontTablePtr, 0, '-', 0, 0, 16, 16, 16, 16, 1);
 add_font_letters(largefontTablePtr, 1, '!', 16, 0, 16, 16, 16, 16, 1);
 add_font_letters(largefontTablePtr, 2, '0', 32, 0, 16, 16, 16, 16, 10);
 add_font_letters(largefontTablePtr, 12, ':', 64, 16, 16, 16, 16, 16, 1);
 add_font_letters(largefontTablePtr, 13, ';', 80, 16, 16, 16, 16, 16, 1);
 add_font_letters(largefontTablePtr, 14, '?', 96, 16, 16, 16, 16, 16, 1);
 add_font_letters(largefontTablePtr, 15, 39, 112, 16, 16, 16, 16, 16, 1);
 add_font_letters(largefontTablePtr, 16, 'A', 0, 32, 16, 16, 16, 16, 26);
 add_font_letters(largefontTablePtr, 42, '(', 32, 80, 16, 16, 16, 16, 1);
 add_font_letters(largefontTablePtr, 43, ')', 48, 80, 16, 16, 16, 16, 1);
 add_font_letters(largefontTablePtr, 44, ',', 64, 80, 16, 16, 16, 16, 1);
 add_font_letters(largefontTablePtr, 45, '.', 80, 80, 16, 16, 16, 16, 1);
 add_font_letters(largefontTablePtr, 46, '/', 96, 80, 16, 16, 16, 16, 1);
 add_font_letters(largefontTablePtr, 47, '%', 112, 80, 16, 16, 16, 16, 1);
 add_font_letters(largefontTablePtr, 48, ' ', 32, 96, 16, 16, 16, 16, 1);
 add_font_letters(largefontTablePtr, 49, 'a', 0, 32, 16, 16, 16, 16, 26);
 add_font_letters(largefontTablePtr, 75, '+', 0, 96, 16, 16, 16, 16, 1);

 fonty[FONT_LARGE] = new KText( KMiscTools::makeFilePath( "Gfx\\Fonts\\large.png"), largefontTablePtr);
 
 if (!fonty[FONT_LARGE])
 {
  log("Couldn't open large font gfx!");
	return false;
 }
 fonty[FONT_LARGE]->getKGraphicPtr()->setTextureQuality(true);
 fonty[FONT_LARGE]->getKGraphicPtr()->allowTextureWrap(true);

 // Small
 add_font_letters(smallfontTablePtr, 0, 'A', 0, 0, 13, 13, 14, 18, 26);
 add_font_letters(smallfontTablePtr, 26, 'a', 0, 0, 13, 13, 14, 18, 26);
 add_font_letters(smallfontTablePtr, 52, ' ', 104, 26, 13, 12, 14, 18, 1);
 add_font_letters(smallfontTablePtr, 53, '-', 0, 39, 13, 13, 14, 18, 1);
 add_font_letters(smallfontTablePtr, 54, '.', 26, 52, 13, 13, 14, 18, 1);
 add_font_letters(smallfontTablePtr, 55, ',', 39, 52, 13, 13, 14, 18, 1);
 add_font_letters(smallfontTablePtr, 56, '0', 13, 39, 13, 13, 14, 18, 10);
 add_font_letters(smallfontTablePtr, 66, '%', 52, 52, 13, 13, 14, 18, 1);
 add_font_letters(smallfontTablePtr, 67, ':', 65, 52, 13, 13, 14, 18, 1);

 fonty[FONT_SMALL] = new KText( KMiscTools::makeFilePath( "Gfx\\Fonts\\small.png"), smallfontTablePtr);

 if (!fonty[FONT_SMALL])
 {
  log("Couldn't open small font gfx!");
	return false;
 }
 fonty[FONT_SMALL]->getKGraphicPtr()->setTextureQuality(true);
 fonty[FONT_SMALL]->getKGraphicPtr()->allowTextureWrap(false);

 log("Done");

 return true;
}

void add_font_letters(struct KFont *table, int place, int c, int x, int y, int w, int h, int w2, int h2, int num)
{
 int n;

 for (n = 0 ; n < num ; n++)
 {
  table[place]._c = c;
	table[place]._x1 = x;
	table[place]._y1 = y;
	table[place]._w = w2;
	table[place]._h = h2;
	table[place]._x2 = x + w - 2;
	table[place]._y2 = y + h - 2;
  x += w;
	
	if (128 - x < w)
	{
   x = 0;
	 y += h;
	}

	place++;
	c++;
 }
}

bool load_all_images(void)
{
 // Load Sprite images
 log_no_cr("Loading Sprites : ");
 gfx = LoadListOfBitmaps( KMiscTools::makeFilePath("Gfx\\Sprites\\"), "png", 3);
   
 if ( gfx.empty() )
 {
  log("Could not load sprite bitmaps!");
  //KMiscTools::messageBox("Error!", "Could not load sprite bitmaps!" );
  //return false;
 }
 log("Done.");

 log("Done.");

 return true;
}

void do_exit_cleanup(void)
{
 int n, num;

 num = gfx.size();
 log("Destroying %d gfx..", num);

 for (n = 0 ; n < num ; n++)
 {
  gfx[n]->freePicture(); 
 }

 log("Freeing config files..");
 save_options();
 free_all_config_data();

 //net_shutdown();
}

int loop(void)
{
 // This is a very important function, it is called to do our inner loop logic.
 // This function sees if we need to do a logic update. If not then it returns.
 
 // If we do, then it does the logic for how many frames needed, updates the gfx
 // and then returns the number of frames processed.
 
 int logic, frames, throwaway;
 
 frames = 0;
 mouse.old_left_click = mouse.left_click;

 //if (game.exit_key) return 1;

 // Use timer to work out how many 1/240 frames we need to do logic for.
 // NOTE: logic increases upto 240 in one second and then timer is reset (for better
 // accuracy). logic is decreased below..
 
 logic = check_timer(240);

 Rest(5);
 /*
 // If the game is switched out then rest to save CPU and batteries.
 if (game.switched_out)
 {
  rest(1);
 }
 else
 {
  if (option[OPTION_CPU_USAGE].choice == 1) rest(0); // share with OS
  if (option[OPTION_CPU_USAGE].choice == 2) rest(1); // share with OS and shutdown to save batteries.
 }
 */
 
 // Frames to do - frames done (both in last second).
 logic = logic - timer.logic_frames;
   
 // We have a frame to do:  
 if (logic > 0)
 {   
  if (logic > 60) logic = 8; // 240 / 60 = 4fps, so if we go below this then slow down.     
  frames = logic; // How many frames to do.
  
	if (game.exit_key) frames = 1;

  do
  {
   check_for_exit_key();
   do_scene_logic();

   logic--;
  } while (logic > 0);
  
  // Done logic, now draw:
  //if (!game.switched_out || option[OPTION_SCREEN].choice == 0)
  {
   draw_scene();
   refresh_screen();
   
  }

  timer.logic_frames += frames; // Keep track of how many logic frames done so far.
 }
 
 return frames;
}

void wait_time(int frames)
{
 // This lets us pause the game flow, while still doing all logic and animations.
 // We can use this once we have set some effects and particles going until they have done.
 
 int fr;
 //frames = 1;
 //frames = frames / 4;
 fr = 0;

 do
 {
  fr = fr + loop();
  
  // Immediate exit on these keys.
  if (game.exit_key) return;
      
 } while (fr < frames);
}


void ingame_loop(void)
{
 int s, wiz, casts, x, y;
 bool done, worked, real = false;
 char name[MAX_STRING];
 log("Starting Ingame Loop");

 reset_timer();

 // Unlike in Chaos Funk, this is split into the 3 phases and the structure is much simpler.
 // By using the loop function we can streamline the design and make things much easier.
 
 // Yes, we use a few goto labels, instead of do( ) while loops.
 
 // Remove particles from title screen first!
 //delete_all_particles();
 
 mouse.alpha.current = 0.0;
 mouse.alpha.target = 1.0; // Fade in mouse.
 mouse.alpha.speed = 0.025;

 // Find current spell list to load from
 find_option_choice_variables(CONFIG_OPTIONS, "SPELL", "SPELL LIST", NULL, NULL, NULL, name);

 // Is this different from the spell list currently loaded?
 if (strcmp(total_spells.loaded_spell_list_name, name) != 0)
 {
  // Ok, we need to clear the old spell data, and load from the new list.
  load_spells();
 }

 // Blank board.
 setup_board();
 setup_panels();
 
 // Setup Wizards (using wizard option data).
 setup_wizards();
 setup_wizard_positions();
 
 // Setup Spell Lists.
 setup_spell_lists();

 // Clear bonus spell stat for default spells.
 for (wiz = 0 ; wiz < MAX_WIZARDS ; wiz++)
 {
  clear_bonus_spell_stat_in_spell_list(wiz);
 }

 // Reset game variables.
 game.winner = -1;
 game.turn = 0;
 game.balance = 0;
 
 bar_effect.alpha.current = 0.0;
 bar_effect.alpha.target = 0.0;
 bar_effect.alpha.speed = 0.025;
      
 start:
 
 board_info.selected_layer = PIECE;
 game.turn++; // Increase turn number.
 increase_all_pieces_turn_number();

  
 // Reset status flags for each piece on the board.
 restore_all_pieces_status_flags();

 // Analyse new situation..
 AI_analyse_board();

 // SPELL SELECT PHASE
 // ------------------
 
 game.phase = PHASE_SPELLSELECT;
 
 // Remove any cursor highlight.
 board_info.highlight_alpha.target = 0.0;
 
 // Remove old panel info.
 sprintf(panel.info_area_line[0], "");
 sprintf(panel.info_area_line[1], "");

 // Go through all wizards in turn.
 for (wiz = 0 ; wiz < MAX_WIZARDS ; wiz++)
 {
  clear_highlight_board();
  game.end_turn = false; // Reset end turn flag. Set to true to go to next wizard.
  
  if (!wizard[wiz].alive) continue; // Skip dead or unused wizards.

	game.current_wizard = wiz;
	board_info.selected_x = wizard[game.current_wizard].x;
  board_info.selected_y = wizard[game.current_wizard].y;

	// New turn, do we get a new spell?
  find_option_choice_variables(CONFIG_OPTIONS, "SPELL", "NEW SPELL EVERY X TURNS", &x, NULL, NULL, NULL);
 
  // Unless it is NEVER, see if we get a new spell this turn..
  if (x != 0 && game.turn > 1)
  {
   if ((game.turn - 1) % x == 0) add_spells(1, game.current_wizard); // Give new spell to our wizard.
  }

	// Is wizard on ground, or mounted?
	board_info.selected_layer = PIECE;
	if (board[board_info.selected_x][board_info.selected_y][MOUNTED].gfx != -1) board_info.selected_layer = MOUNTED; 

  board_info.selected_alpha.target = 1.0;
  panel.spell_area_control_alpha.target = 0.0;

	// Hide spells at first..
  for (s = 0 ; s < MAX_WIZ_SPELLS ; s++)
  {
   spell_list[s][game.current_wizard].alpha.current = 0.0; 
   spell_list[s][game.current_wizard].alpha.target = 0.0;
  }

  if (!wizard[game.current_wizard].human)
  {
   AI_select_spell(); // Computer player, so select spell.
	 request_sound_effect("CREATURE", "CREATURE", "END TURN", false);
   continue;
  }

  // Mark which spells can be cast.
	check_if_can_cast_all_spells(game.current_wizard);
  hide_highlight_board(true);

	// Start bar effect for new player.
  bar_effect.mode = BAR_MODE_NEWTURN;
  bar_effect.alpha.target = 1.0;
  sprintf(bar_effect.text, "%s'S TURN", wizard[game.current_wizard].name);
 
	sprintf(panel.info_area_line[0], "");
	sprintf(panel.info_area_line[1], "");

  // Wait until we click a mouse key.
  done = false;
	do
  {
   loop();
   if (mouse.left_click && !mouse.old_left_click) done = true;
   if (game.exit_key) goto end;

  } while (!done);
 
  bar_effect.alpha.target = 0.0;
  done = false;
  if (game.exit_key) goto end;

	panel.spell_area_control_alpha.target = 1.0;

  // Fade in spells..
  for (s = 0 ; s < MAX_WIZ_SPELLS ; s++)
  {
   spell_list[s][game.current_wizard].alpha.current = 0.0 - (0.1 * ((panel.spell_icon_amount * s) / 500));    
   spell_list[s][game.current_wizard].alpha.target = 1.0;
  }

	// Now we will let the player pick a spell..
  do
  {
   loop();

	 // Have we clicked on a spell icon? Is it a unused spell? Have we space to cast this spell?
   if (mouse.left_click && !mouse.old_left_click && mouse.over_spell_icon != -1 && 
   spell_list[mouse.over_spell_icon][game.current_wizard].spell != -1)
   {
    select_spell(mouse.over_spell_icon);
    game.end_turn = true;
   }

	 // Have we clicked on No Spell instead?
	 if (mouse.left_click && !mouse.old_left_click && mouse.over_control_icon == CONTROL_NO_SPELL)
	 {
    select_spell(NO_SPELL);
    game.end_turn = true;
	 }

	 if (game.exit_key) goto end;
  
  } while (!game.end_turn);
 
	// Fade out spell icons.
  for (s = 0 ; s < MAX_WIZ_SPELLS ; s++)
  {
	 spell_list[s][game.current_wizard].alpha.current = 1.0 + (0.1 * ((panel.spell_icon_amount * s) / 500));     
	 spell_list[s][game.current_wizard].alpha.target = 0.0;
  }
	wait_time(200);
	request_sound_effect("CREATURE", "CREATURE", "END TURN", false);
 }

 if (game.exit_key) goto end;


 // SPELL CAST PHASE
 // ------------------
 
 game.phase = PHASE_SPELLCAST;
 
 // Go through all wizards in turn.
 for (wiz = 0 ; wiz < MAX_WIZARDS ; wiz++)
 {
  // Remove any bonus spell stats..
	clear_bonus_spell_stat_in_spell_list(wiz);

  clear_highlight_board();
  game.end_turn = false; // Reset end turn flag. Set to true to go to next wizard.
  
  if (!wizard[wiz].alive) continue; // Skip dead or unused wizards.
  
	if (wizard[wiz].spell == -1) continue; // Wizard has no spell to cast.

	game.casting_a_spell = false;
  game.done_spell_chance = false;

	game.current_wizard = wiz;
	casts = spell[wizard[game.current_wizard].spell].casts;

	board_info.selected_x = wizard[game.current_wizard].x;
  board_info.selected_y = wizard[game.current_wizard].y;
	
	// Is wizard on ground, or mounted?
	board_info.selected_layer = PIECE;
	if (board[board_info.selected_x][board_info.selected_y][MOUNTED].gfx != -1) board_info.selected_layer = MOUNTED; 

  board_info.selected_alpha.target = 1.0;
  
	sprintf(panel.info_area_line[0], "%s CASTS %s", wizard[game.current_wizard].name, 
	spell[wizard[game.current_wizard].spell].name);
  sprintf(panel.info_area_line[1], "");
  
  AI_cast:

	if (!wizard[game.current_wizard].human)
  {
	 panel.spell_area_control_alpha.target = 0.0;

	 // Target is our wizard if our spell has no range.
	 ai[game.current_wizard].target_x = wizard[game.current_wizard].x;
   ai[game.current_wizard].target_y = wizard[game.current_wizard].y;

	 // If spell needs a target, then find one..
   if (spell[wizard[game.current_wizard].spell].range != 0) AI_select_spell_square(); // Computer player, so select square.

	 // Fade out old highlight.
   board_info.highlight_alpha.target = 0.0;
   
   if (game.exit_key) goto end; // Abort game if exit keys are pressed.
   wait_time(50);
      
	 x = ai[game.current_wizard].target_x;
	 y = ai[game.current_wizard].target_y;
	 if (spell[wizard[game.current_wizard].spell].autocast) autocast_spell(&x, &y);

	 if (x != -1 && y != -1)
	 {
		worked = try_to_cast_spell(x, y);    
    wait_time(150);
		if (!worked)
		{
     spell_fails();
		 continue;
		}

	  casts--;
		if (casts > 0) goto AI_cast;
		spell_succeeds();
	 } 
	 else
	 {
	  sprintf(panel.info_area_line[0], "%s IS CANCELLING %s", wizard[game.current_wizard].name, 
		spell[wizard[game.current_wizard].spell].name);
    sprintf(panel.info_area_line[1], "");
	 }
   continue;
  }

	panel.spell_area_control_alpha.target = 1.0;

  do
  {
   // Do main logic and drawing code.
   loop();

   if (game.exit_key) goto end; // Abort game if exit keys are pressed.
   
   // Check for icons.
   if (mouse.left_click && !mouse.old_left_click && mouse.over_control_icon == CONTROL_SKIP_SPELL)
   {
    // Skip Spell icon
    game.end_turn = true;
    wait_time(200);
		request_sound_effect("CREATURE", "CREATURE", "END TURN", false);
   }   
  
	 // Deal with spells that are cast automatically (i.e. Law 1, Magic Shield, etc..) These spells have a range of 0.
	 if (spell[wizard[game.current_wizard].spell].range == 0 || spell[wizard[game.current_wizard].spell].autocast)
	 {
    // Remove spell highlights.   
		hide_highlight_board(false);
    board_info.highlight_alpha.target = 0.0;

    cast:

		game.casting_a_spell = true;

		x = wizard[game.current_wizard].x;
		y = wizard[game.current_wizard].y;
		if (spell[wizard[game.current_wizard].spell].autocast) autocast_spell(&x, &y);

		if (x != -1 && y != -1)
		{
		 worked = try_to_cast_spell(x, y);  
		 
		 casts--;
	
     if (casts < 1 || !worked)
		 {
		  if (!worked)
			{
       spell_fails();
			}
			else
			{
       spell_succeeds();
			}

			game.end_turn = true;
		 }
		 else
		 {
			wait_time(150);
			game.casting_a_spell = false;

			goto cast;
		 }
		}
		else
		{
    game.end_turn = true;
		request_sound_effect("CREATURE", "CREATURE", "END TURN", false);
    wait_time(100);
		}
	 }

   if (mouse.over_board_x != -1)
   {
    // Can we cast spell here?
    if (can_we_cast_spell_here(mouse.over_board_x, mouse.over_board_y))
    {
     // We can cast spell here, so set highlight.
     board_info.highlight_type = HIGHLIGHT_CAN_CAST;
     board_info.highlight_alpha.target = 1.0;
     board_info.highlight_x = mouse.over_board_x;
     board_info.highlight_y = mouse.over_board_y;
     
     // If we have clicked mouse button then cast spell.
     if (mouse.left_click && !mouse.old_left_click)
     {
      // Remove spell highlights.   
			hide_highlight_board(false);
      board_info.highlight_alpha.target = 0.0;
			game.casting_a_spell = true;
			
			worked = try_to_cast_spell(mouse.over_board_x, mouse.over_board_y);  

			casts--;
	
      if (casts < 1 || !worked)
			{
			 if (!worked)
			 {
        spell_fails();
			 }
			 else
			 {
        spell_succeeds();
			 }

			 game.end_turn = true;
			 request_sound_effect("CREATURE", "CREATURE", "END TURN", false);
			}
			else
			{
			 wait_time(150);
       game.casting_a_spell = false;
			}
		 }
    }
    else
    {
     // Reset highlight.
     board_info.highlight_alpha.target = 0.0;  
    }
   }
   else
   {
    // Reset highlight
    board_info.highlight_alpha.target = 0.0;      
   }
  
  } while (!game.end_turn);

 }
 
 // Do we have a winner?
 check_for_winner();
 if (game.winner != -1) goto end;

 // Deal with growing and disappearing before movement phase..
 do_growing_and_disappearing();

 // Do we have a winner?
 check_for_winner();
 if (game.winner != -1) goto end;

 // Check for engaged if necessary..
 do_engaged_rule();

 // MOVEMENT PHASE
 // --------------
 
 game.phase = PHASE_MOVEMENT;
 sprintf(panel.info_area_line[0], "");
 sprintf(panel.info_area_line[1], "");

 // Go through all wizards in turn (+1 to include Independents)
 for (wiz = 0 ; wiz < MAX_WIZARDS + 1 ; wiz++)
 {
  game.end_turn = false;
  if (!wizard[wiz].alive) continue; // Skip dead or unused wizards.
  s = count_wizard_creatures_on_board(wiz);
  if (s < 1) continue; // Skip wizards turn with no pieces left (i.e. independents)

	game.current_wizard = wiz;
  unselect_piece(); // just to clear any old selected values.
  board_info.selected_alpha.target = 0.0;

  // Check for exit keys.
  if (game.exit_key) goto end; // Abort game if exit keys are pressed.
  
  if (!wizard[game.current_wizard].human)
  {
   panel.spell_area_control_alpha.target = 0.0;
   AI_do_movement();
   check_for_winner();
   if (game.winner != -1) goto end;
   continue;
  }
  
	panel.spell_area_control_alpha.target = 1.0;

  do
  {
   // Do main logic and drawing code.
   loop();
  
   if (game.exit_key) goto end; // Abort game if exit keys are pressed.
   
   // Check for icons.
   // ----------------
   
   // END TURN ICON
   if (mouse.left_click && !mouse.old_left_click && mouse.over_control_icon == CONTROL_END_TURN)
   {
    game.end_turn = true;
		request_sound_effect("CREATURE", "CREATURE", "END TURN", false);
		clear_highlight_board();
    wait_time(100);
   }
   
   // END MOVE ICON
   if ((mouse.left_click && !mouse.old_left_click && mouse.over_control_icon == CONTROL_END_MOVE))
   {
    board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].has_moved = true;
    board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].has_attacked = true;
    
		request_sound_effect("CREATURE", "CREATURE", "CANCEL", true);

		clear_highlight_board();

		if (board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].has_shot)
		{
		 end_move_piece();
		}
		else
		{
     set_highlight_board(board_info.selected_x, board_info.selected_y, 
		 board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].ranged_combat_range, PIECE, 
		 false, false, true, true, true, "");

		 request_sound_effect("CREATURE", "CREATURE", "SHOOTING PHASE", true);
		}

		wait_time(50);
   }
   
   // DISMOUNT ICON
   if (mouse.left_click && !mouse.old_left_click && mouse.over_control_icon == CONTROL_DISMOUNT)
   {
    board_info.selected_layer = MOUNTED; // Switch to rider.

		if (board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].flying)
	  {
     // Show flying creatures possible moves..
	   set_highlight_board(board_info.selected_x, board_info.selected_y, board[board_info.selected_x][board_info.selected_y]
		 [board_info.selected_layer].movement, PIECE, board[board_info.selected_x][board_info.selected_y]
		 [board_info.selected_layer].can_ride, true, true, false, true, "");
	  }
	  else
	  {
	   // Harder.. Show land based creatures possible moves.
     highlight_land_movement_options(board_info.selected_x, board_info.selected_y);
	  }

    wait_time(5);  
   }

	 // END FIRE ICON
   if ((mouse.left_click && !mouse.old_left_click && mouse.over_control_icon == CONTROL_END_FIRE))
   {
    end_move_piece();
		request_sound_effect("CREATURE", "CREATURE", "CANCEL", true);

		clear_highlight_board();
    wait_time(50);
   }
   
   if (mouse.over_board_x != -1)
   {
    // Clicked in playing area?
    
    if (!board_info.selected_state)
    {
     // We have not yet selected a piece, so try to select. This fails if it's not
     // one of our wizards pieces and has movement points left.
     try_to_select_piece(mouse.over_board_x, mouse.over_board_y, mouse.left_click);
    }
    else
    {
     // Jump to try to move piece function. This does all neccessary checks and can
     // Then decide on moving, attacking, or trying to mount.
     try_to_move_piece(mouse.over_board_x, mouse.over_board_y, mouse.left_click);

     // Do we have a winner?
     check_for_winner();
     if (game.winner != -1) goto end;
     
    }
   }
   if (game.exit_key) goto end; // Abort game if exit keys are pressed.
  } while (!game.end_turn);
 } 
 
 if (game.winner != -1) goto end;
 
 goto start; // Labels make extra braces for do ( ) while unneccesary.


 end:
 mouse.alpha.target = 0.0;

 if (game.winner != -1) do_victory_loop();
}

void menu_loop(void)
{
 int a, b, w;
 bool done = false;

 //setup_options(CONFIG_WIZARDS, "WIZARDS");
 setup_options(CONFIG_MENUS, "TITLE");
 
 reset_timer();
 
 load_music();
 mouse.alpha.current = 0.0;
 mouse.alpha.target = 1.0; // Fade in mouse.
 mouse.alpha.speed = 0.025;
 
 play_music();

 do
 {
  // Do main logic and drawing code.
  loop();

	//if (game.exit_key) done = true;

	if (strlen(menu_info.action) > 0)
	{
	 if (strcmp(menu_info.action, "CHANGE MENU") == 0)
	 {
	  fade_out_options();
		wait_time(50);
		setup_options(menu_info.new_config, menu_info.new_section_name);
	  
		if (menu_info.new_config == CONFIG_WIZARDS && check_option_choice(CONFIG_WIZARDS, "WIZARDS", 
		"RANDOMISE EACH PLAY", "YES"))
		{
     for (w = 0 ; w < MAX_WIZARDS ; w++)
		 {
      randomise_wizard_name(w);
		 }		 
	  }
	 }
	 if (strcmp(menu_info.action, "RANDOM NAMES") == 0)
	 {
    for (w = menu_info.var1 ; w <= menu_info.var2 ; w++)
		{
     randomise_wizard_name(w);
		}
	 }

   if (strcmp(menu_info.action, "START GAME") == 0) done = true;
   if (strcmp(menu_info.action, "EXIT GAME") == 0)
	 {
	  game.exit = true;
	  done = true;
	 }
	 sprintf(menu_info.action, "");
	}

 } while (!done);

 music.volume.target = 0.0;
 wait_time(240);
 music.mod->pauseModule();
}

void check_for_exit_key(void)
{
 game.old_exit_key = game.exit_key;
 game.exit_key = false;

 if (CheckQuit() || Key(K_VK_ESCAPE))
 {
	game.exit_key = true;
 }

 //log("exit key: %d, old exit key: %d", game.exit_key, game.old_exit_key);
}

void do_growing_and_disappearing(void)
{
 int x, y, r, sp, effect, var1;
 char name[MAX_STRING];

 for (y = 0 ; y < MAX_BOARD_HEIGHT ; y++)
 {
  for (x = 0 ; x < MAX_BOARD_WIDTH ; x++)
	{
	 if (board[x][y][PIECE].gfx == -1) continue;
   
	 // Should piece grow?
	 if (board[x][y][PIECE].grow_chance > 0 && !board[x][y][PIECE].just_grown)
	 {
	  r = Rand() % 100;
    if (r < board[x][y][PIECE].grow_chance) grow_piece(x, y);
	 }

	 // Should piece disappear?
   r = Rand() % 100;
	 if (r < board[x][y][PIECE].chance_of_disappearing && 
	 (!board[x][y][PIECE].only_disappear_when_ridden || 
	 (board[x][y][PIECE].only_disappear_when_ridden && board[x][y][MOUNTED].gfx != -1)))
	 {
	  // Yes! So kill piece and do custom special effect.
	  effect = board[x][y][PIECE].disappear_effect;
    var1 = board[x][y][PIECE].disappear_effect_var1;
    sp = board[x][y][PIECE].spells_when_disappeared;

		sprintf(name, "%s", board[x][y][PIECE].name);
    kill_piece(x, y, -1, false);

		if (effect != EFFECT_NONE)
		{
		 do_effect(effect, x, y, var1, var1);
		 request_sound_effect(name, "CREATURE", "DISAPPEARS", true);
		 //wait_time(100);
		}

		if (sp > 0)
		{
		 sp = add_spells(sp, board[x][y][PIECE].owner); // Give some new spells to this wizard!

		 if (sp > 0)
		 {
	    if (sp == 1) sprintf(panel.info_area_line[0], "NEW SPELL FOR %s", wizard[game.current_wizard].name);
      if (sp > 1) sprintf(panel.info_area_line[0], "NEW SPELLS FOR %s", wizard[game.current_wizard].name);
		  sprintf(panel.info_area_line[1], "");
			request_sound_effect("WIZARD", "WIZARD", "GETS BONUS SPELLS", true);
		 }
		}
		else
		{
     if (wizard_here(x, y) != -1) request_sound_effect(name, "CREATURE", "MOUNT IS KILLED WHILE RIDDEN", true);
		}

	 }

	}
 }
}

void do_victory_loop(void)
{
 bool done;
 int frame;

 // Start bar effect for new player.
 bar_effect.mode = BAR_MODE_WINNER;
 bar_effect.alpha.target = 1.0;
 sprintf(bar_effect.text, "%s", wizard[game.current_wizard].name);
 
 sprintf(panel.info_area_line[0], "");
 sprintf(panel.info_area_line[1], "");

 // Wait until we click a mouse key.
 done = false;
 frame = 0;
 request_sound_effect("VICTORY", "VICTORY", "WINNER", false);

 wait_time(25);

 do
 {
  loop();
  if (mouse.left_click && !mouse.old_left_click) done = true;
  if (game.exit_key) done = true;

	frame++;
	if (frame >= 24)
	{
	 do_effect(EFFECT_FIREWORK, IRand(board_info.start_x, (board_info.board_width * board_info.square_width) + board_info.start_x), 
	 IRand(board_info.start_y, (board_info.board_height * board_info.square_height) + board_info.start_y), 64, 0);
	 frame = 0;
	}

 } while (!done);
 
 bar_effect.alpha.target = 0.0;
}