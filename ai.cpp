// AI (.Cpp)
// ---------

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
#include "ai.hpp"
#include "options.hpp"

struct ai_t ai[MAX_WIZARDS + 1];

void AI_setup(void)
{
 int w;

 // Set up AI 'personalities'
 for (w = 0 ; w < MAX_WIZARDS ; w++)
 {
  ai[w].risk = IRand(2, 4);
 }
}

void AI_analyse_board(void)
{
 int w, w2, num, strength, risk;
 char text[512];

 num = 0;

 // First find threat values for every alive wizard.
 for (w = 0 ; w < MAX_WIZARDS ; w++)
 {
  game.current_wizard = w;

  if (!wizard[w].alive)
	{
   ai[w].threat = 0;
	}
	else
	{
	 num++;
   ai[w].threat = find_threat_value_to_wizard(w);
	}

	//log("w: %d, threat: %d", w, ai[w].threat);
 }

 num--; // Number of enemy wizards alive;

 // Now find how strong we are in comparison to the other enemy wizards and their pieces..
 for (w = 0 ; w < MAX_WIZARDS ; w++)
 {
  if (!wizard[w].alive) continue;
  strength = 0;

	for (w2 = 0 ; w2 < MAX_WIZARDS ; w2++)
  {
   if (!wizard[w2].alive) continue;
	 if (w == w2) continue;
   strength += ai[w2].threat;
	}
	strength -= (ai[w].threat * num);
  ai[w].strength = strength;

	if (ai[w].strength >= 0)
	{
   risk = 3 + (ai[w].strength / 1000);
	}
	else
	{
   if (ai[w].strength >= -3000)
	 {
    risk = 3 - ((-ai[w].strength) / 1000); 
	 }
	 else
	 {
    risk = ((-ai[w].strength) / 1000) - 3;
	 }
	}
	risk = MID(0, risk, 9);
  if (game.turn > 1) ai[w].risk = risk;

	if (game.debug_to_chat)
	{
	 //log("w: %d, strength: %d", w, ai[w].strength);
	 sprintf(text, "w: %d, strength: %d, risk: %d", w, ai[w].strength, ai[w].risk);
	 add_chat_line(text, wizard[w].col);
	}
 }
}

