/**
 * linked_list.c
 * Copyright (c) 2025 Vinodh Kumar Markapuram <GreenHex@gmail.com>
 * 17-May-2025
 *
 * - Duplicates not allowed.
 * - Can't handle strings, by themselves or in struct.
 *
 */

// #define NDEBUG
// #define DEBUG

#include "linked_list.h"

#include <assert.h>

typedef enum s_d
{
	FWD,
	REV
} search_direction;

static node_t *find_in_list(lb_t *lb,
							void *ptr,
							search_direction direction,
							int (*cmp_func)(void *ptr1, void *ptr2, int elem_size));
int compare_function(void *ptr1,
					 void *ptr2,
					 int elem_size);

// returns number of items in list
unsigned int add_to_list(lb_t *lb,
						 void *ptr)
{
	if (lb == NULL)
	{
		printf("ERROR: add_to_list(): List not initialized\n");
		return ERR_NOT_INIT;
	}
	assert(lb);

	if (lb->size_of_elem == 0)
	{
		printf("ERROR: add_to_list(): Node size is zero. List not initialized properly.\n");
		return ERR_NODE_SIZE_ZERO;
	}
	assert(lb->size_of_elem);

	if (ptr == NULL)
	{
		printf("ERROR: add_to_list(): Node ptr is NULL\n");
		return ERR_NODE_PTR_IS_NULL;
	}
	assert(ptr);

	if (find_in_list(lb, ptr, REV, compare_function))
		return ERR_NODE_IN_LIST; // not added

	node_t *node = calloc(1, sizeof(struct node_t));
	assert(node);

	node->ptr = calloc(1, lb->size_of_elem);
	assert(node->ptr);
	memcpy(node->ptr, ptr, lb->size_of_elem);

	// create first element, set last element
	if ((lb->head == NULL) || (lb->last == NULL))
	{
		lb->head = lb->last = node;
	}
	else // others...
	{
		node->prev = lb->last;

		lb->last->next = node;

		lb->last = node;
	}

	return ++(lb->num_nodes);
}

// returns number of items in list
unsigned int remove_from_list(lb_t *lb,
							  void *ptr)
{
	if (lb == NULL)
	{
		printf("ERROR: remove_from_list(): List not initialized\n");
		return ERR_NOT_INIT;
	}
	assert(lb);

	if (ptr == NULL)
	{
		printf("ERROR: remove_from_list(): Node ptr is NULL\n");
		return ERR_NODE_PTR_IS_NULL;
	}
	assert(ptr);

	node_t *node = NULL;

	if ((node = find_in_list(lb, ptr, REV, compare_function)))
	{
		if (node == lb->head) // 1st element
		{
			if (node->next != NULL)
			{
				lb->head = node->next;
				lb->head->prev = NULL;
			}
			else // ...is also the last
			{
				lb->head = NULL;
				lb->last = NULL;
			}
		}
		else if (node->next == NULL) // last element
		{
			node->prev->next = NULL;
			lb->last = node->prev;
		}
		else
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}

		assert(node->ptr);
		if (node->ptr != NULL)
		{
			free(node->ptr);
			node->ptr = NULL;
		}

		assert(node);
		if (node != NULL)
		{
			free(node);
			node = NULL;
		}

		--(lb->num_nodes);
	}

	return lb->num_nodes;
}

static node_t *find_in_list(lb_t *lb,
							void *ptr,
							search_direction direction,
							int (*cmp_func)(void *ptr1, void *ptr2, int elem_size))
{
	bool found = false;

	if (lb == NULL)
	{
		printf("ERROR: find_in_list(): List not initialized\n");
		return NULL;
	}
	assert(lb);

	if (ptr == NULL)
	{
		printf("ERROR: find_in_list(): Node ptr is NULL\n");
		return NULL;
	}
	assert(ptr);

	if ((lb->head == NULL) || (lb->last == NULL))
	{
		return NULL;
	}

	assert(ptr);
	assert(lb->head);
	assert(lb->last);

	node_t *node = (direction == REV) ? lb->last : lb->head;

	if (cmp_func == NULL)
	{
		cmp_func = lb->cmp_func;
	}

	if (cmp_func == NULL)
	{
		cmp_func = compare_function;
		printf("find_in_list(): Compare function not defined, using memcmp()\n");
	}

	assert(cmp_func);

	while (node)
	{

#ifdef DEBUG
		printf("find_in_list(): compare_function() result: %d\n", compare_function(node->ptr, ptr, lb->size_of_elem));
#endif /* DEBUG */

		if (!cmp_func(node->ptr, ptr, lb->size_of_elem))
		{
			found = true;
			break;
		}
		node = (direction == REV) ? node->prev : node->next;
	}
	return found ? node : NULL;
}

lb_t *init_list(unsigned int elem_size)
{
	assert(elem_size);

	if (!elem_size)
	{
		printf("ERROR: init_list(): Node size is zero\n");
		return NULL;
	}

	if (elem_size < 0)
	{
		printf("ERROR: init_list(): Node size is less than zero\n");
		return NULL;
	}

	lb_t *lb = NULL;
	lb = calloc(1, sizeof(struct lb_t));
	assert(lb);

	lb->size_of_elem = elem_size;

#ifdef DEBUG
	printf("Element size %d\n", lb->size_of_elem);
#endif /* DEBUG */

	return lb;
}

