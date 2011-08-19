// Scene (.Cpp)
// ------------

#include <ptk.h>
#include <vector>
#include <list>
#include <sstream>
#include "math.h"
#include "gl.h"
#include "KPTK.h"

using namespace std;

#include "bridge.hpp"
#include "game.hpp"
#include "log.hpp"
#include "timer.hpp"
#include "scene.hpp"
#include "config.hpp"
#include "piece.hpp"
#include "board.hpp"
#include "chat.hpp"
#include "wizard.hpp"

#include "net.hpp"
#include "panel.hpp"
#include "spell.hpp"
#include "options.hpp"
#include "sound.hpp"

// Gfx Images.
vector <KGraphic *> gfx;
vector <KGraphic *> piece_gfx;
vector <KGraphic *> spell_icon_gfx;

// The list of active sprites.
list < sprite_t > sprites;

KText *fonty[MAX_FONTS];

struct mouse_t mouse;
struct bar_effect_t bar_effect;

struct KFont largefontTablePtr[256];
struct KFont smallfontTablePtr[256];

// Alpha logic code. Used to deal with moving our current value towards our target value
// at different speeds. Good for fading effects in and out, but could be done better. :)

void do_alpha_logic(struct alpha *alpha_pointer)
{
  // Increase alpha value.
  if (alpha_pointer->current < alpha_pointer->target)
  {
   alpha_pointer->current += alpha_pointer->speed;
   
   // Clip current value to target if gone over.
   if (alpha_pointer->current > alpha_pointer->target)
   alpha_pointer->current = alpha_pointer->target;   
  }
  
  // Decrease alpha value.
  if (alpha_pointer->current > alpha_pointer->target)
  {
   alpha_pointer->current -= alpha_pointer->speed;
   
   // Clip current value to target if gone over.
   if (alpha_pointer->current < alpha_pointer->target)
   alpha_pointer->current = alpha_pointer->target;   
  }   
}

// Done every frame for all logic (regardless of section of game we are in)
void do_scene_logic(void)
{
 //do_chat_logic();
 //net_process();
 do_music_logic();

 if (!CheckWindowFocus()) return;

 do_mouse_logic();

 if (game.stage == GAME_MENUS)
 {
  do_options(false);
 }

 if (game.stage == GAME_INGAME)
 {
	do_alpha_logic(&bar_effect.alpha);
 
  do_alpha_logic(&board_info.highlight_alpha);
  do_alpha_logic(&board_info.selected_alpha);
  board_info.highlight_cycle += 0.02;
  do_highlight_board_logic();

  do_panels_logic();
	do_pieces_animation_logic();
	do_sprites_logic();
 }
}

// Drawing code called every frame (deals with fitting buffer into current screenmode)
void draw_scene(void)
{
 float x, y, a, scale;
 int x2, y2;

 // Find angle to fit 1280 x 960 area into current screen size..
 x = ScreenWidth() / 1280.0;
 y = ScreenHeight() / 960.0;
 a = MIN(x, y);

 // Also, find x and y amounts to centre new screen.
 x = -(1280.0 - (ScreenWidth())) / 2.0;
 y = (960.0 - (ScreenHeight())) / 2.0;

 // We have to scale the x and y by the new viewing angle too.
 x = x * a;
 y = y * a;
 gwin->setWorldView(x, y, 0, a, true);

 // Now we can draw all the gfx:
 if (game.stage == GAME_INGAME)
 {
  draw_board();
  draw_panels();
  draw_sprites();
  draw_mouse();
  draw_bar_effect();
 }
 if (game.stage == GAME_MENUS)
 {
  do_options(true);
	draw_mouse();
 }

 scale = (float)ScreenHeight() / (float)ScreenWidth();

 //log("height: %d, width: %d, scale: %f, x: %f, y: %f, a: %f", ScreenHeight(), ScreenWidth(), scale, x, y, a);

 // Draw black borders to get rid of overdraw from clouds.
 if (scale > 0.75) // Portrait display
 {
  gfx[0]->drawRect(0, -(ScreenHeight() / a), 1280, 0, 0.0, 0.0, 0.0, 1.0);
  gfx[0]->drawRect(0, 960, 1280, 992 / a, 0.0, 0.0, 0.0, 1.0);
 }
 if (scale < 0.75) // Widescreen
 {
  gfx[0]->drawRect(-(ScreenWidth() / a), 0, 0, (ScreenHeight() / a) + 1, 0.0, 0.0, 0.0, 1.0);
  gfx[0]->drawRect(1280, 0, ScreenWidth() / a, (ScreenHeight() / a) + 1, 0.0, 0.0, 0.0, 1.0);
 }
}

