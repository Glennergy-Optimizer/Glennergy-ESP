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

int DataParser_Initialize(DataParser* data_parser);

void DataParser_Parse(DataParser* data_parser);

void DataParser_Dispose(DataParser* data_parser);
#endif