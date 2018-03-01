//
//
//
//
//



#ifndef DLL_H
#define DLL_H

#include "bare_defines.h"

struct DllNode 
{
	struct DllNode *next;
	struct DllNode *prev;
  void *data;
};
//Total 12 Bytes

static inline void DllInitNode(struct DllNode *new_node, void *data)
{
	new_node->next = new_node;
	new_node->prev = new_node;
	new_node->data = data;
}

struct DllNode* DllAddBefore(struct DllNode *node_in_list, 
	struct DllNode *node_to_add);

struct DllNode* DllAddAfter(struct DllNode *node_in_list, 
	struct DllNode *node_to_add);

ALWAYS_INLINE void* DllGetAfter(struct DllNode *origin)
{
	return origin->next;
}

ALWAYS_INLINE void* DllGetBefore(struct DllNode *origin)
{
	return origin->prev;
}


void * DllRemove(struct DllNode *node_to_remove);

#endif
