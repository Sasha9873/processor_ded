#ifndef WORK_WITH_STR_FROM_FILE_H
#define WORK_WITH_STR_FROM_FILE_H

	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <stdbool.h>
	#include <stdarg.h>

	#include "cpu_errors_enum_naming.h"
	#include "third-party/stack_different_types/errors_enum_naming.h"

	typedef struct Labels{
		char* label_name;
		size_t code_num;

	} Labels;

	typedef struct file_information{
		const char* file_name;
		size_t size;
		size_t n_strings;
		char* buffer;
		char** text; ///< pointers to the beginning of the strs in the text

		Labels* labels;
		size_t n_labels;
		
	} file_information;


	file_information* read_text_from_file_to_buff(const char* file_name, errors* error);  ///< ' ' also means new str
	char* read_text_from_file_to_buff_for_proc(const char* file_name, errors* error, size_t* buff_size);
	size_t get_file_size(FILE* file_ptr);
	FILE* open_file(const char* file_name, const char* mode, errors* error);
	void parse_buffer(file_information* file_info, errors* error);


	size_t get_n_strings(char* buffer, size_t buff_size, ...);  //<in va_args can be pointer to count labels;  strings ends with '\n' or ' '
	file_information* delete_file_info(file_information* file_info);

	void buble_sort_strs(file_information* file_info, int (*compare)(const void *, const void *), errors* error);
	void print_strs(file_information* file_info);
	void swap_pointers(void* first, void* second);


#endif