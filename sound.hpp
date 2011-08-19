// Sound (.Hpp)
// ------------

#define MAX_SOUNDS				1024

struct sound_t
{
 char name[512][MAX_SOUNDS];
 KSound *sample[MAX_SOUNDS];
 int available_sounds;
};

struct music_t
{
 KSound *mod;
 struct alpha volume;
};

extern struct sound_t sound;
extern struct music_t music;

bool setup_sound_system(void);
bool load_music(void);
void play_music(void);
void do_music_logic(void);
bool load_all_memory_sounds(void);
bool load_sound( char *f , bool isFolder, void *userData );
int return_sound_number(char *name);
int play_sound(char *name, bool wait);
int request_sound_effect(char *specific, char *general, char *behaviour, bool wait);