/*
* This header is used for unit-testing some of the functions in user-space.
*/
#ifndef COMPATIBILITY_H
#define COMPATIBILITY_H
#ifdef __KERNEL__
#include <linux/string.h>
#include <linux/types.h>
#else
#include <string.h>
#include <stdbool.h>
#endif
#endif