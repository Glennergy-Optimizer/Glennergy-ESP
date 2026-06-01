#include "Cache.h"
#include <string.h>

static const char *TAG = "Cache";

int Cache_Initialize(Cache_t *cache)
{
    if (cache == NULL)
    {
        ESP_LOGW(TAG, "Failed to initialize cache");
        return 1;
    }

    cache->data = NULL;
}

int Cache_WriteFileJSON(Cache_t *cache, const char *data, const char *file_name)
{
    Spiffs_t spiffs;
    int result = Spiffs_Initialize(&spiffs, file_name);

    if (result != 0)
    {
        ESP_LOGW(TAG, "Failed to initialize spiffs");
        return 1;
    }

    result = Spiffs_WriteToFileJSON(&spiffs, data);

    if (result != 0)
    {
        ESP_LOGW(TAG, "Failed to write json file");
        return 2;
    }

    Spiffs_Dispose(&spiffs);
    return 0;
}

int Cache_LoadFileJSON(Cache_t *cache, const char *file_name)
{
    Spiffs_t spiffs;

    int result = Spiffs_Initialize(&spiffs, file_name);

    if (result != 0)
    {
        ESP_LOGW(TAG, "Failed to initialize spiffs");
        return 1;
    }

    char *json = Spiffs_ReadFromFileJSON(&spiffs);

    if (json == NULL)
    {
        Spiffs_Dispose(&spiffs);
        return 2;
    }

    cache->data = strdup(json);

    Spiffs_Dispose(&spiffs);

    if (cache->data == NULL)
    {
        return 3;
    }

    return 0;
}


void Cache_Dispose(Cache_t* cache)
{
    if(cache != NULL && cache->data != NULL)
    {
        free(cache->data);
        cache = NULL;
    }
}