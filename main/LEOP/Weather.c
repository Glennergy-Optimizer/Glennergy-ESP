#include "Weather.h"
#include "../HTTP.h"
#include "../JSONParser/DataParser.h"
#include "esp_log.h"

static const char *TAG = "Weather";

int Weather_Initialize(WeatherList *w_list)
{
    ESP_LOGI(TAG, "CRASH 1");
    w_list->count = 0;
    for (int i = 0; i < 96; i++)
    {
        w_list->weather[i] = (Weather){0};
    }
    ESP_LOGI(TAG, "CRASH 2");
    return 0;
}

int Weather_Fetch(const char *url, WeatherList *w_list)
{
    HTTPResponse http_response = {0};

    HTTPClient_GET(url, &http_response);

    if (http_response.data == NULL)
    {
        ESP_LOGW(TAG, "HTTP response is invalid");
        return 1;
    }

    int ret = DataParser_ParseWeather(http_response.data, w_list);

    if(ret != 0)
    {
        return 2;
    }

    for (int i = 0; i < w_list->count; i++)
    {
        ESP_LOGI(TAG, "%.f", w_list->weather[i].temp);
    }

    HTTPClient_Dispose(&http_response);

    return 0;
}

void Weather_Dispose(WeatherList *w_list)
{
    w_list->count = 0;

    for (int i = 0; i < 96; i++)
    {
        w_list->weather[i] = (Weather){0};
    }
}