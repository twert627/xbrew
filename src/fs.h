#ifndef FS_H
#define FS_H

/* File system utils for xbrew */
int copy_dir_contents(char *path, char *to);
int copy_file(const char *from, char *to);

#endif