// Chat (.Cpp)
// -----------

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
#include "chat.hpp"

#include "net.hpp"

struct chat_t chat;
struct chat_line_t chat_line[MAX_CHAT_LINES];
struct user_t user[MAX_USERS];

void setup_users(void)
{
 int u;

 for (u = 0 ; u < MAX_USERS ; u++)
 {
  sprintf(user[u].name, "\n");
  user[u].id = -1;
 }
 chat.num_users = 0;
}
/*
int find_free_user_spot(void)
{
 int u;

 for (u = 1 ; u < MAX_USERS ; u++)
 {
  if (user[u].id == -1)
	{
	 return u;
	}
 }
 return -1;
}

void add_user(char *name, int u)
{
 char str[512];
 int m;

 if (user[u].id != -1) return; // Already been added.

 sprintf(str, "%s has connected.", name);
 add_chat_line(str, Rgba(1.0, 1.0, 1.0));

 // Strip position byte from name..
 for (m = 0 ; m < strlen(name) ; m++)
 {
  name[m] = name[m + 1];
 }
 name[m + 1] = 0;

 sprintf(user[u].name, "%s", name);
 user[u].id = u;
 chat.num_users++;
}

void remove_user(int id)
{
 int u, s;
 char str[512];

 if (user[id].id == -1) return; // Already been removed.

 //sprintf(str, "Removing : %s", user[id].name);
 //add_chat_line(str);

 sprintf(str, "%s has disconnected.", user[id].name);
 add_chat_line(str, Rgba(1.0, 1.0, 1.0));

 user[id].id = -1;
 chat.num_users--;
}
*/
void setup_chat_lines(void)
{
 int l, s;

 chat.mode = CHAT_MODE_SERVER_QUESTION;

 for (l = 0 ; l < MAX_CHAT_LINES ; l++)
 {
	for (s = 0 ; s < strlen(chat_line[l].text) ; s++)
	{
   chat_line[l].text[s] = '\n';
	}
 }
 chat.line = 0;

 //add_chat_line("Chat Test..", Rgba(1.0, 1.0, 1.0));
 //add_chat_line("Server? Y/N", Rgba(1.0, 1.0, 1.0));
}

