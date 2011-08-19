// Log (.Cpp)
// --------

#include <stdio.h>
#include <stdarg.h>

#include "log.hpp"
#include "Game.hpp"

// Logging functions:
// ------------------

FILE *log_file;

int open_log(char *filename)
{
 if ((log_file = fopen(filename, "w")) != NULL) return 0; /* Opened succesfully. */

 return -1; /* Failed. */
}

int log(char *format, ...)
{
 va_list ptr; /* get an arg pointer */
 int status = -1;
 
 if (log_file != NULL)
 {
  /* initialize ptr to point to the first argument after the format string */
  va_start(ptr, format);
 
  /* Write to logfile. */
  status = vfprintf(log_file, format, ptr); // Write passed text.
  fprintf(log_file, "\n"); // New line..
 
  va_end(ptr);
 
  /* Flush file. */
  fflush(log_file);
 }

 return status;
}

// Same as log, except it doesn't write a Carriage Return character at the
// end of the line.
int log_no_cr(char *format, ...)
{
 va_list ptr; /* get an arg pointer */
 int status = -1;
 
 if (log_file != NULL)
 {
  /* initialize ptr to point to the first argument after the format string */
  va_start(ptr, format);
 
  /* Write to logfile. */
  status = vfprintf(log_file, format, ptr); // Write passed text.
 
  va_end(ptr);
 
  /* Flush file. */
  fflush(log_file);
 }

 return status;
}

int close_log(void)
{
 int status = -1;
 
 if (log_file != NULL)
 {
  fflush(log_file);
  fclose(log_file);
  return 0;
 }
 
 return status;
}

// Support functions.

int log_header(void)
{
 // Write the log header file with the details of the game name, author etc.
 // These are all defines listed in "game.h"
 
 log(GAME_NAME);
 log("----------------------------------------------------");
 log("Version : %s, Started : %s", GAME_VERSION, GAME_DATE);
 log("Author : %s", GAME_AUTHOR);
 log("----------------------------------------------------");
 log("");
 
 return 0;
}

