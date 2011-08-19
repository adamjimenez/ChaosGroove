// Config (.Cpp)
// -----------

// This replaces PTK's config code to avoid problems of spaces in old config code.

#include <ptk.h>
#include <vector>
#include <list>
#include <sstream>
#include "KPTK.h"
#include <sys/stat.h>

using namespace std;

#include <stdlib.h>
#include <stdio.h>
#include "bridge.hpp"
#include "config.hpp"
#include "log.hpp"
#include "scene.hpp"
#include "game.hpp"

struct config_t config[MAX_CONFIG_FILES];

bool set_config_file_new(int cfg, char *name, bool log_error)
{
 int length, r, b, pos;
 FILE *f;
 char *tmp;
 char new_filename[MAX_PATH];

 if (config[cfg].data != NULL)
 {
	free(config[cfg].data);
  config[cfg].data = NULL;
	config[cfg].length = 0;
 }

 sprintf(new_filename, "%s", KMiscTools::makeFilePath(name) );
 //log("%s", new_filename);

 f = fopen(new_filename, "rb");
 if (!f)
 {
  if (log_error) log("Can't open ini file: %s for reading..", new_filename);
  return false;
 }
 
 slprintf(config[cfg].name, MAX_STRING, "%s", new_filename);

 length = FileSize(new_filename);
 tmp = NULL;

 if (length == 0) return false;

 if (length > 0) 
 {
  tmp = (char *)calloc(length, 1);
   
  if (!tmp)
  {
   log("Can't reserve memory for esf file : %s", new_filename);
   return false;
  }
  
	pos = 0;
  for (r = 0 ; r < length ; r++)
	{
   b = fgetc(f);

	 // Add any non-space characters to memory, so stripping out spaces.
	 //if (b != ' ')
	 {
    tmp[pos] = b;
		pos++;
	 }
	}
   
  // Now we will pass it to the config functions.
	SetConfigData(cfg, tmp, pos);

  // And then we will free tmp..
  if (tmp) free(tmp);
 }
 fclose(f);
 
 return true;
}

bool write_config_file(int cfg)
{
 FILE *f;
 int a;

 if (config[cfg].data == NULL) return false;

 f = fopen(config[cfg].name, "wb");
 if (!f)
 {
  log("Can't open ini file: %s for writing..", config[cfg].name);
  return false;
 }

 for (a = 0 ; a < config[cfg].length ; a++)
 {
  fputc(config[cfg].data[a], f);
 }

 fclose(f);
}

void SetConfigData(int cfg, char *data, int len)
{
 if (config[cfg].data != NULL)
 {
	free(config[cfg].data);
  config[cfg].data = NULL;
	config[cfg].length = 0;
 }

 config[cfg].data = (char *)calloc(len + 1, 1);

 memcpy(config[cfg].data , data , len);
 config[cfg].data[len] = 0;
 config[cfg].length = len;
}

void find_random_line_from_text_config(int cfg, char *text)
{
 int a, b, c;

 a = IRand(0, config[cfg].length);

 // Find start of name.
 for (c = 0 ; c < 512 ; c++)
 {
  if (a == 0 || config[cfg].data[a - 1] == 13) break;
	a--;
 }

 if (a > 0) a++;
 b = a;

 // Find end of name.
 for (c = 0 ; c < 512 ; c++)
 {
  if (b == config[cfg].length || config[cfg].data[b + 1] == 13) break;
	b++;
 }

 memset(text, 0, MAX_STRING);
 memcpy(text, config[cfg].data + a, (b - a) + 1);
}

bool GetConfigString(int cfg, char *section, char *variable, char *text, long size)
{
 if (config[cfg].data == NULL) return false;

 char		*sectionPosition  ;
char		*variablePosition  ;

char		sectionName[256] ;
char		variableName[256] ;
long		endofline ;

char		*posstart, *posend ;

	if ( size <= 1 )		
	{
//		MessageBox( 0,"empty buffer",0,0 ) ;
		return false ;
	}
	
	memset( text , 0 , size ) ; 

	//first find the section
	slprintf( sectionName ,255, "[%s]" , section ) ;
  sectionName[255] = 0;

	sectionPosition = strstr( config[cfg].data , sectionName );
	posstart = sectionPosition ;

	if ( sectionPosition == NULL )	
	{
	 return false ;
	}

	//try to locate the variable
	slprintf( variableName , 255 , "%s" , variable ) ;
  variableName[255] = 0;

	variablePosition = sectionPosition;

  start:
	variablePosition =  strstr( variablePosition , variableName );	
	if ( variablePosition == NULL ) return false ;

	posend = variablePosition + strlen( variableName ) + 1;
	if (*posend != ' ' && *posend != '=')
	{
	 variablePosition = posend;
   goto start;	 
	}
	//variablePosition++;
	posend = variablePosition ;
	
	//check if there wasn't a new section between the variable and the section
	sectionPosition+=strlen(sectionName ) ;
	do
	{
		if  ( *posstart == 0x0d )
		{
			posstart++ ;
			if  ( *posstart == '[' )
			{
				return false ;
			}
		}

		if  ( *posstart == 0x0a )
		{
			posstart++ ;
			if  ( *posstart == '[' )
			{
				return false ;
			}
		}
		posstart++ ; 
			
	}while( posstart < posend ) ;
	
	
	variablePosition += strlen( variableName ) + 1 ;
	
	if (*variablePosition != ' ' && *variablePosition != '=') return false; // Not a variable!

	//find the start of the variable data.
	do
	{
	 variablePosition++;
	} while (*variablePosition == ' ' || *variablePosition == '=');

	//find the end of the line
	endofline = 0 ;
	do
	{
		endofline++ ;
	}while( ( variablePosition[endofline] != 0x0d ) && ( variablePosition[endofline] != 0 ) &&
	variablePosition < config[cfg].data + config[cfg].length);

	if (  (size - 1) <= endofline ) 
	{
		endofline = size - 1 ;
	}

	memcpy( text , variablePosition  ,endofline ) ;
	text[endofline] = '\0' ;

	//log("name: %s, result: %s, endline: %d", variableName, text, endofline);
	return true ;
}

