#ifndef DATAPARSER_H
#define DATAPARSER_H

#include "../HTTP.h"
#include "../LEOP/Recommendation.h"

typedef struct{
    Recommendation rec[96];
}ParsedData;

typedef struct
{
    HTTPResponse http_response;
    ParsedData parsed_data;
} DataParser;

#endif