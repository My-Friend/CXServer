#pragma once
#include <iostream>
#include <assert.h>
#include <stdint.h>
#include <set>
#include <map>
#include <hash_map>
#include <list>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include "../common/tool/noncopyable.h"
#include "../common/tool/ScopeGuard.h"

using namespace std;

#define STATIC_ASSERT_ARRAY_LENGTH(arr, len) static_assert(sizeof(arr)/sizeof(arr[0])==(len), #arr)
#define STATIC_ASSERT_ARRAY_ARRAY(arrA, arrB) static_assert(sizeof(arrA)/sizeof(arrA[0])==sizeof(arrB)/sizeof(arrB[0]), #arrA)

#define ZeroMemoryThis          memset(this, 0, sizeof(*this))
#define ZeroMemoryArray(arr)    memset(arr, 0, sizeof(arr))

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))
#define ONE_DAY_SEC   (24*3600)

template <typename T> int SUM_ARR(T* arr, int size){
    int sum(0);
    for (int j = 0; j < size; ++j) sum += arr[j];
    return sum;
}

typedef int64_t		int64;
typedef int32_t		int32;
typedef int16_t		int16;
typedef int8_t		int8;
typedef uint64_t	uint64;
typedef uint32_t	uint32;
typedef uint16_t	uint16;
typedef uint8_t		uint8;

typedef unsigned int		uint;

typedef std::vector< std::pair<int, int> > IntPairVec;
