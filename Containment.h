#ifndef CONTAINMENT_H
#define CONTAINMENT_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// Define these to change how lenient we are with our capacity.
#ifndef CONTAINMENT_CAPACITY_RULE_PRE
#define CONTAINMENT_CAPACITY_RULE_PRE 2 *
#endif

#ifndef CONTAINMENT_CAPACITY_RULE_POST
#define CONTAINMENT_CAPACITY_RULE_POST + 2
#endif

#define DEFINE_ARRAY_TYPE(x) \
typedef struct \
{ \
	size_t length; \
	size_t capacity; \
	x * data; \
} x##_Array ; \
\
x##_Array x##_Array_Create(); \
x x##_Array_Get( x##_Array* a, size_t index); \
x* x##_Array_GetPtr( x##_Array* a, size_t index); \
void x##_Array_Set( x##_Array* a, size_t index, x value ); \
void x##_Array_Append( x##_Array* a, x item); \
void x##_Array_AppendMany( x##_Array* a, const x * items, size_t count); \
void x##_Array_PopBack( x##_Array* a); \
size_t x##_Array_Length( x##_Array* a); \
x * x##_Array_Begin( x##_Array* a); \
x * x##_Array_End( x##_Array* a); \
x x##_Array_Front( x##_Array* a); \
x x##_Array_Back( x##_Array* a); \
void x##_Array_Clear( x##_Array* a); \
void x##_Array_ShrinkToFit( x##_Array* a); \
void x##_Array_Resize( x##_Array* a, size_t length); \
void x##_Array_SetCapacity( x##_Array* a, size_t capacity ); \
void x##_Array_Reserve( x##_Array* a, size_t capacity); \
void x##_Array_Grow( x##_Array* a, size_t minCapacity); \
\
inline x##_Array x##_Array_Create() \
{ \
	x##_Array a; \
	a.length = 0; \
	a.capacity = 0; \
	a.data = NULL; \
	return a; \
} \
inline x x##_Array_Get( x##_Array* a, size_t index) \
{ \
	return a->data[index]; \
} \
inline x* x##_Array_GetPtr( x##_Array* a, size_t index) \
{ \
	return & (a->data[index]); \
} \
inline void x##_Array_Set( x##_Array* a, size_t index, x value ) \
{ \
	a->data[index] = value; \
} \
inline void x##_Array_Append( x##_Array* a, x item) \
{ \
	if (a->length + 1 > a->capacity) \
		x##_Array_Grow(a, a->length + 1); \
	a->data[a->length++] = item; \
}\
inline void x##_Array_AppendMany( x##_Array* a, const x * items, size_t count) \
{ \
	if (a->capacity <= a->length + count) \
		x##_Array_Grow(a, a->length + count); \
\
	memcpy(&a->data[a->length], items, count * sizeof( x ) ); \
	a->length += count; \
}\
inline void x##_Array_PopBack( x##_Array* a) \
{ \
	assert(a->length > 0); \
	a->length--; \
} \
inline size_t x##_Array_Length( x##_Array* a) \
{ \
	return a->length; \
} \
inline size_t x##_Array_Capacity(x##_Array* a) \
{ \
	return a->capacity; \
} \
inline x * x##_Array_Begin(x##_Array* a) \
{ \
	return a->data; \
} \
inline x * x##_Array_End(x##_Array* a) \
{ \
	return a->data + a->length; \
} \
inline x x##_Array_Front(x##_Array* a) \
{ \
	assert(a->length > 0); \
	return * ( x##_Array_Begin(a) ); \
} \
inline x x##_Array_Back(x##_Array* a) \
{ \
	assert(a->length > 0); \
	return * ( x##_Array_End(a) - 1 ); \
} \
inline void x##_Array_Clear(x##_Array* a) \
{ \
	x##_Array_Resize(a, 0); \
} \
inline void x##_Array_ShrinkToFit(x##_Array* a) \
{ \
	x##_Array_SetCapacity(a, a->length); \
} \
inline void x##_Array_Resize(x##_Array* a, size_t length) \
{ \
	if (length > a->capacity) \
		x##_Array_Grow(a, length); \
	a->length = length; \
} \
inline void x##_Array_SetCapacity( x##_Array* a, size_t capacity ) \
{ \
	if (capacity == a->capacity) \
		return; \
	\
	if (capacity < a->capacity) \
		x##_Array_Resize(a, capacity); \
\
	x * data = NULL; \
	if (capacity > 0) \
	{ \
		data = ( x * ) malloc( capacity * sizeof( x ) ); \
		memcpy(data, a->data, a->length * sizeof( x ) ); \
	} \
	free( a->data ); \
	a->data = data; \
	a->capacity = capacity; \
} \
inline void x##_Array_Reserve( x##_Array* a, size_t capacity) \
{ \
	if (capacity > a->capacity) \
		x##_Array_SetCapacity(a, capacity); \
} \
inline void x##_Array_Grow( x##_Array* a, size_t minCapacity) \
{ \
	size_t capacity = CONTAINMENT_CAPACITY_RULE_PRE a->capacity CONTAINMENT_CAPACITY_RULE_POST; \
	if (capacity < minCapacity) \
		capacity = minCapacity; \
	x##_Array_SetCapacity(a, capacity); \
}


DEFINE_ARRAY_TYPE(int32_t);

#endif