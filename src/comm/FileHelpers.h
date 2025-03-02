#ifndef FILE_HELPERS_H_
#define FILE_HELPERS_H_

#include "Common.h"

///////////////
//  DEFINES  //
///////////////

#define READ_BUF_SIZE 2048

#ifdef __linux__
#define SLASH '/'
#else
#define SLASH '\\'
#endif

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

/* function to help load bytes from a file easier */
unsigned char * fileHelpers_getBytes(char * file, unsigned int * size);
/* function to help load text from a file easier */
char * fileHelpers_getText(char * file, unsigned int * size);
/* function to help write bytes to a file */
void fileHelpers_writeBytes(char * file, void * bytes, unsigned int size);
/* creates a dir */
void fileHelpers_createDir(char * dir);
/* cd to a dir */
void fileHelpers_cd(char * dir);
/* get the patch of a given file */
char * fileHelpers_getDir(char * file);
/* name of the file from a full path */
char * fileHelpers_getName(char * file);

#endif
