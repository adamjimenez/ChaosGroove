// Effect (.Cpp)
// -------------

#include <ptk.h>
#include <vector>
#include <list>
#include <sstream>
#include "KPTK.h"

using namespace std;

#include "bridge.hpp"
#include "game.hpp"
#include "log.hpp"
#include "timer.hpp"
#include "scene.hpp"
#include "config.hpp"
#include "board.hpp"
#include "piece.hpp"
#include "wizard.hpp"
#include "panel.hpp"
#include "spell.hpp"
#include "effect.hpp"

int find_effect_by_name(char *name, int def)
{
 int s;

 if (strcmp(name, "EFFECT_MAGIC_ATTACK") == 0) return EFFECT_MAGIC_ATTACK;
 if (strcmp(name, "EFFECT_EXPLODE") == 0) return EFFECT_EXPLODE;
 if (strcmp(name, "EFFECT_ATTACK") == 0) return EFFECT_ATTACK;
 if (strcmp(name, "EFFECT_SPELLCAST") == 0) return EFFECT_SPELLCAST;
 if (strcmp(name, "EFFECT_FIRE") == 0) return EFFECT_FIRE;
 if (strcmp(name, "EFFECT_NONE") == 0) return EFFECT_NONE;

 return def;
}

void do_effect(int effect, int x, int y, int var1, int var2)
{
 if (effect == EFFECT_MAGIC_ATTACK) do_magic_attack_effect(x, y, var1, var2);
 if (effect == EFFECT_ATTACK) do_attack_effect(x, y, var1);
 if (effect == EFFECT_EXPLODE) do_explode_effect(x, y, var1);
 if (effect == EFFECT_FIREWORK) do_firework_effect(x, y, var1);
 if (effect == EFFECT_SPELLCAST) do_spell_casting_effect(x, y, var1);
 if (effect == EFFECT_FIRE) do_fire_effect(x, y, var1);
}

void do_missile_effect(int g, float speed, int x1, int y1, int x2, int y2)
{
 sprite_t s;
 int t, r;
 
 float x, y, angle, angle2, time, dx, dy;

 if (g != 38 && g != 47)
 {
	do_plain_missile_effect(g, speed, x1, y1, x2, y2);
  return;
 }

 clear_sprite_data(s);

 x1 = (x1 * board_info.square_width) + (board_info.square_width / 2) + board_info.start_x;
 y1 = (y1 * board_info.square_height) + (board_info.square_height / 2) + board_info.start_y;
 x2 = (x2 * board_info.square_width) + (board_info.square_width / 2) + board_info.start_x;
 y2 = (y2 * board_info.square_height) + (board_info.square_height / 2) + board_info.start_y;

 x = x2 - x1;
 y = y2 - y1;

 angle = atan2(y, x);
 angle2 = angle;

 speed *= 2.5;
 s.gfx = g;
 s.w = gfx[s.gfx]->getWidth();
 s.h = gfx[s.gfx]->getHeight();
 s.additive_draw = true;

 if (g == 38)
 {
  s.w = s.w * 0.75;
	s.h = s.h * 0.75;
	s.w_add = 0.01;
	s.h_add = 0.01;
 }

 dx = cos(angle) * speed;
 dy = sin(angle) * speed;

 if (ABS(x) > ABS(y))
 {
  time = ABS(x) / ABS(dx);
 }
 else
 {
  time = ABS(y) / ABS(dy);
 }

 time *= board_info.scale;

 x = x1;
 y = y1;
 for (t = 0 ; t < (int)time ; t++)
 {
  for (r = 0 ; r < 4 ; r ++)
	{
  s.x = x;
	s.y = y;
	s.angle = 0;
	s.alpha = 1.0;

	angle = angle2 + FRand(-45.0, 45.0);
  angle = DEG_TO_RAD(angle);
  s.angle = angle; 
 
  speed = FRand(0.15, 0.25);
  s.dx = cos(angle) * speed;
  s.dy = sin(angle) * speed;

	s.time = 0;

	s.angle_add = 0.001;
	s.alpha_add = -0.01;
  
	add_sprite(s);
	}
	x += dx;
	y += dy;
	wait_time(1);

	if (game.exit_key) return;
 }
}


