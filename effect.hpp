// Effect (.Hpp)
// -------------

#define EFFECT_NONE										0
#define EFFECT_EXPLODE								1
#define EFFECT_SPELLCAST							2
#define EFFECT_ATTACK									3
#define EFFECT_MAGIC_ATTACK						4
#define EFFECT_FIREWORK								5
#define EFFECT_FIRE										6

int find_effect_by_name(char *name, int def);
void do_missile_effect(int gfx, float speed, int x1, int y1, int x2, int y2);
void do_plain_missile_effect(int g, float speed, int x1, int y1, int x2, int y2);
void do_spell_casting_effect(int x, int y, int amount);
void do_attack_effect(int x, int y, int times);
void do_magic_attack_effect(int x, int y, int time, int g);
void do_explode_effect(int x, int y, int amount);
void do_effect(int effect, int x, int y, int var1, int var2);
void do_fire_effect(int x, int y, int amount);
void do_wizard_dying_effect(int x, int y, int amount);
void do_firework_effect(int x, int y, int amount);