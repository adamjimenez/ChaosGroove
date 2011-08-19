// Panel (.Cpp)
// ------------

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
//#include "RakNetworkFactory.h"
//#include "RakPeerInterface.h"
//#include "MessageIdentifiers.h"
//#include "net.hpp"
#include "chat.hpp"
#include "wizard.hpp"
#include "panel.hpp"
#include "spell.hpp"
#include "options.hpp"

struct panel_t panel;

void setup_panels(void)
{
 // Details for ingame UI panels..

 panel.info_area_x = board_info.start_x - board_info.border_size;
 panel.info_area_y = board_info.start_y + (board_info.board_height * board_info.square_height) + board_info.border_size;
 panel.info_area_w = 1120 - panel.info_area_x;
 panel.info_area_h = 96;//960 - panel.info_area_y;

 panel.spell_area_x = 1120;
 panel.spell_area_y = 0;
 panel.spell_area_w = 1280 - panel.spell_area_x;
 panel.spell_area_h = 960 - panel.spell_area_y;
 panel.spell_icon_w = 64;
 panel.spell_icon_h = 64;
 panel.spell_icon_space_x = 8;
 panel.spell_icon_space_y = 8;
 panel.spell_icon_amount = 22;

 find_option_choice_variables(CONFIG_OPTIONS, "SPELL", "MAXIMUM AMOUNT OF SPELLS VISIBLE", &panel.spell_icon_amount, &panel. spell_icon_w, &panel.spell_icon_space_x, NULL);

 panel.spell_icon_h = panel.spell_icon_w;

 panel.spell_area_control_alpha.current = 0.0;
 panel.spell_area_control_alpha.target = 0.0;
 panel.spell_area_control_alpha.speed = 0.01;

 panel.info_area_alpha.current = 0.0;
 panel.info_area_alpha.target = 0.0;
 panel.info_area_alpha.speed = 0.01;
}

