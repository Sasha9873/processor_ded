/*!\file
*/

#include "assembler.h"

#define GENERATE_CMD(name, num, ...)\
	if(!(strcasecmp(file_info->text[cur_str], #name)))     \
	{								\
		int arg = 0;				\
			fprintf(stderr, "cur_str = %d max = %d str = %s %d %d %d\n", cur_str, file_info->n_strings, file_info->text[cur_str], num, arg_mask, num & arg_mask);\
		if(is_reg(file_info->text[cur_str + 1]))				\
		{ \
			++cur_str; \
			print_in_file_and_buff(2, file_to_write, buffer, &index, num + reg_mask, file_info->text[cur_str][1] - 'a'); \
		} \
		else if((num & arg_mask))		\
		{							\
			++cur_str;	\
			arg = atoi(file_info->text[cur_str]);  \
			if(arg == 0 && file_info->text[cur_str][0] != '0')  \
					fprintf(stderr, RED "Wrong reg, you have written: %s\n" RST, file_info->text[cur_str]);	\
			print_in_file_and_buff(2, file_to_write, buffer, &index, num + arg_mask, arg);	\
		}	\
		else  \
		{	\
			print_in_file_and_buff(1, file_to_write, buffer, &index, num);	\
		}	\
	}\
	else 


int is_reg(char* str)
{
	if(!str)
		return 0;

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

errors print_in_file_and_buff(size_t n_numbers, FILE* file_ptr, char* buffer, size_t* index, ...)  //args
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

errors print_in_file_with_byte_code(FILE* file_with_code, char* buffer, size_t index)
{
	if(!file_with_code || !buffer)
		return BAD_POINTER;

	write(fileno(file_with_code), &VERSION, sizeof(char));
	write(fileno(file_with_code), buffer, index * sizeof(char));

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


	char* buffer = calloc(file_info->n_strings, sizeof(char));
	size_t index = 0;


	for(size_t cur_str = 0; cur_str < file_info->n_strings; ++cur_str)
	{
		if(!file_info->text[cur_str])
			break;
		
		#include "commands_dsl.h"

		/*else*/
		{
			fprintf(stderr, RED "Unknown command: <%s>\n" RST, file_info->text[cur_str]);
		}
	}


	fclose(file_to_write);

	fprintf(stderr, "buffer:\n");
	for(size_t i = 0; i < index; ++i)
		fprintf(stderr, "%d ", buffer[i]);

	print_in_file_with_byte_code(file_with_code, buffer, index);

	fclose(file_with_code);
	free(buffer);

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

#undef GENERATE_CMD
