/*!\file
*/

#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

#include "stack_struct.h"

static const int N_REGS = 4;

struct proc{

	Stack* stk;

	int* regs;

	int* code;
};

#endif