void do_panels_logic(void)
{
 int s;

 panel.info_area_alpha.target = 0.0;
 board_info.grid_alpha.target = 0.0;

 // SPELLS
 // ------

 // Is the mouse over a human player's spell icon instead? And we aren't holding the mouse over the spell icon?
 if (mouse.over_spell_icon != -1 && wizard[game.current_wizard].human)
 {
	s = spell_list[mouse.over_spell_icon][game.current_wizard].spell;

  // Have we already used this spell?
	if (s == -1)
	{
   // We have already used the spell at this position, so blank any old spell highlights.
	 clear_highlight_board();
	}
	else
	{
	 // Deal with highlight on board where we can cast..
	 if (mouse.old_over_spell_icon != mouse.over_spell_icon)
	 {
	  // Highlight on board where we can cast this spell.
	  highlight_where_spell_can_be_cast(s, true);
	 }

	 // Deal with getting piece stats to show if over spell which makes new piece..
	 if (spell[s].piece_ID != -1)
	 {
	  panel.stats_piece = piece[spell[s].piece_ID];
	 }
 	 
	 // Make spell details (or piece stats for spell) visible.
	 panel.last_spell_icon_over = s;
	 panel.info_area_alpha.target = 2.0;
	 panel.info_area_show_piece_stats = spell[s].show_piece_stats;
	}

 }

 mouse.old_over_spell_icon = mouse.over_spell_icon;


 // BOARD
 // -----

 // Is the mouse over a square on the board?
 if (mouse.over_board_x != -1)
 {
  // Is there a non BLANK piece here?
  if (board[mouse.over_board_x][mouse.over_board_y][PIECE].gfx != BLANK)
	{
	 board_info.current_x = mouse.over_board_x;
	 board_info.current_y = mouse.over_board_y;

   panel.info_area_alpha.target = 2.0;
	 panel.stats_layer = PIECE;

	 if (board_info.selected_state) panel.stats_layer = board_info.selected_layer;

	 panel.stats_piece = board[mouse.over_board_x][mouse.over_board_y][panel.stats_layer];

	 panel.last_spell_icon_over = -1; // Not over a spell now..
	 panel.info_area_show_piece_stats = true;

	 // Are we looking at an enemy wizards pieces and we are not selected? If so highlight..
	 if (panel.stats_piece.owner != game.current_wizard && !board_info.selected_state &&
	 (mouse.over_board_x != mouse.old_over_board_x || mouse.over_board_y != mouse.old_over_board_y))
	 {
	  highlight_wizard_pieces(panel.stats_piece.owner);
	 }

	 // Looking at our pieces in the movement phase, but not yet selected?
	 if (panel.stats_piece.owner == game.current_wizard && 
	 (mouse.over_board_x != mouse.old_over_board_x || mouse.over_board_y != mouse.old_over_board_y))
	 {
	  if ((!panel.stats_piece.has_moved || !panel.stats_piece.has_attacked) && !board_info.selected_state && 
		game.phase == PHASE_MOVEMENT)
	  {
	   if (panel.stats_piece.flying)
		 {
			// Show flying creatures possible moves..
	    if (board_info.selected_layer == PIECE && board[mouse.over_board_x][mouse.over_board_y][PIECE].engaged)
	    {
	     // Engaged, so show enemies directly around us.
			 //add_chat_line("Engaged.", Rgba(1.0, 1.0, 1.0));
	     set_highlight_board(mouse.over_board_x, mouse.over_board_y, 1.0, PIECE, false, false, true, false, true, "");
	    }
	    else
	    {
	     // Not engaged, so show full range.
			 //add_chat_line("Not Engaged.", Rgba(1.0, 1.0, 1.0));
	     set_highlight_board(mouse.over_board_x, mouse.over_board_y, panel.stats_piece.movement, PIECE, 
	     panel.stats_piece.can_ride, true, true, false, true, "");
	    }
	   }
	   else
	   {
		  // Harder.. Show land based creatures possible moves.
      highlight_land_movement_options(mouse.over_board_x, mouse.over_board_y);
	   }
		}
		if (!board_info.selected_state)
		{
		 if (panel.stats_piece.has_moved && panel.stats_piece.has_attacked && panel.stats_piece.has_shot) clear_highlight_board();
		
		 // Also, highlight our creatures!
		 highlight_wizard_pieces(game.current_wizard);
		}
	 }
	}
	else
	{
	 // Maybe a body here?
	 if (board[mouse.over_board_x][mouse.over_board_y][BODY].gfx != BLANK)
	 {
	  board_info.current_x = mouse.over_board_x;
	  board_info.current_y = mouse.over_board_y;

    panel.info_area_alpha.target = 2.0;
	  panel.stats_piece = board[mouse.over_board_x][mouse.over_board_y][BODY];
		panel.stats_layer = BODY;
	  panel.last_spell_icon_over = -1; // Not over a spell now..
		panel.info_area_show_piece_stats = true;

		if (game.phase == PHASE_MOVEMENT && !board_info.selected_state) clear_highlight_board();
	 }
	 else
	 {
    // Over a BLANK piece, but we have a creature already selected?
    if (board_info.selected_state)
	  {
     panel.info_area_alpha.target = 2.0;
	   panel.stats_piece = board[board_info.selected_x][board_info.selected_y][board_info.selected_layer];
		 panel.stats_layer = board_info.selected_layer;
	   panel.last_spell_icon_over = -1; // Not over a spell now..
		 panel.info_area_show_piece_stats = true;
	  }
		else
		{
		 // Over a BLANK piece and we don't have a creature already selected..
		 clear_highlight_board();
		 if (mouse.right_click) highlight_wizard_pieces(-1); // Show owners by colour..
		}
	 }
	}
 }

 mouse.old_over_board_x = mouse.over_board_x;
 mouse.old_over_board_y = mouse.over_board_y;

 // SPELLCASTING PHASE
 // ------------------

 // If we are in SPELLCASTING phase (and not in the process of casting a spell), then overlay 
 // spell target positions (in case we are looking at enemy pieces)
 if (game.phase == PHASE_SPELLCAST && !game.casting_a_spell)
 {
	s = wizard[game.current_wizard].spell;
  highlight_where_spell_can_be_cast(s, false);
 }

 do_alpha_logic(&panel.info_area_alpha);
 do_alpha_logic(&panel.spell_area_control_alpha);
 do_alpha_logic(&board_info.grid_alpha);

 board_info.cloud_x += 0.25;
 if (board_info.cloud_x > MAX((board_info.square_width * board_info.board_width), 
 (board_info.square_height * board_info.board_height))) board_info.cloud_x = 0.0;

 // Now do spell icon alpha logic..
 for (s = 0 ; s < MAX_WIZ_SPELLS ; s++)
 {
  do_alpha_logic(&spell_list[s][game.current_wizard].alpha);    
 }
}

