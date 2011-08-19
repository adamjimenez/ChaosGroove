// Wizard (.Hpp)

#ifndef __Wizard_hpp__
#define __Wizard_hpp__

#define MAX_WIZARDS     8
#define INDEPENDENT     MAX_WIZARDS

struct wizard_t
{
 char name[80];
 bool alive;
 int x;
 int y;
 Rgba col;
 int spell, ability;
 
 bool spell_is_illusion, human;
};

extern struct wizard_t wizard[MAX_WIZARDS + 1];

void setup_wizards(void);
void setup_wizard_names(void);
void randomise_wizard_name(int w);
void setup_wizard_colours(void);
void setup_wizard_positions(void);
int wizard_here(int x, int y);
void kill_wizard(int w, int by);
void kill_wizards_creatures(int w, bool all);

void check_for_winner(void);

#endif