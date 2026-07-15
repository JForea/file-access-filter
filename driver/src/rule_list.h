#ifndef RULE_LIST_H
#define RULE_LIST_H

#include <linux/types.h>

int rule_list_init(const char **rules, size_t count);

int add_rule(const char *rule);
int remove_rule(const char *mask);
int remove_all_rules();

bool does_match_any_rule(const char *mask);

#endif