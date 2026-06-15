#ifndef SPIFFS_H
#define SPIFFS_H

#include "esp_spiffs.h"
#include <stdbool.h>


int Spiffs_Initialize();

int Spiffs_WriteToFile(const char* file_name, const char *content);

int Spiffs_ReadFromFile(const char* file_name, char *buffer, size_t buffer_size);

int Spiffs_WriteToFileJSON(const char *file_name, const char *raw_data);

char *Spiffs_ReadFromFileJSON(const char *file_name);




#endif