void draw_panels(void)
{
 char text[512];

 // Set glow colour (from wizard).
 gfx[8]->setBlitColor(wizard[game.current_wizard].col.r, wizard[game.current_wizard].col.g, 
 wizard[game.current_wizard].col.b, 1.0);

 // Blank any old cloud data in panel.
 gfx[0]->drawRect(0, panel.info_area_y, 1280, 960, 0.0, 0.0, 0.0, 1.0);

 // Now draw 'reflection' glow effect on info panel.
 gfx[8]->stretchAlphaRect(1, 1, gfx[8]->getWidth() - 1, gfx[8]->getHeight() - 1,
 panel.info_area_x, panel.info_area_y, panel.info_area_x + panel.info_area_w, panel.info_area_y + panel.info_area_h); 

 // Now draw 'reflection' glow effect on spell panel.
 gfx[8]->stretchAlphaRect(1, 1, gfx[8]->getWidth() - 1, gfx[8]->getHeight() - 1,
 board_info.start_x + (board_info.board_width * board_info.square_width) + board_info.border_size, panel.spell_area_y, 
 1280, panel.spell_area_y + panel.spell_area_h); 

 if (panel.info_area_show_piece_stats)
 {
  //panel.stats_piece.owner = game.current_wizard;

  // Draw stats for this spells piece in info panel.
  draw_creature_stats(panel.stats_piece);
 }
 else
 {
  // Show spell details in info panel instead.
  draw_spell_info();
 }

 if (mouse.over_spell_icon != -1 && spell[spell_list[mouse.over_spell_icon][game.current_wizard].spell].can_be_illusion &&
 spell_list[mouse.over_spell_icon][game.current_wizard].cast && 
 check_option_choice(CONFIG_OPTIONS, "GAME", "ILLUSIONS", "YES"))
 {
  if (mouse.spell_is_illusion)
	{
   draw_text("CAST AS ILLUSION", panel.info_area_x, panel.info_area_x + panel.info_area_w, 
	 panel.info_area_y + 6, FONT_LARGE, 0, Rgba(1.0, 1.0, 1.0), 1.0, TEXT_CENTRE);
	}
	else
	{
   draw_text("CAST AS REAL", panel.info_area_x, panel.info_area_x + panel.info_area_w, 
	 panel.info_area_y + 6, FONT_LARGE, 0, Rgba(1.0, 1.0, 1.0), 1.0, TEXT_CENTRE);
	}
 }

 // Can't cast this spell!
 if (mouse.over_spell_icon != -1 && !spell_list[mouse.over_spell_icon][game.current_wizard].cast)
 {
  draw_text("NO PLACE TO CAST SPELL!", panel.info_area_x, panel.info_area_x + panel.info_area_w, 
	panel.info_area_y + 6, FONT_LARGE, 0, Rgba(1.0, 0.0, 0.0), 1.0, TEXT_CENTRE);
 }

 // Also try to draw alternate text when over a blank board sqaure.
 draw_alternate_info_panel();

 // Draw list of spells.
 if (game.phase == PHASE_SPELLSELECT)
 {
  draw_spell_icons();
 }

 // Draw control icons at top.
 draw_control_icons();

 // Draw FPS if needed.
 if (game.show_fps)
 {
  sprintf(text, "FPS: %d", timer.gfx_fps);
  draw_text(text, 1130, 0, 900, FONT_LARGE, 0, Rgba(1.0, 1.0, 0.5), 1.0, TEXT_LEFT);
 }
}

void draw_alternate_info_panel(void)
{
 int x, y, c;
 float alpha;
 char text[MAX_STRING];

 alpha = 1.0 - panel.info_area_alpha.current;
 if (game.AI_debug || game.debug_to_chat) alpha = 1.0;

 if (alpha <= 0.0) return;
 if (alpha > 1.0) alpha = 1.0;

 x = panel.info_area_x;
 y = panel.info_area_y + 6;

 if (game.phase != PHASE_MOVEMENT && !game.AI_debug && !game.debug_to_chat)
 {
  sprintf(text, "%s", panel.info_area_line[0]);
  capitalize_text(text);
  strip_underscores_from_text(text);

  draw_text(text, x, 0, y, FONT_LARGE, 0, Rgba(1.0, 1.0, 0.5), alpha, TEXT_LEFT);

  sprintf(text, "%s", panel.info_area_line[1]);
  capitalize_text(text);
  strip_underscores_from_text(text);

  draw_text(text, x, panel.info_area_x + panel.info_area_w + 2, y, FONT_LARGE, 0, panel.info_area_line_rgba[1], alpha, TEXT_RIGHT);
 
  return;
 }
 
 y -= 7;

 for (c = 0 ; c < MAX_CHAT_LINES ; c++)
 {
  draw_text(chat_line[c].text, x, 0, y, FONT_SMALL, 0, chat_line[c].rgba, alpha, TEXT_LEFT);
	y += 12;
 }
}

