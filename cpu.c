/*!\file
*/

#include "cpu.h" 

void make_jump(Proc* proc, errors* error)
{
	int jmp_to_cmd = (int)proc->code[proc->cur_cmd_num];
	if(jmp_to_cmd < 0 || jmp_to_cmd >= proc->n_commands)
	{
		*error = BAD_JMP;
		CHECKPROC(*error);
	}
	proc->cur_cmd_num = jmp_to_cmd + 1; //+1 due to buffer[0] = VERSION of assembler commands
}

int check_if_should_jump(Proc* proc, errors* error)
{
	elem_type first_elem = stack_pop(proc->stk, error);
	if(*error != ALL_OK)
	{
		print_parse_cpu_error(*error, proc->file_with_cpu_errors);
	}

	elem_type second_elem = stack_pop(proc->stk, error);
	if(*error != ALL_OK)
	{
		print_parse_cpu_error(*error, proc->file_with_cpu_errors);
	}
}

Proc* proc_ctor(errors* error, ...) //can give char* buffer and its length in __VA_ARGS__

{
	Proc* proc = (Proc*)calloc(1, sizeof(Proc));

	printf("proc = %p\n", proc);

    if(!proc)
    {
        *error = NOT_MEMORY;
        return NULL;
    }

    proc->stk = stack_ctor(error);
    if(*error != ALL_OK)
    {
    	if(proc->stk)
    	{
    		free(proc->stk);
    		proc->stk = NULL;
    	}

    	free(proc);
    	proc = NULL;

    	return NULL;
    }

    proc->regs = (int*)calloc(N_REGS, sizeof(int));
    if(!(proc->regs))
    {
    	free(proc->stk);
    	proc->stk = NULL;
    	
    	free(proc);
    	proc = NULL;

    	return NULL;
    }

    proc->code = NULL;
    proc->n_commands = 0;
    proc->cur_cmd_num = 0;

    va_list args;
	va_start(args, error);

	if(args)
	{
		proc->code = va_arg(args, char*);
		if(proc->code)
			proc->n_commands = sizeof(proc->code);
		proc->n_commands = va_arg(args, size_t);
	}

	printf("code = %p\n", proc->code);
	printf("buffer: \n<");
	for(size_t i = 0; i < proc->n_commands; ++i)
		printf("%d <%c>\n", proc->code[i], proc->code[i]);
	printf(">\n");

	va_end(args);

	return proc;
}

Proc* proc_dtor(Proc* proc)
{
	if(!proc)
		return proc;
	
    if(proc->file_with_cpu_errors != proc->stk->file_with_stack_errors)
    {
    	if(proc->file_with_cpu_errors)
        	fclose(proc->file_with_cpu_errors);
        proc->file_with_cpu_errors = NULL;
    }

	proc->stk = stack_dtor(proc->stk);
		
    if(proc->regs)
    {
    	for(size_t i = 0; i < N_REGS; ++i)
    		proc->regs[i] = POISON;
    	free(proc->regs);
    	proc->regs = NULL;
    }	

    if(proc->code)
    {
    	for(size_t i = 0; i < proc->n_commands; ++i)
    		proc->code[i] = POISON;

    	free(proc->code);
    	proc->code = NULL;
    }
    
    proc->n_commands = POISON;
    proc->cur_cmd_num = POISON;


    free(proc);
    proc = NULL;

    return proc;
}

size_t proc_ok(Proc* proc)
{
    size_t error = ALL_OK;
    if(!proc)
    {
        error += 1 << abs(BAD_PROC_POINTER);
        return error;
    }
    
    if(!(proc->code))
        error += 1 << abs(BAD_CODE_POINTER);
    
    if(!(proc->regs))
        error += 1 << abs(BAD_REGS_POINTER);

    if(!(proc->stk))
        error += 1 << abs(BAD_STK_POINTER);

    if(proc->cur_cmd_num >= proc->n_commands)
    	error += 1 << abs(CUR_BIGGER_N_CMD);

    
    return error;

}

void print_parse_cpu_error(cpu_errors error, ...) //in va_args file_ptr
{
    va_list args;
    va_start(args, error);
    
    FILE* file_ptr;
    if(args)
        file_ptr = va_arg(args, FILE*);
    else
        file_ptr = stderr;

    va_end(args);


    size_t n_cpu_errors = 0;
    size_t error_mask = 1;
    size_t n_bit = 0;

    while(error)    
    {
        if(error & error_mask)
        {
            if(!n_cpu_errors)
                fprintf(file_ptr, "\n\n");
			
			colorful_or_style_print(file_ptr, RED);
            
            fprintf(file_ptr, "ERROR_%lu = -%lu. This means: %s\n", ++n_cpu_errors, n_bit, cpu_error_names[n_bit]);

			delete_colour(file_ptr);

            error -= error_mask;
        }

        ++n_bit;
        error_mask <<= 1;
    }

    fprintf(file_ptr, "\n");
}

void set_colour_cpu_code(Proc* proc, size_t curr_num)
{
	if(curr_num == proc->cur_cmd_num)
		fprintf(proc->file_with_cpu_errors, BOLD GREEN);
	else if(curr_num > proc->cur_cmd_num)
	{
		fprintf(proc->file_with_cpu_errors, GREY);
	}
	else if(curr_num < proc->cur_cmd_num)
	{
		fprintf(proc->file_with_cpu_errors, BLUE);
	}
}

