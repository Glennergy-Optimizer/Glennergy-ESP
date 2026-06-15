#ifndef RECOMMENDATION_H
#define RECOMMENDATION_H

#include "stddef.h"
#include "../Cache/Cache.h"

typedef struct{
    bool recommendation_fetched;
}RecommendationStatus;

typedef struct{
    int id;
    double recommendation;
    char timestamp[20];
}Recommendation;

typedef struct{
    Recommendation rec[96];
    size_t count;
    Cache_t cache;
    RecommendationStatus status;
}RecommendationList;


int Recommendation_Initialize(RecommendationList *r_list);

int Recommendation_Fetch(const char *url, RecommendationList *r_list);

int Recommendation_FetchCache(RecommendationList* r_list);

void Recommendation_Dispose(RecommendationList* r_list);


#endif