#pragma once

typedef int Sorter(const void* const, const void* const);

int s_nearest_sprite_first(const void* const a, const void* const b);

int s_furthest_sprite_first(const void* const a, const void* const b);

int s_largest_int_first(const void* const a, const void* const b);

int s_descend_tris(const void* const a, const void* const b);