err_t clear_list(lb_t *lb)
{
	if (lb == NULL)
	{
		printf("ERROR: clear_list(): List not initialized\n");
		return ERR_NOT_INIT;
	}
	assert(lb);

	if ((lb->head == NULL) || (lb->num_nodes == 0))
	{
		printf("clear_list(): List has no nodes\n");
		return ERR_NO_NODES;
	}
	else
	{
		while (lb->head)
		{
			node_t *new_head = lb->head->next;

			assert(lb->head->ptr);
			if (lb->head->ptr != NULL)
			{
				free(lb->head->ptr);
				lb->head->ptr = NULL;
			}

			free(lb->head);
			lb->head = NULL;

			lb->head = new_head;

			--(lb->num_nodes);
		}
		lb->head = NULL;
		lb->last = NULL;

#ifdef DEBUG
		struct mallinfo2 m2 = mallinfo2();
		printf("free_list(): num_nodes: %d, Mem: %zu\n", num_nodes, m2.uordblks);
#endif /* DEBUG */
	}

	return ERR_OK;
}

err_t free_list(lb_t *lb)
{
	if (lb == NULL)
	{
		printf("ERROR: free_list(): List not initialized\n");
		return ERR_NOT_INIT;
	}
	assert(lb);

	if ((lb->head == NULL) || (lb->num_nodes == 0))
	{
		printf("free_list(): List has no elements, freeing list block (lb)\n");
	}
	else
	{
		clear_list(lb);
	}

	free(lb);
	lb = NULL;

	return ERR_OK;
}

unsigned int num_nodes_in_list(lb_t *lb)
{
	return lb->num_nodes;
}

int print_list(lb_t *lb,
			   void (*prt_func)(void *ptr))
{
	if (lb == NULL)
	{
		printf("ERROR: print_list(): List not initialized\n");
		return ERR_NOT_INIT;
	}

	assert(lb);

	if (prt_func == NULL)
	{
		prt_func = lb->prt_func;
	}

	assert(prt_func);

	if (prt_func == NULL)
	{
		printf("ERROR: print_list(): Cannot print list, prt_func() is undefined\n");
		return ERR_PRT_FUNC_NDEF;
	}

	if (lb->head)
	{
		node_t *node = lb->head;

		while (node)
		{
			(*prt_func)(node->ptr);
			node = node->next;
		}
	}
	printf("[%d %s]\n", lb->num_nodes, lb->num_nodes == 1 ? "node" : "nodes");

#ifdef DEBUG
	struct mallinfo2 m2 = mallinfo2();
	printf("print_list(): num_nodes: %d, Mem: %zu\n", lb->num_nodes, m2.uordblks);

	// malloc_stats();
#endif /* DEBUG */

	return ERR_OK;
}

err_t set_print_list_function(lb_t *lb,
							  void (*prt_func)(void *ptr))
{
	if (lb == NULL)
	{
		printf("ERROR: set_print_list_function(): List not initialized\n");
		return ERR_NOT_INIT;
	}

	assert(lb);

	if (prt_func)
	{
		lb->prt_func = prt_func;
	}

	if (prt_func == NULL)
	{
		printf("ERROR: set_print_list_function(): prt_func() is undefined\n");
		return ERR_PRT_FUNC_NDEF;
	}
	assert(prt_func);

	return ERR_OK;
}

// compare_function() should return 0 on match
err_t set_compare_function(lb_t *lb,
						   int (*cmp_func)(void *ptr1, void *ptr2, int elem_size))
{
	if (lb == NULL)
	{
		printf("ERROR: set_compare_function(): List not initialized\n");
		return ERR_NOT_INIT;
	}

	assert(lb);

	if (lb && cmp_func)
	{
		lb->cmp_func = cmp_func;
	}

	return ERR_OK;
}

int compare_function(void *ptr1,
					 void *ptr2,
					 int elem_size)
{
	return memcmp(ptr1, ptr2, elem_size);
}

// Utilities...

const char *err_to_str(err_t err)
{
	if (err > 0)
	{
		err = ERR_UNKNOWN;
	}
	return err_strerr[-err];
}

node_t *get_first_node(lb_t *lb)
{
	if (lb == NULL)
	{
		printf("ERROR: get_first_node(): List not initialized\n");
		return NULL;
	}
	assert(lb);

	if (lb->last == NULL)
	{
		printf("ERROR: get_last_node(): No first node, this is serious.\n");
		return NULL;
	}
	assert(lb->head);

	return lb->head;
}

node_t *get_last_node(lb_t *lb)
{
	if (lb == NULL)
	{
		printf("ERROR: get_last_node(): List not initialized\n");
		return NULL;
	}
	assert(lb);

	if (lb->last == NULL)
	{
		printf("ERROR: get_last_node(): No last node, this is serious.\n");
		return NULL;
	}
	assert(lb->last);

	return lb->last;
}

node_t *get_next_node(lb_t *lb, node_t *node)
{
	if (lb == NULL)
	{
		printf("ERROR: get_next_node(): List not initialized\n");
		return NULL;
	}
	assert(lb);

	if (node == NULL)
	{
		printf("ERROR: get_next_node(): Node ptr is NULL\n");
		return NULL;
	}
	assert(node);

	if (node->next == NULL)
	{
		printf("get_next_node(): This is the last node.\n");
		return NULL;
	}
	assert(node->next);

	return node->next;
}

node_t *get_prev_node(lb_t *lb, node_t *node)
{
	if (lb == NULL)
	{
		printf("ERROR: get_prev_node(): List not initialized\n");
		return NULL;
	}
	assert(lb);

	if (node == NULL)
	{
		printf("ERROR: get_prev_node(): Node ptr is NULL\n");
		return NULL;
	}
	assert(node);

	if (node->prev == NULL)
	{
		printf("get_prev_node(): This is the first node.\n");
		return NULL;
	}
	assert(node->prev);

	return node->prev;
}