int GetConfigInt(int cfg, char *name, char *var, int def)
{
 char values[256] ;

 if (GetConfigString(cfg, name, var, values, 256) == false) return def;
 //if ( iniReader->getString( name, var, values, 256 ) == false )	return def ;

 return	atoi( values ) ;
}

float GetConfigFloat(int cfg, char *name, char *var, float def)
{
 char values[256] ;

 if (GetConfigString(cfg, name, var, values, 256) == false) return def;
 //if ( iniReader->getString( name, var, values, 256 ) == false )	return def ;

 return	(float)atof( values ) ;
}

int GetConfigYes(int cfg, char *name, char *var, int def)
{
 char values[256];
 
 if (GetConfigString(cfg, name, var, values, 256) == false) return def;
 //if (iniReader->getString( name, var, values, 256 ) == false)	return def;
 if (values[0] == 89 || values[0] == 121) return true; // If check for 'Y' or 'y' as first letter, return True

 return false;
}


bool change_config_text(int cfg, char	*section , char *variable, char	*buffer)
{
	char		*sectionPosition  ;
	char		*variablePosition  ;

	char		sectionName[256] ;
	char		variableName[256] ;
	long		endofline ;

	char		*posstart, *posend ;
  char    *data;
	int s, size;

	if ( config[cfg].data == NULL )		return false ;

	//first find the section
	slprintf( sectionName ,255, "[%s]" , section ) ;
	sectionName[255] = 0;

	sectionPosition = strstr( config[cfg].data , sectionName );
	posstart = sectionPosition ;

	if ( sectionPosition == NULL )	
	{
		return false ;
	}

	//try to locate the variable
	slprintf( variableName , 255 , "%s" , variable ) ;
	variableName[255] = 0;

	variablePosition =  strstr( sectionPosition , variableName );	
	if ( variablePosition == NULL )		return false ;
	posend = variablePosition ;

	//check if there wasn't a new section between the variable and the section
	sectionPosition+=strlen(sectionName ) ;
	do
	{
		if  ( *posstart == 0x0d )
		{
			posstart++ ;
			if  ( *posstart == '[' )
			{
				return false ;
			}
		}

		if  ( *posstart == 0x0a )
		{
			posstart++ ;
			if  ( *posstart == '[' )
			{
				return false ;
			}
		}
		posstart++ ; 

	}while( posstart < posend ) ;


	variablePosition += strlen( variableName ) ;

	//find the start of the variable data.
	do
	{
	 variablePosition++;
	} while (*variablePosition == ' ' || *variablePosition == '=');

	//find the end of the line
	endofline = 0 ;
	do
	{
		endofline++ ;
	}while( ( variablePosition[endofline] != 0x0d ) &&          ( variablePosition[endofline] ));

  size = (config[cfg].length - endofline) + strlen(buffer);
  s = (variablePosition - config[cfg].data) + endofline;

  data = (char *)calloc(size, 1);

	memcpy(data, config[cfg].data, variablePosition - config[cfg].data);
	memcpy(data + (variablePosition - config[cfg].data), buffer, strlen(buffer));
	memcpy(data + (variablePosition - config[cfg].data + strlen(buffer)), variablePosition + endofline, 
	(config[cfg].length - s));

	// Free old data, and set to use new data.
	free(config[cfg].data);
	config[cfg].data = data;
	config[cfg].length = size;

	return true ;
}

void free_all_config_data(void)
{
 int c;

 for (c = 0 ; c < MAX_CONFIG_FILES ; c++)
 {
  free(config[c].data);
 }
}

void strip_underscores_from_text(char *text)
{
 int t;

 for (t = 0 ; t < (signed)strlen(text) ; t++)
 {
  if (text[t] == '_') text[t] = ' ';
 }
}

void capitalize_text(char *text)
{
 int t;

 for (t = 0 ; t < (signed)strlen(text) ; t++)
 {
  if (text[t] >= 97 && text[t] <= 122) text[t] -= 32; 
 }
}