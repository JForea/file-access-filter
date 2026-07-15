#include "rule_list.h"
#include "helpers.h"

#include <linux/list.h>
#include <linux/hashtable.h>
#include <linux/slab.h>

typedef struct {
    char* mask;
    struct list_head node;
} fad_rule;

typedef struct {
    struct list_head rules;
} rule_list;

static rule_list rl;

static int rule_list_init(const char **rules, size_t count) {
    INIT_LIST_HEAD(&rl.rules);

    if (rules == NULL || count == 0) {
        return 0;
    }

    for (size_t i = 0; i < count; i++) {
        add_rule(rules[i]);
    }

    return 0;
}

static bool does_exist(const char* mask) {
    /* TODO: add check in hashtable */
    return false;
}

static int add_rule(const char* mask) {
    fad_rule *rule;
    rule = kzalloc(sizeof(fad_rule), GFP_KERNEL);
    if (!rule)
        return -ENOMEM;
        
    rule->mask = kstrdup(mask, GFP_KERNEL);

    list_add(&rule->node, &rl.rules);

    /* TODO: add to hashtable */

    return 0;
}

static int remove_rule(const char* mask) {
    fad_rule *rule; /* TODO: get from hashtable */

    list_del(&rule->node);
    
    kfree(rule->mask);
    kfree(rule);

    /* TODO: remove from hashtable */

    return 0;
}

static int remove_all_rules() {
    fad_rule *rule, *tmp;

    list_for_each_entry_safe(rule, tmp, &rl.rules, node) {
        list_del(&rule->node);
        kfree(rule->mask);
        kfree(rule);

        /* TODO: remove from hashtable */
    }

    return 0;
}

static bool does_match_any_rule(const char* file) {
    fad_rule *rule;

    list_for_each_entry(rule, &rl.rules, node) {
        if (does_match(rule->mask, file)) {
            return true;
        }
    }

    return false;
}
