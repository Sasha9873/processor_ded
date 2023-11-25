/*!\file
*/

	#ifndef ASSEMBLER_H_INCLUDED
	#define ASSEMBLER_H_INCLUDED

	#include "work_with_strs_from_file.h"
	#include "commands.h"

	#include <stdarg.h>
	#include <unistd.h>
	#include <stdio.h>


	static const int MAX_FILE_NAME = 30;
	static const int MAX_COMMAND_LEN = 10;
	static const int N_REGS = 4;

	static const int VERSION = 2;  //commands version
	static const int ARG_MASK = 0x20;
	static const int REG_MASK = 0x40;

#endif
