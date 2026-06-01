#include "Spiffs.h"
#include "esp_log.h"
#include "jansson.h"

static const char *TAG = "Spiffs";

int Spiffs_Initialize(Spiffs_t *spiffs, const char *file)
{
    spiffs->spiffs_config.base_path = "/spiffs";
    spiffs->spiffs_config.partition_label = NULL;
    spiffs->spiffs_config.max_files = 5;
    spiffs->spiffs_config.format_if_mount_failed = true;

    snprintf(spiffs->filepath, sizeof(spiffs->filepath), "%s/%s", spiffs->spiffs_config.base_path, file);

    if (esp_vfs_spiffs_register(&spiffs->spiffs_config) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize spiffs filesystem.");
        return 1;
    }

    return 0;
}

int Spiffs_WriteToFileJSON(Spiffs_t* spiffs, const char* raw_data)
{
    json_error_t error;
    json_t* root = json_loads(raw_data, 0, &error);

    if(root == NULL)
    {
        ESP_LOGW(TAG, "Failed to load json data");
        return 1;
    }

    int result = json_dump_file(root, spiffs->filepath, JSON_INDENT(4));

    if(result < 0)
    {
        ESP_LOGW(TAG, "Failed to dump json file");
        return 2;
    }


    
    return 0;
}

char* Spiffs_ReadFromFileJSON(Spiffs_t* spiffs)
{
    json_error_t error;
    json_t* root = json_load_file(spiffs->filepath, 0, &error);

    if(root == NULL)
    {
        ESP_LOGW(TAG, "Failed to load json file");
        return 1;
    }

    return json_dumps(root, 0);
}

int Spiffs_WriteToFile(Spiffs_t *spiffs, const char *content)
{
    FILE *file = fopen(spiffs->filepath, "w");

    if (file == NULL)
    {
        ESP_LOGW(TAG, "Failed to create/open file");
        return 1;
    }

    fprintf(file, content);
    fclose(file);
    return 0;
}

int Spiffs_ReadFromFile(Spiffs_t *spiffs, char *buffer, size_t buffer_size)
{
    FILE *file = fopen(spiffs->filepath, "r");

    if (file == NULL)
    {
        ESP_LOGW(TAG, "Failed to create/open file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    if(file_size >= buffer_size)
    {
        ESP_LOGE(TAG, "Buffer is too small");

        return 1;
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);

    buffer[bytes_read] = '\0';

    fclose(file);


    return 0;
}


void Spiffs_Dispose(Spiffs_t* spiffs)
{
    spiffs->filepath[0] = '\0';
}