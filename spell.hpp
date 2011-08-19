// Spell (.Hpp)

#ifndef __Spell_hpp__
#define __Spell_hpp__

#define MAX_WIZ_SPELLS        256
#define MAX_POS_SPELLS        999

struct spell_t
{
 int piece_ID;
 char name[40];
 char desc[512];
 char new_piece_name[40];
 char piece_name[40];

 bool create_piece, show_piece_stats, own, empty, enemy, wizards, swap_board, move_piece, cannot_move_after_spell;
 bool change_owner_to_our_wizard, line_of_sight, chance_against_magic_resistance, magic_attack, can_be_illusion;
 bool destroy_illusion, remove_spell_after_use, target_cannot_move, on_kill_create_piece, kill_growths;

 int gfx, chance, balance, range, layer, move_piece_to_layer, casts, autocast, cast_effect; 
 int cast_effect_var1, cast_effect_var2, missile_gfx, attack_value;
 int change_undead_status_to, change_attack_undead_to, change_gfx_to, change_movement_amount_to;
 int change_flying_to, add_to_defence, add_to_combat, change_ranged_combat_attack_to, change_ranged_combat_range_to;
 int change_shadow_form_to;

 float missile_speed;
};

struct total_spells_t
{
 int num;
 char loaded_spell_list_name[MAX_STRING];
};

struct spell_list_t
{
 int spell;
 bool cast, bonus;
 struct alpha alpha;    
};

extern struct spell_list_t spell_list[MAX_WIZ_SPELLS][MAX_WIZARDS + 1];
extern struct total_spells_t total_spells;
extern struct spell_t spell[MAX_POS_SPELLS];

void load_spells(void);
void setup_spell_lists(void);
int add_spells(int sp, int w);
void clear_bonus_spell_stat_in_spell_list(int w);
void sort_spell_list(int w);
void select_spell(int s);

int return_spell_chance(int sp);
bool can_we_cast_spell_here(int x, int y);
void check_if_can_cast_all_spells(int wiz);

bool try_to_cast_spell(int x, int y);
void spell_succeeds(void);
void spell_fails(void);
Rgba return_spell_chance_col(int chance);
int highlight_where_spell_can_be_cast(int s, bool clear);
void autocast_spell(int *best_x, int *best_y);

int find_layer_by_name(char *name, int def);
int amount_of_this_spell_in_list(int sp);

void stop_growths(void);
void kill_growths(void);

#endif 