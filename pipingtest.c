#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>

int main(void) {
    // fd_set is a struct that represents a set of file descriptors that can be used by the select() function
    // functionally it is an array of bits
    fd_set readfds;
    
    // macro that takes a pointer to a fd_set and initializes it so that all values are 0
    FD_ZERO(&readfds);

    // macro that takes a file descriptor, and a pointer to a fd_set
    // adds the file descriptor to the fd_set
    FD_SET(STDIN_FILENO, &readfds);

    // a struct which specifies how long select() should wait to see which file descriptors are ready
    // since we are looking for which files are currently ready, we don't want select() to wait, so the time is set to 0
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    // Parameters for select:
    //  - the number of file descriptors to check
    //  - readfds: a fd_set to be checked if they are beind ready to read
    //  - writefds: a fd_set to be checked if they are being ready to write
    //  - exceptfds: a fd_set to be check for errors/exception conditions pending
    //  - and a timeval to specify how long it should wait for the file descriptors to be ready
    // select() modifies the fd_sets readfds, writefds, and exceptfds. Each file descriptor in the set is modified based on wether we can read from it, write to it, and if it has any exceptions/errors pending respectively
    //  - You can use the macro FD_ISSET to check if a certain file descriptor has been set
    // select () returns the total number of files from all three fd_sets that are ready, it will return 0 if the timeout has expired, and -1 if there was an error
    // If there was an error, the following errno conditions will be set
    //  - EBADF if a file descriptor is invalid
    //  - EINTR if select() was interrupted by a signal - any signal will interupt select() and force it to return
    //  - EINVAL if the timeout argument is invalid - one of the values is either too large, or negative
    int select_returnval = select(1, &readfds, NULL, NULL, &timeout);

    // tests if we can read from the STDIN_FILENO and prints the result to the console
    if(select_returnval > 0) {
        printf("There are files ready\n");
    }

    if (FD_ISSET(STDIN_FILENO, &readfds) == 0)
    {
        printf("The file descriptor STDIN_FILENO is NOT READY to be read\n");
    } else {
        printf("The file descriptor STDIN_FILENO is READY to be read\n");
    }

}