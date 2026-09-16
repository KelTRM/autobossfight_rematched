#pragma once

#include<stddef.h>

#define Offset(type,field)			((void*)&(((type*)0)->field))
#define GetDataAtOffset(value,offset,type)	((type*)(((size_t)value) + (size_t)(offset)))
