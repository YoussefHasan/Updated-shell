#include "uShell.h"

int main(int argc, char** argv)
{
	print_greeting();
	char command_line[MAX_LINE];
	ssize_t read_size = 0;
	
	while (1)
	{
		char *current_pwd = PWD();
		char *shell_msg = malloc(1024);
		strcpy(shell_msg, "3m el kawkab@");
		strcat(shell_msg, current_pwd);
		strcat(shell_msg, "  laghiny $ ");
		write(STDOUT, shell_msg, strlen(shell_msg));
		read_size = read(STDIN, command_line, MAX_LINE);
		char *token = strtok(command_line, " \n");
		while (token != NULL)
		{
			if (strcmp(token, "mypwd") == 0)
			{
				printf("%s\n", current_pwd);
				break;
			}
			else if (strcmp(token, "myenv") == 0)
			{
				envir();
				break;
			}
			else if  (strcmp(token, "mycd") == 0)
			{
				char *new_path = strtok(NULL, " \n");
				change_dir(new_path);
				break;
			}
			else if (strcmp((char*) "myecho", (char*) token) == 0)
			{
        			while ((token = strtok(NULL, " \n")) != NULL)
				{
					write(STDOUT, token, strlen(token));
					write(STDOUT, " ", 1);
				}
				printf("\n");
			}
			else if  (strcmp(token, "mytype") == 0)
			{
				char *cmd = strtok(NULL, " \n");
				printf("Type of command: %s\n", type(cmd));
				break;
			}
			else if (strcmp(token, "mycp") == 0)
			{
				char *source_path = strtok(NULL, " \n");
				char *target_path = strtok(NULL, " \n");
				cp(source_path, target_path);
				break;
			}
			else if (strcmp(token, "mymv") == 0)
			{
				char *source_path = strtok(NULL, " \n");
				char *target_path = strtok(NULL, " \n");
				mv(source_path, target_path);
				break;
			}
			else if  (strcmp(token, "myhelp") == 0)
			{
				display_guide();
				break;
			}
			else if  (strcmp(token, "myexit") == 0)
			{
				printf("Are you sure you want to exit? (y/n)\n");
				char confirm;
				scanf("%c", &confirm);
				if (confirm == 'y')
					{shell_exit();}
				else
				{
					printf("unrecognized option\n");
					break;
				}
			}
			else
			{
				char* leftovers;
				char* cmd = NULL;
				while ((leftovers = strtok(NULL, " \n")) != NULL)
				{
					if (cmd == NULL)
					{
						cmd = (char*)malloc(strlen(leftovers) + 1);
						strcpy(cmd, leftovers);
					}
					else
					{
						char* temp = (char*)malloc((strlen(cmd) + strlen(leftovers) + 1) * sizeof(char));
						sprintf(temp, "%s %s", cmd, leftovers);
						free(cmd);
						cmd = temp;
					}
				}
				if (cmd != NULL)
				{
					system(cmd); //the execution func in the shell.c did not work, replaced it with system() and still not working,I think the problem is with reading the cmd so I'll be updating it in a while
					free(cmd);
					break;
				}
			}
		}
		free(shell_msg);
	}
	return 0;
}
