#pragma once

/* Common C headers */
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>

#ifndef DEBUG
#define DEBUG
#endif

#define Debug(...) printf()

