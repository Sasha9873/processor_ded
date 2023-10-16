#ifndef MAIN_ONEGIN_H
#define  MAIN_ONEGIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#include "work_with_strs_from_file.h"
//#include "qsort.h"

const static size_t N_LINES = 2;
const static size_t MAX_STR_LEN = 100;
const static size_t MAX_N_STRS = 100;
const static size_t MAX_FILE_NAME = 20;


struct file_information{
	const char* file_name;
	size_t size;
	size_t n_strings;
	char* buffer;
	char** text; ///pointers to the beginning of the strs in the text

} typedef file_information;

enum error_t{
	OK 		  = 0,	
	ENOMEM    = -1,
	EOPENFILE = -2,
};


void parse_buffer(file_information* file_info, error_t* error);
file_information* delete_file_info(file_information* file_info);
file_information* read_text_from_file_to_buff(const char* file_name, error_t* error);
void buble_sort_strs(file_information* file_info, int (*compare)(const void *, const void *), error_t* error);
void print_strs(file_information* file_info);

void swap_pointers(void* first, void* second);


#endif

