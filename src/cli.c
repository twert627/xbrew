#include "common.h"

#include "cli.h"
#include "upgrade.h"
#include "version.h"
#include "fs.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const char *usage =
    "\n"
    "  xbrew <command> [options]\n"
    "\n"
    "  Options:\n"
    "\n"
    "    -h, --help     Output this message\n"
    "    -v, --version  Output version information\n"
    "\n"
    "  Commands:\n"
    "\n"
    "    init [console...]    Start a new project\n"
    "    i, install [name...] Install one or more packages\n"
    "    up, update [name...] Update one or more packages\n"
    "    uninstall [name...]  Uninstall executables\n"
    "    upgrade [version]    Upgrade xbrew to a specified or latest "
    "version\n"
    "    configure [name...]  Configure one or more packages\n"
    "    build [name...]      Build one or more packages\n"
    "    search [query]       Search for packages\n"
    "    help <xbrew>         Display help for xbrew\n"
    "";

#define BADKEY -1
#define TDS 1
#define NDS 2
#define SWITCH 3

typedef struct {
  char *key;
  int val;
} t_symstruct;

static t_symstruct lookuptable[] = {
    {"3ds", TDS}, {"nds", NDS}, {"switch", SWITCH}};

#define NKEYS (sizeof(lookuptable) / sizeof(t_symstruct))

int key_from_string(char *key) {
  long unsigned int i;
  for (i = 0; i < NKEYS; i++) {
    t_symstruct *sym = &lookuptable[i];
    if (strcmp(sym->key, key) == 0)
      return sym->val;
  }
  return BADKEY;
}

void cli_init(char *name, char *console) {
  printf(GRN);
  printf("Creating new project %s...\n", name);
  printf(reset);

  int check;
#ifdef __linux__
  check = mkdir(name, 0775);
#else
  check = _mkdir(name);
#endif

  /* Make project directory if check passes */
  if (!check) {
#ifdef __linux__
    mkdir(name, 0775);
#else
    _mkdir(name);
#endif

    /* Copy over boilerplate */
#ifdef __linux__
    switch (key_from_string(console)) {
      case TDS:
        copy_dir_contents("templates/3ds", name);
        copy_dir_contents("templates/_common", name);
        break;
      case NDS:
        copy_dir_contents("templates/nds", name);
        copy_dir_contents("templates/_common", name);
        break;
      case SWITCH:
        copy_dir_contents("templates/switch", name);
        copy_dir_contents("templates/_common", name);
        break;
      case BADKEY:
        printf(RED);
        printf("xbrew forgot what the console was, please report this bug!\n");
        printf(reset);
        _exit(1);
        break;
    }
#else

#endif

    printf(GRN);
    printf("Successfully created project named %s!\n", name);
    printf(reset);
    _exit(0);
  } else {
    printf(RED);
    printf("Unable to create directory, does it already exist?\n");
    printf(reset);
    _exit(1);
  }
}

int cli_args(char **argv) {
  /* usage */
  if (NULL == argv[1] || 0 == strncmp(argv[1], "-h", 2) ||
      0 == strncmp(argv[1], "--help", 6)) {
    printf("%s\n", usage);
    return 0;
  }

  /* version */
  if (0 == strncmp(argv[1], "-v", 2) || 0 == strncmp(argv[1], "--version", 9)) {
    printf("%s\n", xbrew_version);
    return 0;
  }

  /* Project init */
  if (0 == strncmp(argv[1], "init", 4)) {
    printf("Enter project name\n");
    char project[256];
    scanf("%s", project);
    /* String check */
    if (strlen(project) > 0x100) {
      printf(RED);
      printf("265 character limit! Please choose a smaller name\n");
      printf(reset);
      _exit(1);
    }

    if (argv[2] != NULL) {
      switch (key_from_string(argv[2])) {
      case TDS:
        cli_init(project, "3ds");
        break;
      case NDS:
        cli_init(project, "nds");
        break;
      case SWITCH:
        cli_init(project, "switch");
        break;
      case BADKEY:
        _exit(1);
      }
    }
    cli_init(project, NULL);
  }

  /* Upgrade xbrew */
  if (0 == strncmp(argv[1], "up", 2) || 0 == strncmp(argv[1], "upgrade", 7)) {
    upgrade();
  }

  /* unknown */
  if (0 == strncmp(argv[1], "-", 1)) {
    fprintf(stderr, "Unknown option: \"%s\"\n", argv[1]);
    return 1;
  }
  return 0;
}
