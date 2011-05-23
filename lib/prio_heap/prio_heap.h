#ifndef _LTTNG_PRIO_HEAP_H
#define _LTTNG_PRIO_HEAP_H

/*
 * prio_heap.h
 *
 * Static-sized priority heap containing pointers. Based on CLRS,
 * chapter 6.
 *
 * Copyright 2011 - Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 */

#include <linux/gfp.h>

struct ptr_heap {
	size_t size, max;
	void **ptrs;
	int (*gt)(void *a, void *b);
};

/**
 * heap_maximum - return the largest element in the heap
 * @heap: the heap to be operated on
 *
 * Returns the largest element in the heap, without performing any modification
 * to the heap structure. Returns NULL if the heap is empty.
 */
static inline void *heap_maximum(const struct ptr_heap *heap)
{
	return heap->size ? heap->ptrs[0] : NULL;
}

/**
 * heap_init - initialize the heap
 * @heap: the heap to initialize
 * @size: maximum number of elements
 * @gfp: allocation flags
 * @gt: function to compare the elements
 *
 * Returns -ENOMEM if out of memory.
 */
extern int heap_init(struct ptr_heap *heap, size_t size,
		     gfp_t gfpmask, int gt(void *a, void *b));

/**
 * heap_free - free the heap
 * @heap: the heap to free
 */
extern void heap_free(struct ptr_heap *heap);

/**
 * heap_insert - insert an element into the heap
 * @heap: the heap to be operated on
 *
 * Insert an element into the heap. If the heap is full, return the
 * largest element between those previously present in the heap and the
 * element being added, else return NULL.
 */
extern void *heap_insert(struct ptr_heap *heap, void *p);

/**
 * heap_remove - remove the largest element from the heap
 * @heap: the heap to be operated on
 *
 * Returns the largest element in the heap. It removes this element from the
 * heap. Returns NULL if the heap is empty.
 */
extern void *heap_remove(struct ptr_heap *heap);

/**
 * heap_cherrypick - remove a given element from the heap
 * @heap: the heap to be operated on
 * @p: the element
 *
 * Remove the given element from the heap. Return the element if present, else
 * return NULL. This algorithm has a complexity of O(n), which is higher than
 * O(log(n)) provided by the rest of this API.
 */
extern void *heap_cherrypick(struct ptr_heap *heap, void *p);

/**
 * heap_replace_max - replace the the largest element from the heap
 * @heap: the heap to be operated on
 * @p: the pointer to be inserted as topmost element replacement
 *
 * Returns the largest element in the heap. It removes this element from the
 * heap. The heap is rebalanced only once after the insertion. Returns NULL if
 * the heap is empty.
 *
 * This is the equivalent of calling heap_remove() and then heap_insert(), but
 * it only rebalances the heap once.
 */
extern void *heap_replace_max(struct ptr_heap *heap, void *p);

#endif /* _LTTNG_PRIO_HEAP_H */