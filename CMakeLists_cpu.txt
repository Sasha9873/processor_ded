cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.

project(processor)


set(SOURCE_EXE cpu.c)		# Установка переменной со списком исходников для исполняемого файла

set(SOURCE_LIB work_with_strs_from_file.h work_with_strs_from_file.c
	stack_func.c stack_struct.h stack_func.h commands.h processor_struct.h)			# То же самое, но для библиотеки

add_library(cpu STATIC ${SOURCE_LIB})	# Создание статической библиотеки с именем bmp

add_executable(main ${SOURCE_EXE})	# Создает исполняемый файл с именем main

target_link_libraries(main cpu)		# Линковка программы с библиотекой 