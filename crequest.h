#ifndef CREQUEST_H_
#define CREQUEST_H_

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AUTHORIZATION "{authorization}"

struct MemoryStruct {
  char *memory;
  size_t size;
};

#define SET_OPT(curl, option, value) curl_easy_setopt(curl, option, value)

void set_curl_options(CURL *curl, const char *url, const char *payload,
                      struct curl_slist *headers);
size_t WriteCallback(const char *ptr, size_t size, size_t nmemb, void *userdata);

size_t WriteCallbackWrapper(const void *contents, size_t size, size_t nmemb, void *userp);


CURLcode make_request(const char *url, const char *payload,
                      const char *headers_array[]);

#endif // CREQUEST_H_

#ifdef CREQUEST_IMPLEMENTATION

void set_curl_options(CURL *curl, const char *url, const char *payload,
                      struct curl_slist *headers) {
  SET_OPT(curl, CURLOPT_URL, url);
  SET_OPT(curl, CURLOPT_POSTFIELDS, payload);
  SET_OPT(curl, CURLOPT_CUSTOMREQUEST, "POST");
  SET_OPT(curl, CURLOPT_HTTPHEADER, headers);
}

size_t WriteCallback(const char *ptr, size_t size, size_t nmemb, void *userdata) {
  struct MemoryStruct *mem = (struct MemoryStruct *)userdata;
  size_t realSize = size * nmemb;
  mem->memory = (char *)realloc(mem->memory, mem->size + realSize + 1);
  if (mem->memory == NULL) {
    // Out of memory!
    printf("Out of memory!\n");
    return 0;
  }
  memcpy(&(mem->memory[mem->size]), ptr, realSize);
  mem->size += realSize;
  mem->memory[mem->size] = 0; // Null-terminate the memory buffer
  return realSize;
}

size_t WriteCallbackWrapper(const void *contents, size_t size, size_t nmemb, void *userp) {
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
  return WriteCallback(contents, size, nmemb, mem);
}

CURLcode make_request(const char *url, const char *payload,
                      const char *headers_array[]) {
  CURL *curl = curl_easy_init();
  struct MemoryStruct response_data = {0};

  if (curl) {
    struct curl_slist *headers = NULL;
    for (int i = 0; headers_array[i]; i++) {
      headers = curl_slist_append(headers, headers_array[i]);
    }

    set_curl_options(curl, url, payload, headers);
    // SET_OPT(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    SET_OPT(curl, CURLOPT_WRITEFUNCTION, WriteCallbackWrapper);
    SET_OPT(curl, CURLOPT_WRITEDATA, &response_data);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }

  // Process the response_data if needed
  printf("Response: %s\n", response_data.memory);

  // Clean up allocated memory
  free(response_data.memory);

  return CURLE_OK;
}

#endif // CREQUEST_IMPLEMENTATION
