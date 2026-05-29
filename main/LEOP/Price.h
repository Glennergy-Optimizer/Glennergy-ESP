#ifndef PRICE_H
#define PRICE_H

#include <stddef.h>

typedef struct
{
    double current_prices;
    char timestamp[20];
} Price;

typedef struct
{
    Price price[96];
    size_t count;
} PriceList;

int Price_Initialize(PriceList *r_list);

int Price_Fetch(const char *url, PriceList *r_list);

void Price_Dispose(PriceList *r_list);

#endif
