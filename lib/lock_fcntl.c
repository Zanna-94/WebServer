#include "../basic.h"
#include <fcntl.h>
#include <errno.h>

static struct flock	lock_it, unlock_it;
static int		lock_fd = -1;
/* fcntl() will fail if my_lock_init() not called */

void my_lock_init(char *pathname)
{
    char	lock_file[1024];

    /* must copy caller's string, in case it's a constant */
    strncpy(lock_file, pathname, sizeof(lock_file));
    if ( (lock_fd = mkstemp(lock_file)) < 0) {
        fprintf(stderr, "mkstemp error");
        exit(1);
    }

    if (unlink(lock_file) == -1) { 	/* but lock_fd remains open */
        fprintf(stderr, "error in unlink for %s", lock_file);
        exit(1);
    }
    lock_it.l_type = F_WRLCK;
    lock_it.l_whence = SEEK_SET;
    lock_it.l_start = 0;
    lock_it.l_len = 0;

    unlock_it.l_type = F_UNLCK;
    unlock_it.l_whence = SEEK_SET;
    unlock_it.l_start = 0;
    unlock_it.l_len = 0;
}

void my_lock_wait()
{
    while ( fcntl(lock_fd, F_SETLKW, &lock_it) < 0) {
        if (errno == EINTR)
            continue;
        else {
            fprintf(stderr, "error fcntl in my_lock_wait");
            exit(1);
        }
    }
}

void my_lock_release()
{
    if (fcntl(lock_fd, F_SETLKW, &unlock_it) < 0) {
        fprintf(stderr, "error fcntl in my_lock_release");
        exit(1);
    }
}
