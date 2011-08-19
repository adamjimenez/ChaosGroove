// Piece (.Hpp)

#ifndef __Piece_hpp__
#define __Piece_hpp__ 

#define MAX_PIECES      999

// Define for blank map/piece element
#define BLANK           -1

#define TYPE_WIZARD     0
#define TYPE_CREATURE   1
#define TYPE_GROWTH     2

struct piece_data
{
 char name[40], group_name[40], grow_piece_name[40];

 bool adjacent_in_group;

 int type;
 int owner;
 int turn;
 
 int gfx, start_gfx, time_next_frame, time_between_frames, gfx_frames, body_gfx;
 bool ping_pong_anim, ping_pong_anim_forward;
 
 int combat;
 int defence;
 
 int ranged_combat_attack;
 int ranged_combat_range;
 int ranged_combat_gfx;
 
 int movement;
 int flex;
 int magic_resist;

 int chance_of_disappearing, only_disappear_when_ridden, spells_when_disappeared, disappear_effect, disappear_effect_var1;
 int grow_chance, grow_attack, grow_die_chance;
 int ranged_combat_effect, ranged_combat_effect_var1;

 bool flying, rideable, can_ride, undead, can_attack_undead, shadow_form, grow_covers, just_grown, engaged;
 bool selected, dead, has_moved, has_attacked, has_shot, transparent_to_line_of_sight, any_wizard_can_ride;
 bool illusion, disbelieved, can_be_illusion;
 int chance;
};

// We construct a list of all possible pieces from this struct.
extern struct piece_data piece[MAX_PIECES];

void load_all_pieces(void);
void do_pieces_animation_logic(void);
int find_piece_by_name(char *name);

void create_new_piece(char *name, int x, int y);
void kill_piece(int x, int y, int by, bool sound);

bool try_to_select_piece(int x, int y, bool real);
void unselect_piece(void);

void try_to_move_piece(int x, int y, bool real);

void move_piece(int x, int y);
void move_piece_along_route(int x, int y);
void ride_piece(int x, int y);
bool do_attack_calculation(int att, int def);
void attack_piece(int x, int y);
void shoot_piece(int x, int y);
void end_move_piece(void);

void grow_piece(int x, int y);

void restore_all_pieces_status_flags(void);
void increase_all_pieces_turn_number(void);

bool any_enemies_around_piece(int tx, int ty);
bool engaged_around_piece(int tx, int ty);
bool adjacent_in_hostile_group(int tx, int ty, char *group);
bool any_pieces_we_can_move(void);
int pick_random_piece_by_group_name(char *name);

void swap_pieces(int x1, int y1, int x2, int y2);
bool is_piece_friendly(int x, int y, int layer);

#endif