void add_chat_line(char *text, Rgba rgba)
{
 int l;

 if (game.debug_to_chat) log("%s", text);

 if (chat.line < MAX_CHAT_LINES)
 {
  sprintf(chat_line[chat.line].text, text);
	chat_line[chat.line].rgba = rgba;
	chat.line++;
	return;
 }

 // Ok, have to shift old lines up..
 for (l = 1 ; l < MAX_CHAT_LINES ; l++)
 {
  sprintf(chat_line[l - 1].text, "%s", chat_line[l].text);
	chat_line[l - 1].rgba = chat_line[l].rgba;
 }

 sprintf(chat_line[MAX_CHAT_LINES - 1].text, text);
 chat_line[MAX_CHAT_LINES - 1].rgba = rgba;
}
/*

void do_chat_logic(void)
{
 int k, key, pressed;
 char str[512];

 //if (Key(9)) RestoreWindow();
 if (!CheckWindowFocus()) return; // Not active window..

 chat.input_time++;
 if (chat.input_time > 7)
 {
  chat.input_time = 0;
	Sleep(1);
 }

 pressed = -1;
 for (k = 0 ; k < 256 ; k++)
 {
  key = Key(k);
  if (key && chat.old_key[k] == 0)
	{
	 pressed = k;
	 if (k != 22) chat.old_key[k] = 50;
	 if (k == 22) chat.old_key[k] = 25;
	}
	if (chat.old_key[k] > 0) chat.old_key[k]--;
 }
 if (pressed == -1) return;
 //log("pressed: %d", pressed);

 key = -1;

 if (pressed >= 25 && pressed <= 50) key = pressed + 72; // a - z
 if (pressed >= 25 && pressed <= 50 && ( Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT) ) ) key = pressed + 40; // A - Z

 if (pressed >= 51 && pressed <= 60) key = pressed - 3; // 0 - 9
 if (pressed == 52 && (Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT)) ) key = 33; // !
 if (pressed == 53 && (Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT)) ) key = 34; // "
 if (pressed == 54 && (Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT)) ) key = 35; // #
 if (pressed == 55 && (Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT)) ) key = 36; // $
 if (pressed == 56 && (Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT)) ) key = 37; // %
 if (pressed == 57 && (Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT)) ) key = 94; // ^
 if (pressed == 58 && (Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT)) ) key = 38; // &
 if (pressed == 59 && (Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT)) ) key = 42; // *
 if (pressed == 60 && (Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT)) ) key = 40; // (
 if (pressed == 51 && (Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT)) ) key = 41; // )

 if (pressed == 4) key = 32; // Space

 if (pressed == 86) key = 46; // .
 if (pressed == 84) key = 44; // ,
 if (pressed == 86 && (Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT)) ) key = 62; // >
 if (pressed == 84 && (Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT)) ) key = 60; // <

 if (pressed == 89) key = 91; // [
 if (pressed == 91) key = 93; // ]
 if (pressed == 89 && (Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT)) ) key = 123; // {
 if (pressed == 91 && (Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT)) ) key = 125; // }

 if (pressed == 85) key = 45; // -
 if (pressed == 83) key = 61; // =
 if (pressed == 85 && (Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT)) ) key = 95; // _
 if (pressed == 83 && (Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT)) ) key = 43; // +

 if (pressed == 82) key = 59; // ;
 if (pressed == 88) key = 39; // '
 if (pressed == 92) key = 35; // #
 if (pressed == 82 && (Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT)) ) key = 58; // :
 if (pressed == 88 && (Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT)) ) key = 64; // @
 if (pressed == 92 && (Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT)) ) key = 126; // ~

 if (pressed == 87)
 {
  key = 47; // '/'
  if (Key(K_VK_L_SHIFT) || Key(K_VK_R_SHIFT)) key = 63; // '?'
 }
 if (pressed == 22 && chat.char_pos > 0)
 {
  // Backspace..
	chat.text[chat.char_pos - 1] = 0;
	chat.char_pos--;
 }

 if (key != -1 && chat.char_pos < 60)
 {
  chat.text[chat.char_pos] = key;
	//log("key: %d, pressed: %d", key, pressed);
	chat.char_pos++;
 }
 if (pressed == 7)
 {
  // Return..
	chat.text[chat.char_pos] = '\n';

  sprintf(str, "%s : %s", chat.name, chat.text);

  add_chat_line(chat.text, Rgba(1.0, 1.0, 1.0));
	do_chat_question_logic();
	
	for (k = 0 ; k < strlen(chat.text) ; k++)
  {
   chat.text[k] = '\n';
  }

	if (chat.mode == CHAT_MODE_CHATTING)
	{
	 //
_send_message(str, MESSAGE_CHAT);
	}

	chat.char_pos = 0;
 }
}

void do_chat_question_logic(void)
{
 bool server = false;

 if (chat.mode == CHAT_MODE_CHATTING) return;

 if (chat.mode == CHAT_MODE_SERVER_QUESTION)
 {
  if (chat.text[0] == 'y' || chat.text[0] == 'Y') server = true;

  //net_setup(server, true);
  chat.mode = CHAT_MODE_NAME_QUESTION;

	add_chat_line("", Rgba(1.0, 1.0, 1.0));
	if (server) add_chat_line("SERVER MODE", Rgba(1.0, 1.0, 1.0));
	if (!server) add_chat_line("CLIENT MODE", Rgba(1.0, 1.0, 1.0));
	add_chat_line("", Rgba(1.0, 1.0, 1.0));
	add_chat_line("Please enter your username..", Rgba(1.0, 1.0, 1.0));
  return;
 }
 if (chat.mode == CHAT_MODE_NAME_QUESTION)
 {
  sprintf(chat.name, "%s", chat.text);
	
	//if (net.server)
	{
	 //net_setup(true, false);
	 chat.mode = CHAT_MODE_CHATTING;
	 return;
	}

	chat.mode = CHAT_MODE_IP_QUESTION;

	add_chat_line("", Rgba(1.0, 1.0, 1.0));
	add_chat_line("Enter server IP or hit enter for 127.0.0.1", Rgba(1.0, 1.0, 1.0));
	return;
 }

 if (chat.mode == CHAT_MODE_IP_QUESTION)
 {
	if (chat.text[0] == 0)
	{
	 //sprintf(net.IP, "127.0.0.1");
	}
	else
	{
  // sprintf(net.IP, chat.text);
	}
  add_chat_line(net.IP, Rgba(1.0, 1.0, 1.0));

	//net_setup(net.server, false);
	chat.mode = CHAT_MODE_CHATTING;
	return;
 }
}

void draw_chat_text(void)
{
 int l, x, y, user_y;

 y = ScreenHeight() - 24;

 ClearScreen();
 gfx[0]->drawRect(0, y, ScreenWidth(), ScreenHeight(), 0.0, 0.0, 0.5, 1.0);
 
 x = ScreenWidth() - 140;
 gfx[0]->drawRect(x, 0, ScreenWidth(), y, 0.25, 0.0, 0.25, 1.0);

 user_y = 0;
 for (l = 0 ; l < MAX_USERS ; l++)
 {
  if (user[l].id != -1)
	{
   draw_text(user[l].name, x, ScreenWidth(), user_y, FONT_SMALL, 0, Rgba(1.0, 1.0, 1.0), 1.0, TEXT_LEFT);
	 user_y += 20;
	}
 }

 for (l = 0 ; l < MAX_CHAT_LINES ; l++)
 {
  draw_text(chat_line[l].text, 0, ScreenWidth(), l * 20, FONT_SMALL, 0, Rgba(1.0, 1.0, 1.0), 1.0, TEXT_LEFT);
 }

 draw_text(chat.text, 0, ScreenWidth(), y, FONT_SMALL, 0, Rgba(1.0, 1.0, 1.0), 1.0, TEXT_LEFT);
}
*/