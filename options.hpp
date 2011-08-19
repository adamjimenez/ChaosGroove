// Options (.Hpp)

#ifndef __Options_hpp__
#define __Options_hpp__

#define MAX_OPTIONS							32
#define MAX_CHOICES							16

#define MENU_WIZARDS						0

#define BUTTON_EXIT							0
#define BUTTON_CONTINUE					1

struct option_t
{
 int choice;
};

struct menu_info_t
{
 int config, new_config, option_clicked, var1, var2;
 char section_name[512], action[512], new_section_name[512];
 float scrolly_pos;
 struct alpha alpha;
};

extern struct option_t option[MAX_OPTIONS];
extern struct menu_info_t menu_info;

bool load_options(void);
void save_options(void);
void setup_options(int cfg, char *section);
void fade_out_options(void);
int find_option(int cfg, char *option);
bool check_option_choice(int cfg, char *section, char *option, char *choice);
int check_option_choice_int(int cfg, char *section, char *option, int def);
int find_current_option_choice(int cfg, char *section, char *option);
bool find_option_choice_variables(int cfg, char *section, char *option, int *var1, int *var2, int *var3, char *text_var1);
void do_options(bool draw);
void draw_scrolly(void);

#endif