void AI_select_spell(void)
{
 int x, y, a, s, r, sp, score, best_score, value, best_value, best_x, best_y, best_spell, extra_combat, extra_defence;
 bool illusion, best_illusion;
 char text[MAX_STRING];

 sprintf(panel.info_area_line[0], "%s IS SELECTING A SPELL", wizard[game.current_wizard].name);
 sprintf(panel.info_area_line[1], "");
 
 // * TODO:
 // * We really need to count the number of enemy growths (pieces which grow another identical piece)
 // * And this use this for looking at the anti-growth spells.


 //log("AI Wizard : %d - selecting spell..", game.current_wizard);
 //log("");
 
  // Now find how much our wizard has been 'improved'
 sprintf(text, "WIZARD_%d", game.current_wizard);
 r = find_piece_by_name(text);

 extra_combat = board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].combat - piece[r].combat;
 extra_defence = board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].defence - piece[r].defence;

 best_spell = NO_SPELL;
 best_score = 0;
 //log("");

 r = ai[game.current_wizard].risk;

  for (s = 0 ; s < MAX_WIZ_SPELLS ; s++)
  {
   sp = spell_list[s][game.current_wizard].spell;

   if (sp == -1) continue;
	 if (highlight_where_spell_can_be_cast(sp, true) == 0) continue; // No space to cast this spell!

	 //log("spell: %s", spell[sp].name);
   score = 0;
   illusion = false;

	 // If spell is a creature piece.
	 if (spell[sp].piece_ID != -1)
	 {
	  score = AI_rate_spell_piece(sp, r);

		if (piece[spell[sp].piece_ID].grow_chance > 0)
		{
     // This piece grows!
		 score += 100;
		 if (ai[game.current_wizard].strength < 0) score += -(ai[game.current_wizard].strength / 20);

     // Now factor in chance of growing & attacking..
		 score += piece[spell[sp].piece_ID].grow_chance + piece[spell[sp].piece_ID].grow_attack * 5;
		}
	 }
	 else
	 {
	  best_value = 0;
    best_x = -1;
		best_y = -1;

    // Ok, deal with spells that affect enemy pieces.
		if (spell[sp].enemy || spell[sp].own)
		{
     for (y = 0 ; y < board_info.board_height ; y++)
     {
      for (x = 0 ; x < board_info.board_width ; x++)
      {
       if (highlight_board[x][y].type == HIGHLIGHT_NONE) continue; // Skip empty squares.
	     if (highlight_board[x][y].type == HIGHLIGHT_CAN_RIDE && !spell[sp].own) continue; // Can't cast on own creatures.

			 // Rate value of piece on map.
			 value = AI_rate_piece(x, y, wizard[game.current_wizard].x, wizard[game.current_wizard].y, PIECE, true);

       // If this piece has a rider, then also rate this piece too.
       if (board[x][y][MOUNTED].gfx != BLANK) value = value + AI_rate_piece(x, y,
			 wizard[game.current_wizard].x, wizard[game.current_wizard].y, MOUNTED, true);

			 if (is_piece_friendly(x, y, PIECE) && strlen(spell[sp].new_piece_name) > 0)
			 {
				value = (5000 - value * 2) - (board[x][y][PIECE].magic_resist * 20);
			  //log("x: %d, y: %d, name: %s, value: %d", x, y, board[x][y][PIECE].name, value);
			 }

			 if (spell[sp].magic_attack)
		   {
			  if (wizard_here(x, y) != -1 && board[x][y][MOUNTED].gfx == -1)
			  {
				 a = find_current_option_choice(CONFIG_OPTIONS, "GAME", "MAGIC ATTACK ON WIZARD");

         // If no effect on wizard then there is currently no point in magic attacking a wizard who is not riding..
			   if (a == 0) continue;
				 if (a == 1) value += 250;
				 if (a == 2) value += 1000;
			  }
		   }

			 // Deal with disbelieve..
			 if (spell[sp].destroy_illusion)
			 {
        if (board[x][y][PIECE].disbelieved) continue; // This piece has already been disbelieved and is real..
			 }

       // Is this piece rated better than our existing best valued one?
       if (value > best_value)
       {
        // Yes, so store the details of this as our new best piece.
        best_value = value;
        best_x = x;
        best_y = y;
       }
			}
		 }

		 if (best_x == -1) continue; // No targets, so look at next spell.
		 //log("* best_x : %d, best_y: %d, best_value: %d", best_x, best_y, best_value);

		 if (spell[sp].change_owner_to_our_wizard) score += best_value * 0.1; // Subversion
		 if (strlen(spell[sp].new_piece_name) > 0) score += best_value * 0.09; // Mutation
		 if (spell[sp].chance_against_magic_resistance) score -= (board[best_x][best_y][PIECE].magic_resist) * 10;
		 if (spell[sp].attack_value > 0) score += (best_value * spell[sp].attack_value) / 100; // Magic Missiles
		 if (spell[sp].magic_attack) score += (best_value * 0.05) - (board[best_x][best_y][PIECE].magic_resist) * 10; // Magic Attack
		 if (spell[sp].destroy_illusion)
		 {
			a = 100 - board[best_x][best_y][PIECE].chance; // Let lower chance creatures result in a stronger multiplier.
			score += (best_value * a * 2) / 1250;
		 }
		
		}
	 }
	 
	 // Deal with spells that are cast automatically on wizard..
	 if (spell[sp].range == 0)
	 {
	  if (spell[sp].change_shadow_form_to &&
    !board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].shadow_form)
		{
		 // Let's make shadow form less attractive at first (due to being temporary), but get more attractive
		 // the more a wizard is threatened. I.e. we can run and be more defensive.
		 score += 100;
		 
		 if (ai[game.current_wizard].strength < 0) score += -(ai[game.current_wizard].strength / 12); // Used in defence
		 score *= amount_of_this_spell_in_list(sp);
		}

	  // Defence
    if (spell[sp].add_to_defence > extra_defence)
		{
		 score += (spell[sp].add_to_defence * 25);
		 if (board_info.selected_layer == PIECE) score *= 2; // Not riding anything yet, so more vunerable.
		 if (ai[game.current_wizard].strength < 0) score += -(ai[game.current_wizard].strength / 10); // Used in defence
		 score *= amount_of_this_spell_in_list(sp);
		}
		else
		{
     if (spell[sp].add_to_defence > 0) continue; // This is a spell which will weaken us now!
		}

		// Combat
		if (spell[sp].add_to_combat > extra_combat)
		{
		 score += (spell[sp].add_to_combat * 25);
		 if (ai[game.current_wizard].strength > 0) score += (ai[game.current_wizard].strength / 10); // Used in attack
		 score *= amount_of_this_spell_in_list(sp);
		}
		else
		{
     if (spell[sp].add_to_combat > 0) continue; // This is a spell which will weaken us now!
		}

		// Movement
		if (spell[sp].change_movement_amount_to >
		board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].movement)
		{
		 score += (spell[sp].change_movement_amount_to * 15);
		 score += ABS((ai[game.current_wizard].strength / 10)); // Used in defence & attack
		 score *= amount_of_this_spell_in_list(sp);
		}
		else
		{
     if (spell[sp].change_movement_amount_to > 0) continue; // This is a spell which will weaken us now!
		}

		// Does this spell make us fly?
		if (spell[sp].change_flying_to == 1 &&
		!board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].flying) score += 100;

		// Ranged Combat
		if (spell[sp].change_ranged_combat_attack_to >
		board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].ranged_combat_attack)
		{
		 score += (spell[sp].change_ranged_combat_attack_to * 10) * spell[sp].change_ranged_combat_range_to;
		 score += ABS(ai[game.current_wizard].strength / 10); // Used in defence & attack
		 score *= amount_of_this_spell_in_list(sp);
		}
		else
		{
     if (spell[sp].change_ranged_combat_attack_to > 0) continue; // This is a spell which will weaken us now!
		}

		// Turmoil
		if (spell[sp].swap_board)
		{
     if (ai[game.current_wizard].strength > -4000) continue; // Not dangerous enough to cast!
		 if (IRand(0, 99) > 80) score = 9999; // Cast it and hope for the best!
		}
	 }

	 // Teleport
   if (spell[sp].move_piece)
	 {
    if (ai[game.current_wizard].strength > -4000) continue; // Not dangerous enough to cast!
		if (IRand(0, 99) > 60) score = 9999; // Cast it and hope for the best!
	 }

	 // Add Law / Chaos bonus.. (To make law-1, law-2, chaos-1 & chaos-2 attractive!)
	 score += ABS(spell[sp].balance);

	 // Spells that cast more pieces are more valuable.
	 if (spell[sp].casts > 1) score *= spell[sp].casts * 0.75;

	 // See if this spell is too risky to cast..
   if (return_spell_chance(sp) < 100 - (r * 10))
	 {
	  // Wait.. we could cast this spell as an illusion and it's not a 100% chance spell!
		if (spell[sp].can_be_illusion && return_spell_chance(sp) < 100)
		{   
		 // Find how weak we are and rate that between 1 and 10 (10 being very threatened)
		 a = -ai[game.current_wizard].strength / 1000;
		 a = MID(1, a, 15);

		 //if (score > 0) log("* name: %s, num: %d, score: %d, a: %d, chance: %d", spell[sp].name, sp, score, a,
		 //return_spell_chance(sp));

		 // Roll against how weak we are and see if we want to try for an illusion..
		 if (IRand(0, 20) < a)
		 {
      illusion = true;
		 }
		 else
		 {
		  score = 0; // Spell is too dangerous to cast at the moment.
		  //log("Too risky!");
		 }
		}
	 }
   
	 //if (score > 0) log("name: %s, num: %d, score: %d", spell[sp].name, sp, score);

	 // Best piece found so far..
	 if (score > best_score)
	 {
    best_score = score;
	  best_spell = s;
		best_illusion = illusion;
	 }
	}

	// Now let's see what spell we came up with..
	//log("best score: %d, best spell: %d, name: %s", best_score, best_spell,
	//spell[spell_list[best_spell][game.current_wizard].spell].name);

  if (best_spell != NO_SPELL)
	{
	 if (game.debug_to_chat)
	 {
	  sprintf(text, "Wizard %d selecting spell %d : %s", game.current_wizard, best_spell,
		spell[spell_list[best_spell][game.current_wizard].spell].name);

	  add_chat_line(text, wizard[game.current_wizard].col);
	 }
	}

 // No spells left!
 if (best_spell == NO_SPELL)
 {
	if (game.debug_to_chat)
	{
	 sprintf(text, "Wizard %d has not found a spell", game.current_wizard);
	 add_chat_line(text, wizard[game.current_wizard].col);
	}
 }

 select_spell(best_spell);
 wizard[game.current_wizard].spell_is_illusion = best_illusion;

 wait_time(200);
}

