//
//
//
//
//

#include <stdio.h>
#include "dll.h"

int main(void)
{

	DllCreateStatic(list,0);
	DllCreateStatic(list1,0);
	DllCreateStatic(list2,0);
	DllCreateStatic(list3,0);

	DllAddAfter(&list, &list3);
	DllAddAfter(&list, &list2);
	DllAddAfter(&list, &list1);

	printf("\n\n %u \n\n %u \n\n %u \n\n %u \n\n end", &list, &list1, &list2, &list3);
	

	printf("\n\n %u \n\n %u \n\n %u \n\n %u \n\n end", list.list.next, 
	list1.list.next, list2.list.next, list3.list.next);


	return 1;
}
