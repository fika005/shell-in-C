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

void jobs_sigchld() {

}

void jobs_destroy() {
}

int main(void)
{
    init_ui();
    signal(SIGINT, SIG_IGN);
    signal(SIGCHLD, jobs_sigchld);
    struct elist *list = elist_create(0, sizeof(char **));
    char *command;
    while (true) {
        command = read_command();
        if (command == NULL) {
            break;
        }

        LOG("Input command: %s\n", command);
        char *next_tok = command;
        char *curr_tok;
        while ((curr_tok = next_token(&next_tok, " \t\n")) != NULL) {
            elist_add(list, &curr_tok);
        }
        char *null = (char *) 0;
        elist_add(list, &null);

        if (elist_size(list) == 1) {
            LOGP("Empty command\n");
            continue;
        }
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
            int status;
            wait(&status);
            elist_clear(list);
        }
       
     }
    destroy_ui();
    jobs_destroy();
    hist_destroy();
    elist_destroy(list);

    return 0;
}

