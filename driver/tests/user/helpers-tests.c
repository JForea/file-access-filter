#include "../../src/helpers.h"

#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

typedef struct {
    const char *mask;
    const char *file;
    bool expected;
} does_match_test_case;

static void test_does_match() {
    const does_match_test_case test_cases[] = {
        {"a", "a", true},
        {"?", "a", true},
        {"*", "a", true},
        {"*", "ab", true},
        {"ab", "a", false},
        {"?", "ab", false},
        {"*a*", "a", true},
        {"ba*", "abaaa", false},
        {"ab*?*cd***", "abiiicd", true},
        {"ab?cd", "abcd", false},
        {NULL, "a", false},
        {"a", NULL, false},
        {NULL, NULL, false},
        {"", "", true},
        {"abc", "abc\0\0", true},
    };

    for (size_t i = 0; i < sizeof(test_cases) / sizeof(does_match_test_case); ++i) {
        bool actual = does_match(test_cases[i].mask, test_cases[i].file);

        CU_ASSERT_EQUAL(actual, test_cases[i].expected);
    }
}

int main() {
    CU_pSuite suite;
    unsigned int failures;

    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    suite = CU_add_suite("does_match", NULL, NULL);
    if (!suite || !CU_add_test(suite, "Wildcard matching", test_does_match)) {
        CU_cleanup_registry();
		return CU_get_error();
    }

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();

	failures = CU_get_number_of_failures();
	CU_cleanup_registry();

	return failures;
}
