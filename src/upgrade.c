#include "upgrade.h"
#include "cli.h"
#include "http-get.h"
#include "parson.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Credit to clib for version fetch :) */
#define CURRENT_VERSION_URL                                                    \
  "https://api.github.com/repos/twert627/xbrew/releases/latest"

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

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
  size_t written;
  written = fwrite(ptr, size, nmemb, stream);
  return written;
}

int upgrade() {
  CURL *curl;
  FILE *fp;
  CURLcode res;

  const char *current_version = fetch_current_version();

  if (0 != strncmp(xbrew_version, current_version, strlen(current_version))) {
    printf(BLU);
    printf("New version of xbrew found...\n");
    printf(reset);
    printf(YEL);
    printf("Would you like to upgrade to %s? [Y/n]\n", current_version);
    printf(reset);
    char *resp = NULL;
    scanf("%s", resp);
    if (NULL == resp || 0 == strncmp(resp, "Y", 1) ||
        0 == strncmp(resp, "y", 1)) {
      printf("Updgrading xbrew to %s from %s...\n", current_version,
             xbrew_version);
      char download_url[128] =
          "https://github.com/twert627/xbrew/releases/download/";
      memcpy(download_url + strlen(download_url), current_version,
             strlen(current_version));
      memcpy(download_url + strlen(download_url), "/xbrew", 6);
      char fout[1024] = "./xbrew";
      curl = curl_easy_init();
      char *location;
      long response_code;
      if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, download_url);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
          printf(RED);
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));
          printf(reset);
        } else {
          res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
          if (res == CURLE_OK && (response_code / 100) != 3) {
            /* 300 codes are redirects (eg. GitHub release downloads) */
            res =
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            /* Check response code because GitHub */
            if (res == CURLE_OK && (response_code / 100) != 3) {
              printf(RED);
              fprintf(stderr, "Not a recursive redirect!\n");
              printf(reset);
            } else {
              location = NULL;
              res = curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &location);
              if ((res == CURLE_OK) && location) {
                printf(GRN);
                printf("Redirected to: %s\n", location);
                fp = fopen(fout, "wb");
                if (fp == NULL) {
                  printf(RED);
                  printf("File is empty! curl failed to read it!\n");
                  printf(reset);
                }
                curl_easy_setopt(curl, CURLOPT_URL, location);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
                printf(reset);
              }
            }
            printf(RED);
            fprintf(stderr, "Not a redirect!\n");
            printf(reset);
          }
        }

        curl_easy_cleanup(curl);
        fclose(fp);
      }

#ifdef __linux__
      system("");
#endif
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