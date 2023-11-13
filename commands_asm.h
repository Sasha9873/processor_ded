/*!\file
*/

#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED
	
	typedef enum commands{
		CMD_HLT  	 = -1,
		CMD_OUT  	 = 1,
		CMD_IN 	 	 = 2,
		CMD_PUSH	 = 3,
		CMD_POP  	 = 4,
		CMD_ADD  	 = 5,
		CMD_SUB  	 = 6,
		CMD_MUL  	 = 7,
		CMD_DIV  	 = 8,
		CMD_REG_POP  = 9,
		CMD_REG_PUSH = 10,
    } commands;

    static const int VERSION = 1;  //commands version

#endif