int proc_dump(Proc* proc, cpu_errors reason)
{
	if(!proc)
    {
    	fprintf(stderr, "Proc[%p]\n{\n}\n", proc);

        return BAD_PROC_POINTER;
    }

	if(!(proc->file_with_cpu_errors))
    {
        proc->file_with_cpu_errors = stderr;
    }

    if(reason != 0)
    {
    	colorful_or_style_print(proc->file_with_cpu_errors, RED);
            
        fprintf(proc->file_with_cpu_errors, "\nDump was called because %s(error = %d)\n", cpu_error_names[abs(reason)], reason);

		delete_colour(proc->file_with_cpu_errors);
    }
       

    fprintf(proc->file_with_cpu_errors, "Proc[%p]", proc);

    size_t error = proc_ok(proc);

    if(error == ALL_OK)
        fprintf(proc->file_with_cpu_errors, "(ok)\n");
    else
    {   
        print_parse_cpu_error(error, proc->file_with_cpu_errors);
    }


	fprintf(proc->file_with_cpu_errors, "{\n\nregs[%p]:\n", proc->regs);
	if(proc->regs)
	{
		fprintf(proc->file_with_cpu_errors, "num ");
		for(size_t i = 0; i < N_REGS; ++i)
		{
			fprintf(proc->file_with_cpu_errors, "%3ld", i);
		}

		fprintf(proc->file_with_cpu_errors, "\n");
		
		fprintf(proc->file_with_cpu_errors, "val ");

		for(size_t i = 0; i < N_REGS; ++i)
		{
			fprintf(proc->file_with_cpu_errors, "%3d", proc->regs[i]);
		}
		fprintf(proc->file_with_cpu_errors, "\n\n");

	}

	fprintf(proc->file_with_cpu_errors, "n_commands = %lu\n", proc->n_commands);
	fprintf(proc->file_with_cpu_errors, "curr_command = %lu\n", proc->cur_cmd_num);

	
	fprintf(proc->file_with_cpu_errors, "\ncode[%p]:\n", proc->code);


	if(proc->code)
	{
		for(size_t i = 0; i < proc->n_commands; ++i)
		{
			fprintf(proc->file_with_cpu_errors, "%3ld", i);
		}

		fprintf(proc->file_with_cpu_errors, "\n");
		
		for(size_t i = 0; i < proc->n_commands; ++i)
		{
			set_colour_cpu_code(proc, i);

			fprintf(proc->file_with_cpu_errors, "%3d", proc->code[i]);
		}
		fprintf(proc->file_with_cpu_errors, RST "\n");

		for(size_t i = 0; i < proc->cur_cmd_num; ++i)
		{
			fprintf(proc->file_with_cpu_errors, "---");
		}
		fprintf(proc->file_with_cpu_errors, "--^\n\n");



	}

	FILE* save_stk_file_with_errors = proc->stk->file_with_stack_errors;
	proc->stk->file_with_stack_errors = proc->file_with_cpu_errors;
	
	stack_dump(proc->stk, ALL_OK);

	proc->stk->file_with_stack_errors = save_stk_file_with_errors;
	
	fprintf(proc->file_with_cpu_errors, "}\n\n\n");

	return ALL_OK;
}

#define GENERATE_CMD(name, num, ...)\
	case CMD_##name:	\
		__VA_ARGS__;	\
		break;

int run_proc(Proc* proc)
{
	if(!proc)
		return BAD_POINTER;

	int error = ALL_OK;

	proc->cur_cmd_num = 0;
	int cmd;
	printf("%p version = %d\n", proc, proc->code[0]);

	if(proc->code[0] != VERSION)
		return WRONG_CMD_VERSION;
	++(proc->cur_cmd_num);

	CHECKPROC(ALL_OK);
	while(proc->cur_cmd_num < proc->n_commands)
	{
		cmd = (int)proc->code[proc->cur_cmd_num++];

		switch(cmd)
		{
			#include "commands.dsl"
		}
	}

	return error;
}

#undef GENERATE_CMD


int main(int argc, char** argv)
{
	char file_name[MAX_FILE_NAME];

	if(argc < 2)  //no file name
	{
		strncpy(file_name, "code.out", MAX_FILE_NAME - 1);
	}
	else
	{
		strncpy(file_name, argv[1], MAX_FILE_NAME - 1);
		//printf("file = %s\n", file_name);
	}

	int error = ALL_OK;
	size_t buff_size = 0;
	char* buffer = read_text_from_file_to_buff_for_proc(file_name, &error, &buff_size);
	if(error != ALL_OK)
	{
		fprintf(stderr, RED "error = %d\n" RST, error);
		return error;
	}

	printf("2)buffer: \n<");
	for(size_t i = 0; i < buff_size; ++i)
		printf("%d <%c>\n", buffer[i], buffer[i]);
	printf(">\n");


	Proc* proc = proc_ctor(&error, buffer, buff_size);
	//proc->file_with_cpu_errors = fopen("file_with_cpu_errors.txt", "wb");
	proc->stk->file_with_stack_errors = fopen("file_with_stack_errors.txt", "wb");


	error = run_proc(proc);
	if(error != ALL_OK)
	{
		proc_dump(proc, error);
		return error;
	}	

	proc_dtor(proc);
/**/
	return 0;
}