void AI_select_spell_square(void)
{
 int x, y, p, best_x, best_y, x1, y1, x2, y2, range;
 float dist, best_dist;
 bool defensive;
 char name[MAX_STRING];

 // Reset best values
 best_dist = 0.0;
 best_x = -1;
 best_y = -1;
 dist = 0.0;

 if (game.debug_to_chat)
 {
	sprintf(name, "wizard :%d is selecting a spell square", game.current_wizard);
	add_chat_line(name, wizard[game.current_wizard].col);
 }

 // Find target first.
 AI_select_target(wizard[game.current_wizard].x, wizard[game.current_wizard].y, -1, true, false);
 range = highlight_where_spell_can_be_cast(wizard[game.current_wizard].spell, true);

 // No Targets?
 if (range == 0) goto end;

 defensive = false;

 // Check if this is a defensive spell that we should cast nearest our wizard, i.e. castle..
 p = spell[wizard[game.current_wizard].spell].piece_ID;
 if (p != -1)
 {
  if (piece[p].rideable) defensive = true;
 }

 wait_time(50);

 for (y = 0 ; y < board_info.board_height ; y++)
 {
  for (x = 0 ; x < board_info.board_width ; x++)
  {
   // Skip any non-highlighted squares.
   if (highlight_board[x][y].type == HIGHLIGHT_NONE) continue;

	 if (highlight_board[x][y].type == HIGHLIGHT_CAN_MOVE)
	 {
    // How far away is this square from our wizard?
    range = workout_range(wizard[game.current_wizard].x, wizard[game.current_wizard].y, x, y);   

    // Ok, find how close this square is to our target..
    dist = workout_range(ai[game.current_wizard].target_x, ai[game.current_wizard].target_y, x, y);

		if (dist < 2.0 && p != -1 && piece[p].grow_chance > 0 && piece[p].defence > 0)
		{
     // Ok, we have a creature that grows, but can be destroyed. So don't cast too close to enemy!
			dist += 1.0;
		}

		dist = 1000 - (dist * 4) - (range);

		if (defensive) dist = dist - (range * 4);

		// Find best place for wizard to move to..
		if (spell[wizard[game.current_wizard].spell].move_piece)
		{
     dist = 9999 - AI_select_target(x, y, -1, true, true);
		}
	 }
	 if (highlight_board[x][y].type == HIGHLIGHT_CAN_ATTACK)
	 {
    // Rate value of piece on map.
		dist = AI_rate_piece(x, y, wizard[game.current_wizard].x, wizard[game.current_wizard].y, PIECE, true);

    // If this piece has a rider, then also rate this piece too.
    if (board[x][y][MOUNTED].gfx != BLANK) dist += AI_rate_piece(x, y,
		wizard[game.current_wizard].x, wizard[game.current_wizard].y, MOUNTED, true);

		if (spell[wizard[game.current_wizard].spell].magic_attack)
		{
		 if (wizard_here(x, y) != -1 && board[x][y][MOUNTED].gfx == -1)
		 {
      // There is currently no point in magic attacking a wizard who is not riding..
			continue;
		 }
		}
	 }
	 if (highlight_board[x][y].type == HIGHLIGHT_CAN_RIDE)
	 {
    dist = AI_rate_piece(x, y, wizard[game.current_wizard].x, wizard[game.current_wizard].y, PIECE, true);

		if (is_piece_friendly(x, y, PIECE) && 
		strlen(spell[wizard[game.current_wizard].spell].new_piece_name) > 0)
		{
		 dist = (5000 - dist * 2) - (board[x][y][PIECE].magic_resist * 20);
		}
	 }

	 if (dist > best_dist)
   {
    best_dist = dist;
    best_x = x;
    best_y = y;
	 }
  }    
 }
 
 end:

 // Store target square for spell casting..
 ai[game.current_wizard].target_x = best_x;
 ai[game.current_wizard].target_y = best_y;

 if (best_x == -1 && game.debug_to_chat)
 {
  add_chat_line("Can't find a place to cast spell!", wizard[game.current_wizard].col);		 
 }

 // Pause if in AI debug mode..
 if (game.AI_debug)
 {
  
  do
	{
   loop();
	} while (!mouse.right_click && !game.exit_key);
 }
 wait_time(50);
}

