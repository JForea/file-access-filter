#include "IoctlHelpers.hpp"

#include <stdexcept>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdint>
#include <string.h>
#include <cerrno>

#include <fad/fad_ioctl.h>

const std::string Device = std::string("/dev/") + DEVICE_NAME;

const std::string FdOpenErrMessage = "Couldn't open file descriptor.";
const std::string FdOpenAccessErrorMessage = "You don't have permission to interact with file access driver.";
const std::string IoctlErrorMessage = "Error during ioctl.";

fad_ioctl_rule FormRule(const char* mask) {
    return {
        .mask = static_cast<__u64>(
            reinterpret_cast<std::uintptr_t>(mask)
        ),
        .length = static_cast<__u32>(strlen(mask)),
    };
}

int OpenDevice() {
    int fd = open(Device.c_str(), O_RDWR);
    if (fd < 0) {
        if (errno == EACCES)
            throw std::runtime_error(FdOpenAccessErrorMessage);
        throw std::runtime_error(FdOpenErrMessage);
    }

    return fd;
}

void AddMask(const char* mask) {
    int fd = OpenDevice();

    fad_ioctl_rule rule = FormRule(mask);

    if (ioctl(fd, FAD_ADD, &rule) < 0) {
        close(fd);
        if (errno == EEXIST)
            throw std::runtime_error("Mask already exists.");
        throw std::runtime_error(IoctlErrorMessage);
    }

    close(fd);
}

void RemoveMask(const char* mask) {
    int fd = OpenDevice();

    fad_ioctl_rule rule = FormRule(mask);

    if (ioctl(fd, FAD_DEL, &rule) < 0) {
        close(fd);
        if (errno == ENOENT)
            throw std::runtime_error("Mask doesn't exist.");
        throw std::runtime_error(IoctlErrorMessage);
    }

    close(fd);
}

void ClearMasks() {
    int fd = OpenDevice();

    if (ioctl(fd, FAD_CLEAR) < 0) {
        close(fd);
        throw std::runtime_error(IoctlErrorMessage);
    }

    close(fd);
}
