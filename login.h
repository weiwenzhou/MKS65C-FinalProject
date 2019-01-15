#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

char * rStrip(char * s);
char * lStrip(char * str);
char * strip(char * str);
int logging();