int AI_select_target(int tx, int ty, float range, bool combine, bool wiz)
{
 // This function is used to find a target for movement and spell casting.
 
 // If combine is true then the total 'threat' value from this tx and ty square is returned.
 // If combine is false then the threat from the most dangerous piece is returned instead.
 
 // In both cases, the target x and y is stored in our AI struct.
 
 int x, y, best_x, best_y, value, best_value, total;
 float dist;

 best_value = -1; // Reset default best_value.
 best_x = -1;
 best_y = -1;
 total = 0;
 
 for (y = 0 ; y < board_info.board_height ; y++)
 {
  for (x = 0 ; x < board_info.board_width ; x++)
  {
   if (board[x][y][PIECE].gfx == BLANK) continue; // Skip empty squares.
   if (is_piece_friendly(x, y, PIECE)) continue; // Only looking for enemies.
   
	 dist = workout_range(x, y, tx, ty);
	 if (range != -1 && dist > range + 0.5) continue;

   value = AI_rate_piece(x, y, tx, ty, PIECE, wiz); // Rate value of piece on map.

   // If this piece has a rider, then also rate this piece too.
   if (board[x][y][MOUNTED].gfx != BLANK) value = value + AI_rate_piece(x, y, tx, ty, MOUNTED, wiz);

	 // Is there a piece that has been covered here?
	 if (board[x][y][PIECE_COVERED].gfx != -1)
	 {
    // Yes!
		if (board[x][y][PIECE_COVERED].owner == game.current_wizard)
		{
		 // It's our piece, so make it more important to destroy this covering growth.
     value += AI_rate_piece(x, y, tx, ty, PIECE_COVERED, wiz) * 1.5;
		}
		else
		{
		 // It's a trapped enemy piece, so make it more important NOT to destroy this covering growth.
     value -= AI_rate_piece(x, y, tx, ty, PIECE_COVERED, wiz) * 1.5;     
		}
	 }

   total = total + value;
   
   // Is this piece rated better than our existing best valued one?
   if (value > best_value)
   {
    // Yes, so store the details of this as our new best piece.
    best_value = value;
    best_x = x;
    best_y = y;
   }
  }
 }
 
 ai[game.current_wizard].target_x = best_x;
 ai[game.current_wizard].target_y = best_y;
 
 // If combine flag then return total values.
 if (combine) return total;
 
 // Ok, just return value of most dangerous creature.
 return best_value;
}