void draw_creature_stats(piece_data creature)
{
 // We pass a copy of the creature stats and use this for the stat data.
 // This is so we can use the same routine with a piece already on the board
 // Or a spell version. By passing a copy we can look at stats that have been
 // modified from the original spell version, i.e. magic sword, or new creature
 // affecting spells.
 char text[MAX_STRING], text2[MAX_STRING];
 int x, y, a, sp, chance, movement, add;
 float alpha;

 if (game.AI_debug || game.debug_to_chat) return;

 alpha = panel.info_area_alpha.current;

 if (alpha <= 0.0) return;
 if (alpha > 1.0) alpha = 1.0;

 x = panel.info_area_x;
 y = panel.info_area_y + 6;

 // Find piece name.
 sprintf(text, "%s", creature.name);
 
 // Is this piece a wizard who is not on a mount?
 if (board_info.current_x != -1 && mouse.over_board_x != -1 && wizard_here(board_info.current_x, board_info.current_y) != -1 && board[board_info.current_x][board_info.current_y][MOUNTED].gfx == -1)
 {
  // Yes, so get real name from wizard list.
	sprintf(text, "%s", wizard[board[board_info.current_x][board_info.current_y][PIECE].owner].name);
 }
 capitalize_text(text);
 strip_underscores_from_text(text);

 // Top Line
 // --------

 // Draw piece name.
 a = draw_text(text, x, 0, y, FONT_LARGE, 0, Rgba(1.0, 1.0, 0.5), alpha, TEXT_LEFT);
 
 // Are we looking at a dead body?
 if (creature.dead)
 {
  draw_text("(DEAD)", x + a + 16, 0, y, FONT_LARGE, 0, Rgba(1.0, 0.0, 0.0), alpha, TEXT_LEFT);
 }

 // Is this creature being ridden?
 if (board_info.current_x != -1 && mouse.over_board_x != -1 && panel.stats_layer == PIECE && 
 board[board_info.current_x][board_info.current_y][MOUNTED].gfx != -1) 
 {
  if (wizard_here(board_info.current_x, board_info.current_y) != -1)
	{
	 sprintf(text, "(%s)", wizard[board[board_info.current_x][board_info.current_y][MOUNTED].owner].name);
	}
	else
	{
	 sprintf(text, "(%s)", board[board_info.current_x][board_info.current_y][MOUNTED].name);
	}
	capitalize_text(text);
  strip_underscores_from_text(text);

	draw_text(text, x + a + 16, 0, y, FONT_LARGE, 0, 
	wizard[board[board_info.current_x][board_info.current_y][MOUNTED].owner].col, alpha, TEXT_LEFT);
 }
 
 if (board_info.current_x != -1 && mouse.over_board_x != -1 && panel.stats_layer == PIECE && 
 board[board_info.current_x][board_info.current_y][PIECE_COVERED].gfx != -1) 
 {
  sprintf(text, "(COVERING %s)", board[board_info.current_x][board_info.current_y][PIECE_COVERED].name);
  capitalize_text(text);
  strip_underscores_from_text(text);

	draw_text(text, x + a + 16, 0, y, FONT_LARGE, 0, 
	wizard[board[board_info.current_x][board_info.current_y][PIECE_COVERED].owner].col, alpha, TEXT_LEFT);
 }

 // Piece's 'owner'
 if (game.phase == PHASE_SPELLSELECT && panel.last_spell_icon_over != -1)
 {    
  if (wizard[game.current_wizard].ability > 0)
	{
   sprintf(text, "+%d%% SPELL ABILITY", wizard[game.current_wizard].ability);
   draw_text(text, x, panel.info_area_x + panel.info_area_w, y, 
   FONT_LARGE, 0, Rgba(0.0, 1.0, 0.0), alpha, TEXT_RIGHT);
	}
 }
 else
 {
  draw_text(wizard[creature.owner].name, x, panel.info_area_x + panel.info_area_w, y, 
  FONT_LARGE, 0, wizard[creature.owner].col, alpha, TEXT_RIGHT);
 }

 // Is this creature currently selected?
 if (creature.selected && game.phase == PHASE_MOVEMENT
 && !board[board_info.current_x][board_info.current_y][board_info.selected_layer].has_moved) 
 {
	if (board_info.selected_engaged)
	{
   draw_text("ENGAGED TO ENEMY!", x, x + panel.info_area_w, y, FONT_LARGE, 0, Rgba(1.0, 0.0, 0.0), alpha, TEXT_CENTRE);
	}
	else
	{
	 draw_text("SELECTED", x, x + panel.info_area_w, y, FONT_LARGE, 0, Rgba(1.0, 1.0, 0.0), alpha, TEXT_CENTRE);
	}
 }


 // Details below Top Line
 // ----------------------

 sprintf(text, "");
 if (creature.flying) strcat(text, "Flying, ");
 if (creature.rideable) strcat(text, "Rideable, ");
 if (creature.undead) strcat(text, "Undead, ");
 if (creature.shadow_form) strcat(text, "Shadow Form, ");
 //if (creature.illusion) strcat(text, "Illusion, ");

 if (strlen(text) > 0) text[strlen(text) - 2] = '\0';

 y = y + 39;

 draw_text(text, x, panel.info_area_x + panel.info_area_w, y, 
 FONT_SMALL, 1, Rgba(1.0, 0.5, 0.0), alpha, TEXT_LEFT);

 sprintf(text, "World : - Neutral -");
 if (game.balance < -3) sprintf(text, "World : - Chaos %d -",(-game.balance / 4));
 if (game.balance > 3) sprintf(text, "World : - Law %d -",(game.balance / 4));

 draw_text(text, x, panel.info_area_x + panel.info_area_w, y, 
 FONT_SMALL, 1, Rgba(0.0, 1.0, 0.0), alpha, TEXT_CENTRE);

 // Draw spell specific details..
 if (game.phase == PHASE_SPELLSELECT && panel.last_spell_icon_over != -1)
 {
  sprintf(text, "- Neutral -"); 
  
  sp = panel.last_spell_icon_over;
  if (spell[sp].balance < 0) sprintf(text, "- Chaos %d -", -spell[sp].balance);
  if (spell[sp].balance > 0) sprintf(text, "- Law %d -", spell[sp].balance);
  
	draw_text(text, x, panel.info_area_x + panel.info_area_w, y, 
  FONT_SMALL, 1, Rgba(1.0, 0.5, 0.0), alpha, TEXT_RIGHT); 
 }

 // Draw BOTTOM 2 line Stats..
 // --------------------------

 y = y + 18;
 draw_text("Combat", x, 0, y, FONT_SMALL, 1, Rgba(0.5, 0.5, 0.5), alpha, TEXT_LEFT);
 
 if (creature.combat > 0)
 {
  sprintf(text, "%d", creature.combat); 
  draw_text(text, x + 132, 0, y, FONT_SMALL, 1, Rgba(0.0, 1.0, 0.0), alpha, TEXT_LEFT);
 }
 else
 {
  draw_text("-", x + 132, 0, y, FONT_SMALL, 1, Rgba(1.0, 0.0, 0.0), alpha, TEXT_LEFT);
 }

 draw_text("Defence", x, 0, y + 18, FONT_SMALL, 1, Rgba(0.5, 0.5, 0.5), alpha, TEXT_LEFT);
 
 a = MID(0, creature.defence + (creature.shadow_form * 3), 9);
  
 // Deal with Undeads.
 if (creature.undead)
 {
  find_option_choice_variables(CONFIG_OPTIONS, "GAME", "NORMAL CREATURE ATTACKS UNDEADS", &add, NULL, NULL, NULL);
  if (add != -1) a= MID(0, a + add, 9);
 }

 if (a > 0)
 {
  sprintf(text, "%d", a);
  draw_text(text, x + 132, 0, y + 18, FONT_SMALL, 1, Rgba(0.0, 1.0, 0.0), alpha, TEXT_LEFT);
 }
 else
 {
  draw_text("-", x + 132, 0, y + 18, FONT_SMALL, 1, Rgba(1.0, 0.0, 0.0), alpha, TEXT_LEFT);
 }

 x = x + 285;
 draw_text("Ranged Combat", x, 0, y, FONT_SMALL, 1, Rgba(0.5, 0.5, 0.5), alpha, TEXT_LEFT);

 if (creature.ranged_combat_attack > 0)
 {
	sprintf(text, "%d", creature.ranged_combat_attack);
	draw_text(text, x + 228, 0, y, FONT_SMALL, 1, Rgba(0.0, 1.0, 0.0), alpha, TEXT_LEFT);
 }
 else
 {
	draw_text("-", x + 228, 0, y, FONT_SMALL, 1, Rgba(1.0, 0.0, 0.0), alpha, TEXT_LEFT);
 }

 
 draw_text("Range", x, 0, y + 18, FONT_SMALL, 1, Rgba(0.5, 0.5, 0.5), alpha, TEXT_LEFT);

 if (creature.ranged_combat_range > 0)
 {
	sprintf(text, "%d", creature.ranged_combat_range);
	draw_text(text, x + 228, 0, y + 18, FONT_SMALL, 1, Rgba(0.0, 1.0, 0.0), alpha, TEXT_LEFT);
 }
 else
 {
	draw_text("-", x + 228, 0, y + 18, FONT_SMALL, 1, Rgba(1.0, 0.0, 0.0), alpha, TEXT_LEFT);
 }

 x = x + 343;
 draw_text("Movement", x, 0, y, FONT_SMALL, 1, Rgba(0.5, 0.5, 0.5), alpha, TEXT_LEFT);

 a = creature.movement;
 if (creature.shadow_form && a < 3) a = 3;

 if (a > 0)
 {
	sprintf(text, "%d", a);
	draw_text(text, x + 164, 0, y, FONT_SMALL, 1, Rgba(0.0, 1.0, 0.0), alpha, TEXT_LEFT);
 }
 else
 {
	draw_text("-", x + 164, 0, y, FONT_SMALL, 1, Rgba(1.0, 0.0, 0.0), alpha, TEXT_LEFT);
 }
 
 draw_text("Manoeuvre", x, 0, y + 18, FONT_SMALL, 1, Rgba(0.5, 0.5, 0.5), alpha, TEXT_LEFT);

 if (creature.flex > 0)
 {
	sprintf(text, "%d", creature.flex);
	draw_text(text, x + 164, 0, y + 18, FONT_SMALL, 1, Rgba(0.0, 1.0, 0.0), alpha, TEXT_LEFT);
 }
 else
 {
	draw_text("-", x + 164, 0, y + 18, FONT_SMALL, 1, Rgba(1.0, 0.0, 0.0), alpha, TEXT_LEFT);
 }

 x = x + 245;

 draw_text("Resistance", panel.info_area_x + 912, 0, y, FONT_SMALL, 1, Rgba(0.5, 0.5, 0.5), alpha, TEXT_LEFT);

 if (creature.magic_resist > 0)
 {
	sprintf(text, "%d", creature.magic_resist);
	draw_text(text, 0, panel.info_area_x + panel.info_area_w, y, FONT_SMALL, 1, Rgba(0.0, 1.0, 0.0), alpha, TEXT_RIGHT);
 }
 else
 {
	draw_text("-", 0, panel.info_area_x + panel.info_area_w, y, FONT_SMALL, 1, Rgba(1.0, 0.0, 0.0), alpha, TEXT_RIGHT);
 }

 draw_text("Chance", panel.info_area_x + 912, 0, y + 18, FONT_SMALL, 1, Rgba(0.5, 0.5, 0.5), alpha, TEXT_LEFT);
 
 chance = creature.chance;
 
 if (game.phase == PHASE_SPELLSELECT && panel.last_spell_icon_over != -1)
 {    
  chance = return_spell_chance(panel.last_spell_icon_over);
 }
 
 sprintf(text, "%d%%", chance);
 draw_text(text, x, panel.info_area_x + panel.info_area_w, y + 18, FONT_SMALL, 1, 
 return_spell_chance_col(chance), alpha, TEXT_RIGHT);
}

