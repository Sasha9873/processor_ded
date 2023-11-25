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

	GENERATE_CMD(PUSH, 0x23, {
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

		elem_type first_elem = stack_pop(proc->stk, &error);
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

		error = stack_push(proc->stk, first_elem / second_elem);
				
		CHECKPROC(error);
	})

	GENERATE_CMD(REG_PUSH, 0x43, {
		//CHECKPROC(ALL_OK);

		int reg_num = (int)proc->code[proc->cur_cmd_num++];
		error = stack_push(proc->stk, proc->regs[reg_num]);
				
		CHECKPROC(error);
	})

	GENERATE_CMD(REG_POP, 0x44, {
		//CHECKPROC(ALL_OK);

		int reg_num = (int)proc->code[proc->cur_cmd_num++];
		proc->regs[reg_num] = stack_pop(proc->stk, &error);
				
		CHECKPROC(error);
	})

	GENERATE_CMD(JMP, 0x29, {
		//CHECKPROC(ALL_OK);

		make_jump(proc, &error);
		if(error != ALL_OK)
			break;
		
		CHECKPROC(error);
	})

	GENERATE_CMD(JA, 0x2A, {
		//CHECKPROC(ALL_OK);

		elem_type first_elem = stack_pop(proc->stk, &error);
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

		if(first_elem > second_elem)
		{
			make_jump(proc, &error);
			if(error != ALL_OK)
				break;
		}
		else
			++proc->cur_cmd_num;

		
		CHECKPROC(error);
	})

	GENERATE_CMD(JB, 0x2B, {
		//CHECKPROC(ALL_OK);

		elem_type first_elem = stack_pop(proc->stk, &error);
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

		if(first_elem < second_elem)
		{
			make_jump(proc, &error);
			if(error != ALL_OK)
				break;
		}	
		else
			++proc->cur_cmd_num;
		
		CHECKPROC(error);
	})

	GENERATE_CMD(JAE, 0x2C, {
		//CHECKPROC(ALL_OK);

		elem_type first_elem = stack_pop(proc->stk, &error);
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

		if(first_elem >= second_elem)
		{
			make_jump(proc, &error);
			if(error != ALL_OK)
				break;
		}	
		else
			++proc->cur_cmd_num;
		
		CHECKPROC(error);
	})

	GENERATE_CMD(JBE, 0x2D, {
		//CHECKPROC(ALL_OK);

		elem_type first_elem = stack_pop(proc->stk, &error);
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

		if(first_elem <= second_elem)
		{
			make_jump(proc, &error);
			if(error != ALL_OK)
				break;
		}	
		else
			++proc->cur_cmd_num;
		
		CHECKPROC(error);
	})

	GENERATE_CMD(JE, 0x2E, {
		//CHECKPROC(ALL_OK);

		elem_type first_elem = stack_pop(proc->stk, &error);
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

		if(first_elem == second_elem)
		{
			make_jump(proc, &error);
			if(error != ALL_OK)
				break;
		}	
		else
			++proc->cur_cmd_num;
		
		CHECKPROC(error);
	})

	GENERATE_CMD(JNE, 0x2F, {
		//CHECKPROC(ALL_OK);

		elem_type first_elem = stack_pop(proc->stk, &error);
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

		if(first_elem != second_elem)
		{
			make_jump(proc, &error);
			if(error != ALL_OK)
				break;
		}	
		else
			++proc->cur_cmd_num;
		
		CHECKPROC(error);
	})

	GENERATE_CMD(JM, 0x30, {
		//CHECKPROC(ALL_OK);

		if(1)
		{
			make_jump(proc, &error);
			if(error != ALL_OK)
				break;
		}	
		else
			++proc->cur_cmd_num;
		
		CHECKPROC(error);
	})

	