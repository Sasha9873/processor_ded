/*!\file
*/

#ifndef PROCESSOR_STRUCT_H_INCLUDED
#define PROCESSOR_STRUCT_H_INCLUDED

    #include "third-party/stack_different_types/errors_enum_naming.h"
    #include "commands.h"
    #include "work_with_strs_from_file.h"

    #include <stdlib.h>
    #include <stdio.h>

    static const int MAX_FILE_NAME = 30;

    errors disassemble(char* buffer, size_t buff_size, FILE* file_ptr);

    static const int VERSION = 1;  //commands version
    static const int arg_mask = 0b10000;
    static const int reg_mask = 0b100000;


#endif
