/*!\file
*/

#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

	#include "stack_struct.h"

	static const int N_REGS = 4;

	typedef struct Proc{

		Stack* stk;

		int* regs;

		int* code;
	}Proc;

	/**
     * Creates(allocates memory) data(massif with numbers which we push and pop from stack).
     *
     * @param [in] *error  Pointer to error. At the end of the program 0 if all is normal, NOT_MEMORY if we have not enough memory, 
     * BAD_POINTER if stk == NULL.
     *
     * @returns Pointer to stack
     */
    Proc* proc_ctor(errors* error);

#endif
