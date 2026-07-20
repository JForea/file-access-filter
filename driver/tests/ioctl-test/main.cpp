#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdint>

#include "fad_ioctl.h"

#define DEVICE "/dev/fad"

int main()
{
    int fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    const char mask[] = "*/test/*.txt";

    fad_ioctl_rule rule = {
        .mask = static_cast<decltype(fad_ioctl_rule::mask)>(
            reinterpret_cast<std::uintptr_t>(mask)
        ),
        .length = std::strlen(mask)
    };

    if (ioctl(fd, FAD_ADD, &rule) < 0) {
        perror("ioctl FAD_ADD");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}