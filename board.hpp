// Board (.HPP)

#ifndef __Board_hpp__
#define __Board_hpp__ 

// Maximum sizes for maps and levels.
#define MAX_BOARD_WIDTH         80
#define MAX_BOARD_HEIGHT        80
#define MAX_BOARD_LAYERS				5

// Defines for the 3 different board layers

// The BODY layer stores all body details and is used for raise dead.
// The PIECE layer stores all the normal piece info.
// The MOUNTED layer stores details of wizards in trees, castes or riding mounts.
// Using a seperate layer lets us add creatures on mounts in the future.

#define NO_LAYER				 -1
#define BODY              0
#define PIECE             1
#define MOUNTED           2
#define PIECE_COVERED			3
#define MOUNTED_COVERED		4

#define HIGHLIGHT_GFX_BOX								 12
#define HIGHLIGHT_GFX_SPELL							 14
#define HIGHLIGHT_GFX_FLY								 30

#define HIGHLIGHT_NONE                   0
#define HIGHLIGHT_CAN_CAST               1
#define HIGHLIGHT_CAN_MOVE               2
#define HIGHLIGHT_NO_MOVES_LEFT          3
#define HIGHLIGHT_CAN_ATTACK             4
#define HIGHLIGHT_CAN_RIDE							 5
#define HIGHLIGHT_ENEMY									 6
#define HIGHLIGHT_OUR_PIECE							 7
#define HIGHLIGHT_OUR_PIECE_MOVED				 8
#define HIGHLIGHT_GREY									 9

struct board_info_t
{
 int board_width, board_height;
 int square_width, square_height;
 int start_x, start_y, border_size;
 
 int selected_x, selected_y, selected_layer;
 bool selected_state, selected_engaged;
 struct alpha selected_alpha;
 
 int current_x, current_y;

 float highlight_cycle, scale;
 int highlight_type;
 int highlight_x;
 int highlight_y;
 struct alpha highlight_alpha;

 struct alpha grid_alpha;

 float cloud_x;
 float cloud_y;   
};

struct highlight_data
{
 Rgba rgba;
 struct alpha alpha;
 bool explored;
 float move;
 int gfx, type, from_x, from_y, AI_score;
};

extern struct board_info_t board_info;

// Our board is constructed out of piece data structs directly.
// This lets us directly manipulate the data on the board which we would not be able
// to do if we used an ID to refer to a piece.
extern struct piece_data board[MAX_BOARD_WIDTH][MAX_BOARD_HEIGHT][MAX_BOARD_LAYERS];

// Now we have our highlight board data.
extern struct highlight_data highlight_board[MAX_BOARD_WIDTH][MAX_BOARD_HEIGHT];

void setup_board(void);
void do_highlight_board_logic(void);
void clear_highlight_board(void);
void hide_highlight_board(bool instant);
void highlight_wizard_pieces(int wiz);
int set_highlight_board(int tx, int ty, float range, int layer, bool own, bool empty, bool enemy, bool los, 
bool clear, char *group);

void set_board_highlight(int type, int x, int y);
int return_highlight_type(int x1, int y1, int layer1, int x2, int y2, int layer2);

void highlight_land_movement_options(int tx, int ty);
void explore_movement_from_board_square(int tx, int ty);
void draw_land_movement_route(int x, int y);
void clear_land_movement_route(void);

void store_board_piece_mouse_over(void);
void draw_board(void);
void draw_board_grid(void);
void draw_background_clouds(void);
void draw_selected_effect(void);
void draw_highlight_effect(void);
void draw_background_clouds(void);

float workout_range(int x1, int y1, int x2, int y2);
bool line_of_sight(int x1, int y1, int x2, int y2);

void swap_all_board(void);
void do_engaged_rule(void);
int count_wizard_creatures_on_board(int w);

void add_creatures_randomly_over_board(int num, char *name);

#endif
