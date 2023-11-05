#include "stack_struct.h"
#include "stack_func.h"
#include "processor_struct.h"
#include "commands.h"
#include "work_with_strs_from_file.h"


static const size_t MAX_FILE_NAME = 30;

int main(int argc, char** argv)
{
	char file_name[MAX_FILE_NAME];

	if(argc < 2)  //no file name
	{
		strncpy(file_name, "code.txt", MAX_FILE_NAME - 1);
	}
	else
	{
		strncpy(file_name, argv[1], MAX_FILE_NAME - 1);
		//printf("file = %s\n", file_name);
	}

	errors error = ALL_OK;
	char* buffer = read_text_from_file_to_buff_for_proc(file_name, &error);
	if(error != ALL_OK)
	{
		fprintf(stderr, RED "error = %d\n" RST, error);
		return error;
	}


	// if(buffer)
	free(buffer);

	return 0;
}