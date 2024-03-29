/**
 * @file
 *
 * Contains shell main and all the helper functions to process and run the commands.
 */
#include <fcntl.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "history.h"
#include "logger.h"
#include "ui.h"
#include "elist.h"
#include "util.h"
#define HIST_LIM 100
static size_t num_children = 0;
static bool running = true;
int last_cmd_status = 0;

void jobs_sigchld() {
    num_children--;
}

void jobs_destroy() {
}

int process_builtins(char *command, struct elist *list) {
    char **args = elist_get(list, 0);
    if (strcmp(args[0], "cd") == 0) {
	if (elist_size(list) == 2) {
	    chdir(getenv("HOME"));
	} else {
	    chdir(args[1]);
	}
	return 0;
    } else if (strcmp(args[0], "history") == 0) {
	hist_print();
	return 0;
    } else if (strcmp(args[0], "jobs") == 0) {
	return 0;
    } else if (strcmp(args[0], "exit") == 0) {
	LOGP("Exiting! Bye!\n");
	running = false;
	return 0;
    }
    return 1;
}

void process_command(char *command, struct elist *list) {
    bool is_bg = false;
    LOG("Input command: %s\n", command);
    char *cmd = command;
    next_token(&cmd, "#");
    if (command[0] == '!') {
	if (strlen(command) > 1) {
	    if (command[1] == '!') {
		const char *eq_cmd = hist_search_cnum(hist_last_cnum());
		if (eq_cmd != NULL) {
		    command = strdup(eq_cmd);
		}
	    } else if (command[1] >= 48 && command[1] < 58) {
		const char *eq_cmd = hist_search_cnum(atoi(command + 1));
		if (eq_cmd != NULL) {
		    command = strdup(eq_cmd);
		}
	    } else {
		const char *eq_cmd = hist_search_prefix(command + 1);
		if (eq_cmd != NULL) {
		    command = strdup(eq_cmd);
		}
	    }
	}
    }

    char *command_copy = strdup(command);
    if (command[strlen(command) - 1] == '&') {
	is_bg = true;
	command[strlen(command) - 1] = '\0';
    }
    char *next_tok = command;
    char *curr_tok;
    while ((curr_tok = next_token(&next_tok, " \t\n")) != NULL) {
	elist_add(list, &curr_tok);
    }
    char *null = (char *) 0;
    elist_add(list, &null);

    if (elist_size(list) == 1) {
	LOGP("Empty command\n");
	elist_clear(list);
	free(command_copy);
	return;
    }
    if (command[0] != '!') {
	hist_add(command_copy);
    } else {
        free(command_copy);
    }
    if (process_builtins(command, list) == 0) {
	elist_clear(list);
	return;
    } 
    num_children++;
    pid_t child = fork();
    if (child == -1) {
	perror("fork");
    } else if (child == 0){
	char **args = elist_get(list, 0);
	if (execvp(args[0], args) == -1) {
	    perror("execvp");
	    close(fileno(stderr));
	    close(fileno(stdout));
	    close(fileno(stdin));
	    exit(1);
	}
    } else {
	if (!is_bg) {
	    int status;
	    wait(&status);
	    last_cmd_status = status;
	}
	elist_clear(list);
    }

}
int main(void)
{
    init_ui();
    hist_init(HIST_LIM);
    signal(SIGINT, SIG_IGN);
    signal(SIGCHLD, jobs_sigchld);
    struct elist *tokens = elist_create(0, sizeof(char **));
    while (running) {
	char *command;
	command = read_command();
	if (command == NULL) {
	    break;
	}
	process_command(command, tokens);      
    }
    destroy_ui();
    jobs_destroy();
    hist_destroy();
    elist_destroy(tokens);

    return 0;
}

