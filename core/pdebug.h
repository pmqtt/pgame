#ifndef PDEBUG_H
#define PDEBUG_H
#include <iostream>
#include <string>

#define P_UNUSED(x) (void)x;
#define BREAKPOINT() asm("int $3");
#endif
