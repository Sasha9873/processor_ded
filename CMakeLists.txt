cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.

project(processor)


set(SOURCE_EXE cpu.c)		# Установка переменной со списком исходников для исполняемого файла
set(SOURCE_ASM_EXE assembler.c)


set(SOURCE_LIB work_with_strs_from_file.h work_with_strs_from_file.c
	third-party/stack_different_types/stack_func.c third-party/stack_different_types/stack_struct.h third-party/stack_different_types/stack_func.h commands.h processor_struct.h)			# То же самое, но для библиотеки
set(SOURCE_ASM_LIB work_with_strs_from_file.h work_with_strs_from_file.c third-party/stack_different_types/stack_func.c third-party/stack_different_types/stack_struct.h third-party/stack_different_types/stack_func.h commands.h)	


add_library(cpu STATIC ${SOURCE_LIB})	# Создание статической библиотеки с именем cpu
add_library(asm STATIC ${SOURCE_ASM_LIB})

add_executable(main ${SOURCE_EXE})	# Создает исполняемый файл с именем main
add_executable(main_asm ${SOURCE_ASM_EXE})

target_link_libraries(main cpu)		# Линковка программы с библиотекой 
target_link_libraries(main_asm asm)	