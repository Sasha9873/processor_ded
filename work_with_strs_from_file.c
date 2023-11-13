#include "work_with_strs_from_file.h"


size_t get_file_size(FILE* file_ptr)
{
	fseek(file_ptr, 0, SEEK_END);

	long int size = ftell(file_ptr);
	if(size == -1)
		return 0;

	fseek(file_ptr, 0, SEEK_SET);

	return (size_t)size;
}

FILE* open_file(const char* file_name, const char* mode, errors* error)
{
	FILE* file_ptr = fopen(file_name, mode);

	printf("file_ptr = %p\n", file_ptr);

	if(!file_ptr)
	{
		if(error)
			*error = ERROR_OPEN_FILE;
		return NULL;
	}

	return file_ptr;
}

int* read_text_from_file_to_buff_for_proc(const char* file_name, errors* error, size_t* buff_size)
{	
	FILE* file_ptr = open_file(file_name, "rb", error);
	if(!file_ptr)
	{
		*error = ERROR_OPEN_FILE;
		return NULL;
	}

	size_t size = get_file_size(file_ptr);


	//char* buffer = (int*)calloc(size + 1, sizeof(char)); 
	int* buffer = (int*)calloc(size, sizeof(int)); 
	if(!buffer)
	{
		fclose(file_ptr);
		*error = NOT_MEMORY;
		return NULL;
	}

	printf("%lu %lu %lu\n", size, sizeof(char*), sizeof(buffer)/sizeof(buffer[0]));

	*buff_size = size;
	fread(buffer, sizeof(char), *buff_size, file_ptr);

	printf("%lu %lu %lu\n", size, sizeof(buffer), sizeof(buffer)/sizeof(buffer[0]));
	printf("buffer: \n<");
	for(size_t i = 0; i < size; ++i)
		printf("%d <%c>\n", buffer[i], buffer[i]);
	printf(">\n");

	fclose(file_ptr);

	return buffer;
}

///returns buff
file_information* read_text_from_file_to_buff(const char* file_name, errors* error)
{
	file_information* file_info = (file_information*)calloc(1, sizeof(file_information));
	
	file_info->file_name = file_name;

	
	FILE* file_ptr = open_file(file_name, "rb", error);
	if(!file_ptr)
	{
		*error = ERROR_OPEN_FILE;
		return NULL;
	}

	file_info->size = get_file_size(file_ptr);


	file_info->buffer = (char*)calloc(file_info->size + 2, sizeof(char)); 
	fread(file_info->buffer, sizeof(char), file_info->size, file_ptr);

	printf("buffer: \n%s\n", file_info->buffer);


	fclose(file_ptr);

	return file_info;
}


size_t get_n_strings(char* buffer, size_t buff_size)
{
	size_t n_strings = 0;
	for(size_t i = 0; i < buff_size; ++i)
	{
		if(buffer[i] == '\n' || buffer[i] == ' ' || buffer[i] == '\t')
		{
			++n_strings;

			while(i + 1 < buff_size && (buffer[i + 1] == '\n' || buffer[i + 1] == ' ' || buffer[i + 1] == '\t'))
				++i;
			if(i + 2 < buff_size && i + 1 < buff_size)
				printf("cur sym <%c%c> n_strs = %lu i = %lu\n", buffer[i+1], buffer[i+2], n_strings, i);
			else if(i + 1 < buff_size)
				printf("cur sym <%c> n_strs = %lu i = %lu\n", buffer[i+1], n_strings, i);
		}
		else if(buffer[i] == ';')
		{
			if((i != 0) && (buffer[i - 1] == ' ' || buffer[i - 1] == '\t'))
				--n_strings;

			while(i < buff_size &&  buffer[i] != '\n')
				++i;
			while(i + 1 < buff_size && (buffer[i + 1] == '\n' || buffer[i + 1] == ' ' || buffer[i + 1] == '\t'))
				++i;
			
			++n_strings;
		}
	}

	return n_strings;
}

