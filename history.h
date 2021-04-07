/**
 * @file
 *
 * Contains shell history data structures and retrieval functions.
 */

#ifndef _HISTORY_H_
#define _HISTORY_H_

/**
 * Initializes the history management module and sets the number of history
 * items to be retained
 *
 * @param limit Number of history items to retain
 */
void hist_init(unsigned int limit);

/**
 * Destroys history data structures and frees any memory that was allocated.
 */
void hist_destroy(void);

/**
 * Adds a new command to the history.
 *
 * @param cmd The command to add to the history
 */
void hist_add(const char *cmd);

/**
 * Prints the history list in chronological order, starting with the oldest
 * command and printing up to the most recent command.
 */
void hist_print(void);

/**
 * Searches backwards chronologically through the history for a particular
 * command line prefix. For example, a prefix of 'abc' would match the most
 * recent command that started with 'abc'.
 *
 * @param prefix The prefix to search for. If prefix is an empty string, returns
 * the most recent command.
 *
 * @return First matching command found with the given prefix, or NULL if none
 * found.
 */
const char *hist_search_prefix(char *prefix);

/**
 * Searches for a history entry with a matching command number. Note that this
 * is *NOT* the array index of the command, but rather the monotonically
 * increasing command number.
 *
 * @param command_number The command number to search for
 *
 * @return matching command, or NULL if not found.
 */
const char *hist_search_cnum(int command_number);


/**
 * Retrieves the 'command number' of the last command. Note that this is *NOT*
 * the array index, but rather the monotonically increasing command number.
 */
unsigned int hist_last_cnum(void);

#endif