int AI_select_ranged_target(void)
{
 // This function is used to find a target for movement and spell casting.
 
 // If combine is true then the total 'threat' value from this tx and ty square is returned.
 // If combine is false then the threat from the most dangerous piece is returned instead.
 
 // In both cases, the target x and y is stored in our AI struct.
 
 int x, y, tx, ty, best_x, best_y, value, best_value, total;
 
 best_value = -1; // Reset default best_value.
 best_x = -1;
 best_y = -1;
 total = 0;
 
 tx = board_info.selected_x;
 ty = board_info.selected_y;

 // If we have shadow form, don't find any valid targets to shoot.
 if (board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].shadow_form) goto skip;

 for (y = 0 ; y < board_info.board_height ; y++)
 {
  for (x = 0 ; x < board_info.board_width ; x++)
  {
	 if (highlight_board[x][y].type != HIGHLIGHT_CAN_ATTACK) continue;
   
   value = AI_rate_piece(x, y, tx, ty, PIECE, false); // Rate value of piece on map.

   // If this piece has a rider, then also rate this piece too.
   if (board[x][y][MOUNTED].gfx != BLANK) value = value + AI_rate_piece(x, y, tx, ty, MOUNTED, false);

   total = total + value;
   
   // Is this piece rated better than our existing best valued one?
   if (value > best_value)
   {
    // Yes, so store the details of this as our new best piece.
    best_value = value;
    best_x = x;
    best_y = y;
   }
  }
 }
 
 skip:

 ai[game.current_wizard].target_x = best_x;
 ai[game.current_wizard].target_y = best_y;
 
 // Ok, return value of most dangerous creature.
 return best_value;
}

int AI_rate_spell_piece(int sp, int risk)
{
 int p, wx, wy, strength, final_value, def, add, value = 0;

 wx = wizard[game.current_wizard].x;
 wy = wizard[game.current_wizard].y;

 strength = ai[game.current_wizard].strength / 1000;

 // Work out value of 'creature' spell..
 p = spell[sp].piece_ID;

 // Attack.
 value = (piece[p].combat * 6) + (piece[p].ranged_combat_attack * 6) + piece[p].ranged_combat_range;
 if (strength > 0) value += (ABS(strength) * 4); // We are strong, so more attack to kill enemies!

 // Add defence.
 def = piece[p].defence;
 if (def == 0) def = 15; // Invunerable! But normally can't move..

 if (piece[p].undead)
 {
  find_option_choice_variables(CONFIG_OPTIONS, "GAME", "NORMAL CREATURE ATTACKS UNDEADS", &add, NULL, NULL, NULL);
	if (add == -1) add = 5;
  def += add;
 }

 value += (def * 4);
 if (strength < 0) value += ABS(strength); // Under threat, we want more defence.

 // Add movement.
 value += (piece[p].movement * 3);

 // Status flags.
 value += (piece[p].undead * 15);
 value += (piece[p].rideable * 5);
 value += (piece[p].flying * 10);

 // If piece can't move, or attack, then reduce scores!
 if (piece[p].movement == 0) value /= 2;
 if (piece[p].combat == 0) value /= 2;

 // If piece is rideable then increase score!
 if (piece[p].rideable)
 {
  value *= 1.5;
	if (board[wx][wy][MOUNTED].gfx == -1) value *= 2; // We aren't riding anything, so this looks good!
 }

 final_value = value;

 //log("name: %s - rated: %d - chance: %d, Final Value: %d", piece[p].name, value, spell[sp].chance, final_value);

 return final_value;
}

