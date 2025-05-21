/**
 * main.c
 * Copyright (c) 2025 Vinodh Kumar Markapuram <GreenHex@gmail.com>
 * 18-May-2025
 *
 * - Duplicates not allowed.
 * - Can't handle strings, either by themselves or in a struct.
 *
 */
// #define NDEBUG
#include <assert.h>

#include "linked_list.h"

void pr_fn(void *ptr);

typedef struct s_t
{
	int i;
	float f;
	char c;
} s1_t;

void pr_fn(void *ptr)
{
	s1_t *a = (s1_t *)ptr;
	printf("%d %f %c, ", a->i, a->f, a->c);
}

int test(void)
{
	s1_t s1;
	s1_t s2;
	s1_t s3;

	memset(&s1, 0, sizeof(s1_t));
	memset(&s2, 0, sizeof(s1_t));
	memset(&s3, 0, sizeof(s1_t));

	lb_t *lb = init_list(sizeof(s1_t));
	assert(lb);

	s1.i = 10;
	s1.f = 9.141;
	s1.c = 'a';

	add_to_list(lb, &s1);

	// remove_from_list(lb, &s1);

	s3.c = 'i';
	add_to_list(lb, &s3);
	print_list(lb, pr_fn);

	remove_from_list(lb, &s3);

	print_list(lb, pr_fn);

	free_list(lb);

	return 0;
}

int main()
{
	test();

	return 0;
}