void draw_spell_info(void)
{
 char text[MAX_STRING];
 int x, y, a, sp, chance, movement;
 float alpha;

 if (game.AI_debug || game.debug_to_chat) return;

 alpha = panel.info_area_alpha.current;

 if (alpha <= 0.0) return;
 if (alpha > 1.0) alpha = 1.0;

 x = panel.info_area_x;
 y = panel.info_area_y + 6;

 // Find spell name.
 sprintf(text, "%s", spell[panel.last_spell_icon_over].name);
 capitalize_text(text);
 strip_underscores_from_text(text);

 // Top Line
 // --------

 // Draw spell name.
 a = draw_text(text, x, 0, y, FONT_LARGE, 0, Rgba(1.0, 1.0, 0.5), alpha, TEXT_LEFT);

 // Current wizards name.
 if (wizard[game.current_wizard].ability > 0)
 {
  sprintf(text, "+%d%% SPELL ABILITY", wizard[game.current_wizard].ability);
  draw_text(text, x, panel.info_area_x + panel.info_area_w, y, 
  FONT_LARGE, 0, Rgba(0.0, 1.0, 0.0), alpha, TEXT_RIGHT);
 }


 // Middle Line
 // -----------

 y += 39;

 sprintf(text, "- Neutral -"); 
  
 sp = panel.last_spell_icon_over;
 if (spell[sp].balance < 0) sprintf(text, "- Chaos %d -", -spell[sp].balance);
 if (spell[sp].balance > 0) sprintf(text, "- Law %d -", spell[sp].balance);
  
 draw_text(text, x, panel.info_area_x + panel.info_area_w, y, 
 FONT_SMALL, 1, Rgba(1.0, 0.5, 0.0), alpha, TEXT_RIGHT); 
 

 // Bottom Line
 // -----------

 y += 16;
 
 draw_text("Chance", x + (952), 0, y + 20, FONT_SMALL, 1, Rgba(0.5, 0.5, 0.5), alpha, TEXT_LEFT);
 
 chance = return_spell_chance(panel.last_spell_icon_over);
 
 sprintf(text, "%d%%", chance);
 draw_text(text, x, panel.info_area_x + panel.info_area_w, y + 20, FONT_SMALL, 1, 
 return_spell_chance_col(chance), alpha, TEXT_RIGHT);

 
 draw_text("Range", x + (952), 0, y + 2, FONT_SMALL, 1, Rgba(0.5, 0.5, 0.5), alpha, TEXT_LEFT);
 
 sprintf(text, "%d", spell[sp].range);
 if (spell[sp].range < 1) sprintf(text, "-");

 draw_text(text, x, panel.info_area_x + panel.info_area_w, y + 2, FONT_SMALL, 1, 
 Rgba(0.0, 1.0, 0.0), alpha, TEXT_RIGHT);

 y -= 16;

 // Description:
 draw_text(spell[sp].desc, x, panel.info_area_x + panel.info_area_w - 196, y, 
 FONT_SMALL, 1, Rgba(1.0, 1.0, 1.0), alpha, TEXT_WRAP);
}

