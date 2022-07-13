#ifndef UPGRADE_H
#define UPGRADE_H

#include "version.h"

int upgrade(void);
const char *fetch_current_version(void);
int fetch_current_package(void);

#endif