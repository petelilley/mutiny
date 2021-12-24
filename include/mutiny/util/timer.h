#ifndef __MT_TIMER_H__
#define __MT_TIMER_H__

#include <time.h>

#define BEGIN_TIMER(x) double x = clock()
#define END_TIMER(x)   x = (double)(clock() - x) / CLOCKS_PER_SEC

#endif // __MT_TIMER_H__
