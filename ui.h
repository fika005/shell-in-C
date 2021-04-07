/**
 * @file
 *
 * Text-based UI functionality. These functions are primarily concerned with
 * interacting with the readline library.
 */

#ifndef _UI_H_
#define _UI_H_

/**
 * Initializes the UI; sets up any required data structures, configures the
 * readline library, sets the locale, etc.
 */
void init_ui(void);

/**
 * Destroys the UI;
 */
void destroy_ui(void);


/**
 * Retrieves the current prompt string.
 *
 * @return Command prompt string
 */
char *prompt_line(void);

/**
 * Retrieves the username associated with this process, which is then added to
 * the command line string.
 *
 * @return user name
 */
char *prompt_username(void);

/**
 * Retrieves the hostname of the machine. Will point to a buffer of no more than
 * HOST_NAME_MAX characters.
 *
 * @return host name
 */
char *prompt_hostname(void);

/**
 * Retrieves the current working directory of the shell
 *
 * @return current working directory
 */
char *prompt_cwd(void);

/**
 * Retrieves the status of the last command that was run. If the command
 * terminated successfully, it will have returned 0 (by convention). The shell
 * interprets a '0' exit code as success and will display a happy emoji or some
 * type of text to indicate the success in the prompt.
 *
 * @return status of the last command that was run
 */
int prompt_status(void);

/**
 * Retrieves the current command number being composed by the user, starting
 * with 1. The command number will also be reflected in the shell history.
 *
 * @return current command number
 */
unsigned int prompt_cmd_num(void);

/**
 * Prompts the user for a command, reads it, and then returns it to be processed
 * by the shell.
 *
 * @return command line entered by the user (WITHOUT the trailing newline \n
 * character)
 */
char *read_command(void);


/**
 * Called by the readline library when the user presses the 'up arrow' key.
 * NOTE: you will most likely not use the parameters passed to this function.
 *
 * @param count repeat count (unused)
 * @param key the keycode for the up arrow key
 *
 * @return 0 on success, nonzero otherwise
 */
int key_up(int count, int key);

/**
 * Called by the readline library when the user presses the 'down arrow' key.
 * NOTE: you will most likely not use the parameters passed to this function.
 *
 * @param count repeat count (unused)
 * @param key the keycode for the down arrow key
 *
 * @return 0 on success, nonzero otherwise
 */
int key_down(int count, int key);

#endif
