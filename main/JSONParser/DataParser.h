#ifndef DATAPARSER_H
#define DATAPARSER_H

#include "../HTTP.h"
#include "../LEOP/Recommendation.h"


int DataParser_Parse(const char* raw_data, RecommendationList* r_list);


#endif