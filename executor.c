#include "executor.h"

/**
 *  
 * @param command The command to execute (e.g., "ls", "pwd", "echo")
 * 
 * @param args Array of arguments: [command, arg1, arg2, ..., NULL]
 *             Example: {"ls", "-l", NULL} or {"echo", "Hello", NULL}
 * 
 * @return Exit status of the command (0=success, non-zero=failure, -1=error)
 * 
 */

int execute_command(char *command, char **args) {
    pid_t pid;
    int status;

    // 1. forks a child process
    pid = fork();

    if (pid < 0) {

        perror("fork");
        return -1;

    }

    // 2. child Process (executes command)
    if (pid == 0) {

        execvp(command, args);
        perror("execvp");
        // exits child and doesnt return
        exit(1);

    }

    // 3. parent process (waits for child to complete)
    // waits for specific Child that was forked
    if (waitpid(pid, &status, 0) < 0) {

        perror("waitpid");
        return -1;

    }

    // so if child exits normally it'll return its exit code
    if (WIFEXITED(status)) {

        return WEXITSTATUS(status);

    }

    // debugging if child is killed by signal will treat as Error
    if (WIFSIGNALED(status)) {

        return -1;

    }

    // anything else is abnormal
    return -1;  

}