#include "Cache.h"
#include <string.h>
#include "esp_log.h"

static const char *TAG = "Cache";

int Cache_Initialize(Cache_t *cache)
{
    if (cache == NULL)
    {
        ESP_LOGW(TAG, "Failed to initialize cache");
        return 1;
    }

    cache->data = NULL;

    return 0;
}

int Cache_WriteFileJSON(Cache_t *cache, const char *data, const char *file_name)
{

    int result = Spiffs_WriteToFileJSON(file_name, data);

    if (result != 0)
    {
        ESP_LOGW(TAG, "Failed to write json file");
        return 2;
    }

    return 0;
}

int Cache_LoadFileJSON(Cache_t *cache, const char *file_name)
{
    char *json = Spiffs_ReadFromFileJSON(file_name);

    if (json == NULL)
    {
        return 2;
    }

    cache->data = strdup(json);

    free(json);

    if (cache->data == NULL)
    {
        return 3;
    }

    return 0;
}

void Cache_Dispose(Cache_t *cache)
{
    if (cache != NULL && cache->data != NULL)
    {
        free(cache->data);
        cache = NULL;
    }
}