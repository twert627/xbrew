#define _XOPEN_SOURCE 500 
#include "fs.h"
#include "cli.h" /* FIXME: Put color here instead of CLI lol */

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __linux__
#define _XOPEN_SOURCE_EXTENDED 1
#include <ftw.h>
#else
/* TODO: Windows */
#endif

/* Credit to <https://www.youtube.com/watch?v=BJPUQMyt1Ns - Low Level Devel>*/
static char *copy_to_path, *copy_from_path;
static int copy_busy = 0;

int copy_file(const char *from, char *to) {
    FILE *ff = fopen(from, "r");

    if(!ff) {
        printf(RED);
        perror("Can't open source file!\n");
        printf(reset);
        return 0;
    }
    FILE *ft = fopen(to, "w");

    if(!ft) {
        printf(RED);
        perror("Can't open dest file!\n");
        printf(reset);
        fclose(ff);
        return 0;
    }

    char buffer[8 * 1024]; /* 8kb */
    size_t r = 0;

    while((r = fread(buffer, 1, sizeof(buffer), ff))) {
        fwrite(buffer, 1, r, ft);
    }

    fclose(ff);
    fclose(ft);
    return 1;
}

static int cp_callback(const char *fpath, const struct stat *sb, int type_flag, struct FTW*ftwbuf) {
    char to_location[1024];
    sprintf(to_location, "%s/%s", copy_to_path, fpath + strlen(copy_from_path) + 1);

    if(type_flag & FTW_D) {
        /* Directory found */
        if(ftwbuf->level == 0) {
            /* Depth check of recursiveness (0 == '/' [root] FIXME) */
            return 0;
        }
        if(mkdir(to_location, 0775)) {
            printf(RED);
            perror("Failed to create directory!\n");
            printf(reset);
        }
    } else if(!copy_file(fpath, to_location)) {
        return -1;
    }
    return 0;
}

int copy_dir_contents(char *path, char *to) {
    if(copy_busy) {
        printf(RED);
        fprintf(stderr, "copy busy\n");
        printf(reset);
        return 0;
    }

    copy_busy = 1;
    copy_to_path = to;
    copy_from_path = path;
    
    int ret = nftw(path, cp_callback, 64, FTW_PHYS);

    copy_busy = 0;
    return ret == 0;
}