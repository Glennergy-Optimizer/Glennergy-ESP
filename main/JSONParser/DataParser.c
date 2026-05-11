#include "DataParser.h"
#include <string.h>
#include "jansson.h"
#include "esp_log.h"

static const char *TAG = "DataParser";

void DataParser_Parse(const char *raw_data, RecommendationList *r_list)
{
    json_error_t error;
    json_t *root = json_loads(raw_data, 0, &error);

    if (root == NULL)
    {
        ESP_LOGE(TAG, "Failed to load data from http response");
        return;
    }

    if (!json_is_array(root))
    {
        ESP_LOGW(TAG, "data is not an array");
        json_decref(root);
        return;
    }

    size_t array_size = json_array_size(root);

    r_list->count = array_size;

    for (int i = 0; i < array_size; i++)
    {
        json_t *obj = json_array_get(root, i);

        json_t *id = json_object_get(obj, "id");
        json_t *rec = json_object_get(obj, "type");
        json_t *timestamp = json_object_get(obj, "timestamp");

        r_list->rec[i].id = json_integer_value(id);
        r_list->rec[i].recommendation = json_real_value(rec);
        strncpy(r_list->rec[i].timestamp,
                json_string_value(timestamp),
                sizeof(r_list->rec[i].timestamp) - 1);

        r_list->rec[i].timestamp[sizeof(r_list->rec[i].timestamp) - 1] = '\0';
    }

    json_decref(root);
    ESP_LOGI(TAG, "%2.f", r_list->rec[0].recommendation);
}
