#ifndef RECOMMENDATION_H
#define RECOMMENDATION_H

#include "stddef.h"

typedef struct{
    int id;
    double recommendation;
    char timestamp[20];
}Recommendation;

typedef struct{
    Recommendation rec[96];
    size_t count;
}RecommendationList;


int Recommendation_Initialize(RecommendationList *r_list);

void Recommendation_Fetch(const char *url, RecommendationList *r_list);


#endif