// Config (.Hpp)
// -----------
#ifndef __Config_hpp__
#define __Config_hpp__ 

#define MAX_CONFIG_FILES						16

#define CONFIG_DISPLAY							0
#define CONFIG_TEMP									1
#define CONFIG_OPTIONS    					2
#define CONFIG_WIZARDS							3
#define CONFIG_MENUS								4
#define CONFIG_WIZARD_NAMES					5
#define CONFIG_SOUNDS								6
#define CONFIG_SCROLLY							7
#define CONFIG_TITLE_MUSIC					8

struct config_t
{
 char name[512];
 char *data;
 int length;
};

extern struct config_t config[MAX_CONFIG_FILES];

bool set_config_file_new(int cfg, char *name, bool log_error);
bool write_config_file(int cfg);
void find_random_line_from_text_config(int cfg, char *text);
int GetConfigInt(int cfg, char *name, char *var, int def);
float GetConfigFloat(int cfg, char *name, char *var, float def);
bool GetConfigString(int cfg, char *name, char *var, char *text, long size);
int GetConfigYes(int cfg, char *name, char *var, int def);
void SetConfigData(int cfg, char *data, int len);
bool change_config_text(int cfg, char *name, char *var, char *text);
void free_all_config_data(void);

void strip_underscores_from_text(char *text);
void capitalize_text(char *text);

#endif
