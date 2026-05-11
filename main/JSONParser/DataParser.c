#include "DataParser.h"
#include "jansson.h"
#include "esp_log.h"

static const char* TAG = "DataParser";


int DataParser_Initialize(DataParser* data_parser)
{
    data_parser = {0};
    data_parser->http_response = {0};

    return 0;
}

void DataParser_Parse(DataParser* data_parser)
{
    json_error_t error;
    json_t* raw_data = json_loads(data_parser->http_response.data, 0, &error);

    if(raw_data == NULL)
    {
        ESP_LOGE(TAG, "Failed to load data from http response");
        return;
    }

    if(!json_is_array(raw_data))
    {
        ESP_LOGW(TAG, "data is not an array");
        return;
    }

    size_t array_size = json_array_size(raw_data);

    for(int i = 0; i < array_size; i++)
    {
        json_t* obj = json_array_get(raw_data, i);

        json_t* id = json_object_get(obj, "id");
        json_t* rec = json_object_get(obj, "type");
        json_t* timestamp = json_object_get(obj, "timestamp");

        data_parser->parsed_data.rec[i].id = json_integer_value(id);
        data_parser->parsed_data.rec[i].recommendation = json_real_value(rec);
        data_parser->parsed_data.rec[i].timestamp = json_string_value(timestamp);

    }

}       






void DataParser_Dispose(DataParser* data_parser)
{
    if(data_parser != NULL && data_parser->http_response.data != NULL)
    {
        free(data_parser->http_response.data);
    }
}