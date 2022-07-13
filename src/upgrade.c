#include "upgrade.h"
#include "cli.h"
#include "http-get.h"
#include "parson.h"
#include "fs.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Credit to clib for version fetch :) */
#define CURRENT_VERSION_URL                                                    \
  "https://api.github.com/repos/twert627/xbrew/releases/latest"

#define XBREW_ASSET_URL "https://api.github.com/repos/twert627/xbrew/tarball/"

const char *fetch_current_version() {
  http_get_response_t *res = http_get(CURRENT_VERSION_URL);

  JSON_Value *root_json = NULL;
  JSON_Object *json_object = NULL;
  char *tag_name = NULL;

  if (!res->ok) {
    printf(RED);
    printf("Couldn't find latest xbrew release!\n");
    printf(reset);
    goto cleanup;
  }

  if (!(root_json = json_parse_string(res->data))) {
    printf(RED);
    printf("Unable to parse JSON response!\n");
    printf(reset);
    goto cleanup;
  }

  if (!(json_object = json_value_get_object(root_json))) {
    printf(RED);
    printf("Unable to parse JSON response object!\n");
    printf(reset);
    goto cleanup;
  }

  tag_name = strdup(json_object_get_string(json_object, "tag_name"));

cleanup:
  if (root_json) {
    json_value_free(root_json);
  }
  http_get_free(res);

  return tag_name;
}

int fetch_current_package() {
  FILE *fp;
  const char *release_version = fetch_current_version();

  char release_asset[100];
  memcpy(release_asset + strlen(release_asset), XBREW_ASSET_URL, sizeof(XBREW_ASSET_URL));
  memcpy(release_asset + strlen(release_asset), release_version, strlen(release_version));
  
  http_get_file(release_asset, "xbrew");
  fp = fopen("xbrew", "r");
  if (fp == NULL) {
    printf(RED);
    printf("Failed to download release, please report this bug\n");
    printf(reset);
    return 1;
  } else {
    printf(BLU);
    printf("xbrew version: %s downloaded.\n", release_version);
    /* TODO: maybe write another program that is just an installer? */
    printf("xbrew is unable to install itself, please run the install script instead\n");
    printf("`$ sudo ./scripts/install.sh`\n");
    printf("Or copy the file to /usr/bin\n");
    printf(reset);


    /*printf("Would you like to install [Y/n]\n");
    printf(reset);
    
    char resp[1];
    scanf("%s", resp);
    if (0 == strncmp(resp, "Y", 1) || 0 == strncmp(resp, "y", 1)) {
      printf("Installing xbrew version %s\n", release_version);
      copy_file("xbrew", "/usr/bin/xbrew");
    } else if (0 == strncmp(resp, "N", 1) || 0 == strncmp(resp, "n", 1)) {
      printf("Not installing...\n");
      return 0;
    } else {
      printf(RED);
      printf("ERROR: Invalid input!\n");
      printf(reset);
      return 1;
    }*/
  }
  return 0;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
  size_t written;
  written = fwrite(ptr, size, nmemb, stream);
  return written;
}

int upgrade() {
  const char *current_version = fetch_current_version();

  if (0 != strncmp(xbrew_version, current_version, strlen(current_version) + 1)) {
    printf(BLU);
    printf("New version of xbrew found...\n");
    printf(reset);
    printf(YEL);
    printf("Would you like to download version %s? [Y/n]\n", current_version);
    printf(reset);
    char resp[1];
    scanf("%s", resp);
    if (0 == strncmp(resp, "Y", 1) || 0 == strncmp(resp, "y", 1)) {
      printf("Updgrading xbrew to %s from %s...\n", current_version,
             xbrew_version);
      fetch_current_package();

    } else if (0 == strncmp(resp, "N", 1) || 0 == strncmp(resp, "n", 1)) {
      printf("Ok, not upgrading");
      return 0;
    } else {
      printf(RED);
      printf("Invalid input!\n");
      printf(reset);
      return 1;
    }
    current_version = NULL;
  }
  return 0;
}