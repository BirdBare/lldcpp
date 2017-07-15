//
///
//
//
//
//







#include "dll.h"

#ifdef DLL_MALLOC
//CREATE NODE FUNCTION
struct DllNode* DllCreateDynamic(struct DllNode *data)
{
  struct DllNode *node = DLL_MALLOC(sizeof(struct DllNode));

  node->data = data;
  node->list.next = node;
  node->list.prev = node;
  
  return node;
}
#endif

//ADD NODE FUNCTIONS
struct DllNode* DllAddPrev(struct DllNode *node_in_list, 
  struct DllNode *node_to_add)
{
  node_to_add->list.next = node_in_list;
  node_to_add->list.prev = node_in_list->list.prev;
  //NodeAddress pointers

  node_in_list->list.prev->list.next = node_to_add;
  node_in_list->list.prev = node_to_add;
  //HeadAddress prev changed

  return node_to_add;
}


struct DllNode* DllAddNext(struct DllNode *node_in_list, 
  struct DllNode *node_to_add)
{
  node_to_add->list.prev = node_in_list;
  node_to_add->list.next = node_in_list->list.next;
  //NodeAddress pointers

  node_in_list->list.next->list.prev = node_to_add;
  node_in_list->list.next = node_to_add;
  //HeadAddress prev changed

  return node_to_add;
}
//END ADD FUNCTIONS

void* DllRemove(struct DllNode *node_to_remove)
{
  struct DllNode *next = node_to_remove->list.next; 
  struct DllNode *prev = node_to_remove->list.prev; 
  //place in variable to help compiler make it faster

  next->list.prev = prev;
  prev->list.next = next;
  //remove NodeAddress

  return node_to_remove->data;
}




