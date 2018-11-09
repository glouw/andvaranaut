#pragma once

typedef int Sorter(const void* const, const void* const);

int s_nearest_first(const void* const, const void* const);

int s_furthest_first(const void* const, const void* const);
