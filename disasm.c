/*!\file
*/

#include "disasm.h"

errors disassemble(int* buffer, size_t buff_size, FILE* file_ptr)
{
	if(!buffer)
		return BAD_POINTER;

	int error = ALL_OK;

	int cmd;


	for(size_t cur_cmd_num = 0; cur_cmd_num < buff_size; ++cur_cmd_num)
	{
		cmd = buffer[cur_cmd_num];

		switch(cmd)
		{
			case CMD_PUSH:
			{
				fprintf(file_ptr, "push %d\n", buffer[++cur_cmd_num]);

				break;
			}
			
			case CMD_REG_PUSH:
			{
				fprintf(file_ptr, "push r%cx\n", buffer[++cur_cmd_num] + 'a');

				break;
			}

			case CMD_POP:
			{
				fprintf(file_ptr, "pop\n");

				break;
			}
			
			case CMD_REG_POP:
			{
				fprintf(file_ptr, "pop r%cx\n", buffer[++cur_cmd_num] + 'a');

				break;
			}

			case CMD_IN:
			{
				
				fprintf(file_ptr, "in\n");

				break;
			} 

			case CMD_ADD:
			{
				fprintf(file_ptr, "add\n");

				break;
			} 

			case CMD_SUB:
			{
				fprintf(file_ptr, "sub\n");

				break;
			} 

			case CMD_MUL:
			{
				fprintf(file_ptr, "mul\n");

				break;
			} 

			case CMD_DIV:
			{
				
				fprintf(file_ptr, "div\n");

				break;
			} 

			case CMD_OUT:
			{
				fprintf(file_ptr, "out\n");

				break;
			} 

			case CMD_HLT:
			{
				fprintf(file_ptr, "hlt\n");

				break;
			}	
			
		}
	}

	return error;
}

int main(int argc, char** argv)
{
	errors error = ALL_OK;
	size_t buff_size = 0;
	
	char file_name[MAX_FILE_NAME];

	if(argc < 2)  //no file name
	{
		strncpy(file_name, "code.out", MAX_FILE_NAME - 1);
	}
	else
	{
		strncpy(file_name, argv[1], MAX_FILE_NAME - 1);
	}

	int* buffer = read_text_from_file_to_buff_for_proc(file_name, &error, &buff_size);
	if(error != ALL_OK)
	{
		fprintf(stderr, RED "error = %d\n" RST, error);
		return error;
	}

	FILE* file_ptr = open_file("disasm.txt", "wb", &error);

	fprintf(file_ptr, ";Disasembly of %s\n\n", file_name);

	error = disassemble(buffer, buff_size, file_ptr);
	if(error != ALL_OK)
	{
		if(buffer)
		{
			free(buffer);
			buffer = NULL;
		}
		fprintf(stderr, RED "error = %d\n" RST, error);
		return error;
	}

	fclose(file_ptr);

	return 0;
}