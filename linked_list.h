/**
 * linked_list.h
 * Copyright (c) 2025 Vinodh Kumar Markapuram <GreenHex@gmail.com>
 * 17-May-2025
 *
 */
#pragma once

// #define DEBUG

#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum err_t
{
	ERR_OK = 0,
	ERR_NOT_INIT = -1,
	ERR_NODE_SIZE_ZERO = -2,
	ERR_NODE_SIZE_NEGATIVE = -3,
	ERR_NODE_PTR_IS_NULL = -4,
	ERR_CMP_FUNC_NDEF = -5,
	ERR_PRT_FUNC_NDEF = -6,
	ERR_NO_NODES = -7,
	ERR_NODE_IN_LIST = -8,
	ERR_UNKNOWN = -9,
	ERR_NO_FIRST_NODE = -10,
	ERR_NO_LAST_NODE = -11
} err_t;

static const char *err_strerr[] = {
	"No error",
	"List not initialized",
	"Node size is zero",
	"Node size is less than zero"
	"Node ptr is NULL",
	"compare_func() function not defined",
	"prt_func() is not defined",
	"List has no nodes",
	"Node already in list"
	"Unknown error",
	"No first node, this is serious",
	"No last node, this is serious",
};

typedef struct node_t
{
	void *ptr;
	struct node_t *prev;
	struct node_t *next;
} node_t;

typedef struct lb_t
{
	unsigned int num_nodes;
	unsigned int size_of_elem;
	int (*cmp_func)(void *ptr1, void *ptr2, int elem_size);
	void (*prt_func)(void *ptr);
	node_t *head;
	node_t *last;
} lb_t;

lb_t *init_list(unsigned int elem_size);
unsigned int add_to_list(lb_t *lb,
						 void *ptr);
unsigned int remove_from_list(lb_t *lb,
							  void *ptr);
err_t clear_list(lb_t *lb);
err_t free_list(lb_t *lb);
unsigned int num_nodes_in_list(lb_t *lb);
int print_list(lb_t *lb,
			   void (*prt_func)(void *ptr));
err_t set_print_list_function(lb_t *lb,
							  void (*prt_func)(void *ptr));
err_t set_compare_function(lb_t *lb,
						   int (*cmp_func)(void *ptr1, void *ptr2, int elem_size));

const char *err_to_str(err_t err);

node_t *get_first_node(lb_t *lb);
node_t *get_last_node(lb_t *lb);
node_t *get_next_node(lb_t *lb, node_t *node);
node_t *get_prev_node(lb_t *lb, node_t *node);

#endif /* _LINKED_LIST_H_ */
