#pragma once

#include"../entity.h"

typedef struct Transformation {
	char *Name;
	
	Energy_t Cost;
	Energy_t Upkeep;

	uint64_t TransformationMultiplier;
} Transformation_t;