int AI_rate_piece(int x, int y, int tx, int ty, int layer, bool wiz)
{
 // This rates a piece at x, y based on it's stats and the distances from both
 // the current wizard, and from tx, ty.
 
 // We should avoid hardcoding preferences, to make the piece data user-definable.
 
 float total, range_from_wizard, range_from_selected, old_range_from_wizard, old_range_from_selected;
 int value, w, add;
 
 range_from_selected = workout_range(tx, ty, x, y);
 range_from_wizard = workout_range(wizard[game.current_wizard].x, wizard[game.current_wizard].y, x, y);
 old_range_from_selected = range_from_selected;
 old_range_from_wizard = range_from_wizard;

 if (wiz)
 {
  // Wizard movement, so wizard x & y becomes tx & ty.
	range_from_wizard = range_from_selected;
 }

 // Create a value for how far his piece can move + how far it can shoot.
 total = (board[x][y][layer].movement + board[x][y][layer].ranged_combat_range) * 0.75;
 if (board[x][y][layer].flying) total = total * 1.5;

 // Use this to reduce the distance, so the real danger of fast & shooting creatures
 // is recognised.
 if (total > 0)
 {
  range_from_selected = range_from_selected / total;
  range_from_wizard = range_from_wizard / total;
  
  if (range_from_selected < 0.25) range_from_selected = 0.25; // Keep to at least 1.0;
  if (range_from_wizard < 0.25) range_from_wizard = 0.25; // Keep to at least 1.0;
 }
 
 // Make the closer distances much more important.
 range_from_selected = 99.0 / range_from_selected;
 range_from_wizard = 799.0 / range_from_wizard; // The wizard is our most important piece.

 // Now work out rating value..
 value = (board[x][y][layer].combat * 6 + board[x][y][layer].ranged_combat_attack) * 6;
 
 // Add defence.
 value = value + (board[x][y][layer].defence * 3);
 
 // Deal with Undeads.
 if (board[x][y][layer].undead)
 {
  find_option_choice_variables(CONFIG_OPTIONS, "GAME", "NORMAL CREATURE ATTACKS UNDEADS", &add, NULL, NULL, NULL);
  if (add != -1) value += (add * 3);
 }

 if (board[x][y][layer].combat == 0) value /= 6; // No attack, then this is not so important.
 if (board[x][y][layer].movement == 0) value /= 4; // No movement, then this is not so important.

 // Is there a wizard here? If so and it's an enemy then increase value!
 w = wizard_here(x, y);
 if (w != -1 && w != game.current_wizard)
 {
	range_from_selected = range_from_selected / 4.0;
	range_from_wizard = range_from_wizard / 1.5;
	value = value + 500;
 }

 // This enemy can reach and attack us!
 if (board[x][y][layer].movement + 1.5 > old_range_from_selected && board[x][y][layer].combat > 0) value *= 2.0;

 // This enemy can reach and attack our wizard!
 if (board[x][y][layer].movement + 1.5 > old_range_from_wizard && board[x][y][layer].combat > 0) value *= 3.0;

 // Add distances.
 value = value + (int)range_from_selected + (int)range_from_wizard;

 if (board[x][y][layer].defence == 0) value = 0; // There is no point targetting a piece we cannot kill!

 // If looking for wizard movement then see if the threat is not real (no combat or can't move next to us..)
 if (wiz)
 {
  if (board[x][y][layer].combat == 0 || (board[x][y][layer].movement == 0 && old_range_from_wizard >= 2.0)) value = MAX(board_info.board_width, board_info.board_height) - old_range_from_selected;
 }

 // If we are next to a piece that can grow, then it's score rises a lot (depending on how much chance & attack it has
 // of growing)
 if (old_range_from_selected < 2.0 && board[x][y][layer].grow_chance > 0)
 {
	value += 5000 + (board[x][y][layer].grow_chance * (board[x][y][layer].grow_attack * 5));
  if (!wiz && board[x][y][layer].defence == 0) value = 1;
 }
 highlight_board[x][y].AI_score = value;

 return value;
}

void AI_do_movement(void)
{
 // This looks at all our pieces on the board and then moves them one by one.
 // We now use our turn values to move the oldest pieces first..
 
 int x, y, t;
 
 for (t = game.turn + 1 ; t >= 0 ; t--)
 {
  for (y = 0 ; y < board_info.board_height ; y++)
  {
   for (x = 0 ; x < board_info.board_width ; x++)
   {
    if (board[x][y][PIECE].gfx == BLANK) continue;
    if (board[x][y][PIECE].owner != game.current_wizard) continue; // Don't look at enemy pieces.
   
    // Has this piece already moved?
    if (board[x][y][PIECE].has_moved && board[x][y][PIECE].has_attacked && board[x][y][PIECE].has_shot) continue;  

    if (board[x][y][PIECE].turn != t) continue; // Skip piece for now if not on the current turn.

    if (try_to_select_piece(x, y, true))
		{
		 AI_move_piece();
		}
		else
		{
     continue;
		}

    check_for_winner();
    if (game.winner != -1) return; // We've won!
   
    board_info.selected_state = false;
   
    if (game.exit_key) return; // Abort game if exit keys are pressed.
    
    board_info.selected_alpha.target = 0.0; 
    wait_time(25);  
   }
  }
 
 }
}

