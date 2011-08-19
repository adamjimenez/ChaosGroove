// Options (.Cpp)
// -------------

// Options code for dealing with menus and other variables. Needs better checking to stop partial option text being read. i.e. needs to check for < & > around text.

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
#include "effect.hpp"
#include "options.hpp"

struct option_t option[MAX_OPTIONS];
struct menu_info_t menu_info;

bool load_options(void)
{
 char text[MAX_STRING], file[MAX_STRING];

 sprintf(text, "\\text_files\\game_options.ini");
 sprintf(file, "%s", KMiscTools::makeFilePath(text));
 
 if (!set_config_file_new(CONFIG_OPTIONS, file, false))
 {
	log("Can't open game options config file!");
  return false;
 }

 sprintf(text, "\\text_files\\menus.ini");
 sprintf(file, "%s", KMiscTools::makeFilePath(text));
 
 if (!set_config_file_new(CONFIG_MENUS, file, false))
 {
	log("Can't open game options config file!");
  return false;
 }

 sprintf(text, "\\text_files\\wizard_options.ini");
 sprintf(file, "%s", KMiscTools::makeFilePath(text));
 
 if (!set_config_file_new(CONFIG_WIZARDS, file, false))
 {
	log("Can't open game options config file!");
  return false;
 }

 sprintf(text, "\\text_files\\sounds.ini");
 sprintf(file, "%s", KMiscTools::makeFilePath(text));
 
 if (!set_config_file_new(CONFIG_SOUNDS, file, false))
 {
	log("Can't open sounds config file!");
  return false;
 }

 // Now load a name list
 sprintf(text, "\\text_files\\wizard_names\\funk.ini");
 sprintf(file, "%s", KMiscTools::makeFilePath(text));
 if (!set_config_file_new(CONFIG_WIZARD_NAMES, file, false))
 {
  log("Can't open wizard names!");
 }

 // Load Scrolly text
 sprintf(text, "\\text_files\\Scrolly.ini");
 sprintf(file, "%s", KMiscTools::makeFilePath(text));
 if (!set_config_file_new(CONFIG_SCROLLY, file, false))
 {
  log("Can't open Scrolly text!");
 }
 menu_info.scrolly_pos = 0;

 return true;
}

void save_options(void)
{
 write_config_file(CONFIG_OPTIONS);
 write_config_file(CONFIG_WIZARDS);
}

void setup_options(int cfg, char *section)
{
 int w;

 sprintf(menu_info.section_name, "%s", section);
 menu_info.config = cfg;
 menu_info.alpha.current = 0.0;
 menu_info.alpha.target = 1.0;
 menu_info.alpha.speed = 0.02;

 //log("config: %d, section_name: %s", menu_info.config, menu_info.section_name);
}

void fade_out_options(void)
{
 menu_info.alpha.target = 0.0;
}

int find_option(int cfg, char *option)
{
 char *pos, *opt;
 char result[80];
 int size;

 // Find option by name
 pos = strstr( config[cfg].data, option );
 if (pos == NULL) return -1;

 // Now go backwards until we find the ']' of the preceding section name
 do
 {
  pos--;
 } while ( *pos != ']' );

 size = 0;
 pos--;

 do
 {
  pos--;
	size++;
 } while ( *pos != '_' );
 
 pos++;

 memset(result , 0 , 80 ) ;
 memcpy(result, pos, size);
 result[size + 1] = 0;

 return atoi(result);
}

bool check_option_choice(int cfg, char *section, char *option, char *choice)
{
 int o, c;
 char name[MAX_STRING], name2[MAX_STRING], text[MAX_STRING];

 o = find_option(cfg, option);
 if (o == -1) return false;

 sprintf(name, "%s_%d", section, o);
 c = GetConfigInt(cfg, name, "OPTION_CURRENT_CHOICE", -1);
 if (c == -1) return false;

 sprintf(name2, "OPTION_CHOICE_%d", c);
 GetConfigString(cfg, name, name2, text, MAX_STRING);
 if (strcmp(text, choice) == 0) return true;

 return false;
}

int check_option_choice_int(int cfg, char *section, char *option, int def)
{
 int o, c;
 char name[MAX_STRING], name2[MAX_STRING], text[MAX_STRING];

 o = find_option(cfg, option);
 if (o == -1) return def;

 sprintf(name, "%s_%d", section, o);
 c = GetConfigInt(cfg, name, "OPTION_CURRENT_CHOICE", -1);
 if (c == -1) return def;

 sprintf(name2, "OPTION_CHOICE_%d", c);
 GetConfigString(cfg, name, name2, text, MAX_STRING);
 
 return atoi(text);
}

