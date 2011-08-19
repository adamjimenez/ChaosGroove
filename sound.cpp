// Sound (.Cpp)
// ------------

#include <ptk.h>
#include <vector>
#include <list>
#include <sstream>
#include <string.h>
#include "KPTK.h"
#include "time.h"

using namespace std;

#include "bridge.hpp"
#include "game.hpp"
#include "log.hpp"
#include "timer.hpp"
#include "scene.hpp"
#include "config.hpp"
#include "board.hpp"
#include "piece.hpp"

#include "net.hpp"
#include "chat.hpp"
#include "wizard.hpp"
#include "panel.hpp"
#include "spell.hpp"
#include "ai.hpp"
#include "effect.hpp"
#include "options.hpp"
#include "Ksound.h"
#include "sound.hpp"

struct sound_t sound;
struct music_t music;

bool setup_sound_system(void)
{
 log("Setting up sounds..");

 load_all_memory_sounds();

 music.volume.current = 0.0;
 music.volume.target = 1.0;
 music.volume.speed = 0.005;

 return true;
}

bool load_music(void)
{
 char name[MAX_STRING], text[MAX_STRING];

 sprintf(name, "\\text_files\\title_music.ini");
 sprintf(name, "%s", KMiscTools::makeFilePath(name));
 
 if (!set_config_file_new(CONFIG_TITLE_MUSIC, name, false))
 {
	log("Can't open title_music config file!");
  return false;
 }
 
 if (music.mod)
 {
	music.mod->stopModule();
 }

 music.mod = NULL;
 music.mod = new KSound;
 
 find_random_line_from_text_config(CONFIG_TITLE_MUSIC, text);
 log("Loading Music: %s", text);

 sprintf(name, "Music\\%s.mod", text);
 sprintf(name, "%s", KMiscTools::makeFilePath(name));

 if (!music.mod->loadModule( name ) )
 {
	log("Couldn't load music from : %s", name);
	return false;
 }
 log("");

//myMusic->playModule( true ) ; //play and loop
 return true;
}

void play_music(void)
{
 if (music.mod)
 {
	music.mod->stopModule();
	music.mod->playModule( true );
	
	//music.volume.current = 0.0;
	music.volume.target = 1.0;
	wait_time(240);
 }
}

void do_music_logic(void)
{
 int c;

 do_alpha_logic(&music.volume);
 if (music.mod)
 {
  find_option_choice_variables(CONFIG_OPTIONS, "SOUND", "MUSIC VOLUME", &c, NULL, NULL, NULL);
	music.mod->setVolume(music.volume.current * c);

  c = find_current_option_choice(CONFIG_OPTIONS, "SOUND", "SOUND BALANCE");

	if (c == 0) music.mod->setPan(-100);
  if (c == 1) music.mod->setPan(0);
	if (c == 2) music.mod->setPan(100);
 }

 // Switched away?
 if (!CheckWindowFocus())
 {
  if (music.mod && music.mod->isPlaying())
  {
   music.mod->pauseModule();
  }
 }
 else
 {
  // No, but have we just switched back in?
  if (music.mod && !music.mod->isPlaying())
	{
	 music.mod->continueModule();
	}
 }
}

bool load_all_memory_sounds(void)
{
 int s;
 char name[MAX_STRING], filepath[MAX_STRING];

 // We leave our first entry as free for one off loaded sounds..
 sound.available_sounds = 1;

 sprintf(name, "Sounds\\Memory\\*.*");
 sprintf(filepath, "%s", KMiscTools::makeFilePath(name));
 KMiscTools::enumerateFolder( filepath , load_sound ) ;

 log("Loaded %d sounds into memory..", sound.available_sounds);

 return true;
}

 bool load_sound( char *f , bool isFolder, void *userData )
 {
  char name[MAX_STRING];

  if (isFolder) return true; // Skip folders.

	sprintf(name, "Sounds\\Memory\\%s", f);
  sprintf(name, "%s", KMiscTools::makeFilePath(name));

	sound.sample[sound.available_sounds] = NULL;
	sound.sample[sound.available_sounds] = new KSound;

	if (!sound.sample[sound.available_sounds]->loadSample(name)) return true; // Can't load this sound.

	sprintf(sound.name[sound.available_sounds], "%s", f);
  sound.available_sounds++;

  //log("%s, %s", name, f);

	if (sound.available_sounds >= MAX_SOUNDS) return false; // Can't load any more sounds..

  return true;
 }

 int return_sound_number(char *name)
 {
  int s;
  char new_name[MAX_STRING];

	for (s = 0 ; s < sound.available_sounds ; s++)
	{
	 sprintf(new_name, "%s.wav", name);
   if (stricmp(new_name, sound.name[s]) == 0) return s;

	 sprintf(new_name, "%s.ogg", name);
   if (stricmp(new_name, sound.name[s]) == 0) return s;
	}

	return -1; // We couldn't find this sound!
 }

