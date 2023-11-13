/*!\file
*/

#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

	#include "third-party/stack_different_types/stack_struct.h"
	#include "third-party/stack_different_types/stack_func.h"
	#include "processor_struct.h"
	#include "commands.h"
	#include "work_with_strs_from_file.h"

	#include <stdarg.h>

	static const size_t MAX_FILE_NAME = 30;
	static const int VERSION = 1;  //commands version

	#define RED "\033[0;31m"
    #define RST "\x1b[0m" 
    #define GREEN "\033[0;32m"
    #define ORANGE "\033[0;33m"
    #define BLUE "\033[0;34m"
    #define GREY "\033[0;2m"

    #define BOLD "\033[0;1m"


	/**
     * Creates(allocates memory) data(massif with numbers which we push and pop from stack).
     *
     * @param [in] *error  Pointer to error. At the end of the program 0 if all is normal, NOT_MEMORY if we have not enough memory, 
     * BAD_POINTER if stk == NULL.
     *
     * @returns Pointer to stack
     */
    Proc* proc_ctor(errors* error, ...); //can give char* buffer and its length in __VA_ARGS__
	Proc* proc_dtor(Proc* proc);

	size_t proc_ok(Proc* proc);
	void print_parse_cpu_error(cpu_errors error, ...); //in va_args file_ptr
	int proc_dump(Proc* proc, cpu_errors reason);

	int run_proc(Proc* proc);

#endif