// Basically tell OpenGL to draw screen.
void refresh_screen(void)
{
 UpdateScreen();
 timer.gfx_frames++; // Keep track of 'screens' drawn.
}

void do_mouse_logic(void)
{
 int x, y, x2, y2, t, s, offset_x;
 float fx, fy;

 GetMouseMickeys(&x, &y);

 fx = x;
 fy = y;
 AccelerateMouseMickeys(&fx, &fy, 20, 40);

 mouse.x += fx;
 mouse.y += fy;

 mouse.x = MID(16, mouse.x, 1280 - 16);
 mouse.y = MID(16, mouse.y, 960 - 16);
 mouse.rgba = Rgba(1.0, 1.0, 1.0);

 if (game.stage == GAME_INGAME)
 {
  mouse.over_board_x = -1;
  mouse.over_board_y = -1;

  // Is mouse over board?
  if (mouse.x - 16 >= board_info.start_x && mouse.x - 16 <= board_info.start_x + (board_info.board_width * board_info.square_width)
  &&	 mouse.y - 16 >= board_info.start_y && mouse.y - 16 <= board_info.start_y + (board_info.board_height * board_info.square_height))
  {
   mouse.over_board_x = (mouse.x - board_info.start_x - 16) / board_info.square_width;
   mouse.over_board_y = (mouse.y - board_info.start_y - 16) / board_info.square_height;
  }

  // Is mouse over spell icon?
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

  mouse.over_spell_icon = -1;

	for (s = 0 ; s < panel.spell_icon_amount ; s++)
  { 
    if (mouse.x - 16 >= x && mouse.x - 16 <= x + panel.spell_icon_w &&
		   mouse.y - 16 >= y && mouse.y - 16 <= y + panel.spell_icon_h)
	  {
     if (spell_list[0][game.current_wizard].alpha.current >= 1.0)
		 {
			mouse.over_spell_icon = t;
		  mouse.spell_is_illusion = false;
		 
			if (spell[spell_list[t][game.current_wizard].spell].can_be_illusion && mouse.y - 16 >= y + (panel.spell_icon_h / 2)
			&& return_spell_chance(spell_list[t][game.current_wizard].spell) < 100)
			{
       mouse.spell_is_illusion = true; // Illusion
			 mouse.rgba = Rgba(1.0, 0.5, 0.0);
			}
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

  find_control_icon_mouse_over();
 }

 //mouse.old_left_click = mouse.left_click;
 //mouse.old_right_click = mouse.right_click;
 mouse.left_click = LeftMouseButton();
 mouse.right_click = RightMouseButton();

 do_alpha_logic(&mouse.alpha);
}

void draw_mouse(void)
{
 gfx[9]->setBlitColor(mouse.rgba.r, mouse.rgba.g, mouse.rgba.b, 1.0);
 BlitTransform(gfx[9], mouse.x, mouse.y, 32, 32, 0.0, mouse.alpha.current);
}

void draw_bar_effect(void)
{
 float x, y, w, h;
 char text[MAX_STRING];

 if (bar_effect.alpha.current < 0.0) return; // Bar effect invisible..

 w = (panel.info_area_w * bar_effect.alpha.current) / 2.0;
 x = board_info.start_x + ((board_info.board_width * board_info.square_width) / 2);
 y = board_info.start_y + ((board_info.board_height * board_info.square_height) / 2) + 1;
 h = 80;

 gfx[13]->stretchAlphaRect(0, 0, 16, 4, x - w, y - h, x + w, y - h + 4, 1.0, 0.0); // Top
 gfx[13]->stretchAlphaRect(0, 4, 16, 12, x - w, y - h + 4, x + w, y + h - 4, 1.0, 0.0); // Middle
 gfx[13]->stretchAlphaRect(0, 12, 16, 16, x - w, y + h - 4, x + w, y + h, 1.0, 0.0); // Bottom

 y = y - (h * 0.75) + 1;
 
 if (bar_effect.mode == BAR_MODE_NEWTURN)
 {
	draw_text(bar_effect.text, board_info.start_x, board_info.start_x + (panel.info_area_w), y + 1, FONT_LARGE, 0, 
  wizard[game.current_wizard].col, bar_effect.alpha.current, TEXT_CENTRE);

  sprintf(text, "- Neutral -");
  if (game.balance < -3) sprintf(text, "- Chaos %d -",(-game.balance / 4));
  if (game.balance > 3) sprintf(text, "- Law %d -",(game.balance / 4));

  y += 48;
  draw_text(text, board_info.start_x, board_info.start_x + (panel.info_area_w), y, FONT_SMALL, 1, 
  Rgba( 1.0, 0.5, 0.0 ), bar_effect.alpha.current, TEXT_CENTRE);
 
  sprintf(text, "Turn : %d", game.turn);
  y += 30;
  draw_text(text, board_info.start_x, board_info.start_x + (panel.info_area_w), y, FONT_SMALL, 1, 
  Rgba( 0.5, 0.5, 0.5 ), bar_effect.alpha.current, TEXT_CENTRE);

  if (game.phase == PHASE_SPELLSELECT) sprintf(text, "Spell selection phase");
  if (game.phase == PHASE_SPELLCAST) sprintf(text, "Spell casting phase");
  if (game.phase == PHASE_MOVEMENT) sprintf(text, "Movement phase");

  y += 20;
  draw_text(text, board_info.start_x, board_info.start_x + (panel.info_area_w), y, FONT_SMALL, 1, 
  Rgba( 1.0, 1.0, 1.0 ), bar_effect.alpha.current, TEXT_CENTRE);
 }

 if (bar_effect.mode == BAR_MODE_WINNER)
 {
	draw_text(bar_effect.text, board_info.start_x, board_info.start_x + (panel.info_area_w), y + 88, FONT_LARGE, 0, 
  wizard[game.current_wizard].col, bar_effect.alpha.current, TEXT_CENTRE);

  gfx[48]->blitAlphaRectFx(0, 0, 255, 48, x - 239, y, 0.0, 1.0, bar_effect.alpha.current); 
  gfx[48]->blitAlphaRectFx(0, 48, 255, 96, x + 16, y, 0.0, 1.0, bar_effect.alpha.current); 
 }
}

// Sprite Code
// ------------

// Clear defaults in one handy function so we don't have to duplicate this elsewhere.
void clear_sprite_data(sprite_t &s)
{
 s.x = 0.0;
 s.y = 0.0;
 s.gfx = 0;
 s.dx = 0.0;
 s.dy = 0.0;
 s.dx_add = 0.0;
 s.dy_add = 0.0;
 s.w = 16;
 s.h = 16;
 s.w_add = 0.0;
 s.h_add = 0.0;
 s.alpha = 1.0;
 s.alpha_add = 0.0;
 s.angle = 0.0;
 s.angle_add = 0.0;
 s.angle_move = true;
 s.update_angle = false;
 s.use_piece_gfx = false;
 s.type = TYPE_PARTICLE;
 s.time = 0;
 s.destroy = false;
 s.rgba = Rgba( 1.0, 1.0, 1.0, 1.0);     
}

// Add sprite to our list of sprites.
void add_sprite(sprite_t &sprite)
{
 sprite.w /= board_info.scale;
 sprite.h /= board_info.scale;
 sprite.w_add /= board_info.scale;
 sprite.h_add /= board_info.scale;
 sprite.dx /= board_info.scale;
 sprite.dy /= board_info.scale;

 sprites.push_back(sprite);
}

void do_sprites_logic(void)
{
 float speed;

 // Start to go through list to do all sprite logic.
 for( list < sprite_t > ::iterator iter = sprites.begin(); iter != sprites.end(); )
 {
  // If sprite has already been marked to be destroyed then remove from list.
  if ( iter->destroy )
  { 
   // Delete the sprite and remove it from the list
   iter = sprites.erase( iter );
	}
  else
  {     
   // Move sprites..

   // Update particles position, alpha and size.
	 if (iter->time <= 0)
	 {
    iter->alpha += iter->alpha_add;

	  iter->x += iter->dx;
    iter->y += iter->dy;
    iter->dx += iter->dx_add;
    iter->dy += iter->dy_add;
    iter->w += iter->w_add;
    iter->h += iter->h_add;

    // Particle is spinning, so update new angle too.
    if (iter->angle_add > 0.0)
    {
     speed = sqrt(iter->dx * iter->dx + iter->dy * iter->dy);   
     iter->angle += iter->angle_add;

		 if (iter->angle_move)
		 {
      iter->dx = cos(iter->angle) * speed;
      iter->dy = sin(iter->angle) * speed;
		 }
	  }

	  if (iter->update_angle)
	  {
     iter->angle = atan2(iter->dy, iter->dx);
	  }

    // Paricles has shrunk, or faded so w or h, or alpha is less than zero. Let's destroy.
    if (iter->w <= 0.0 || iter->h <= 0.0 || iter->alpha <= 0.0 || iter->x > 1280.0 || iter->x + iter->w < 0.0
		|| iter->y > 960.0 || iter->y + iter->h < 0.0) iter->destroy = true;
	 }
	 else
	 {
    iter->time--;
	 }

   iter++;
	}
 }
}

void draw_sprites(void)
{
 for( list < sprite_t > ::iterator iter = sprites.begin(); iter != sprites.end(); )
 {
  // Call appropriate functions.
  if (iter->gfx != -1 && iter->time <= 0)
	{
	 if (!iter->use_piece_gfx)
	 {
	  gfx[iter->gfx]->setBlitColor(iter->rgba.r, iter->rgba.g, iter->rgba.b, iter->rgba.a);
	  if (iter->additive_draw)
	  {
       if (game.opengl) gfx[iter->gfx]->setAlphaMode(GL_SRC_ALPHA, GL_ONE);
	   if (!game.opengl) gfx[iter->gfx]->setAlphaMode(1);  
	  }
	  else
	  {
     gfx[iter->gfx]->setAlphaMode(BLENDER_ALPHA);
	  }

	  if (iter->alpha > 0.0) BlitTransform(gfx[iter->gfx], iter->x, iter->y, iter->w, iter->h, iter->angle, iter->alpha);
	 }
	 else
	 {
    if (iter->alpha > 0.0)
		{
		 SetSolidColour(piece_gfx[iter->gfx], iter->rgba);	
		 piece_gfx[iter->gfx]->setAlphaMode(BLENDER_ALPHA);
		 BlitTransform(piece_gfx[iter->gfx], iter->x, iter->y, iter->w, iter->h, iter->angle, iter->alpha);
		 CancelSolidColour(piece_gfx[iter->gfx]);
		}
	 }
	}
  iter++;
 }

}

void destroy_all_sprites(void)
{
 // Start to go through list.
 for( list < sprite_t > ::iterator iter = sprites.begin(); iter != sprites.end(); )
 {
  // Delete the sprite and remove it from the list
  iter = sprites.erase( iter );
 }
}


int draw_text(char *txt, int x, int x2, int y, int font, int kerning, Rgba col, float alpha, int orient)
{
 int t, s, a, b, st, l, old_x;
 char name[MAX_STRING];

 if (alpha <= 0.0) return 0;
 if (alpha > 1.0) alpha = 1.0;
 
 //if (font != FONT_LARGE) return 0;

 old_x = x;

 fonty[font]->setColor(col.r, col.g, col.b, col.a * alpha);

 if (orient == TEXT_LEFT) fonty[font]->drawStringFromLeft( txt, x, y, kerning);
 if (orient == TEXT_CENTRE) fonty[font]->drawStringCentered( txt, x, x2, y, kerning); 
 if (orient == TEXT_RIGHT) fonty[font]->drawStringFromRight( txt, x2, y, kerning);
 if (orient == TEXT_WRAP) fonty[font]->drawMultiline( txt, x, x2, y, TEXTSTYLE_JUSTIFIED, kerning);


 /*
 if (orient == 99)//TEXT_WRAP) 
 {
  s = 0;
  st = 0;

	// 3 lines..
  for (l = 0 ; l < 3 ; l++) 
	{
	 x = old_x;
   a = 0;
	 b = 0;

	 memset(&name, 0, MAX_STRING);

	 // Find last space in line.
   for (t = st ; t < strlen(txt) ; t++)
	 {
		if (txt[t] == ' ' && t == st) continue; // Skip any odd spaces at start of new line.
    name[a] = txt[t];
		a++;

	  if (txt[t] == ' ' || t == strlen(txt) - 1) 
		{
     if (t == strlen(txt) - 1) b = a + 1;
		 if (x + mainFont[font]->getStringWidth(name, kerning) > x2) break;
		 
		 s = t;
		 b = a;
	  }
	 }
	 name[b] = '\0';

	 // Now draw upto space.
   mainFont[font]->drawStringFromLeft(name, x, y, kerning);

   y += mainFont[font]->getHeightPix() + 2;
	 st = s + 1;

	 if (st >= strlen(txt) - 1) break;
	}
 }
*/
 return strlen(txt) * 16;
}