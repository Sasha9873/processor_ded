#include "work_with_strs_from_file.h"
#include "commands.h"

#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>


static const int MAX_FILE_NAME = 30;
static const int MAX_COMMAND_LEN = 10;
static const int N_REGS = 4;

int is_reg(char* str)
{
	if(strlen(str) < 3)
		return 0;

	for(size_t i = 3; i < strlen(str); ++i)
	{
		if(str[i] != ' ' && str[i] != '\n' && str[i] != '\t')
		{
			printf("str[%lu] = <%c> aaaaa\n", i, str[i]);
			return 0;
		}
	}
		

	if (str[0] != 'r' || str[2] != 'x')
		return 0;

	if((str[1] - 'a') >= 0 && (str[1] - 'a') < N_REGS)
		return 1;
	else
		return 0;
}

void print_in_two_files(int n_numbers, FILE* first_file_ptr, FILE* second_file_ptr, ...)  //args
{
	if(!first_file_ptr || !second_file_ptr)
		return;

	va_list args;
	va_start(args, second_file_ptr);

	if(n_numbers)
	{
		if(!args)
			return;

		int val = va_arg(args, int);

		fprintf(first_file_ptr, "%d", val);
		fprintf(second_file_ptr, "%c", val);
	}
		
	while(--n_numbers && args)
	{
		int val = va_arg(args, int);

		fprintf(first_file_ptr, " %d", val);
		fprintf(second_file_ptr, "%c", val);
	}

	fprintf(first_file_ptr, "\n");

	va_end(args);
}

errors print_in_file_and_buff(size_t n_numbers, FILE* file_ptr, int* buffer, size_t* index, ...)  //args
{
	if(!file_ptr || !buffer)
		return BAD_POINTER;

	va_list args;
	va_start(args, index);


	if(n_numbers)
	{
		if(!args)
			return BAD_POINTER;

		int val = va_arg(args, int);

		fprintf(file_ptr, "%d", val);
		buffer[(*index)++] = val;
	}
		
	while(--n_numbers && args)
	{
		int val = va_arg(args, int);

		fprintf(file_ptr, " %d", val);
		buffer[(*index)++] = val;
	}

	fprintf(file_ptr, "\n");

	va_end(args);
}

