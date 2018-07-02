#ifndef CONTAINMENT_H
#define CONTAINMENT_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#ifdef _WIN64
#define CONTAINMENT_64_BIT
#else
#define CONTAINMENT_32_BIT
#endif
#endif

#if defined(__x86_64__) || defined(__ppc64__) || defined(__aarch64__)
#define CONTAINMENT_64_BIT
#else
#define CONTAINMENT_32_BIT
#endif

// Define these to change how lenient we are with our capacity.
#ifndef CONTAINMENT_CAPACITY_RULE_PRE
#define CONTAINMENT_CAPACITY_RULE_PRE 2 *
#endif

#ifndef CONTAINMENT_CAPACITY_RULE_POST
#define CONTAINMENT_CAPACITY_RULE_POST + 2
#endif

#define CONTAINMENT_END_OF_LIST (size_t)(-1)

typedef struct
{
    size_t hashIndex;
    size_t dataPrev;
    size_t dataIndex;
} FindResult;

#define DEFINE_ARRAY_TYPE(x) \
typedef struct \
{ \
    size_t length; \
    size_t capacity; \
    x * data; \
} x##_Array ; \
\
x##_Array x##_Array_Create(void); \
void x##_Array_Free( x##_Array* a ); \
x x##_Array_Get( x##_Array* a, size_t index); \
x* x##_Array_GetPtr( x##_Array* a, size_t index); \
void x##_Array_Set( x##_Array* a, size_t index, x value ); \
size_t x##_Array_Append( x##_Array* a, x item); \
size_t x##_Array_AppendMany( x##_Array* a, x * items, size_t count); \
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
inline x##_Array x##_Array_Create(void) \
{ \
    x##_Array a; \
    a.length = 0; \
    a.capacity = 0; \
    a.data = NULL; \
    return a; \
} \
inline void x##_Array_Free( x##_Array* a ) \
{ \
	free(a->data); \
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
inline size_t x##_Array_Append( x##_Array* a, x item) \
{ \
    if (a->length + 1 > a->capacity) \
        x##_Array_Grow(a, a->length + 1); \
    a->data[a->length++] = item; \
    return a->length; \
}\
inline size_t x##_Array_AppendMany( x##_Array* a, x * items, size_t count) \
{ \
    if (a->capacity <= a->length + count) \
        x##_Array_Grow(a, a->length + count); \
\
    memcpy(&a->data[a->length], items, count * sizeof( x ) ); \
    a->length += count; \
    return a->length; \
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

DEFINE_ARRAY_TYPE(size_t);

// Note! Must have array for this type defined beforehand.
#define DEFINE_HASHMAP_TYPE(x) \
typedef x *Containment_Ptr_##x ; \
DEFINE_ARRAY_TYPE( Containment_Ptr_##x ); \
typedef struct \
{ \
    uint64_t key; \
    size_t next; \
    x value; \
} x##_Hashmap_Entry; \
DEFINE_ARRAY_TYPE(x##_Hashmap_Entry); \
\
typedef struct \
{ \
    size_t_Array hashes; \
    x##_Hashmap_Entry_Array data; \
} x##_Hashmap; \
\
x##_Hashmap x##_Hashmap_Create(void); \
void x##_Hashmap_Free( x##_Hashmap* h); \
size_t x##_Hashmap_Impl_AddEntry( x##_Hashmap* h, uint64_t key); \
void x##_Hashmap_Impl_Erase( x##_Hashmap* h, FindResult* fr); \
FindResult x##_Hashmap_Impl_FindByKey( x##_Hashmap* h, uint64_t key); \
FindResult x##_Hashmap_Impl_FindByEntry( x##_Hashmap* h, x##_Hashmap_Entry* e); \
size_t x##_Hashmap_Impl_Make( x##_Hashmap* h, uint64_t key); \
void x##_Hashmap_Impl_FindAndErase(x##_Hashmap* h, uint64_t key); \
size_t x##_Hashmap_Impl_FindOrFail(x##_Hashmap* h, uint64_t key); \
size_t x##_Hashmap_Impl_FindOrMake( x##_Hashmap* h, uint64_t key); \
void x##_Hashmap_Impl_Rehash( x##_Hashmap* h, size_t newCapacity); \
void x##_Hashmap_Impl_Grow( x##_Hashmap* h); \
char x##_Hashmap_Impl_Full( x##_Hashmap* h); \
char x##_Hashmap_Has( x##_Hashmap* h, uint64_t key); \
x x##_Hashmap_Get( x##_Hashmap* h, uint64_t key, x defaultValue ); \
x * x##_Hashmap_GetPtr( x##_Hashmap* h, uint64_t key, x * defaultValue ); \
size_t x##_Hashmap_Set( x##_Hashmap* h, uint64_t key, x value ); \
size_t x##_Hashmap_SetPtr( x##_Hashmap* h, uint64_t key, x* value ); \
void x##_Hashmap_Remove( x##_Hashmap* h, uint64_t key ); \
void x##_Hashmap_Reserve( x##_Hashmap* h, size_t capacity ); \
void x##_Hashmap_Clear( x##_Hashmap* h ); \
void x##_Hashmap_Multi_Get( x##_Hashmap* h, uint64_t key, x##_Array * items ); \
void x##_Hashmap_Multi_GetPtrs( x##_Hashmap* h, uint64_t key, Containment_Ptr_##x##_Array * items ); \
size_t x##_Hashmap_Multi_Count( x##_Hashmap* h, uint64_t key ); \
void x##_Hashmap_Multi_Insert( x##_Hashmap* h, uint64_t key, x value ); \
void x##_Hashmap_Multi_InsertMany( x##_Hashmap* h, uint64_t key, x * values, size_t count ); \
x##_Hashmap_Entry* x##_Hashmap_Multi_FindFirst( x##_Hashmap* h, uint64_t key); \
x##_Hashmap_Entry* x##_Hashmap_Multi_FindNext( x##_Hashmap* h, x##_Hashmap_Entry* e); \
void x##_Hashmap_Multi_Remove( x##_Hashmap* h, x##_Hashmap_Entry* e); \
void x##_Hashmap_Multi_RemoveAll(x##_Hashmap* h, uint64_t key); \
\
inline x##_Hashmap x##_Hashmap_Create(void) \
{ \
    x##_Hashmap hashmap; \
    hashmap.hashes = size_t_Array_Create(); \
    hashmap.data = x##_Hashmap_Entry_Array_Create(); \
	return hashmap; \
} \
inline void x##_Hashmap_Free( x##_Hashmap* h ) \
{ \
	size_t_Array_Free(&h->hashes); \
	x##_Hashmap_Entry_Array_Free(&h->data); \
} \
inline size_t x##_Hashmap_Impl_AddEntry( x##_Hashmap* h, uint64_t key) \
{ \
    x##_Hashmap_Entry e; \
    e.key = key; \
    e.next = CONTAINMENT_END_OF_LIST; \
    size_t eIndex = h->data.length; \
    x##_Hashmap_Entry_Array_Append(&h->data, e); \
    return eIndex; \
} \
inline void x##_Hashmap_Impl_Erase( x##_Hashmap* h, FindResult* fr) \
{ \
    if (fr->dataPrev == CONTAINMENT_END_OF_LIST) \
        h->hashes.data[fr->hashIndex] = h->data.data[fr->dataIndex].next; \
    else \
        h->data.data[fr->dataPrev].next = h->data.data[fr->dataIndex].next; \
    \
    x##_Hashmap_Entry_Array_PopBack(&h->data); \
    \
    if (fr->dataIndex == h->data.length) \
        return; \
    \
    FindResult last = x##_Hashmap_Impl_FindByKey(h, h->data.data[fr->dataIndex].key); \
    \
    if (last.dataPrev == CONTAINMENT_END_OF_LIST) \
        h->hashes.data[last.hashIndex] = fr->dataIndex; \
    else \
        h->data.data[last.dataIndex].next = fr->dataIndex; \
} \
inline FindResult x##_Hashmap_Impl_FindByKey( x##_Hashmap* h, uint64_t key) \
{ \
    FindResult fr; \
    fr.hashIndex = CONTAINMENT_END_OF_LIST; \
    fr.dataPrev = CONTAINMENT_END_OF_LIST; \
    fr.dataIndex = CONTAINMENT_END_OF_LIST; \
    \
    if (h->hashes.length == 0) \
        return fr; \
    \
    fr.hashIndex = key % h->hashes.length; \
    fr.dataIndex = h->hashes.data[fr.hashIndex]; \
    while (fr.dataIndex != CONTAINMENT_END_OF_LIST) \
    { \
        if (h->data.data[fr.dataIndex].key == key) \
            return fr; \
        fr.dataPrev = fr.dataIndex; \
        fr.dataIndex = h->data.data[fr.dataIndex].next; \
    } \
    \
    return fr; \
} \
inline FindResult x##_Hashmap_Impl_FindByEntry( x##_Hashmap* h, x##_Hashmap_Entry* e) \
{ \
    FindResult fr; \
    fr.hashIndex = CONTAINMENT_END_OF_LIST; \
    fr.dataPrev = CONTAINMENT_END_OF_LIST; \
    fr.dataIndex = CONTAINMENT_END_OF_LIST; \
\
    if (h->hashes.length == 0 || !e) \
        return fr; \
\
    fr.hashIndex = e->key % h->hashes.length; \
    fr.dataIndex = h->hashes.data[fr.hashIndex]; \
\
    while (fr.dataIndex != CONTAINMENT_END_OF_LIST) \
    { \
        if (&h->data.data[fr.dataIndex] == e) \
            return fr; \
        fr.dataPrev = fr.dataIndex; \
        fr.dataIndex = h->data.data[fr.dataIndex].next; \
    } \
\
    return fr;\
} \
inline size_t x##_Hashmap_Impl_Make( x##_Hashmap* h, uint64_t key) \
{ \
    FindResult fr = x##_Hashmap_Impl_FindByKey(h, key); \
    size_t index = x##_Hashmap_Impl_AddEntry(h, key); \
    \
    if (fr.dataPrev == CONTAINMENT_END_OF_LIST) \
        h->hashes.data[fr.hashIndex] = index; \
    else \
        h->data.data[fr.dataPrev].next = index; \
\
    h->data.data[index].next = fr.dataIndex; \
    return index; \
} \
inline void x##_Hashmap_Impl_FindAndErase( x##_Hashmap* h, uint64_t key) \
{ \
    FindResult fr = x##_Hashmap_Impl_FindByKey(h, key); \
    if (fr.dataIndex != CONTAINMENT_END_OF_LIST) \
        x##_Hashmap_Impl_Erase(h, &fr); \
} \
inline size_t x##_Hashmap_Impl_FindOrFail( x##_Hashmap* h, uint64_t key) \
{ \
    return x##_Hashmap_Impl_FindByKey(h, key).dataIndex; \
} \
inline size_t x##_Hashmap_Impl_FindOrMake( x##_Hashmap* h, uint64_t key) \
{ \
    FindResult fr = x##_Hashmap_Impl_FindByKey(h, key); \
    if (fr.dataIndex != CONTAINMENT_END_OF_LIST) \
        return fr.dataIndex; \
\
    size_t index = x##_Hashmap_Impl_AddEntry(h, key); \
    if (fr.dataPrev == CONTAINMENT_END_OF_LIST) \
        h->hashes.data[fr.hashIndex] = index; \
    else \
        h->data.data[fr.dataPrev].next = index; \
    return index; \
} \
inline void x##_Hashmap_Impl_Rehash( x##_Hashmap* h, size_t newCapacity) \
{ \
    x##_Hashmap nh = x##_Hashmap_Create(); \
    size_t_Array_Resize(&nh.hashes, newCapacity); \
    size_t oldLength = h->data.length; \
    x##_Hashmap_Entry_Array_Reserve(&nh.data, oldLength); \
\
    for (size_t i = 0; i < newCapacity; i++) \
        nh.hashes.data[i] = CONTAINMENT_END_OF_LIST; \
\
    for (size_t i = 0; i < oldLength; i++) \
    { \
        x##_Hashmap_Entry* e = &h->data.data[i]; \
        x##_Hashmap_Multi_Insert(&nh, e->key, e->value); \
    } \
    \
    memcpy(h, &nh, sizeof(nh)); \
    nh = x##_Hashmap_Create(); \
} \
inline void x##_Hashmap_Impl_Grow( x##_Hashmap* h) \
{ \
    size_t newCapacity = CONTAINMENT_CAPACITY_RULE_PRE h->data.length CONTAINMENT_CAPACITY_RULE_POST ; \
    x##_Hashmap_Impl_Rehash(h, newCapacity); \
} \
inline char x##_Hashmap_Impl_Full( x##_Hashmap* h) \
{ \
    float maxLoadCoefficient = 0.75f; \
    return h->data.length >= maxLoadCoefficient * h->hashes.length ? 1 : 0; \
} \
inline char x##_Hashmap_Has( x##_Hashmap* h, uint64_t key) \
{ \
    return x##_Hashmap_Impl_FindOrFail(h, key) != CONTAINMENT_END_OF_LIST; \
} \
inline x x##_Hashmap_Get( x##_Hashmap* h, uint64_t key, x defaultValue ) \
{ \
    size_t index = x##_Hashmap_Impl_FindOrFail(h, key); \
    if (index == CONTAINMENT_END_OF_LIST) \
        return defaultValue; \
    return h->data.data[index].value; \
} \
inline x * x##_Hashmap_GetPtr( x##_Hashmap* h, uint64_t key, x * defaultValue ) \
{ \
    size_t index = x##_Hashmap_Impl_FindOrFail(h, key); \
    if (index == CONTAINMENT_END_OF_LIST) \
        return defaultValue; \
    return &h->data.data[index].value; \
} \
inline size_t x##_Hashmap_Set( x##_Hashmap* h, uint64_t key, x value ) \
{ \
    if (h->hashes.length == 0) \
        x##_Hashmap_Impl_Grow(h); \
\
    size_t index = x##_Hashmap_Impl_FindOrMake(h, key); \
    h->data.data[index].value = value; \
    if (x##_Hashmap_Impl_Full(h)) \
        x##_Hashmap_Impl_Grow(h); \
	return index; \
} \
inline size_t x##_Hashmap_SetPtr( x##_Hashmap* h, uint64_t key, x* value ) \
{ \
    if (h->hashes.length == 0) \
        x##_Hashmap_Impl_Grow(h); \
\
    size_t index = x##_Hashmap_Impl_FindOrMake(h, key); \
    h->data.data[index].value = *value; \
    if (x##_Hashmap_Impl_Full(h)) \
        x##_Hashmap_Impl_Grow(h); \
	return index; \
} \
inline void x##_Hashmap_Remove( x##_Hashmap* h, uint64_t key ) \
{ \
    x##_Hashmap_Impl_FindAndErase(h, key); \
} \
inline void x##_Hashmap_Reserve( x##_Hashmap* h, size_t capacity ) \
{ \
    x##_Hashmap_Impl_Rehash(h, capacity); \
} \
inline void x##_Hashmap_Clear( x##_Hashmap* h ) \
{ \
    size_t_Array_Clear(&h->hashes); \
    x##_Hashmap_Entry_Array_Clear(&h->data); \
} \
inline void x##_Hashmap_Multi_Get( x##_Hashmap* h, uint64_t key, x##_Array * items ) \
{ \
    x##_Hashmap_Entry* e = x##_Hashmap_Multi_FindFirst(h, key); \
    while (e) \
    {\
        x##_Array_Append(items, e->value); \
        e = x##_Hashmap_Multi_FindNext(h, e); \
    } \
} \
inline void x##_Hashmap_Multi_GetPtrs( x##_Hashmap* h, uint64_t key, Containment_Ptr_##x##_Array * items ) \
{ \
    x##_Hashmap_Entry* e = x##_Hashmap_Multi_FindFirst(h, key); \
    while (e) \
    {\
        Containment_Ptr_##x##_Array_Append(items, & (e->value) ); \
        e = x##_Hashmap_Multi_FindNext(h, e); \
    } \
} \
inline size_t x##_Hashmap_Multi_Count( x##_Hashmap* h, uint64_t key ) \
{ \
    x##_Hashmap_Entry* e = x##_Hashmap_Multi_FindFirst(h, key); \
    size_t c = 0; \
    while (e) \
    {\
        c++; \
        e = x##_Hashmap_Multi_FindNext(h, e); \
    } \
    return c; \
} \
inline void x##_Hashmap_Multi_Insert( x##_Hashmap* h, uint64_t key, x value ) \
{ \
    if (h->hashes.length == 0) \
        x##_Hashmap_Impl_Grow(h); \
\
    size_t next = x##_Hashmap_Impl_Make(h, key); \
    h->data.data[next].value = value; \
\
    if (x##_Hashmap_Impl_Full(h)) \
        x##_Hashmap_Impl_Grow(h); \
} \
inline void x##_Hashmap_Multi_InsertMany( x##_Hashmap* h, uint64_t key, x * values, size_t count ) \
{ \
    for (size_t i = 0; i < count; i++) \
    { \
        x##_Hashmap_Multi_Insert(h, key, values[i]); \
    } \
} \
inline x##_Hashmap_Entry* x##_Hashmap_Multi_FindFirst( x##_Hashmap* h, uint64_t key) \
{ \
	size_t index = x##_Hashmap_Impl_FindOrFail(h, key); \
	if (index == CONTAINMENT_END_OF_LIST) \
		return NULL; \
	return &h->data.data[index]; \
} \
inline x##_Hashmap_Entry* x##_Hashmap_Multi_FindNext( x##_Hashmap* h, x##_Hashmap_Entry* e) \
{ \
	if (!e) \
		return NULL; \
\
	size_t index = e->next; \
	while (index != CONTAINMENT_END_OF_LIST) \
	{ \
		if (h->data.data[index].key == e->key) \
			return &h->data.data[index]; \
		index = h->data.data[index].next; \
	} \
	return NULL; \
} \
inline void x##_Hashmap_Multi_Remove( x##_Hashmap* h, x##_Hashmap_Entry* e) \
{ \
	FindResult fr = x##_Hashmap_Impl_FindByEntry(h, e); \
	if (fr.dataIndex != CONTAINMENT_END_OF_LIST) \
		x##_Hashmap_Impl_Erase(h, &fr); \
} \
inline void x##_Hashmap_Multi_RemoveAll(x##_Hashmap* h, uint64_t key) \
{ \
	while (x##_Hashmap_Has(h, key)) \
		x##_Hashmap_Remove(h, key); \
}

DEFINE_HASHMAP_TYPE(size_t)

#define DEFINE_CONTAINMENT_TYPES(x) \
	DEFINE_ARRAY_TYPE(x) \
	DEFINE_HASHMAP_TYPE(x) \

DEFINE_CONTAINMENT_TYPES(char)

DEFINE_CONTAINMENT_TYPES(int8_t)
DEFINE_CONTAINMENT_TYPES(uint8_t)

DEFINE_CONTAINMENT_TYPES(int16_t)
DEFINE_CONTAINMENT_TYPES(uint16_t)

DEFINE_CONTAINMENT_TYPES(int32_t)
DEFINE_CONTAINMENT_TYPES(uint32_t)

DEFINE_CONTAINMENT_TYPES(int64_t)
DEFINE_CONTAINMENT_TYPES(uint64_t)

// Murmur Hash, Source: https://code.google.com/p/smhasher/
inline uint64_t ContainmentHashCustomSeed(const void* key, int32_t len, uint64_t seed)
{
#ifdef CONTAINMENT_64_BIT
	const uint64_t m = 0xc6a4a7935bd1e995ULL;
	const int32_t r = 47;

	uint64_t h = seed ^ (len * m);

	const uint64_t* data = (const uint64_t*)key;
	const uint64_t* end = data + (len / 8);

	while (data != end)
	{
		uint64_t k = *data++;

		k *= m;
		k ^= k >> r;
		k *= m;

		h ^= k;
		h *= m;
	}

	const uint8_t* data2 = (const uint8_t*)data;

	switch (len & 7)
	{
	case 7: h ^= ((uint64_t)( data2[6] )) << 48;
	case 6: h ^= ((uint64_t)( data2[5] )) << 40;
	case 5: h ^= ((uint64_t)( data2[4] )) << 32;
	case 4: h ^= ((uint64_t)( data2[3] )) << 24;
	case 3: h ^= ((uint64_t)( data2[2] )) << 16;
	case 2: h ^= ((uint64_t)( data2[1] )) << 8;
	case 1: h ^= ((uint64_t)( data2[0] ));
			h *= m;
	};

	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
#else
	const uint32_t m = 0x5bd1e995;
	const int32_t r = 24;

	uint32_t h1 = ((uint32_t)(seed)) ^ len;
	uint32_t h2 = (uint32_t)(seed >> 32);

	const uint32_t* data = (const uint32_t*)key;

	while (len >= 8)
	{
		uint32_t k1 = *data++;
		k1 *= m;
		k1 ^= k1 >> r;
		k1 *= m;
		h1 *= m;
		h1 ^= k1;
		len -= 4;

		uint32_t k2 = *data++;
		k2 *= m;
		k2 ^= k2 >> r;
		k2 *= m;
		h2 *= m;
		h2 ^= k2;
		len -= 4;
	}

	if (len >= 4)
	{
		uint32_t k1 = *data++;
		k1 *= m;
		k1 ^= k1 >> r;
		k1 *= m;
		h1 *= m;
		h1 ^= k1;
		len -= 4;
	}

	switch (len)
	{
	case 3: h2 ^= ((uint8_t*)data)[2] << 16;
	case 2: h2 ^= ((uint8_t*)data)[1] << 8;
	case 1: h2 ^= ((uint8_t*)data)[0];
		h2 *= m;
	};

	h1 ^= h2 >> 18;
	h1 *= m;
	h2 ^= h1 >> 22;
	h2 *= m;
	h1 ^= h2 >> 17;
	h1 *= m;
	h2 ^= h1 >> 19;
	h2 *= m;

	uint64_t h = h1;

	h = (h << 32) | h2;

	return h;
#endif
}

inline uint64_t ContainmentHash(const void* key, int32_t len)
{
	return ContainmentHashCustomSeed(key, len, 0x9747b28c);
}
inline uint64_t ContainmentHashStringCustomSeed(const char* key, uint64_t seed)
{
	return ContainmentHashCustomSeed(key, (int32_t) strlen(key), seed);
}
inline uint64_t ContainmentHashString(const char* key)
{
	return ContainmentHash(key, (int32_t) strlen(key));
}

#ifndef CONTAINMENT_DISABLE_SORTHAND
inline uint64_t hash_str(const char* key)
{
	return ContainmentHashString(key);
}
#endif

#endif