void parse_buffer(file_information* file_info, errors* error)
{
	if(!file_info->size)
		return;

	char* const buffer = file_info->buffer;

	if(buffer[file_info->size - 1] != '\n')   /// if the last string in the file does not end with '\n'
	{
		buffer[file_info->size] = '\n';
		++file_info->size;
	}


	size_t n_strings = get_n_strings(buffer, file_info->size);
	file_info->n_strings = n_strings;

	file_info->text = (char**)calloc(n_strings + 1, sizeof(char*));

	printf("size = %ld %c %d n_strings = %ld\n", file_info->size, file_info->buffer[0], file_info->buffer[0], n_strings);
	

	file_info->text[0] = buffer;

	size_t cur_str = 0;
	for(size_t i = 0; i < file_info->size; ++i)
	{
		//printf("buffer[i %d] = <%c> buffer[i+1 %d] = <%c>\n", i, buffer[i], i + 1, buffer[i + 1]);


		if(buffer[i] == '\n' || buffer[i] == ' ' || buffer[i] == '\t')
		{
			buffer[i] = '\0';

			while((i + 1) < file_info->size && (buffer[i + 1] == ' ' || buffer[i + 1] == '\n'))  //delete (in fact just ignore) ' ', '\n' after '\n' and ' '
			{
				//printf("2 buffer[i %d] = <%c> buffer[i+1 %d] = <%c>\n", i, buffer[i], i + 1, buffer[i + 1]);
				buffer[i + 1] = '\0';
				++i;
			}	

			if((i + 1) < file_info->size)
			{
				//printf("i + 1 = %d file_info->size = %d cur_str = %d buf = <%c>\n", i+1, file_info->size, cur_str + 1, buffer[i+1]);
				file_info->text[++cur_str] = &buffer[i + 1];
			}
		}
		else if(buffer[i] == ';')   //for comments in file.txt
		{
			if((i != 0) && buffer[i - 1] == '\0')
				--cur_str;

			buffer[i] = '\0';

			while(i < file_info->size && buffer[i] != '\n')
			{
				buffer[i] = '\0';
				++i;
			}

			while((i + 1) < file_info->size && (buffer[i + 1] == ' ' || buffer[i + 1] == '\n'))  //delete (in fact just ignore) ' ', '\n' after '\n' and ' '
			{
				//printf("2 buffer[i %d] = <%c> buffer[i+1 %d] = <%c>\n", i, buffer[i], i + 1, buffer[i + 1]);
				buffer[i + 1] = '\0';
				++i;
			}	


			if((i + 1) < file_info->size)
			{
				//printf("2i + 1 = %d file_info->size = %d cur_str = %d buf = <%c>\n", i+1, file_info->size, cur_str + 1, buffer[i+1]);
				file_info->text[++cur_str] = &buffer[i + 1];
			}
		}
	}

	//if(cur_str)
		//file_info->n_strings = cur_str;

	printf("after parse_buffer:\n");
	for(size_t i = 0; i < file_info->n_strings; ++i)
		printf("%lu <%s>\n", i, file_info->text[i]);

	
}


file_information* delete_file_info(file_information* file_info)
{
	if(!file_info)
		return NULL;

	if(file_info->text)
	{
		free(file_info->text);
		file_info->text = NULL;
	}

	if(file_info->buffer)
	{
		free(file_info->buffer);
		file_info->buffer = NULL;
	}
	

	free(file_info);
	file_info = NULL;

	return NULL;
}


void buble_sort_strs(file_information* file_info, int (*compare)(const void *, const void *), errors* error)
{
	for(size_t n = 0; n < file_info->n_strings - 1; ++n)
	{
		bool n_swaps = false;
		for(size_t j = 0; j < file_info->n_strings - n - 1; ++ j)
		{
			if(compare(file_info->text[j], file_info->text[j + 1]) > 0)
			{
				swap_pointers(&file_info->text[j], &file_info->text[j + 1]);

				n_swaps = true;/**/
			}
		}

		if(!n_swaps)
			break;
	}
}

void swap_pointers(void* first, void* second)
{
	char* tmp = *(char**)first;
	*(char**)first = *(char**)second;
	*(char**)second = tmp;
}

/*void swap_pointers_to_strs(char** first, char** second)
{
	char* tmp = *first;
	*first = *second;
	*second = tmp;
}

int compare(const void* first_str, const void* second_str)
{
	if(!first_str || !second_str)
		return 0;

    char* str1 = (char*)first_str;
    char* str2 = (char*)second_str;

    size_t i = 0;

    printf("str1 = %s str2 = %s\n", str1, str2);

    while(str1[i] != '\0' && str2[i] != '\0')
    {
        if(str1[i] > str2[i])
            return 1;
        else if(str1[i] < str2[i])
            return -1;

        ++i;
    }

    if(str1[i] == '\0' && str2[i] != '\0')
    	return -1;
    else if(str1[i] != '\0' && str2[i] == '\0')
    	return 1;

    return 0;
}*//**/

void print_strs(file_information* file_info)
{
	printf("<\n");
	if(!file_info){
		printf(">\n");
		return;
	}
	if(!file_info->text){
		printf(">\n");
		return;
	}

	for(size_t i = 0; i < file_info->n_strings; ++i)
	{
		if(file_info->text[i])
			printf("%s\n", file_info->text[i]);
	}
	printf(">\n");
}


//----------------------------------------------------------------------