void draw_spell_icons(void)
{
 // Draw spell icons either side of map view. Skipping any -1 spells which have been used.
 int t, g, x, y, y2, x2, sp, s, offset_x;
 bool spell_icon;
 float alpha;
 
 x = panel.spell_area_x;
 for (t = 0 ; t < 16 ; t++)
 {
  x += panel.spell_icon_w;
	if (x + panel.spell_icon_space_x > 1280) break;
  x += panel.spell_icon_space_x;
 }
 offset_x = panel.spell_area_w - (t * (panel.spell_icon_w + panel.spell_icon_space_x)) + panel.spell_icon_space_x; 
 offset_x = offset_x / 2;

 x = panel.spell_area_x + offset_x;
 y = panel.spell_area_y + 80;
 t = 0;

 for (s = 0 ; s < panel.spell_icon_amount ; s++)
 { 
  sp = spell_list[t][game.current_wizard].spell;
  alpha = (float)spell_list[t][game.current_wizard].alpha.current;
  if (alpha < 0.0) alpha = 0.0;
  if (alpha > 1.0) alpha = 1.0;
  
	g = -1;
	spell_icon = false;

  if (sp > -1)
	{
	 g = spell[sp].gfx;
	 if (spell[sp].piece_ID == -1) spell_icon = true;

	 // Is this a bonus spell?
	 if (spell_list[t][game.current_wizard].bonus)
	 {
	  // Draw Green outline.
		gfx[HIGHLIGHT_GFX_BOX]->setBlitColor(0.0, 1.0, 0.0, alpha);
    gfx[HIGHLIGHT_GFX_BOX]->stretchAlphaRect(0, 0, gfx[HIGHLIGHT_GFX_BOX]->getWidth(), 
		gfx[HIGHLIGHT_GFX_BOX]->getHeight(), x - 4, y - 4, x + panel.spell_icon_w + 4, y + panel.spell_icon_h + 4);
	 }

	 // Can we cast this spell?
	 if (!spell_list[t][game.current_wizard].cast)
	 {
	  // Draw Red outline.
		gfx[HIGHLIGHT_GFX_BOX]->setBlitColor(1.0, 0.0, 0.0, alpha);
    gfx[HIGHLIGHT_GFX_BOX]->stretchAlphaRect(0, 0, gfx[HIGHLIGHT_GFX_BOX]->getWidth(), 
		gfx[HIGHLIGHT_GFX_BOX]->getHeight(), x - 4, y - 4, x + panel.spell_icon_w + 4, y + panel.spell_icon_h + 4);
	 }

	 gfx[0]->drawRect(x, y, x + panel.spell_icon_w, y + panel.spell_icon_h, 0.0, 0.0, 0.0, alpha);
  
   if (g > -1 && !spell_icon)
	 {
	  piece_gfx[g]->setBlitColor(1.0, 1.0, 1.0, alpha);
		piece_gfx[g]->stretchAlphaRect(0, 0, piece_gfx[g]->getWidth(), piece_gfx[g]->getHeight(),
	  x, y, x + panel.spell_icon_w, y + panel.spell_icon_h);
		piece_gfx[g]->setBlitColor(1.0, 1.0, 1.0, 1.0);
	 }

	 if (g > -1 && spell_icon)
	 {
		spell_icon_gfx[g]->setBlitColor(1.0, 1.0, 1.0, alpha);
		spell_icon_gfx[g]->stretchAlphaRect(0, 0, spell_icon_gfx[g]->getWidth(), spell_icon_gfx[g]->getHeight(), 
		x, y, x + panel.spell_icon_w, y + panel.spell_icon_h);
		spell_icon_gfx[g]->setBlitColor(1.0, 1.0, 1.0, alpha);
	 }
  }

	x += panel.spell_icon_w + panel.spell_icon_space_x;
	t++;

	if (x - panel.spell_area_x + panel.spell_icon_w > panel.spell_area_w)
	{
	 x = panel.spell_area_x + offset_x;
	 y += panel.spell_icon_h + panel.spell_icon_space_y;
	}
 }
 
}

