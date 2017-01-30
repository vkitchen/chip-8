#include "unity.h"
#include "string2.h"

void test_string_append(void)
	{
	/* test the normal use case */
	struct string *s = string_new_c("cat");
	string_append_c(s, "dog");
	TEST_ASSERT_EQUAL_STRING(s->str, "catdog");
	TEST_ASSERT_EQUAL_UINT(s->bytes, 6);

	string_free(s);

	/* test an originally empty string */
	s = string_new();
	string_append_c(s, "dog");
	TEST_ASSERT_EQUAL_STRING(s->str, "dog");
	TEST_ASSERT_EQUAL_UINT(s->bytes, 3);
	}

int main(void)
	{
	UNITY_BEGIN();
	RUN_TEST(test_string_append);
	return UNITY_END();
	}
