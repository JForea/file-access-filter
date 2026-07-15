#include "rule_list.h"
#include "helpers.h"

#include <linux/hashtable.h>
#include <linux/jhash.h>
#include <linux/slab.h>
#include <linux/string.h>

#define RULE_HASH_BITS 10

typedef struct {
    char* mask;
    u32 hash;

    struct hlist_node hash_node;
} fad_rule;

static DEFINE_HASHTABLE(rule_hash_table, RULE_HASH_BITS);

static u32 rule_hash(const char *mask) {
    return jhash(mask, strlen(mask), 0);
}

static fad_rule *find_rule(const char* mask) {
    fad_rule *rule;
    u32 hash;

    if (!mask) {
        return NULL;
    }

    hash = rule_hash(mask);

    hash_for_each_possible(rule_hash_table, rule, hash_node, hash) {
        /*
        * Collisions are possible, so we need to chech string for equality.
        */
        if (rule->hash == hash && strcmp(rule->mask, mask) == 0) {
            return rule;
        }
    }

    return NULL;
}

static bool does_exist(const char *mask) {
    if (!mask) {
        return false;
    }

    return find_rule(mask) != NULL;
}


static int rule_list_init(const char *const *rules, size_t count) {
    if (rules == NULL && count != 0) {
        return -EINVAL;
    }

    hash_init(rule_hash_table);

    for (size_t i = 0; i < count; i++) {
        int status = add_rule(rules[i]);
        
        if (status != 0) {
            remove_all_rules();
            return status;
        }
    }

    return 0;
}

static int add_rule(const char* mask) {
    if (!mask) {
        return -EINVAL;
    }

    if (does_exist(mask)) {
        return -EEXIST;
    }

    fad_rule *rule;
    rule = kzalloc(sizeof(fad_rule), GFP_KERNEL);

    if (!rule)
        return -ENOMEM;
        
    rule->mask = kstrdup(mask, GFP_KERNEL);

    if (!rule->mask) {
        kfree(rule);
        return -ENOMEM;
    }

    rule->hash = rule_hash(mask);

    hash_add(rule_hash_table, &rule->hash_node, rule->hash);

    return 0;
}

static void remove_and_free(fad_rule *rule) {
    if (!rule) {
        return;
    }

    hash_del(&rule->hash_node);

    kfree(rule->mask);
    kfree(rule);
}

static int remove_rule(const char* mask) {
    if (!mask) {
        return -EINVAL;
    }

    fad_rule *rule = find_rule(mask);

    if (!rule) {
        return -ENOENT;
    }

    remove_and_free(rule);

    return 0;
}

static void remove_all_rules(void) {
    struct hlist_node *tmp;
    fad_rule *rule;
    int bkt;

    hash_for_each_safe(rule_hash_table, bkt, tmp, rule, hash_node) {
        remove_and_free(rule);
    }
}

static bool does_match_any_rule(const char* file) {
    fad_rule *rule;
    int bkt;

    if (!file) {
        return false;
    }

    hash_for_each(rule_hash_table, bkt, rule, hash_node) {
        if (does_match(rule->mask, file)) {
            return true;
        }
    }

    return false;
}