int find_current_option_choice(int cfg, char *section, char *option)
{
 int o, c;
 char name[MAX_STRING], name2[MAX_STRING], text[MAX_STRING];

 o = find_option(cfg, option);
 if (o == -1) return -1;

 sprintf(name, "%s_%d", section, o);
 c = GetConfigInt(cfg, name, "OPTION_CURRENT_CHOICE", -1);
 
 return c;

}

bool find_option_choice_variables(int cfg, char *section, char *option, int *var1, int *var2, int *var3, char *text_var1)
{
 int o, c;
 char name[MAX_STRING], text[MAX_STRING];

 o = find_option(cfg, option);
 if (o == -1) return false;

 sprintf(name, "%s_%d", section, o);
 c = GetConfigInt(cfg, name, "OPTION_CURRENT_CHOICE", -1);
 if (c == -1) return false;

 sprintf(text, "OPTION_CHOICE_%d_VAR1", c);
 if (var1) *var1 = GetConfigInt(cfg, name, text, -1);
 
 sprintf(text, "OPTION_CHOICE_%d_VAR2", c);
 if (var2) *var2 = GetConfigInt(cfg, name, text, -1);

 sprintf(text, "OPTION_CHOICE_%d_VAR3", c);
 if (var3) *var3 = GetConfigInt(cfg, name, text, -1);

 sprintf(text, "OPTION_CHOICE_%d_TEXT_VAR1", c);
 if (text_var1) GetConfigString(cfg, name, text, text_var1, MAX_STRING);
}

