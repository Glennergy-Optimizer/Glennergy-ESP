#ifndef HTTP_H
#define HTTP_H

#include <stddef.h>
#include "esp_err.h"

/**
 * @file HTTP.h
 * @brief Public API for the HTTP client module.
 *
 * Provides helpers for issuing HTTP GET requests and storing response data in
 * a dynamically allocated buffer.
 *
 * @defgroup HTTP HTTP
 * @brief HTTP client helpers for fetching response data.
 *
 * Uses ESP-IDF HTTP client APIs and requires Wi-Fi connectivity before
 * requests are attempted.
 * @{
 */

/**
 * @brief HTTP response buffer owned by the caller.
 *
 * Response data is accumulated in a heap buffer and tracked by byte length.
 */
typedef struct
{
    char* data;   /**< Pointer to the response buffer. */
    size_t length; /**< Number of valid bytes in data. */
} HTTPResponse;

/**
 * @brief Performs an HTTP GET request and stores the response body.
 *
 * @param[in] url Request URL.
 * @param[out] http_response Response storage used by the HTTP client.
 *
 * @note The request is only attempted when Wi-Fi is connected.
 * @note Response data is written through the ESP-IDF HTTP event callback.
 */
void HTTPClient_GET(const char *url, HTTPResponse* http_response);

/**
 * @brief Releases the heap buffer owned by an HTTPResponse.
 *
 * @param[in,out] http_response Response object whose data buffer should be freed.
 */
void HTTPClient_Dispose(HTTPResponse *http_response);

/** @} */

#endif