void draw_control_icons(void)
{
 // Draw control icons on panel. Which ones depends on the game phase and what is
 // happening in the game.

 float alpha;
 int x, y;
 
 alpha = panel.spell_area_control_alpha.current;

 x = panel.spell_area_x + 12;
 y = panel.spell_area_y;

 if (game.phase == PHASE_SPELLSELECT)
 {
  // Draw No Spell control icon.
  gfx[15]->blitAlphaRectFx(0, 64, 128, 128, x + 4, y + 8, 0.0, 1.0, alpha);
 }
 if (game.phase == PHASE_SPELLCAST)
 {
  // Draw Skip Spell control icon.
  gfx[10]->blitAlphaRectFx(0, 0, 128, 64, x + 4, y + 8, 0.0, 1.0, alpha);
 }
 if (game.phase == PHASE_MOVEMENT && board_info.selected_state &&
 (!board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].has_moved ||
 !board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].has_attacked))
 {
  // Draw End Move control icon.
  gfx[10]->blitAlphaRectFx(0, 64, 128, 128, x + 4, y + 8, 0.0, 1.0, alpha);
 }
 
 if (game.phase == PHASE_MOVEMENT && board_info.selected_state &&
 board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].has_moved &&
 board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].has_attacked &&
 !board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].has_shot)
 {
  // Draw End Fire control icon.
  gfx[17]->blitAlphaRectFx(0, 0, 128, 64, x + 4, y + 8, 0.0, 1.0, alpha);
 }

 if (game.phase == PHASE_MOVEMENT && !board_info.selected_state)
 {
  // Draw End Turn control icon.
  gfx[11]->blitAlphaRectFx(0, 0, 128, 64, x + 4, y + 8, 0.0, 1.0, alpha);
 }
 if (game.phase == PHASE_MOVEMENT && board_info.selected_state && board_info.selected_layer == PIECE &&
 board[board_info.selected_x][board_info.selected_y][MOUNTED].gfx != BLANK &&
 !board[board_info.selected_x][board_info.selected_y][MOUNTED].has_moved)
 {
  // Draw Dismounted control icon.
  gfx[11]->blitAlphaRectFx(0, 64, 128, 128, x + 4, y + 80, 0.0, 1.0, alpha);
 }
}

