//
//
//
//
//



#ifndef DLL_H
#define DLL_H

#include <stdint.h>
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

#define DllCreateStaticCircular(name, data_pointer) \
	struct DllNode name = {{&name,&name},data_pointer}

#define DllCreateStaticLinear(name, data_pointer) \
	struct DllNode name = {{0,0},data_pointer}


struct DllNode* DllCreateDynamicLinear(void *data);

struct DllNode* DllCreateDynamicCircular(void *data);

struct DllList* DllAddPrevLinear(struct DllList *node_in_list, 
	struct DllList *node_to_add);

struct DllList* DllAddNextLinear(struct DllList *node_in_list, 
	struct DllList *node_to_add);

struct DllList* DllAddPrevCircular(struct DllList *node_in_list, 
	struct DllList *node_to_add);

struct DllList* DllAddNextCircular(struct DllList *node_in_list, 
	struct DllList *node_to_add);

ALWAYS_INLINE struct DllList* DllGetNext(struct DllList *origin)
{
	return origin->next;
}

ALWAYS_INLINE struct DllList* DllGetPrev(struct DllList *origin)
{
	return origin->prev;
}


void* DllRemove(struct DllNode *node_to_remove);

#endif
