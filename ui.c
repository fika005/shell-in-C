/**
 * @file
 *
 * Text-based UI functionality. These functions are primarily concerned with
 * interacting with the readline library.
 */
#include <stdio.h>
#include <readline/readline.h>
#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "history.h"
#include "logger.h"
#include "clist.h"
#include <unistd.h>
#include "ui.h"
#define USR_MAX 100
#define PATH_MAX 200
 // used this global variable to set the last command status
extern int last_cmd_status;
static const char *good_str = "😌";
static const char *bad_str  = "🤯";
static bool scripting = false;
static int key_up_count = 0;
char *line = NULL;
size_t line_sz = 0;
char user_name[USR_MAX];
char host_name[_SC_HOST_NAME_MAX + 1];
char *cwd_final = NULL;

static int readline_init(void);

void init_ui(void)
{
    LOGP("Initializing UI...\n");

    char *locale = setlocale(LC_ALL, "en_US.UTF-8");
    LOG("Setting locale: %s\n",
            (locale != NULL) ? locale : "could not set locale!");
    if (isatty(STDIN_FILENO) == false) {
        LOGP("Entering scripting mode\n");
        scripting = true;
    }
    rl_startup_hook = readline_init;
}

void destroy_ui(void)
{
    if (line != NULL) {
    	free(line);
    }
    free(prompt_cwd());
}

char *prompt_line(void)
{
    const char *status = prompt_status() ? bad_str : good_str;

    char cmd_num[25];
    snprintf(cmd_num, 25, "%u", prompt_cmd_num());

    char *user = prompt_username();
    char *host = prompt_hostname();
    char *cwd = prompt_cwd();

    char *format_str = ">>-[%s]-[%s]-[%s@%s:%s]-> ";

    size_t prompt_sz
        = strlen(format_str)
        + strlen(status)
        + strlen(cmd_num)
        + strlen(user)
        + strlen(host)
        + strlen(cwd)
        + 1;

    char *prompt_str =  malloc(sizeof(char) * prompt_sz);

    snprintf(prompt_str, prompt_sz, format_str,
            status,
            cmd_num,
            user,
            host,
            cwd);

    return prompt_str;
}

char *prompt_username(void)
{
    getlogin_r(user_name, USR_MAX);
    return user_name;
}

char *prompt_hostname(void)
{
    gethostname(host_name, _SC_HOST_NAME_MAX + 1);
    return host_name;
}
char *prompt_cwd(void)
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, PATH_MAX) != NULL) {
	char *home_path = getenv("HOME");
	size_t home_path_len = strlen(home_path);
        size_t cwd_len = strlen(cwd);
        if (strncmp(cwd, home_path, home_path_len) == 0) {
            size_t new_cwd_len = 2 + (int) cwd_len - (int) home_path_len;
            if (cwd_final != NULL) {
		free(cwd_final);
            }
            cwd_final = malloc(sizeof(char) * (new_cwd_len + 1));
            snprintf(cwd_final, new_cwd_len, "~%s", &cwd[home_path_len]); 
        } else {
	    if (cwd_final != NULL) {
                 free(cwd_final);
            }
             cwd_final = malloc(sizeof(char) * (cwd_len + 1));
	     strcpy(cwd_final, cwd);
	}
        return cwd_final;
    } else {
       perror("getcwd() error");
       return NULL;
    }
}

int prompt_status(void)
{
    return last_cmd_status;
}

unsigned int prompt_cmd_num(void)
{
    return hist_last_cnum();
}

char *read_command(void)
{
    if (scripting == false) {
        char *prompt = prompt_line();
        char *command = readline(prompt);
        free(prompt);
        return command;
    } else {
        ssize_t read_sz = getline(&line, &line_sz, stdin);
        if (read_sz == -1) {
            perror("getline");
        } else {
            line[read_sz - 1] = '\0';
            return line;
        }
    }
   return NULL;
}

int readline_init(void)
{
    rl_bind_keyseq("\\e[A", key_up);
    rl_bind_keyseq("\\e[B", key_down);
    rl_variable_bind("show-all-if-ambiguous", "on");
    rl_variable_bind("colored-completion-prefix", "on");
    return 0;
}

int key_up(int count, int key)
{
    int max = hist_max();
    if (key_up_count < max) {
	key_up_count++;
    }
    char *command = strdup(hist_search_cnum(hist_last_cnum() - key_up_count + 1));
    rl_replace_line(command, 1);


    rl_point = rl_end;
    free(command);

    return 0;
}

int key_down(int count, int key)
{
    char *command = strdup("");
    if (key_up_count > 1) {
	key_up_count--;
	command = strdup(hist_search_cnum(hist_last_cnum() - key_up_count + 1));
    } else if (key_up_count == 1) {
	key_up_count--;
    }
    rl_replace_line(command, 1);


    rl_point = rl_end;
    free(command);

    return 0;
}

