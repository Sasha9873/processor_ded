/*!\file
*/

#ifndef COLOUR_TEXT_H_INCLUDED
#define COLOUR_TEXT_H_INCLUDED

	#include <stdio.h>


    #define RST "\x1b[0m" 
	#define RED "\033[0;31m"
    #define GREEN "\033[0;32m"
    #define ORANGE "\033[0;33m"
    #define BLUE "\033[0;34m"
    #define GREY "\033[0;2m"

    #define BOLD "\033[0;1m"

	void colorful_or_style_print(FILE* file_ptr, char* colour_or_style);
	void delete_colour(FILE* file_ptr);
	
#endif