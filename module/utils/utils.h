#ifndef _UTILS_H_
#define _UTILS_H_


#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define IS_POWER_OF_TWO(A) ( ((A) != 0) && ((((A) - 1) & (A)) == 0) )
#define array_size(array) (sizeof(array)/sizeof(*array))

#endif // _UTILS_H_
