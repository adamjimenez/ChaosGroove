// Scene (.Hpp)
// ------------

#ifndef __Scene_hpp__
#define __Scene_hpp__ 

extern std::vector <KGraphic *> gfx;
extern std::vector <KGraphic *> piece_gfx;
extern std::vector <KGraphic *> spell_icon_gfx;

#define MAX_COLS               16

#define MAX_FONTS							 4

extern KText *fonty[MAX_FONTS];

#define TYPE_EMITTER           0
#define TYPE_PARTICLE          1

#define FONT_LARGE					   0
#define FONT_SMALL						 1

#define TEXT_LEFT							 0
#define TEXT_CENTRE						 1
#define TEXT_RIGHT						 2
#define TEXT_WRAP							 3

#define GFX_FRONTBOARD				 2

// Code to use for rotations.
#ifndef M_PI
#define M_PI 3.14
#endif

// Handy defines to convert between degrees and radians.
#define DEG_TO_RAD(x) ((M_PI/180.0)*x)
#define RAD_TO_DEG(x) ((180.0/M_PI)*x)

// Sprite Template. This is the basic information that each 'sprite' can hold.
// there's quite a bit altogether..
typedef struct sprite_st
{
 float x, y, dx, dy, dx_add, dy_add, alpha, alpha_add, angle, angle_add, w, h, w_add, h_add;
 int gfx, type, time;
 Rgba rgba;
 bool additive_draw, destroy, update_angle, angle_move, use_piece_gfx;
} sprite_t;


// Alpha struct template for alpha fade in/out effects.
struct alpha
{
 float current;
 float target;
 float speed;
};

struct mouse_t
{
 float x, y;
 bool old_left_click, old_right_click, left_click, right_click, spell_is_illusion;
 int over_board_x, over_board_y, old_over_board_x, old_over_board_y, 
 over_spell_icon, old_over_spell_icon, over_control_icon;
 struct alpha alpha;
 Rgba rgba;
};

#define BAR_MODE_NEWTURN                 0
#define BAR_MODE_WINNER                  1
#define BAR_MODE_DRAW                    2

struct bar_effect_t
{
 struct alpha alpha;
 char text[512];
 int mode;
};

extern struct bar_effect_t bar_effect;

extern KFont largefontTablePtr[256];
extern KFont smallfontTablePtr[256];

extern list < sprite_t > sprites;
extern struct mouse_t mouse;

void do_alpha_logic(struct alpha *alpha_pointer);

void do_scene_logic(void);
void draw_scene(void);
void refresh_screen(void);

void do_mouse_logic(void);
void draw_mouse(void);

void draw_bar_effect(void);

void clear_sprite_data(sprite_t &s);
void add_sprite(sprite_t &sprite);
void do_sprites_logic(void);
void draw_sprites(void);
void destroy_all_sprites(void);

int draw_text(char *txt, int x, int x2, int y, int font, int kerning, Rgba col, float alpha, int orient);

#endif
