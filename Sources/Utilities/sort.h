#ifndef __SORT_H
#define __SORT_H

#include <stdint.h>

#define bubble_sort_fn_def(type) void bubble_sort_##type(type *array, uint32_t size)
	
bubble_sort_fn_def(int8_t);
bubble_sort_fn_def(uint8_t);
bubble_sort_fn_def(int16_t);
bubble_sort_fn_def(uint16_t);
bubble_sort_fn_def(int32_t);
bubble_sort_fn_def(uint32_t);
bubble_sort_fn_def(int64_t);
bubble_sort_fn_def(uint64_t);
bubble_sort_fn_def(float);
bubble_sort_fn_def(double);

#define bubble_sort(type, array, size) bubble_sort_##type(array, size)

#endif /* __SORT_H */
