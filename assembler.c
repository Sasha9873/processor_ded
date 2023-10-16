#include "processor_struct.h"
#include "work_with_strs_from_file.h"
#include "stack_struct.h"
#include "stack_func.h"
#include "commands.h"

static const int MAX_FILE_NAME = 30;
static const int MAX_COMMAND_LEN = 10;

errors assemble(file_information* file_info)
{
	errors error = ALL_OK;

	FILE* file_to_write = open_file("after_assembler.txt", "wb", &error);
	if(error != ALL_OK)
	{
		printf("error = %d\n", error);
		return error;
	}


	for(size_t cur_str = 0; cur_str < file_info->n_strings; ++cur_str)
	{
		if(!strncmp(file_info->text[cur_str], "push", MAX_COMMAND_LEN))
		{
			int arg = atoi(file_info->text[++cur_str]);

			fprintf(file_to_write, "%d %d\n", CMD_PUSH, arg);
		}
	}


	fclose(file_to_write);
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
		printf("error = %d\n", error);
		return error;
	}

	printf("size = %ld %c %d\n", file_info->size, file_info->buffer[0], file_info->buffer[0]);


	parse_buffer(file_info, &error);
	if(error != ALL_OK)
	{
		printf("error = %d\n", error);
		return error;
	}

	print_strs(file_info);


	assemble(file_info);	


	file_info = delete_file_info(file_info);
	return 0;
}