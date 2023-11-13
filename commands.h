/*!\file
*/

#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

	#define GENERATE_CMD(name, num, ...)\
		CMD_##name = num,
	
	typedef enum commands{
		#include "commands_dsl.h"
    } commands;

    #undef GENERATE_CMD

#endif