#ifndef HTTP_H
#define HTTP_H


#include <stddef.h>
#include "esp_err.h"



typedef struct
{
    char* data;
    size_t length;
} HTTPResponse;


void HTTPClient_GET(const char *url, HTTPResponse* http_response);

void HTTPClient_Dispose(HTTPResponse *http_response);

#endif