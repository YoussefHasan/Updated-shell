#include "uShell.h"


void print_greeting()
{
	char *text = "\nHi bud, this is a \033[1muShell\033[0m.\nHere's a simple guide for commands it can execute:\n\t\033[1;3;4mmypwd\033[0m: prints the current working directory.\n\t\033[1;3;4mmyecho\033[0m: prints a string you type.\n\t\033[1;3;4mmycp\033[0m: copies a file from source path to target path.\n\t\033[1;3;4mmymv\033[0m: moves a file from source path to target path.\n\t\033[1;3;4mmycd\033[0m: changes the current working directory.\n\t\033[1;3;4mmyhelp\033[0m: displays some help.\n\t\033[1;3;4mmyexit\033[0m: closes the uShell session.\n\n";
	write(STDOUT, text, strlen(text));
}

char *PWD()
{
	char cwd[MAX_LINE];
	getcwd(cwd, sizeof(cwd));
	return strdup(cwd);
}

int change_dir(char *new_path)
{
	if (chdir(new_path) != 0)
	{
		perror("chdir");
		return -1;
	}
	else
	{
		char current_dir[MAX_LINE];
		if (getcwd(current_dir, MAX_LINE) == NULL)
		{
			perror("getcwd");
			return -1;
		}
        return 0;
    }
}

void display_guide()
{
	char *text = "\nHere are some instructions when dealing with some commands that have arguments\n\t\033[1mcp\033[0m: cp [OPTION] SOURCE_PATH DIRECTORY_PATH.\n\t\tfor the option flag you can use \"-a\" which allows you to append the source content to the end of the target file\n\t\033[1mmv\033[0m: mv [OPTION] SOURCE_PATH DIRECTORY_PATH.\n\t\tfor the option flag you can use \"-f\" which allows you to force overwriting the target file if exists\n\n";
	write(STDOUT, text, strlen(text));
}

void shell_exit()
{
	char *text = "Exiting..\n";
	write(STDOUT, text, strlen(text));
	exit(0);
}

char *get_file_name(char *path)
{
	char *last_slash = strrchr(path, '/');
	if (last_slash == NULL)
		{return path;}		
	last_slash++;
	char *file_name = malloc(strlen(last_slash) + 1);
	strcpy(file_name, last_slash);
	return file_name;
}

int cp(char *source_path, char *target_path)
{
	if (strcmp(source_path, target_path) == 0)
 	{
 		printf("Source and target paths are the same.\n");
		return -1;
        }
        FILE *source_file = fopen(source_path, "r");
	if (source_file == NULL)
	{
		printf("Error: cannot open source file %s\n", source_path);
		return -1;
	}
	FILE *target_file;
	if (access(target_path, F_OK) != -1)
	{
		printf("Target file %s already exists\n", target_path);
		return 1;
	}
	target_file = target_file = fopen(target_path, "w");
	char buffer[MAX_LINE];
	while (fgets(buffer, MAX_LINE, source_file) != NULL)
	{
		fwrite(buffer, strlen(buffer), 1, target_file); 
		break;
	}
	fclose(source_file);
	fclose(target_file);
	return 0;
}

int mv(char *source_path, char *target_path)
{
	if (strcmp(source_path, target_path) == 0)
	{
		printf("Source and target paths are the same.\n");
		return -1;
	}
        FILE *source_file = fopen(source_path, "r");
	if (source_file == NULL)
	{
		printf("Error: cannot open source file %s\n", source_path);
		return -1;
	}
	if (access(target_path, F_OK) != -1)
	{
		printf("Target file already exists.\n");
		return -1;
	}
	FILE *target_file;
	target_file = fopen(target_path, "w");
	char buffer[MAX_LINE];
	while (fgets(buffer, MAX_LINE, source_file) != NULL)
	{
		fwrite(buffer, strlen(buffer), 1, target_file); 
		break;
	}
	if (rename(source_path, target_path) != 0)
	{
		printf("Error: cannot rename file %s to %s\n", source_path, target_path);
        	fclose(source_file);
        	return -1;
	}
	printf("File moved successfully.\n");
	fclose(source_file);	
	return 0;
}
int envir()
{
	extern char ** environ;
	int i = 0;
	while (environ[i] != NULL)
	{
		printf("%s\n", environ[i]);
		i++;
	}
	return 0;
}

char* type(char* cmd)
{
	char* path_env = getenv("PATH");
	static const char* internal_commands[] = {"mycd", "mypwd", "myecho", "myexit", "myhelp", "myenv", "mycp", "mymv"};
	for (int i = 0; i < sizeof(internal_commands) / sizeof(internal_commands[0]); i++)
	{
		if (strcmp(cmd, internal_commands[i]) == 0)
		{
			return "internal";
		}
		break;
	}
	if (access(path_env, F_OK) != -1 && strcmp(path_env, "") != 0)
	{
	struct stat *sb;
	if (stat(path_env, &sb) == 0 && S_ISREG(sb->st_mode))
	{return "Unsupported: command not found in PATH\n";}
	}
	else
	{return "external";}

    return "";
}

void exec_external_command(char* cmd)
{
	char* path_env = getenv("PATH");	// Get the PATH environment variable
	char* dir = strtok(path_env, ":");	// Tokenize the PATH variable into individual directories
	while (dir != NULL)	// Loop through each directory in the PATH
	{
		char* full_cmd = malloc(strlen(dir) + strlen(cmd) + 2);		// Allocate memory for the full command path
	strcpy(full_cmd, dir);		// Copy the current directory to the beginning of the full command path
	strcat(full_cmd, "/");		// Add a '/' at the end of the directory
	strcat(full_cmd, cmd);		// Append the command name to the full command path
	if (access(full_cmd, F_OK) == 0)	// Check if the file exists in this directory
	{	// Command found; execute it
		pid_t pid = fork();	// Create a new process using fork()
		if (pid < 0)	// Error handling for fork()
		{
			perror("fork");
			exit(1);
		}
		else if (pid == 0)	// Child process: execute the command
			{execve(full_cmd, NULL, NULL);}		// Execute the external command
		else
			{waitpid(pid, NULL, 0);}	// Wait for the child process to finish
	}
	free(full_cmd); // Free memory allocated for the full command path
	dir = strtok(NULL, ":"); // Move on to the next directory in the PATH
	}
}

