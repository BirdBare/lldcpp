//
//
//
//
//

#ifndef BARELIST_h
#define BARELIST_h

#include "bare_defines.h"

namespace bare
{

 class BareList
 {
	private:
		BareList *_next;
		BareList *_prev;
		
	public:
		BareList()
		:_next(this), _prev(this)
		{}

		template <typename T, typename ListContainer>
			inline ListContainer& Container(T (ListContainer::*List_Member_Name))
			{
				return *(ListContainer *)( (char *)this - ( 
					(char *)&(( (ListContainer *) 0)->*List_Member_Name) ));	
			}

		bool IsEmpty(void)
		{
			if(_next != this)
				return false;
			//if next doesnt equal itself then it is already part of a list

			return true;
		}

		int Size(void)
		{
			BareList *start = this;
			int count = 0;

			do
			{
				start = start->_next;
				count++;

			} while(start != this);

			return count;
		}

		BareList& Next(void)
		{
			return *_next;
		}

		BareList& Prev(void)
		{
			return *_prev;
		}

		bool InsertNext(BareList &head)
		{
			if(IsEmpty() == false)
				return false;
			
			_next = head._next;
			_prev = &head;

			head._next->_prev = this;
			head._next = this;

			return true;
		}

		bool InsertPrev(BareList &head)
		{
			if(IsEmpty() == false)
				return false;
			
			_prev = head._prev;
			_next = &head;

			head._prev->_next = this;
			head._prev = this;

			return true;
		}

		bool Remove(void)
		{
			if(IsEmpty() == true)
				return false;

			_next->_prev = _prev;
			_prev->_next = _next;

			_prev = this;
			_next = this;
			
			return true;
		}

	};
}



#endif
