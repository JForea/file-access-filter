#ifndef RULE_LIST_H
#define RULE_LIST_H

#include <linux/types.h>

void rule_list_init(void);

int add_rule(const char *rule);
int remove_rule(const char *mask);
void remove_all_rules(void);

bool does_match_any_rule(const char *file);

#endif