//
//
//
//
//



#ifndef DLL_H
#define DLL_H

#include <stdint.h>

struct DllBase
{
  struct DllNode *next;
  struct DllNode *prev;
};

struct DllNode 
{
	struct DllBase list;
  void *data;
};
//Total 12 Bytes

#define DllCreateStaticCircular(name, data_pointer) \
	struct DllNode name = {{&name,&name},data_pointer}

#define DllCreateStaticLinear(name, data_pointer) \
	struct DllNode name = {{0,0},data_pointer}


struct DllNode* DllCreateDynamicLinear(struct DllNode *data);

struct DllNode* DllCreateDynamicCircular(struct DllNode *data);

struct DllNode* DllAddPrev(struct DllNode *node_in_list, 
	struct DllNode *node_to_add);

struct DllNode* DllAddNext(struct DllNode *node_in_list, 
	struct DllNode *node_to_add);

void* DllRemove(struct DllNode *node_to_remove);

#endif
