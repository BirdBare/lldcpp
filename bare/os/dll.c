//
///
//
//
//
//







#include "dll.h"

#ifdef DLL_MALLOC
//CREATE NODE FUNCTION
struct DllNode* DllCreateDynamic(void *data)
{
  struct DllNode *node = DLL_MALLOC(sizeof(struct DllNode));

  node->data = data;
  node->list.next = node;
  node->list.prev = node;
  
  return node;
}
#endif

/*
//ADD NODE FUNCTIONS
struct DllList* DllAddPrevLinear(struct DllList *node_in_list, 
  struct DllList *node_to_add)
{
  node_to_add->next = node_in_list;
  node_to_add->prev = node_in_list->prev;
  //NodeAddress pointers

  node_in_list->prev = node_to_add;
  //HeadAddress prev changed

  return node_to_add;
}



struct DllList* DllAddNextLinear(struct DllList *node_in_list, 
  struct DllList *node_to_add)
{
  node_to_add->prev = node_in_list;
  node_to_add->next = node_in_list->next;
  //NodeAddress pointers

  node_in_list->next = node_to_add;
  //HeadAddress next changed

  return node_to_add;
}
*/

struct DllNode* DllAddBefore(struct DllNode *node_in_list, 
  struct DllNode *node_to_add)
{
  node_to_add->next = node_in_list;
  node_to_add->prev = node_in_list->prev;
  //NodeAddress pointers

  node_in_list->prev->next = node_to_add;
  node_in_list->prev = node_to_add;
  //HeadAddress prev changed

  return node_to_add;
}


struct DllNode* DllAddAfter(struct DllNode *node_in_list, 
  struct DllNode *node_to_add)
{
  node_to_add->prev = node_in_list;
  node_to_add->next = node_in_list->next;
  //NodeAddress pointers

  node_in_list->next->prev = node_to_add;
  node_in_list->next = node_to_add;
  //HeadAddress prev changed

  return node_to_add;
}
//END ADD FUNCTIONS

void * DllRemove(struct DllNode *node_to_remove)
{
  struct DllNode *next = node_to_remove->next; 
  struct DllNode *prev = node_to_remove->prev; 
  //place in variable to help compiler make it faster

  next->prev = prev;
  prev->next = next;
  //remove NodeAddress

	return node_to_remove->data;
}