void find_control_icon_mouse_over(void)
{
 float x, y;

 x = (float)(panel.spell_area_x + 12);
 y = (float)(panel.spell_area_y + 2);

 mouse.over_control_icon = -1;

 if (game.phase == PHASE_SPELLSELECT)
 {
  if (mouse.x - 16 >= x && mouse.x - 16 <= x + 128 && mouse.y - 16 >= y && mouse.y - 16 <= y + 64) 
	{
	 mouse.over_control_icon = CONTROL_NO_SPELL;
	 clear_highlight_board();
	}
 }

 if (game.phase == PHASE_SPELLCAST)
 {
  if (mouse.x - 16 >= x && mouse.x - 16 <= x + 128 && mouse.y - 16 >= y && mouse.y - 16 <= y + 64)
	mouse.over_control_icon = CONTROL_SKIP_SPELL;
 }

 if (game.phase == PHASE_MOVEMENT && board_info.selected_state &&
 (!board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].has_moved ||
 !board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].has_attacked))
 {
  if (mouse.x - 16 >= x && mouse.x - 16 <= x + 128 && mouse.y - 16 >= y && mouse.y - 16 <= y + 64)
	mouse.over_control_icon = CONTROL_END_MOVE;
 }

 if (game.phase == PHASE_MOVEMENT && board_info.selected_state &&
 board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].has_moved &&
 board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].has_attacked &&
 !board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].has_shot)
 {
  if (mouse.x - 16 >= x && mouse.x - 16 <= x + 128 && mouse.y - 16 >= y && mouse.y - 16 <= y + 64)
	mouse.over_control_icon = CONTROL_END_FIRE;
 }
 
 if (game.phase == PHASE_MOVEMENT && !board_info.selected_state)
 {
  if (mouse.x - 16 >= x && mouse.x - 16 <= x + 128 && mouse.y - 16 >= y && mouse.y - 16 <= y + 64)
	mouse.over_control_icon = CONTROL_END_TURN;
 }

 if (game.phase == PHASE_MOVEMENT && board_info.selected_state && board_info.selected_layer == PIECE &&
 board[board_info.selected_x][board_info.selected_y][MOUNTED].gfx != BLANK &&
 !board[board_info.selected_x][board_info.selected_y][MOUNTED].has_moved)
 {
  if (mouse.x - 16 >= x && mouse.x - 16 <= x + 128 && mouse.y - 16 >= y + 72 && mouse.y - 16 <= y + 64 + 72)
	mouse.over_control_icon = CONTROL_DISMOUNT;
 }
}