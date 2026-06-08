#ifndef CACHE_H
#define CACHE_H

#include "../Memory/Spiffs.h"

typedef struct{
    char* data;
}Cache_t;

int Cache_Initialize(Cache_t* cache);

int Cache_WriteFileJSON(Cache_t* cache, const char* data, const char* file_name);

int Cache_LoadFileJSON(Cache_t* cache, const char* file_name);

void Cache_Dispose(Cache_t* cache);


#endif