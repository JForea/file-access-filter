#ifndef RULE_LIST_H
#define RULE_LIST_H

#include <linux/types.h>

static int rule_list_init(const char *const *rules, size_t count);

static int add_rule(const char *rule);
static int remove_rule(const char *mask);
static void remove_all_rules(void);

static bool does_match_any_rule(const char *mask);

#endif