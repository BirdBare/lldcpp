//
//
//
//
//



#ifndef DLL_H
#define DLL_H

#include "bare_defines.h"

struct DllList
{
  struct DllList *next;
  struct DllList *prev;
};

struct DllNode 
{
	struct DllList list;
  void *data;
};
//Total 12 Bytes

static inline void DllInitNode(struct DllNode *new_node, void *data)
{
	new_node->list.next = &new_node->list;
	new_node->list.prev = &new_node->list;
	new_node->data = data;
}

struct DllList* DllAddBefore(struct DllList *node_in_list, 
	struct DllList *node_to_add);

struct DllList* DllAddAfter(struct DllList *node_in_list, 
	struct DllList *node_to_add);

ALWAYS_INLINE void* DllGetNext(struct DllList *origin)
{
	return origin->next;
}

ALWAYS_INLINE void* DllGetPrev(struct DllList *origin)
{
	return origin->prev;
}


void DllRemove(struct DllList *node_to_remove);

#endif
