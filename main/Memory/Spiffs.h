#ifndef SPIFFS_H
#define SPIFFS_H

#include "esp_spiffs.h"

typedef struct{
    esp_vfs_spiffs_conf_t spiffs_config;
    char filepath[50];
}Spiffs_t;


int Spiffs_Initialize(Spiffs_t *spiffs, const char *file);

int Spiffs_WriteToFile(Spiffs_t *spiffs, const char *content);

int Spiffs_ReadFromFile(Spiffs_t *spiffs, char *buffer, size_t buffer_size);

int Spiffs_WriteToFileJSON(Spiffs_t* spiffs, const char* raw_data);

char* Spiffs_ReadFromFileJSON(Spiffs_t* spiffs);

void Spiffs_Dispose(Spiffs_t* spiffs);


#endif
