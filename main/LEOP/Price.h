#ifndef PRICE_H
#define PRICE_H

#include <stddef.h>
#include "../Cache/Cache.h"

typedef struct{
    bool electricity_fetched;
}PriceStatus;

typedef struct
{
    double current_prices;
    char timestamp[20];
} Price;

typedef struct
{
    Price price[96];
    size_t count;
    Cache_t cache;
    PriceStatus status;
} PriceList;

int Price_Initialize(PriceList *r_list);

int Price_Fetch(const char *url, PriceList *r_list);

int Price_FetchCache(PriceList *p_list);

void Price_Dispose(PriceList *r_list);

#endif
