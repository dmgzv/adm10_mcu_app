#include "sort.h"

#define bubble_sort_fn(type) \
static void bubble_sort_swap_##type(type *xp, type *yp) \
{ \
	type temp = *xp; \
	*xp = *yp; \
	*yp = temp; \
}\
void bubble_sort_##type(type *array, uint32_t size) \
{ \
	uint32_t i, j; \
	uint32_t swapped; \
	if ((!array) || (size == 0)) \
		return; \
	for (i = 0; i < size-1; i++) \
	{ \
		swapped = 0; \
		for (j = 0; j < size-i-1; j++) \
		{ \
			if (array[j] > array[j+1]) \
			{ \
				bubble_sort_swap_##type(&array[j], &array[j+1]); \
				swapped = 1; \
			} \
		} \
		if (swapped == 0) \
			break; \
	} \
}

bubble_sort_fn(int8_t)
bubble_sort_fn(uint8_t)
bubble_sort_fn(int16_t)
bubble_sort_fn(uint16_t)
bubble_sort_fn(int32_t)
bubble_sort_fn(uint32_t)
bubble_sort_fn(int64_t)
bubble_sort_fn(uint64_t)
bubble_sort_fn(float)
bubble_sort_fn(double)
