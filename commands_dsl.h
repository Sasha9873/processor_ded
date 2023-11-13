/*!\file
*/

	
	GENERATE_CMD(HLT, 0, {
		proc->cur_cmd_num = proc->n_commands;
	})

	GENERATE_CMD(OUT, 1, {
		//CHECKPROC(ALL_OK);

		elem_type elem = stack_pop(proc->stk, &error);
		stack_push(proc->stk, elem);
		printf("elem = " ELEM_SPECIFIER "\n", elem);
				
		CHECKPROC(ALL_OK);
	})

	GENERATE_CMD(IN, 2, {
		//CHECKPROC(ALL_OK);

		elem_type value = 0;
		scanf("" ELEM_SPECIFIER "", &value);
		stack_push(proc->stk, value);
				
		CHECKPROC(ALL_OK);
	})

	GENERATE_CMD(PUSH, 19, {
		//CHECKPROC(ALL_OK);

		elem_type value = 0;
		scanf("" ELEM_SPECIFIER "", &value);
		stack_push(proc->stk, value);
				
		CHECKPROC(ALL_OK);
	})

	GENERATE_CMD(POP, 4, {
		//CHECKPROC(ALL_OK);

		stack_pop(proc->stk, &error);
				
		CHECKPROC(ALL_OK);
	})

	GENERATE_CMD(ADD, 5, {
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
	})

	GENERATE_CMD(SUB, 6, {
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
	})

	GENERATE_CMD(MUL, 7, {
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
	})

	GENERATE_CMD(DIV, 8, {
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
	})

	GENERATE_CMD(REG_PUSH, 35, {
		//CHECKPROC(ALL_OK);

		int reg_num = (int)proc->code[proc->cur_cmd_num++];
		stack_push(proc->stk, proc->regs[reg_num]);
				
		CHECKPROC(ALL_OK);
	})

	GENERATE_CMD(REG_POP, 36, {
		//CHECKPROC(ALL_OK);

		int reg_num = (int)proc->code[proc->cur_cmd_num++];
		proc->regs[reg_num] = stack_pop(proc->stk, &error);
				
		CHECKPROC(ALL_OK);
	})

