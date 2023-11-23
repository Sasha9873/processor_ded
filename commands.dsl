/*!\file
*/

	
	GENERATE_CMD(HLT, 0, {
		proc->cur_cmd_num = proc->n_commands;

		CHECKPROC(error);
	})

	GENERATE_CMD(OUT, 1, {
		//CHECKPROC(ALL_OK);

		elem_type elem = stack_top(proc->stk, &error);

		colorful_or_style_print(proc->file_with_cpu_errors, GREEN);
		printf("elem = " ELEM_SPECIFIER "\n\n", elem);
		delete_colour(proc->file_with_cpu_errors);
				
		CHECKPROC(error);
	})

	GENERATE_CMD(IN, 2, {
		//CHECKPROC(ALL_OK);

		elem_type value = 0;
		scanf("" ELEM_SPECIFIER "", &value);
		error = stack_push(proc->stk, value);
				
		CHECKPROC(error);
	})

	GENERATE_CMD(PUSH, 0x13, {
		//CHECKPROC(ALL_OK);

		elem_type arg = (elem_type)proc->code[proc->cur_cmd_num++];
		error = stack_push(proc->stk, arg);
				
		CHECKPROC(error);
	})

	GENERATE_CMD(POP, 4, {
		//CHECKPROC(ALL_OK);

		stack_pop(proc->stk, &error);
				
		CHECKPROC(error);
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
				
		CHECKPROC(error);
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

		error = stack_push(proc->stk, elem);
				
		CHECKPROC(error);
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

		error = stack_push(proc->stk, elem);
				
		CHECKPROC(error);
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

		error = stack_push(proc->stk, elem / second_elem);
				
		CHECKPROC(error);
	})

	GENERATE_CMD(REG_PUSH, 0x23, {
		//CHECKPROC(ALL_OK);

		int reg_num = (int)proc->code[proc->cur_cmd_num++];
		error = stack_push(proc->stk, proc->regs[reg_num]);
				
		CHECKPROC(error);
	})

	GENERATE_CMD(REG_POP, 0x24, {
		//CHECKPROC(ALL_OK);

		int reg_num = (int)proc->code[proc->cur_cmd_num++];
		proc->regs[reg_num] = stack_pop(proc->stk, &error);
				
		CHECKPROC(error);
	})

	GENERATE_CMD(JMP, 0x19, {
		//CHECKPROC(ALL_OK);

		int jmp_to_cmd = (int)proc->code[proc->cur_cmd_num];
		if(jmp_to_cmd < 0 || jmp_to_cmd >= proc->n_commands)
		{
			error = BAD_JMP;
			CHECKPROC(error);
			break;
		}
		proc->cur_cmd_num = jmp_to_cmd + 1; //+1 due to buffer[0] = VERSION of assembler commands
		
		CHECKPROC(error);
	})

