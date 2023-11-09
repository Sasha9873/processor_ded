/*!\file
*/

#include "cpu.h" 
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
        fclose(proc->file_with_cpu_errors);
        proc->file_with_cpu_errors = NULL;
    }

	proc->stk = stack_dtor(proc->stk);
		
    if(proc->regs)
    {
    	free(proc->regs);
    	proc->regs = NULL;
    }	

    if(proc->code)
    {
    	free(proc->code);
    	proc->code = NULL;
    }


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
            if(file_ptr == stderr || file_ptr == stdin)
                fprintf(file_ptr, RED "ERROR_%lu = -%lu. This means: %s" RST "\n", ++n_cpu_errors, n_bit, cpu_error_names[n_bit]);
            else
                fprintf(file_ptr, "ERROR_%lu = -%lu. This means: %s\n", ++n_cpu_errors, n_bit, cpu_error_names[n_bit]);

            error -= error_mask;
        }

        ++n_bit;
        error_mask <<= 1;
    }

    fprintf(file_ptr, "\n");
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

    fprintf(proc->file_with_cpu_errors, "Proc[%p]", proc);

    size_t error = proc_ok(proc);

    if(error == ALL_OK)
        fprintf(proc->file_with_cpu_errors, "(ok)\n");
    else
    {   
        print_parse_cpu_error(error, proc->file_with_cpu_errors);
    }


	fprintf(proc->file_with_cpu_errors, "{\nregs[%p]:\n", proc->regs);
	if(proc->regs)
	{
		for(size_t i = 0; i < N_REGS; ++i)
		{
			fprintf(proc->file_with_cpu_errors, "%3ld", i);
		}

		fprintf(proc->file_with_cpu_errors, "\n");
		
		for(size_t i = 0; i < N_REGS; ++i)
		{
			fprintf(proc->file_with_cpu_errors, "%3d", proc->regs[i]);
		}
		fprintf(proc->file_with_cpu_errors, "\n");

	}

	fprintf(proc->file_with_cpu_errors, "n_commands = %lu\n", proc->n_commands);
	fprintf(proc->file_with_cpu_errors, "curr_command = %lu\n", proc->cur_cmd_num - 1);

	
	fprintf(proc->file_with_cpu_errors, "code[%p]:\n", proc->code);


	if(proc->code)
	{
		for(size_t i = 0; i < proc->n_commands; ++i)
		{
			fprintf(proc->file_with_cpu_errors, "%3ld", i);
		}

		fprintf(proc->file_with_cpu_errors, "\n");
		
		for(size_t i = 0; i < proc->n_commands; ++i)
		{
			fprintf(proc->file_with_cpu_errors, "%3d", proc->code[i]);
		}
		fprintf(proc->file_with_cpu_errors, "\n");

		for(size_t i = 0; proc->cur_cmd_num != 0 && i < proc->cur_cmd_num - 1; ++i)
		{
			fprintf(proc->file_with_cpu_errors, "---");
		}
		fprintf(proc->file_with_cpu_errors, "--^\n");



	}

	FILE* save_stk_file_with_errors = proc->stk->file_with_stack_errors;
	proc->stk->file_with_stack_errors = proc->file_with_cpu_errors;
	
	stack_dump(proc->stk, reason);

	proc->stk->file_with_stack_errors = save_stk_file_with_errors;
	
	fprintf(proc->file_with_cpu_errors, "}\n\n\n");

	return ALL_OK;
}