void do_options(bool draw)
{
 int o, c, x, y, w, gap_x, choices, r, g, b, choice, allign, font, exit;
 char text[MAX_STRING], name[MAX_STRING], name2[MAX_STRING], action[MAX_STRING];
 Rgba col;

 sprintf(name, "%s_EXIT", menu_info.section_name);
 exit = GetConfigInt(menu_info.config, name, "EXIT", -1); // Font
 
 if (!draw && game.exit_key && game.old_exit_key) return;

 if (!draw && !game.exit_key)
 {
  do_alpha_logic(&menu_info.alpha);
	menu_info.scrolly_pos += 0.25;

  if (mouse.old_left_click) return;
  if (!mouse.left_click) return;
 }

 //log("game.exit_key: %d", game.exit_key);
 if (draw)
 {
  gfx[43]->stretchAlphaRect(1, 1, gfx[43]->getWidth() - 1, gfx[43]->getHeight() - 1, 0, 0, 1280, 960);

	draw_scrolly();

	if (strcmp(menu_info.section_name, "TITLE") == 0)
	{
	 // Chaos Logo
	 gfx[44]->blitAlphaRectFx(0, 0, 512, 255, 196, 48, 0.0, 1.0, menu_info.alpha.current);
   gfx[44]->blitAlphaRectFx(112, 256, 512, 512, 196 + 512, 48, 0.0, 1.0, menu_info.alpha.current);
 
	 // Groove Logo
	 x = 396;

	 for (o = 0 ; o < 3 ; o++)
	 {
	  gfx[45]->blitAlphaRectFx(o * 32, 0, (o * 32) + 32, 24, (o * 96) + x, 312, 0.0, 1.0, menu_info.alpha.current);
	  gfx[45]->blitAlphaRectFx(o * 32, 28, (o * 32) + 32, 52, ((o + 3) * 96) + x, 312, 0.0, 1.0, menu_info.alpha.current);
	 }

	 // Author text
	 draw_text("a game by richard phipps", 0, 1280, 890, FONT_SMALL, 1, Rgba(0.0, 1.0, 1.0), menu_info.alpha.current, TEXT_CENTRE);
	}
 }

 for (o = 0 ; o < MAX_OPTIONS ; o++)
 {
  sprintf(name, "%s_%d", menu_info.section_name, o);

	// Get font
	font = GetConfigInt(menu_info.config, name, "OPTION_FONTSIZE", 0); // Font
	x = GetConfigInt(menu_info.config, name, "OPTION_X", 2) * 16; // X Position
	y = (GetConfigInt(menu_info.config, name, "OPTION_Y", -1) * 16) + 1; // Y Position

	if (y <= 0) continue; // Skip unused options..

	// Find colours
	r = GetConfigInt(menu_info.config, name, "OPTION_R", 255);
	g = GetConfigInt(menu_info.config, name, "OPTION_G", 255);
	b = GetConfigInt(menu_info.config, name, "OPTION_B", 255);

	// Find allignment
	allign = TEXT_LEFT;
  GetConfigString(menu_info.config, name, "OPTION_ALLIGN", text, MAX_STRING);
  if (strcmp(text, "CENTRE") == 0) allign = TEXT_CENTRE;
  if (strcmp(text, "RIGHT") == 0) allign = TEXT_RIGHT;
  if (strcmp(text, "WRAP") == 0) allign = TEXT_WRAP;

	// Find Option text
  GetConfigString(menu_info.config, name, "OPTION", text, MAX_STRING);

	// Find width and real x position based on text allignment
  w = strlen(text) * 16;
  if (allign == TEXT_CENTRE)
	{
	 x = 640 - (w / 2);
	 allign = TEXT_LEFT;
	}

	if ((!draw && mouse.x - 16 > x && mouse.x - 16 < x + w && mouse.y - 12 > y && 
	mouse.y - 12 < y + 16) || (!draw && game.exit_key && !game.old_exit_key && exit == o))
	{
	 // Clicked on button, so store button Action in menu.
	 GetConfigString(menu_info.config, name, "OPTION_ACTION", menu_info.action, MAX_STRING);
	 GetConfigString(menu_info.config, name, "OPTION_ACTION_MENU_NAME", menu_info.new_section_name, MAX_STRING);
	 menu_info.new_config = GetConfigInt(menu_info.config, name, "OPTION_ACTION_MENU_CONFIG", -1);
	 menu_info.var1 = GetConfigInt(menu_info.config, name, "OPTION_ACTION_VAR1", -1);
	 menu_info.var2 = GetConfigInt(menu_info.config, name, "OPTION_ACTION_VAR2", -1);
	 menu_info.option_clicked = o;
	}

	choice = GetConfigInt(menu_info.config, name, "OPTION_CURRENT_CHOICE", 0);
  choices = GetConfigInt(menu_info.config, name, "OPTION_CHOICE_X", -1);

	if (draw)
	{
	 GetConfigString(menu_info.config, name, "OPTION_ACTION", action, MAX_STRING);

	 // Draw highlighting over option if our mouse is over it and it has an action..
	 if (mouse.x - 16 > x && mouse.x - 16 < x + w && mouse.y - 12 > y && mouse.y - 12 < y + 16 && choices == -1
	 && strlen(action) > 0)
	 {
	  gfx[46]->setAlphaMode(0);
	  gfx[46]->setBlitColor(r, g, b, menu_info.alpha.current);
		gfx[46]->allowTextureWrap(true);
    gfx[46]->stretchAlphaRect(0, 1, 16, 16, x, y, x + w, y + 21);
	 }
	 draw_text(text, x, 1248, y, font, 0, Rgba(r, g, b), menu_info.alpha.current, allign);
	}
	
	// CHOICES
	// -------

	// Get x pos
	x = GetConfigInt(menu_info.config, name, "OPTION_CHOICE_X", 32) * 16;
	gap_x = GetConfigInt(menu_info.config, name, "OPTION_CHOICE_GAP_X", 10) * 16;

	for (c = 0 ; c < MAX_CHOICES ; c++)
	{
   sprintf(name2, "OPTION_CHOICE_%d", c);
   GetConfigString(menu_info.config, name, name2, text, MAX_STRING);

	 if (strlen(text) == 0) break; // No more choices.

   col = Rgba(0.5, 0.5, 0.5);
   if (c == choice) col = Rgba(1.0, 1.0, 1.0);
	 w = strlen(text) * 16;

	 if (draw)
	 {
		// Draw highlighting over option..
	  if (mouse.x - 16 > x && mouse.x - 16 < x + w && mouse.y - 12 > y && mouse.y - 12 < y + 16 && c != choice)
	  {
	  gfx[46]->setAlphaMode(0);
	  gfx[46]->setBlitColor(1.0, 1.0, 1.0, menu_info.alpha.current);
		gfx[46]->allowTextureWrap(true);
    gfx[46]->stretchAlphaRect(0, 1, 16, 16, x, y, x + w, y + 21);
	  }
    
		draw_text(text, x, 1280, y, font, 0, col, menu_info.alpha.current, TEXT_LEFT);
	 }

	 // Clicked on an option choice?
	 if (!draw && mouse.x - 16 > x && mouse.x - 16 < x + w && mouse.y - 12 > y && 
	 mouse.y - 12 < y + 16 && c != choice)
	 {
	  // Ok, change option choice to current one.
	  sprintf(text, "%d", c);
	  change_config_text(menu_info.config, name, "OPTION_CURRENT_CHOICE", text);
	 }

	 if (gap_x > 0)
	 {
    x += w + gap_x;
	 }
	 else
	 {
    y += 24;
	 }
	}
 }
}

void draw_scrolly(void)
{
 char text[81];
 char *pos;
 int c, s, offset;

 s = (int)((menu_info.scrolly_pos / 16)) % config[CONFIG_SCROLLY].length;
 offset = (int)(menu_info.scrolly_pos) % 16;

 pos = config[CONFIG_SCROLLY].data;
 for (c = 0 ; c < 81 ; c++)
 {
  text[c] = pos[s];
	s++;
	if (s > config[CONFIG_SCROLLY].length) s--;
 }

 draw_text(text, 0 - offset, 1280, 930, FONT_LARGE, 0, Rgba(1.0, 1.0, 1.0), 1.0, TEXT_LEFT);
}