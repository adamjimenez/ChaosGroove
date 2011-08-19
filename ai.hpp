// AI (.Hpp)
// ---------

#ifndef __Ai_hpp__
#define __Ai_hpp__

struct ai_t
{
 int risk, aggression, threat, strength;
 
 int target_x;
 int target_y;
};

extern struct ai_t ai[MAX_WIZARDS + 1];

void AI_setup(void);
void AI_analyse_board(void);
void AI_select_spell(void);
void AI_select_spell_square(void);
int AI_select_target(int tx, int ty, float range, bool combine, bool wiz);
int AI_select_ranged_target();
int AI_rate_spell_piece(int sp, int risk);
int AI_rate_piece(int x, int y, int tx, int ty, int layer, bool wiz);
void AI_do_movement(void);
void AI_do_wizard_movement(void);
void AI_move_piece(void);
int find_threat_value_to_wizard(int w);

#endif