void AI_move_piece(void)
{
 // This moves an individual piece until it is finished.
 
 int x, y, best_x, best_y, x1, y1, x2, y2, range, total;
 float dist, best_dist, value, best_value;

 //board_info.highlight_type = HIGHLIGHT_NONE;
 board_info.highlight_alpha.target = 0.0; 

 wait_time(50);
 start:
        
 // Find target first (-1 = anywhere on the board if not engaged)
 dist = -1;
 if (board_info.selected_engaged) dist = 1;

 // * TODO:
 // * We should really look at least 3 or 4 squares ahead to enable all pieces to get around walls and trees
 // * effectively

 if (game.debug_to_chat) log("AI moving piece : %s", board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].name);
 AI_select_target(board_info.selected_x, board_info.selected_y, dist, false, false);
 
 // Pause if in AI debug mode..
 if (game.AI_debug)
 {
  do
	{
   loop();
	} while (!mouse.right_click && !game.exit_key);
 }
 //wait_time(50);
 
 // No target found.. 
 if (ai[game.current_wizard].target_x == -1)
 {
  end_move_piece();
  return;
 }
   
 wait_time(50);

 // Reset best values
 best_value = -1;
 best_dist = 100000.0; // This is so high because for a wizard it is used to find threats.
 best_x = -1;
 best_y = -1;
 
 for (y = 0 ; y < board_info.board_height ; y++)
 {
  for (x = 0 ; x < board_info.board_width ; x++)
  {
   if (highlight_board[x][y].type == HIGHLIGHT_NONE) continue; // Out of move range.
      
   // Ok, find how close this square is to our target..
   dist = workout_range(ai[game.current_wizard].target_x, ai[game.current_wizard].target_y, x, y);
   
   if (highlight_board[x][y].type == HIGHLIGHT_CAN_ATTACK)
	 {
    value = AI_rate_piece(x, y, board_info.selected_x, board_info.selected_y, PIECE, false);
		
		if (value > best_value)
		{
		 best_value = value;
		 best_x = x;
		 best_y = y;
		}
	 }
   
   // Record details of this square if it's closer to our target (or if less threat) and we have no other attack targets
   if (dist < best_dist && best_value < 0)
   {
    best_dist = dist;
    best_x = x;
    best_y = y;
   }
  }
 }

 // Store target square for move..
 ai[game.current_wizard].target_x = best_x;
 ai[game.current_wizard].target_y = best_y;

 // A Wizard should generally avoid enemies instead of moving towards them..
 if (board_info.selected_x == wizard[game.current_wizard].x && board_info.selected_y == wizard[game.current_wizard].y)
 {
  AI_do_wizard_movement();

	
  if (ai[game.current_wizard].target_x == -1 || (ai[game.current_wizard].target_x == board_info.selected_x && 
 ai[game.current_wizard].target_y == board_info.selected_y) && board[board_info.selected_x][board_info.selected_y][MOUNTED].gfx != -1)
	{
   // Our wizard isn't moving, but is mounted.. so maybe can shoot from mount?
	 board_info.selected_layer = MOUNTED;
	}
 }
 
 if (ai[game.current_wizard].target_x == -1 || (ai[game.current_wizard].target_x == board_info.selected_x && 
 ai[game.current_wizard].target_y == board_info.selected_y))
 {
  // If we have no targets left or we need to stand still, then end move.
  //end_move_piece();
	 if (game.debug_to_chat) log("target x: %d, target y: %d", ai[game.current_wizard].target_x, ai[game.current_wizard].target_y);
	board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].has_moved = true;
  board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].has_attacked = true;
	if (game.debug_to_chat) log("Ending piece move");
 }

 // Pause if in AI debug mode..
 if (game.AI_debug)
 {
  do
	{
   loop();
	} while (!mouse.right_click && !game.exit_key);
 }
 
 // Move piece (with all checking and attacks done there)..
 try_to_move_piece(ai[game.current_wizard].target_x, ai[game.current_wizard].target_y, true);

 if (!board_info.selected_state) return; // Piece must have been deselected..
 
 /*
 log("name: %s, x: %d, y: %d, layer: %d, has_moved: %d, engaged: %d, shot: %d", 
 board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].name,
 board_info.selected_x, board_info.selected_y, board_info.selected_layer,
 board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].has_moved, board_info.selected_engaged,
 board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].has_shot);
 */

 if (game.exit_key) return;

 // If we have movement left or we are engaged then continue movement.
 if (!board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].has_moved || board_info.selected_engaged) goto start;

 // Ok, we've finished moving this piece. Can we shoot?
 if (board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].has_shot)
 {

  // No.
  end_move_piece();
	return;
 }
 
 // Shooting
 // --------

 // Highlight enemy pieces that can be shot..

 //log("shooting: %s, x: %d, y: %d, layer: %d", board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].name,
 //board_info.selected_x, board_info.selected_y, board_info.selected_layer);

 total = set_highlight_board(board_info.selected_x, board_info.selected_y, 
 board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].ranged_combat_range, PIECE, 
 false, false, true, true, true, "");

 // Any targets?
 if (total == 0)
 {
  clear_highlight_board();
  board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].has_shot = true;
		
	end_move_piece();
	return;
 }

 range = AI_select_ranged_target();

 if (range == -1 || board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].shadow_form)
 {
  // No pieces we can shoot.
  end_move_piece();
	return;
 }

 wait_time(100); // Wait a second after moving before we shoot.
 shoot_piece(ai[game.current_wizard].target_x, ai[game.current_wizard].target_y);
 end_move_piece();
}

