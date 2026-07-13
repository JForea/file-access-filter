#include "helpers.h"

#include <linux/string.h>

#ifndef SIZE_MAX
    #define SIZE_MAX (~(size_t)0)
#endif

bool does_match(const char* mask, const char* file) {
    size_t mask_size, file_size,
        i_mask, i_file,
        rb_mask, rb_file;

    if (!mask || !file)
        return false;

    mask_size = strlen(mask);
    file_size = strlen(file);

    i_mask = 0;
    i_file = 0;
    rb_mask = SIZE_MAX;
    rb_file = SIZE_MAX;

    while (i_file < file_size) {
        if (i_mask < mask_size && mask[i_mask] == '*') {
            while (i_mask + 1 < mask_size && mask[i_mask + 1] == '*') {
                ++i_mask;
            }

            rb_file = i_file;
            rb_mask = i_mask;

            ++i_mask;
        } else if (i_mask < mask_size &&
                (mask[i_mask] == '?' || mask[i_mask] == file[i_file])) {
            ++i_mask;
            ++i_file;
        } else {
            if (rb_mask == SIZE_MAX) {
                return false;
            }

            i_file = ++rb_file;
            i_mask = rb_mask + 1;
        }
    }

    while (i_mask < mask_size && mask[i_mask] == '*') {
        ++i_mask;
    }

    return i_mask == mask_size;
}