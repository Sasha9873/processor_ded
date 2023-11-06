/*!\file
*/

#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

	#include "stack_struct.h"

	#define DEBAG 1


    #if DEBAG == 0
        #define CHECKPROC(reason)\
            if(!proc || stack_ok(proc->stk) != ALL_OK || reason != ALL_OK){\
                if(proc->stk && proc->file_with_errors)\
                    fprintf(proc->file_with_errors, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);\
                else\
                    fprintf(stderr, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);\
                proc_dump(proc, reason);\
            }
    #else
        #define CHECKPROC(reason)\
            if(proc->stk && proc->file_with_errors)\
                fprintf(proc->file_with_errors, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);\
            else\
                fprintf(stderr, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);\
            proc_dump(proc, reason);
    #endif //DEBAG

    #undef DEBAG

	static const int N_REGS = 4;

	typedef struct Proc{

		Stack* stk;

		int* regs;

		char* code;

		size_t n_commands;

		FILE* file_with_errors;
	}Proc;

	/**
     * Creates(allocates memory) data(massif with numbers which we push and pop from stack).
     *
     * @param [in] *error  Pointer to error. At the end of the program 0 if all is normal, NOT_MEMORY if we have not enough memory, 
     * BAD_POINTER if stk == NULL.
     *
     * @returns Pointer to stack
     */
    Proc* proc_ctor(errors* error, ...);

#endif