void AI_do_wizard_movement(void)
{
 int x, y, wx, wy, value, best_value, best_x, best_y;
 float range;
 bool enemy, ride;
 char name[512];

 // Reset best values
 best_value = 0;
 best_x = -1;
 best_y = -1;
 wx = wizard[game.current_wizard].x;
 wy = wizard[game.current_wizard].y;
 enemy = false;
 ride = false;

 //log("wizard: %d", game.current_wizard);

 // Look at each possible move square and rate it..
 for (y = 0 ; y < board_info.board_height ; y++)
 {
  for (x = 0 ; x < board_info.board_width ; x++)
  {
   if (highlight_board[x][y].type == HIGHLIGHT_NONE && !(x == wx && y == wy)) continue;

	 value = 9999 - AI_select_target(x, y, -1, true, true);

   // * TODO:
	 // * We need to add code to make the wizard get off of worst mounts for better choices..


	 if (highlight_board[x][y].type == HIGHLIGHT_CAN_RIDE)
	 {
    value = AI_rate_piece(x, y, board_info.selected_x, board_info.selected_y, PIECE, false) + 8000;
		if (value > best_value)
		{
		 best_value = value;
		 best_x = x;
		 best_y = y;
		 ride = true;
	  }
	 }
	 if (highlight_board[x][y].type == HIGHLIGHT_CAN_ATTACK && !ride &&
	 !board[board_info.selected_x][board_info.selected_y][board_info.selected_layer].shadow_form)
	 {
	  // Unless our wizard can outrun or outfly this enemy, then attack! Except if we have shadow form..
		range = workout_range(x, y, wx, wy);
		//log("range: %f, movement: %d, x: %d, y: %d, wx: %d, wy: %d", range, board[wx][wy][PIECE].movement, x, y, wx, wy);

		// Is piece too close to avoid?
    if (range - 1.5 < board[wx][wy][PIECE].movement && board[x][y][PIECE].movement > 0)
		{
     value = AI_rate_piece(x, y, board_info.selected_x, board_info.selected_y, PIECE, false) * 5;
		 highlight_board[x][y].AI_score = value;
		 if (value > best_value)
		 {
			best_value = value;
		  best_x = x;
		  best_y = y;
		  enemy = true;
		 }
		}
	 }

	 // Make the squares around the edges a little bit worse to be in (when feeling confident - low risk)
   if ((x == 0 || y == 0 || x == board_info.board_width - 1 || y == board_info.board_height - 1) &&
   ai[game.current_wizard].strength >= 0) value -= 500;

   if ((x == 1 || y == 1 || x == board_info.board_width - 2 || y == board_info.board_height - 2) &&
   ai[game.current_wizard].strength >= 1000) value -= 250;

	 highlight_board[x][y].AI_score = value;

	 if (value > best_value && !enemy && !ride)
	 {
    best_value = value;
		best_x = x;
		best_y = y;
	 }
	}
 }

 //log("wizard: %d at x: %d, y: %d, enemy: %d, ride: %d", game.current_wizard, wx, wy, enemy,ride);

 // Store target square for move..
 ai[game.current_wizard].target_x = best_x;
 ai[game.current_wizard].target_y = best_y;

 if (game.debug_to_chat)
 {
	sprintf(name, "Wizard move target - x: %d, y: %d - value: %d", best_x, best_y, value);
  add_chat_line(name, wizard[game.current_wizard].col);
 }
}

int find_threat_value_to_wizard(int w)
{
 int x, y;

 x = wizard[w].x;
 y = wizard[w].y;

 return AI_select_target(x, y, -1, true, true);
}