#pragma once

#include "Types.h"

Gpu setup(const int res, const char* const name);
void release(const Gpu gpu);
void churn(const Gpu gpu);
void present(const Gpu gpu);