int run_proc(Proc* proc)
{
	if(!proc)
		return BAD_POINTER;

	int error = ALL_OK;

	proc->cur_cmd_num = 0;
	int cmd;
	printf("%p\n", proc);

	CHECKPROC(ALL_OK);
	while(proc->cur_cmd_num < proc->n_commands)
	{
		cmd = (int)proc->code[proc->cur_cmd_num++];

		switch(cmd)
		{
			case CMD_PUSH:
			{
				//CHECKPROC(ALL_OK);

				elem_type arg = (elem_type)proc->code[proc->cur_cmd_num++];
				error = stack_push(proc->stk, arg);
				
				CHECKPROC(ALL_OK);

				break;
			}
			
			case CMD_REG_PUSH:
			{
				//CHECKPROC(ALL_OK);

				int reg_num = (int)proc->code[proc->cur_cmd_num++];
				stack_push(proc->stk, proc->regs[reg_num]);
				
				CHECKPROC(ALL_OK);

				break;
			}

			case CMD_POP:
			{
				//CHECKPROC(ALL_OK);

				stack_pop(proc->stk, &error);
				
				CHECKPROC(ALL_OK);

				break;
			}
			
			case CMD_REG_POP:
			{
				//CHECKPROC(ALL_OK);

				int reg_num = (int)proc->code[proc->cur_cmd_num++];
				proc->regs[reg_num] = stack_pop(proc->stk, &error);
				
				CHECKPROC(ALL_OK);

				break;
			}

			case CMD_IN:
			{
				//CHECKPROC(ALL_OK);

				elem_type value = 0;
				scanf("" ELEM_SPECIFIER "", &value);
				stack_push(proc->stk, value);
				
				CHECKPROC(ALL_OK);

				break;
			} 

			case CMD_ADD:
			{
				//CHECKPROC(ALL_OK);

				elem_type elem = stack_pop(proc->stk, &error);
				if(error != ALL_OK)
				{
					print_parse_cpu_error(error, proc->file_with_cpu_errors);
					break;
				}

				elem += stack_pop(proc->stk, &error);
				if(error != ALL_OK)
				{
					print_parse_cpu_error(error, proc->file_with_cpu_errors);
					break;
				}

				stack_push(proc->stk, elem);
				
				CHECKPROC(ALL_OK);

				break;
			} 

			case CMD_SUB:
			{
				//CHECKPROC(ALL_OK);

				elem_type elem = stack_pop(proc->stk, &error);
				if(error != ALL_OK)
				{
					print_parse_cpu_error(error, proc->file_with_cpu_errors);
					break;
				}

				elem -= stack_pop(proc->stk, &error);
				if(error != ALL_OK)
				{
					print_parse_cpu_error(error, proc->file_with_cpu_errors);
					break;
				}

				stack_push(proc->stk, elem);
				
				CHECKPROC(ALL_OK);

				break;
			} 

			case CMD_MUL:
			{
				//CHECKPROC(ALL_OK);

				elem_type elem = stack_pop(proc->stk, &error);
				if(error != ALL_OK)
				{
					print_parse_cpu_error(error, proc->file_with_cpu_errors);
					break;
				}

				elem *= stack_pop(proc->stk, &error);
				if(error != ALL_OK)
				{
					print_parse_cpu_error(error, proc->file_with_cpu_errors);
					break;
				}

				stack_push(proc->stk, elem);
				
				CHECKPROC(ALL_OK);

				break;
			} 

			case CMD_DIV:
			{
				//CHECKPROC(ALL_OK);

				elem_type elem = stack_pop(proc->stk, &error);
				if(error != ALL_OK)
				{
					print_parse_cpu_error(error, proc->file_with_cpu_errors);
					break;
				}

				elem_type second_elem = stack_pop(proc->stk, &error);
				if(error != ALL_OK)
				{
					print_parse_cpu_error(error, proc->file_with_cpu_errors);
					break;
				}
				if(!second_elem)
				{
					if(proc->file_with_cpu_errors == stderr || proc->file_with_cpu_errors == stdin)
		                fprintf(proc->file_with_cpu_errors, RED "Division by zero" RST "\n");
		            else
		                fprintf(proc->file_with_cpu_errors, "Division by zero");

		            break;
				}

				stack_push(proc->stk, elem / second_elem);
				
				CHECKPROC(ALL_OK);

				break;
			} 

			case CMD_OUT:
			{
				CHECKPROC(ALL_OK);

				elem_type elem = stack_pop(proc->stk, &error);
				CHECKPROC(ALL_OK);
				stack_push(proc->stk, elem);
				CHECKPROC(ALL_OK);
				printf("elem = " ELEM_SPECIFIER "\n", elem);
				
				//CHECKPROC(ALL_OK);

				break;
			} 

			case CMD_HLT:
			{
				proc->cur_cmd_num = proc->n_commands;

				break;
			}	
			
		}
	}

	return error;
}



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

	int error = ALL_OK;
	size_t buff_size = 0;
	char* buffer = read_text_from_file_to_buff_for_proc(file_name, &error, &buff_size);
	if(error != ALL_OK)
	{
		fprintf(stderr, RED "error = %d\n" RST, error);
		return error;
	}


	Proc* proc = proc_ctor(&error, buffer, buff_size);
	//proc->file_with_cpu_errors = fopen("file_with_cpu_errors.txt", "wb");
	proc->stk->file_with_stack_errors = fopen("file_with_stack_errors.txt", "wb");


	run_proc(proc);


	proc_dtor(proc);

	return 0;
}