errors assemble(file_information* file_info)
{
	errors error = ALL_OK;

	FILE* file_to_write = open_file("after_assembler.txt", "wb", &error);
	if(error != ALL_OK)
	{
		fprintf(stderr, "error = %d\n", error);
		return error;
	}

	FILE* file_with_code = open_file("code.out", "wb", &error);
	if(error != ALL_OK)
	{
		fclose(file_to_write);
		fprintf(stderr, RED "error = %d\n" RST, error);
		return error;
	}


	int* buffer = calloc(file_info->n_strings, sizeof(int));
	size_t index = 0;


	for(size_t cur_str = 0; cur_str < file_info->n_strings; ++cur_str)
	{
		if(!file_info->text[cur_str])
			break;
		
		if(!strncmp(file_info->text[cur_str], "push", MAX_COMMAND_LEN))
		{
			++cur_str;

			if(is_reg(file_info->text[cur_str]))
			{
				// fprintf(file_to_write, "%d %d\n", CMD_PUSH, file_info->text[cur_str][1] - 'a' + 1);
				// fprintf(file_with_code, "%d%d", CMD_PUSH, file_info->text[cur_str][1] - 'a' + 1);
				print_in_file_and_buff(2, file_to_write, buffer, &index, CMD_REG_PUSH, file_info->text[cur_str][1] - 'a');
			}
			else
			{
				int arg = atoi(file_info->text[cur_str]);

				if(arg == 0 && file_info->text[cur_str][0] != '0')
					fprintf(stderr, RED "Wrong reg, you have written: %s\n" RST, file_info->text[cur_str]);

				// fprintf(file_to_write, "%d %d\n", CMD_PUSH, arg);
				// fprintf(file_with_code, "%d%d", CMD_PUSH, arg);
				print_in_file_and_buff(2, file_to_write, buffer, &index, CMD_PUSH, arg);
			}	
		}

		else if(!strncmp(file_info->text[cur_str], "pop", MAX_COMMAND_LEN))
		{
			if(is_reg(file_info->text[cur_str + 1]))
			{
				++cur_str;
				// fprintf(file_to_write, "%d %d\n", CMD_POP, file_info->text[cur_str][1] - 'a' + 1);
				// fprintf(file_with_code, "%d%d", CMD_POP, file_info->text[cur_str][1] - 'a' + 1);
				print_in_file_and_buff(2, file_to_write, buffer, &index, CMD_REG_POP, file_info->text[cur_str][1] - 'a');
			}
			else
			{
			// 	fprintf(file_to_write, "%d\n", CMD_POP);
			// 	fprintf(file_with_code, "%d", CMD_POP);
				print_in_file_and_buff(1, file_to_write, buffer, &index, CMD_POP);
			}
		}

		else if(!strncmp(file_info->text[cur_str], "hlt", MAX_COMMAND_LEN))
		{
			// fprintf(file_to_write, "%d\n", CMD_HLT);
			// fprintf(file_with_code, "%d", CMD_HLT);
			print_in_file_and_buff(1, file_to_write, buffer, &index, CMD_HLT);
		}

		else if(!strncmp(file_info->text[cur_str], "add", MAX_COMMAND_LEN))
		{
			//fprintf(file_to_write, "%d\n", CMD_ADD);
			//fprintf(file_with_code, "%d", CMD_ADD);
			print_in_file_and_buff(1, file_to_write, buffer, &index, CMD_ADD);
		}
		
		else if(!strncmp(file_info->text[cur_str], "sub", MAX_COMMAND_LEN))
		{
			//fprintf(file_to_write, "%d\n", CMD_SUB);
			print_in_file_and_buff(1, file_to_write, buffer, &index, CMD_SUB);
		}
		
		else if(!strncmp(file_info->text[cur_str], "mul", MAX_COMMAND_LEN))
		{
			//fprintf(file_to_write, "%d\n", CMD_MUL);
			print_in_file_and_buff(1, file_to_write, buffer, &index, CMD_MUL);
		}
		
		else if(!strncmp(file_info->text[cur_str], "div", MAX_COMMAND_LEN))
		{
			//fprintf(file_to_write, "%d\n", CMD_DIV);
			print_in_file_and_buff(1, file_to_write, buffer, &index, CMD_DIV);
		}

		else if(!strncmp(file_info->text[cur_str], "in", MAX_COMMAND_LEN))
		{
			//fprintf(file_to_write, "%d\n", CMD_IN);
			print_in_file_and_buff(1, file_to_write, buffer, &index, CMD_IN);
		}

		else if(!strncmp(file_info->text[cur_str], "out", MAX_COMMAND_LEN))
		{
			//fprintf(file_to_write, "%d\n", CMD_OUT);
			print_in_file_and_buff(1, file_to_write, buffer, &index, CMD_OUT);
		}

		else
		{
			fprintf(stderr, RED "Unknown command: <%s>\n" RST, file_info->text[cur_str]);
		}
	}


	fclose(file_to_write);

	fprintf(stderr, "buffer:\n");
	for(size_t i = 0; i < index; ++i)
		fprintf(stderr, "%d ", buffer[i]);

	write(fileno(file_with_code), buffer, index * sizeof(int));

	fclose(file_with_code);

	return error;
}

int main(int argc, char** argv)
{
	char file_name[MAX_FILE_NAME];

	if(argc < 2)  //no file name
	{
		strncpy(file_name, "before_assembler.txt", MAX_FILE_NAME - 1);
	}
	else
	{
		strncpy(file_name, argv[1], MAX_FILE_NAME - 1);
		//printf("file = %s\n", file_name);
	}

	errors error = ALL_OK;
	//printf("file_name = %s\n", file_name);
	file_information* file_info = read_text_from_file_to_buff(file_name, &error);
	if(error != ALL_OK)
	{
		fprintf(stderr, RED "error = %d\n" RST, error);

		return error;
	}

	printf("size = %ld %c %d\n", file_info->size, file_info->buffer[0], file_info->buffer[0]);


	parse_buffer(file_info, &error);
	if(error != ALL_OK)
	{
		fprintf(stderr, RED "error = %d\n" RST, error);

		return error;
	}

	print_strs(file_info);


	assemble(file_info);	/**/


	file_info = delete_file_info(file_info);

	return 0;
}