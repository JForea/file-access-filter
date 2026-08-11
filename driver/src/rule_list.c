#include "rule_list.h"
#include "helpers.h"

#include <linux/hashtable.h>
#include <linux/jhash.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/mutex.h>

#define RULE_HASH_BITS 10

typedef struct {
    char* mask;
    u32 hash;

    struct hlist_node hash_node;
} fad_rule;

static DEFINE_HASHTABLE(rule_hash_table, RULE_HASH_BITS);
static DEFINE_MUTEX(hash_table_mutex);

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
        * Collisions are possible, so we need to check string for equality.
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


void rule_list_init(void) {
    hash_init(rule_hash_table);
}

int add_rule(const char* mask) {
    fad_rule *rule;
    int ret = 0;

    if (!mask) {
        return -EINVAL;
    }

    mutex_lock(&hash_table_mutex);

    if (does_exist(mask)) {
        ret = -EEXIST;
        goto cleanup;
    }

    rule = kzalloc(sizeof(fad_rule), GFP_KERNEL);

    if (!rule) {
        ret = -ENOMEM;
        goto cleanup;
    }

    rule->mask = kstrdup(mask, GFP_KERNEL);

    if (!rule->mask) {
        kfree(rule);
        ret = -ENOMEM;
        goto cleanup;
    }

    rule->hash = rule_hash(mask);

    hash_add(rule_hash_table, &rule->hash_node, rule->hash);

cleanup:
    mutex_unlock(&hash_table_mutex);
    return ret;
}

static void remove_and_free(fad_rule *rule) {
    if (!rule) {
        return;
    }

    hash_del(&rule->hash_node);

    kfree(rule->mask);
    kfree(rule);
}

int remove_rule(const char* mask) {
    fad_rule *rule;
    int ret = 0;

    mutex_lock(&hash_table_mutex);

    if (!mask) {
        ret = -EINVAL;
        goto cleanup;
    }

    rule = find_rule(mask);

    if (!rule) {
        ret = -ENOENT;
        goto cleanup;
    }
    
    remove_and_free(rule);

cleanup:
    mutex_unlock(&hash_table_mutex);
    return ret;
}

void remove_all_rules(void) {
    struct hlist_node *tmp;
    fad_rule *rule;
    int bkt;

    mutex_lock(&hash_table_mutex);

    hash_for_each_safe(rule_hash_table, bkt, tmp, rule, hash_node) {
        remove_and_free(rule);
    }

    mutex_unlock(&hash_table_mutex);
}

bool does_match_any_rule(const char* file) {
    fad_rule *rule;
    int bkt;
    bool found = false;

    if (!file) {
        return false;
    }

    mutex_lock(&hash_table_mutex);

    hash_for_each(rule_hash_table, bkt, rule, hash_node) {
        if (does_match(rule->mask, file)) {
            found = true;
            goto cleanup;
        }
    }

cleanup:
    mutex_unlock(&hash_table_mutex);
    return found;
}
