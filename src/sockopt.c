#define _GNU_SOURCE
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "util.h"
#include "desock.h"
#include "syscall.h"

VISIBLE
int setsockopt (int fd, int level, int optname, const void* optval, socklen_t optlen) {
    if (UNLIKELY(!DESOCK_FD(fd))) {
        int r = __socketcall(setsockopt, fd, level, optname, optval, optlen, 0);
        return __syscall_ret(r);
    }
    
    DEBUG_LOG("setsockopt(%d, %d, %d, %p, %lu)", fd, level, optname, optval, optlen);
    return 0;
}
VERSION(setsockopt)

VISIBLE
int getsockopt (int fd, int level, int optname, void* optval, socklen_t * optlen) {
    if (UNLIKELY(!DESOCK_FD(fd))) {
        int r = __socketcall(getsockopt, fd, level, optname, optval, optlen, 0);
        return __syscall_ret(r);
    }

    if ((level == SOL_SOCKET) && (optname == SO_PEERCRED)) {
        struct ucred *cred = optval;

        cred->pid = getpid();
        cred->uid = getuid();
        cred->gid = getgid();
        *optlen = sizeof(*cred);
    }

    DEBUG_LOG("getsockopt(%d, %d, %d, %p, %lu)", fd, level, optname, optval, optlen);
    return 0;
}
VERSION(getsockopt)
