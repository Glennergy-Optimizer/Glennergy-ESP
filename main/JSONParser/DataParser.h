#ifndef DATAPARSER_H
#define DATAPARSER_H

#include "../HTTP.h"
#include "../LEOP/Recommendation.h"
#include "../LEOP/Weather.h"
#include "../LEOP/Price.h"


int DataParser_ParseRecommendation(const char* raw_data, RecommendationList* r_list);

int DataParser_ParseWeather(const char *raw_data, WeatherList *w_list);

int DataParser_ParsePrice(const char *raw_data, PriceList *p_list);

#endif