#include "colour_text.h"

void colorful_or_style_print(FILE* file_ptr, char* colour_or_style)
{
	if(!file_ptr || !colour_or_style)
		return;
	if(file_ptr == stderr || file_ptr == stdin)
        fprintf(file_ptr, colour_or_style);
}

void delete_colour(FILE* file_ptr)
{
	if(!file_ptr)
		return;

	if(file_ptr == stderr)
        fprintf(file_ptr, RST);
}

