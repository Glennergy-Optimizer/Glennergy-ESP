#include "Spiffs.h"
#include "esp_log.h"
#include "jansson.h"

static const char *TAG = "Spiffs";

static esp_vfs_spiffs_conf_t spiffs_config;

int Spiffs_Initialize()
{
    spiffs_config.base_path = "/spiffs";
    spiffs_config.partition_label = NULL;
    spiffs_config.max_files = 5;
    spiffs_config.format_if_mount_failed = true;

    esp_err_t ret = esp_vfs_spiffs_register(&spiffs_config);

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize SPIFFS: %s", esp_err_to_name(ret));
        return 1;
    }

    return 0;
}

int Spiffs_WriteToFileJSON(const char *file_name, const char *raw_data)
{
    json_error_t error;
    json_t *root = json_loads(raw_data, 0, &error);

    if (root == NULL)
    {
        ESP_LOGW(TAG, "Failed to load json data");
        return 1;
    }

    char filepath[50];

    snprintf(filepath, sizeof(filepath), "%s/%s", spiffs_config.base_path, file_name);

    int result = json_dump_file(root, filepath, JSON_INDENT(4));

    if (result < 0)
    {
        ESP_LOGW(TAG, "Failed to dump json file");
        return 2;
    }

    json_decref(root);

    return 0;
}

char *Spiffs_ReadFromFileJSON(const char *file_name)
{

    char filepath[50];

    snprintf(filepath, sizeof(filepath), "%s/%s", spiffs_config.base_path, file_name);

    json_error_t error;
    json_t *root = json_load_file(filepath, 0, &error);

    if (root == NULL)
    {
        ESP_LOGW(TAG, "Failed to load json file");
        return NULL;
    }

    char *dump = json_dumps(root, JSON_INDENT(2));

    json_decref(root);

    return dump;
}

int Spiffs_WriteToFile(const char *file_name, const char *content)
{
    char filepath[50];

    snprintf(filepath, sizeof(filepath), "%s/%s", spiffs_config.base_path, file_name);

    FILE *file = fopen(filepath, "w");

    if (file == NULL)
    {
        ESP_LOGW(TAG, "Failed to create/open file");
        return 1;
    }

    fprintf(file, content);
    fclose(file);
    return 0;
}

int Spiffs_ReadFromFile(const char *file_name, char *buffer, size_t buffer_size)
{
    char filepath[50];

    snprintf(filepath, sizeof(filepath), "%s/%s", spiffs_config.base_path, file_name);

    FILE *file = fopen(filepath, "r");

    if (file == NULL)
    {
        ESP_LOGW(TAG, "Failed to create/open file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    if (file_size >= buffer_size)
    {
        ESP_LOGE(TAG, "Buffer is too small");

        return 1;
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);

    buffer[bytes_read] = '\0';

    fclose(file);

    return 0;
}

