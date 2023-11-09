/*!\file
*/

#ifndef PROCESSOR_STRUCT_H_INCLUDED
#define PROCESSOR_STRUCT_H_INCLUDED

	#include "third-party/stack_different_types/stack_struct.h"
    #include "cpu_errors_enum_naming.h"

	#define DEBUG 1


    #if DEBUG == 0
        #define CHECKPROC(reason)\
            if(!proc || stack_ok(proc->stk) != ALL_OK || reason != ALL_OK){\
                if(proc->stk && proc->file_with_cpu_errors)\
                    fprintf(proc->file_with_cpu_errors, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);\
                else\
                    fprintf(stderr, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);\
                proc_dump(proc, reason);\
            }
    #else
        #define CHECKPROC(reason)\
            if(proc->stk && proc->file_with_cpu_errors)\
                fprintf(proc->file_with_cpu_errors, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);\
            else\
                fprintf(stderr, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);\
            proc_dump(proc, reason);
    #endif //DEBUG

    #undef DEBUG

	static const int N_REGS = 4;

	typedef struct Proc{

		Stack* stk;

		int* regs;

		char* code;

		size_t n_commands;
		size_t cur_cmd_num;

		FILE* file_with_cpu_errors;
	}Proc;


#endif
