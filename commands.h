/*!\file
*/

#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

	#define GENERATE_CMD(name, num, ...)\
		CMD_##name = num,
	
	typedef enum commands{
		#include "commands.dsl"
    } commands;

    #undef GENERATE_CMD

#endif