int play_sound(char *name, bool wait)
{
 int s, v;
	
 s = return_sound_number(name);
 //log("name: %s, s: %d", name, s);
 if (s == -1) return -1; // Couldn't find sound name!

 find_option_choice_variables(CONFIG_OPTIONS, "SOUND", "SOUND EFFECTS VOLUME", &v, NULL, NULL, NULL);
 if (v <= 0) return s;

 sound.sample[s]->playSample();
 sound.sample[s]->setVolume(v);

 v = find_current_option_choice(CONFIG_OPTIONS, "SOUND", "SOUND BALANCE");

 if (v == 0) sound.sample[s]->setPan(-100);
 if (v == 1) sound.sample[s]->setPan(0);
 if (v == 2) sound.sample[s]->setPan(100);

 if (wait && !game.AI_debug)
 do
 {
  wait_time(1);
 } while (sound.sample[s]->isPlaying());

 return s;
}

 int request_sound_effect(char *specific, char *general, char *behaviour, bool wait)
 {
  bool ret;
	int c, choices;
  char name[MAX_STRING], var[MAX_STRING], result[MAX_STRING];

	sprintf(name, "%s..%s", specific, behaviour);
	//log("name: %s", name);

  jump:
	GetConfigString(CONFIG_SOUNDS, name, "SAMPLE 1", result, MAX_STRING);
	if (strlen(result) == 0) GetConfigString(CONFIG_SOUNDS, name, "GOTO 1", result, MAX_STRING);

	// Did we find anything with our specific request?
	if (strlen(result) == 0)
	{
	 // No. So, let's try our general request..
   sprintf(name, "%s..%s", general, behaviour);
	 GetConfigString(CONFIG_SOUNDS, name, "SAMPLE 1", result, MAX_STRING);
	 if (strlen(result) == 0) GetConfigString(CONFIG_SOUNDS, name, "GOTO 1", result, MAX_STRING);
	}

	if (strlen(result) == 0) return -1; // Couldn't find any sample options.

	// Ok, we've found at least one option, now let's see how many choices there are..
  c = 0;
	do
	{
	 c++;
   sprintf(var, "SAMPLE %d", c);
   GetConfigString(CONFIG_SOUNDS, name, var, result, MAX_STRING);

	 // Did we find a Sample entry?
	 if (strlen(result) == 0)
	 {
		// No? Maybe a Goto jump instead?
    sprintf(var, "GOTO %d", c);
    GetConfigString(CONFIG_SOUNDS, name, var, result, MAX_STRING);
	 }
	} while (strlen(result) > 0);

	// Number of choices found.
	choices = c - 1;

	//log("choices: %d", choices);

	c = IRand(1, choices);
  sprintf(var, "SAMPLE %d", c);
  GetConfigString(CONFIG_SOUNDS, name, var, result, MAX_STRING);

	// Not a sample?
	if (strlen(result) == 0)
	{
   sprintf(var, "GOTO %d", c);
   GetConfigString(CONFIG_SOUNDS, name, var, result, MAX_STRING);
	
	 // It's a goto instead..
	 if (strlen(result) > 0)
	 {
	  sprintf(name, "%s", result);
		//log("GOTO : %s", name);
		goto jump; // Repick a sample..
	 }
	}

	// Play sound
	return play_sound(result, wait);
 }