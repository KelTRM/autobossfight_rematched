#pragma once

#include"transformation.h"

Energy_t DefaultMaintainTransformation(Transformation_t *Self, Entity_t *Possessor);
Health_t DefaultGetDamage(Transformation_t *Self, Entity_t *From, Health_t BaseDamage);
