#include "LEOP_Fetcher.h"
#include "../WiFi.h"
#include "esp_log.h"

static const char *TAG = "LEOP";

QueueHandle_t leop_queue = NULL;

int LEOPFetcher_Initialize(LEOPData *leop_data, uint32_t interval)
{

    if (leop_data == NULL)
    {
        ESP_LOGE(TAG, "leop_data is NULL");
        return -1;
    }

    Recommendation_Initialize(&leop_data->recommendations);
    Weather_Initialize(&leop_data->weather);
    Price_Initialize(&leop_data->price_list);

    //leop_data->leop_conf.time_interval = interval;

    leop_data->leop_status.electricity_fetched = false;
    leop_data->leop_status.recommendation_fetched = false;
    leop_data->leop_status.weather_fetched = false;

    leop_queue = xQueueCreate(1, sizeof(LEOPData *));

    if (leop_queue == NULL)
    {
        ESP_LOGW(TAG, "Failed to create LEOP queue");
        return -1;
    }

    return 0;
}

void LEOPFetcher_Work(void *arg)
{
    LEOPData *leop_data = (LEOPData *)arg;

    if (leop_data == NULL)
        return;

    while (1)
    {
        if (WiFi_IsConnected())
        {

            leop_data->leop_status.recommendation_fetched =
                (Recommendation_Fetch("http://10.0.0.3:8080/id=2?recommendation", &leop_data->recommendations) == 0);

            leop_data->leop_status.weather_fetched =
                (Weather_Fetch("http://10.0.0.3:8080/id=2?weather", &leop_data->weather) == 0);

            leop_data->leop_status.electricity_fetched =
                (Price_Fetch("http://10.0.0.3:8080/id=2?price", &leop_data->price_list) == 0);

            if (xQueueSend(leop_queue, &leop_data, 0) != pdPASS)
            {
                ESP_LOGW(TAG, "Failed to send data to leop queue");
            }
        }
        else
        {
            ESP_LOGI(TAG, "WiFi is not connected, waiting 3 seconds until retrying");
            vTaskDelay(pdMS_TO_TICKS(3000));
            continue;
        }

        // Delay by our fetch time interval in minutes.
        // From MS, * 1000 to get S, * 60 to get M
        vTaskDelay(pdMS_TO_TICKS((*leop_data->leop_conf.time_interval * 1000 * 60)));
    }
}