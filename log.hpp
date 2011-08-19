// Log (.Hpp)
// ----------

#ifndef __log_hpp__
#define __log_hpp__ 

#include <stdlib.h>
#include <stdio.h>

extern FILE *log_file;

// Logging functions:
// ------------------

int open_log(char *filename);
int log(char *format, ...);
int log_no_cr(char *format, ...);
int close_log(void);

int log_header(void);

#endif
