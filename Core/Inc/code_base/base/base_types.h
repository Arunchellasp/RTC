#ifndef BASE_TYPES_H
#define BASE_TYPES_H

#include<stdlib.h>
#include<string.h>

// ==========================================================
//  Fixed-Width Custom Type Definitions (with descriptions)
// ==========================================================

// ---- Signed Integers ----
typedef int64_t  i64;   // 64-bit signed integer  | base: long long int | large numbers, timestamps, file sizes
typedef int32_t  i32;   // 32-bit signed integer  | base: int           | general-purpose integer, counters
typedef int16_t  i16;   // 16-bit signed integer  | base: short int     | sensor readings, short math
typedef int8_t   i08;   // 8-bit signed integer   | base: signed char   | small signed values, buffers
typedef int8_t   i8;    // 8-bit signed integer   | base: signed char   | same as i08

// ---- Unsigned Integers ----
typedef uint64_t u64;   // 64-bit unsigned integer | base: unsigned long long int | large counters, system uptime
typedef uint32_t u32;   // 32-bit unsigned integer | base: unsigned int           | IDs, array indexes, tick counts
typedef uint16_t u16;   // 16-bit unsigned integer | base: unsigned short int     | ADC data, pixel values
typedef uint8_t  u08;   // 8-bit unsigned integer  | base: unsigned char          | byte data, digital I/O, buffers
typedef uint8_t  u8;    // 8-bit unsigned integer  | base: unsigned char          | same as u08

// ---- Boolean / Bit Flags ----
typedef i64 b64;        // 64-bit boolean flag | base: long long int | alignment or large flag set
typedef i32 b32;        // 32-bit boolean flag | base: int           | CPU-aligned boolean/int flag
typedef i16 b16;        // 16-bit boolean flag | base: short int     | short-sized flag field
typedef i08 b08;        // 8-bit boolean flag  | base: signed char   | compact status flags
typedef i8  b8;         // 8-bit boolean flag  | base: signed char   | same as b08

// ---- Floating-Point (Real) ----
typedef float  r32;     // 32-bit floating point | base: float  | real numbers, coordinates, sensor data
typedef double r64;     // 64-bit floating point | base: double | precise calculations, time, distance



// ---------------------------------------------
// 1. internal → used for functions or variables 
//    that should be visible only in this file.
// ---------------------------------------------        
#define internal static

// ---------------------------------------------
// 2. local_persist → used for variables that
//    keep their value between function calls.
// ---------------------------------------------
#define local_persist static

// ---------------------------------------------
// 3. global_variable → used for variables that
//    are global but file-local (only visible inside this .c file).
// ---------------------------------------------
#define global_variable static


#define Glue_(A,B) A##B
#define Glue(A,B) Glue_(A,B)
#define Swap(T,a,b) do{T t__ = a; a = b; b = t__;}while(0)

#define Assert(exp) {if(!(exp)){while(1);}}
#define ArrayCount(arr)(sizeof(arr) / sizeof(arr[0]))
#define StaticAssert(exp, id) global_variable u8 Glue(id, __LINE__)[(exp)?1:-1]

#define Member(type, member) (((type *) 0)->member)
#define OffsetOf(type, member) (&Member(type, member)) 
#define InvalidCodePath Assert(!"Invalid code path")
#define InvalidDefaultCase default: {Invalid_Code_Path;} break;

#define Modf(X) (((X)<0.0f)?-(X):(X))
#define Max(A,B) ((A)>(B))?(A):(B)
#define Min(A,B) ((A)<(B))?(A):(B)
#define Clamp(A,X,B) (((X)<(A))?(A):((X)>(B))?(B):(X))
#define ClampBot(A,X) Max(A,X)
#define ClampTop(X,B) Min(X,B)



#define Map(x, in_min, in_max, out_min, out_max) \
((Clamp((in_min), (x), (in_max)) - (in_min)) * ((out_max) - (out_min)) / ((in_max) - (in_min)) + (out_min))


#define KB(val) (1024 * (val))
#define MB(val) (1024 * KB(val))
#define GB(val) (1024 * MB(val))

#define DeferIt(it) Glue(it, __LINE__)
#define DeferLoop(begin, end) for(int DeferIt(_i_) = ((begin), 0); !DeferIt(_i_); DeferIt(_i_) += 1, (end))
#define FOURCC(str) (((u32)(str[0]) << 0) | ((u32)(str[1]) << 8) | ((u32)(str[2]) << 16) | ((u32)(str[3]) << 24))

//
//~ rsb: Memory Operation Macros
//

#define MemoryCopy(dest, src, size) memmove((dest), (src), (size))
#define MemorySet(dest, byte, size) memset((dest), (byte), (size))

#define MemoryCopyStruct(d,s)       MemoryCopy((d), (s), sizeof(*(d)))
#define MemoryCopyArray(d,s)        MemoryCopy((d), (s), sizeof(d))

#define MemoryZero(s,z)             MemorySet((s), 0, (z))
#define MemoryZeroStruct(s)         MemoryZero((s), sizeof(*(s)))
#define MemoryZeroArray(a)          MemoryZero((a), sizeof((a)))

#define PI 3.14159265359f

#endif //TYPES_H