void do_plain_missile_effect(int g, float speed, int x1, int y1, int x2, int y2)
{
 sprite_t s;

 float x, y, angle, time;

 clear_sprite_data(s);

 x1 = (x1 * board_info.square_width) + (board_info.square_width / 2) + board_info.start_x;
 y1 = (y1 * board_info.square_height) + (board_info.square_height / 2) + board_info.start_y;
 x2 = (x2 * board_info.square_width) + (board_info.square_width / 2) + board_info.start_x;
 y2 = (y2 * board_info.square_height) + (board_info.square_height / 2) + board_info.start_y;

 x = x2 - x1;
 y = y2 - y1;

 angle = atan2(y, x);

 s.x = x1;
 s.y = y1;

 s.gfx = g;
 s.w = gfx[s.gfx]->getWidth();
 s.h = gfx[s.gfx]->getHeight();
 s.dx = cos(angle) * speed;
 s.dy = sin(angle) * speed;
 s.angle = angle;

 if (ABS(x) > ABS(y))
 {
  time = ABS(x) / ABS(s.dx);
 }
 else
 {
  time = ABS(y) / ABS(s.dy);
 }

 time *= board_info.scale;
 add_sprite(s);

 wait_time((int)time);

 destroy_all_sprites();
}


void do_spell_casting_effect(int x, int y, int amount)
{
 int t, r;
 float angle, speed;
 sprite_t s;

 for (t = 0 ; t < amount ; t++)
 {
  for (r = 0 ; r < 2 ; r++)
  {
   // sprite details.
   clear_sprite_data(s);

   s.gfx = 19;
	 s.time = t;
	 s.x = (x * board_info.square_width) + (board_info.square_width / 2) + board_info.start_x + FRand(-4, 4);
   s.y = (y * board_info.square_height) + (board_info.square_height / 2) + board_info.start_y + FRand(-4, 4);

   s.w = gfx[s.gfx]->getWidth();
   s.h = gfx[s.gfx]->getHeight();
   s.w_add = FRand(-0.2, -0.3);
   s.h_add = s.w_add;

	 s.alpha = FRand(1.0, 1.0);
   //s.alpha_add = FRand(-0.01, -0.02);
	 s.angle_add = FRand(0.002, 0.008);
   s.additive_draw = true;
  
   angle = FRand(0.0, 360.0);
   angle = DEG_TO_RAD(angle);
   s.angle = angle; 
 
   speed = FRand(0.15, 0.25);
   s.dx = cos(angle) * speed;
   s.dy = sin(angle) * speed;

   add_sprite(s);
  }
 }
}

void do_attack_effect(int x, int y, int times)
{
 int r, t, rx, ry;
 sprite_t s;
 float angle, speed;

 for (t = 0 ; t < times ; t++)
 {
  rx = (Rand() % 9) - 4;
  ry = (Rand() % 9) - 4;
  
  for (r = 0 ; r < 32 ; r++)
  {
   // Particle details.
	 clear_sprite_data(s);

   s.gfx = 18;
	 s.time = t * 50;
	 s.x = (x * board_info.square_width) + (board_info.square_width / 2) + board_info.start_x + rx;
   s.y = (y * board_info.square_height) + (board_info.square_height / 2) + board_info.start_y + ry;

   s.w = gfx[s.gfx]->getWidth();
   s.h = gfx[s.gfx]->getHeight();

   s.w_add = FRand(-0.25, -0.5);
 
   angle = FRand(0.0, 360.0);

   angle = DEG_TO_RAD(angle);
   s.angle = angle; 
 
   speed = FRand(1.0, 2.0);
   s.dx = cos(angle) * speed;
   s.dy = sin(angle) * speed;
	 s.additive_draw = true;
   
   add_sprite(s);
  }
 }

}

void do_explode_effect(int x, int y, int amount)
{
 int r, t, rx, ry;
 sprite_t s;
 float angle, speed;

  rx = (Rand() % 9) - 4;
  ry = (Rand() % 9) - 4;
  
  for (r = 0 ; r < amount ; r++)
  {
   // Particle details.
	 clear_sprite_data(s);

   s.gfx = 40;
	 s.x = (x * board_info.square_width) + (board_info.square_width / 2) + board_info.start_x + rx;
   s.y = (y * board_info.square_height) + (board_info.square_height / 2) + board_info.start_y + ry;

   s.w = gfx[s.gfx]->getWidth();
   s.h = gfx[s.gfx]->getHeight();

   s.w_add = FRand(-0.2, -0.4);
   //s.alpha_add = -0.01;

   angle = FRand(0.0, 360.0);

   angle = DEG_TO_RAD(angle);
   s.angle = angle; 
 
   speed = FRand(1.0, 2.0);
   s.dx = cos(angle) * speed;
   s.dy = sin(angle) * speed;
	 //s.dy_add = 0.01;
	 s.additive_draw = true;
	 s.update_angle = true;
   
   add_sprite(s);
  }
}

