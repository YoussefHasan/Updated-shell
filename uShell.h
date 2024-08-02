#ifndef USHELL_H
#define USHELL_H

#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>


#define STDIN		0
#define STDOUT		1
#define STDERR		2
#define O_RDONLY	0	
#define O_WRONLY	1
#define O_RDWR		2


#define MAX_LINE       1024

void print_greeting();
char *PWD();
int change_dir(char *new_path);
void shell_exit();
void display_guide();
char* type(char* cmd);
int envir();
void exec_external_command(char* cmd);
char *get_file_name(char *path);
int cp(char *source_path, char *target_path);
int mv(char *source_path, char *target_path);


#endif
