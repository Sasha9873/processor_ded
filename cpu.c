#include "stack_struct.h"
#include "stack_func.h"
#include "processor_struct.h"
#include "commands.h"
#include "work_with_strs_from_file.h"

#include <stdarg.h>


Proc* proc_ctor(errors* error, ...)  //can give char* buffer and its length in __VA_ARGS__
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
	
    if(proc->file_with_errors != proc->stk->file_with_errors)
    {
        fclose(proc->file_with_errors);
        proc->file_with_errors = NULL;
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

int proc_dump(Proc* proc, errors reason)
{
	if(!(proc->file_with_errors))
    {
        proc->file_with_errors = stderr;
    }

    fprintf(proc->file_with_errors, "Proc[%p]\n{\n", proc);


	fprintf(proc->file_with_errors, "regs[%p]:\n", proc->regs);
	if(proc->regs)
	{
		for(size_t i = 0; i < N_REGS; ++i)
		{
			fprintf(proc->file_with_errors, "%3ld", i);
		}

		fprintf(proc->file_with_errors, "\n");
		
		for(size_t i = 0; i < N_REGS; ++i)
		{
			fprintf(proc->file_with_errors, "%3d", proc->regs[i]);
		}
		fprintf(proc->file_with_errors, "\n");

	}

	fprintf(proc->file_with_errors, "n_commands = %ld\n", proc->n_commands);
	
	fprintf(proc->file_with_errors, "code[%p]:\n", proc->code);

	if(proc->code)
	{
		for(size_t i = 0; i < proc->n_commands; ++i)
		{
			fprintf(proc->file_with_errors, "%3ld", i);
		}

		fprintf(proc->file_with_errors, "\n");
		
		for(size_t i = 0; i < proc->n_commands; ++i)
		{
			fprintf(proc->file_with_errors, "%3d", proc->code[i]);
		}
		fprintf(proc->file_with_errors, "\n");

	}

	proc->stk->file_with_errors = proc->file_with_errors;
	stack_dump(proc->stk, reason);
	
	fprintf(proc->file_with_errors, "}\n");

}



errors run_proc(Proc* proc)
{
	if(!proc)
		return BAD_POINTER;

	errors error = ALL_OK;

	size_t cur_cmd_num = 0;
	int cmd;
	printf("%p\n", proc);

	proc_dump(proc, ALL_OK);
	while(cur_cmd_num < proc->n_commands)
	{
		cmd = (int)proc->code[cur_cmd_num++];

		switch(cmd)
		{
			case CMD_PUSH:
			{
				CHECKPROC(ALL_OK);

				int arg = (int)proc->code[cur_cmd_num++];
				stack_push(proc->stk, arg);
				
				CHECKPROC(ALL_OK);

				break;
			}
		case CMD_REG_PUSH:
			{
				CHECKPROC(ALL_OK);

				int reg_num = (int)proc->code[cur_cmd_num++];
				stack_push(proc->stk, proc->regs[reg_num]);
				
				CHECKPROC(ALL_OK);

				break;
			}
			case CMD_REG_POP:
			{
				CHECKPROC(ALL_OK);

				int arg = (int)proc->code[cur_cmd_num++];
				proc->regs[arg] = stack_pop(proc->stk, &error);
				proc_dump(proc, ALL_OK);
				
				CHECKPROC(ALL_OK);

				break;
			}
			case CMD_POP:
			{
				CHECKPROC(ALL_OK);

				stack_pop(proc->stk, &error);
				proc_dump(proc, ALL_OK);
				
				CHECKPROC(ALL_OK);

				break;
			}

		}
	}

	return error;
}

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
	size_t buff_size = 0;
	char* buffer = read_text_from_file_to_buff_for_proc(file_name, &error, &buff_size);
	if(error != ALL_OK)
	{
		fprintf(stderr, RED "error = %d\n" RST, error);
		return error;
	}


	Proc* proc = proc_ctor(&error, buffer, buff_size);
	proc->file_with_errors = fopen("file_with_errors.txt", "wb");


	run_proc(proc);


	proc_dtor(proc);

	return 0;
}