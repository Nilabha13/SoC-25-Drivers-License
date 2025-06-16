#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define MAX_CWD_LENGTH 256
#define MAX_BG_PROCESSES 64

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
        tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
        strcpy(tokens[tokenNo++], token);
        tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}


int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];
	char  **tokens;
	int i;

	pid_t bg_pids[MAX_BG_PROCESSES];
	int num_bg_processes = 0;

	while(1) {
		pid_t bg_pid;
		while((bg_pid = waitpid(-1, NULL, WNOHANG)) > 0){
			printf("Shell: Background process finished\n");
			for(i = 0; i < num_bg_processes; i++){
				if(bg_pids[i] == bg_pid){
					bg_pids[i] = bg_pids[--num_bg_processes];
					break;
				}
			}
		}

		char cwd[MAX_CWD_LENGTH];
		if(getcwd(cwd, MAX_CWD_LENGTH) == NULL){
			perror("getcwd failed");
			strcpy(cwd, "?");
		}

		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("%s $ ", cwd);
		scanf("%[^\n]", line);
		getchar();
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);
   
		// --- SHELL CODE START ---

		if(tokens[0] == NULL)
			goto cleanup;
		
		if(strcmp(tokens[0], "exit") == 0){
			for(i = 0; i < num_bg_processes; i++){
				kill(bg_pids[i], SIGTERM);
				waitpid(bg_pids[i], NULL, 0);
			}
			goto cleanup_and_exit;
		}

		if(strcmp(tokens[0], "cd") == 0){
			if(tokens[1] == NULL || tokens[2] != NULL){
				fprintf(stderr, "Usage: cd <directory>\n");
			} else{
				if(chdir(tokens[1]) != 0)
					perror("chdir failed");
			}
			goto cleanup;
		}

		int is_background_process = 0;
		for(i = 0; tokens[i] != NULL; i++);
		if(i > 0 && strcmp(tokens[i-1], "&") == 0){
			is_background_process = 1;
			free(tokens[i-1]);
			tokens[i-1] = NULL;
		}

		if(is_background_process && num_bg_processes >= MAX_BG_PROCESSES){
			fprintf(stderr, "Too many background processes. Cannot execute another.");
			goto cleanup;
		}

		pid_t pid = fork();
		if(pid < 0){
			perror("fork failed");
		} else if(pid == 0){
			execvp(tokens[0], tokens);
			perror("execvp failed");
			exit(EXIT_FAILURE);
		} else{
			if(!is_background_process){
				int status;
				waitpid(pid, &status, 0);
				if(WIFEXITED(status))
					printf("EXITSTATUS: %d\n", WEXITSTATUS(status));
				else
					printf("EXITSTATUS: (abnormal exit)\n");
			} else{
				bg_pids[num_bg_processes++] = pid;
			}
		}

		// --- SHELL CODE END ---
       
    	cleanup:
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);
		continue;

		cleanup_and_exit:
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);
		break;
	}

	return 0;
}