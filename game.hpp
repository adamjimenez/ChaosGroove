// Game (.Hpp)
// -----------

#ifndef __Game_hpp__
#define __Game_hpp__ 

#define GAME_NAME                   "Chaos Groove"
#define GAME_VERSION                "0.9"
#define GAME_AUTHOR                 "Richard Phipps"
#define GAME_DATE                   "10.04.07"
#define GAME_LAUNCH_DATE            "Unknown"

#define MAX_STRING									512

// The different parts of the game.
#define GAME_TITLE						0
#define GAME_INGAME						1
#define GAME_MENUS						2
#define GAME_EXIT							3

// The three phases in the ingame part.
#define PHASE_SPELLSELECT           0
#define PHASE_SPELLCAST             1
#define PHASE_MOVEMENT              2

struct game_t
{
 bool release_version, opengl, editor_mode, exit, exit_key, old_exit_key, end_turn, AI_debug, debug_to_chat, show_fps;
 bool casting_a_spell, done_spell_chance;
 int current_wizard, num_players, winner;
 int stage, phase, turn, balance;
};

struct screen_t
{
 int w, h, desktop_w, desktop_h;
 bool dx, windowed;
};

extern struct game_t game;
extern struct screen_t screen;

bool first_time_setup(void);
bool setup_display(void);
bool setup_fonts(void);
void add_font_letters(struct KFont *table, int place, int c, int x, int y, int w, int h, int w2, int h2, int num);
bool load_all_images(void);
void do_exit_cleanup(void);

void wait_time(int amount);
int loop(void);
void ingame_loop(void);
void menu_loop(void);

void check_for_exit_key(void);
void do_growing_and_disappearing(void);

void do_victory_loop(void);

#endif