void do_magic_attack_effect(int x, int y, int time, int g)
{
 int r, t, rx, ry;
 sprite_t s;
 float angle, speed;

 for (t = 0 ; t < time ; t++)
 {
  rx = (Rand() % 9) - 4;
  ry = (Rand() % 9) - 4;
  
  for (r = 0 ; r < 2 ; r++)
  {
   // Particle details.
	 clear_sprite_data(s);

   s.gfx = g;
	 s.time = t * 3;
	 s.x = (x * board_info.square_width) + (board_info.square_width / 2) + board_info.start_x + rx;
   s.y = (y * board_info.square_height) + (board_info.square_height / 2) + board_info.start_y + ry;

   s.w = gfx[s.gfx]->getWidth();
	 s.w = 1;
   s.h = gfx[s.gfx]->getHeight();
   s.h_add = -0.5;
	 s.w_add = 1.0;
	 s.alpha_add = -0.005;
   //s.w_add = FRand(-0.15, -0.25);
 
   angle = FRand(0.0, 360.0);

   angle = DEG_TO_RAD(angle);
   s.angle = angle; 
 
   speed = 0.5;//FRand(0.25, 0.5);
   s.dx = cos(angle) * speed;
   s.dy = sin(angle) * speed;
	 s.additive_draw = true;
   
   add_sprite(s);
  }
 }
}

void do_fire_effect(int x, int y, int times)
{
 int r, t, rx, ry;
 sprite_t s;
 float angle, speed;

 for (t = 0 ; t < times ; t++)
 {
  rx = IRand(-8, 8);
  ry = IRand(-8, 8);

  for (r = 0 ; r < 4 ; r++)
  {
   // Particle details.
	 clear_sprite_data(s);

   s.gfx = 38;
	 s.time = t * 3;
	 s.x = (x * board_info.square_width) + (board_info.square_width / 2) + board_info.start_x + rx;
   s.y = (y * board_info.square_height) + (board_info.square_height / 2) + board_info.start_y + ry;

   s.w = gfx[s.gfx]->getWidth();
   s.h = gfx[s.gfx]->getHeight();

	 angle = FRand(-0.2, -0.45);
   s.w_add = angle;
	 s.h_add = angle;
 
   angle = FRand(0.0, 360.0);

   angle = DEG_TO_RAD(angle);
   s.angle = angle;
	 s.angle_add = FRand(-0.005, 0.005);
 
   speed = FRand(0.15, 0.4);

   s.dx = cos(angle) * speed;
   s.dy = sin(angle) * speed;
	 s.additive_draw = true;
	 s.angle_move = false;

   add_sprite(s);
  }
 }

}

void do_wizard_dying_effect(int x, int y, int times)
{
 int r, t, rx, ry;
 sprite_t s;
 float angle, speed;

 // Stop animation.
 board[x][y][PIECE].gfx_frames = 1;

 for (t = 0 ; t < times ; t++)
 {
  //rx = IRand(-8, 8);
  //ry = IRand(-8, 8);

  for (r = 0 ; r < 4 ; r++)
  {
   // Particle details.
	 clear_sprite_data(s);

   s.gfx = board[x][y][PIECE].gfx;
	 s.use_piece_gfx = true;
	 s.time = t * 4;
	 s.x = (x * board_info.square_width) + (board_info.square_width / 2) + board_info.start_x;
   s.y = (y * board_info.square_height) + (board_info.square_height / 2) + board_info.start_y;

   s.w = 64;
   s.h = 64;

   angle = t * 6 + (90 * r) % 360;
	 angle = DEG_TO_RAD(angle);
   speed = 4.0;

   s.dx = cos(angle) * speed;
   s.dy = sin(angle) * speed;
	 s.rgba = wizard[t % 8].col;
	 s.alpha = 1.0;
	 s.additive_draw = true;
	 s.angle_move = false;

   add_sprite(s);
  }
 }

}

void do_firework_effect(int x, int y, int amount)
{
 int r, t, rx, ry;
 sprite_t s;
 float angle, speed;
  
  for (r = 0 ; r < amount ; r++)
  {
   // Particle details.
	 clear_sprite_data(s);

   s.gfx = 49;
	 s.x = x;
   s.y = y;

   s.w = gfx[s.gfx]->getWidth();
   s.h = gfx[s.gfx]->getHeight();

   s.w_add = FRand(-0.25, -0.4);
   //s.alpha_add = -0.01;

   angle = FRand(0.0, 360.0);

   angle = DEG_TO_RAD(angle);
   s.angle = angle; 
 
   speed = FRand(3.0, 4.0);
   s.dx = cos(angle) * speed;
   s.dy = sin(angle) * speed;
	 s.dx_add = -(s.dx / 270);
	 s.dy_add = -(s.dy / 270);
	 //s.dy_add = 0.01;
	 s.additive_draw = true;
	 s.update_angle = true;
	 s.rgba = wizard[game.current_wizard].col;
   
   add_sprite(s);
  }
}