#ifndef HTTP_H
#define HTTP_H

#include "esp_err.h"

#include "freertos/queue.h"
#include <stdint.h>

typedef struct
{
    char* data;
    size_t length;
} HTTPResponse;

extern QueueHandle_t http_queue;

void HTTPClient_GET(const char *url, HTTPResponse* http_response);

#endif