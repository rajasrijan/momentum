/*
 * Copyright 2009-2019 Srijan Kumar Sharma
 * 
 * This file is part of Momentum.
 * 
 * Momentum is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Momentum is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Momentum.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _LIST_H_
#define _LIST_H_
namespace std
{
	template <class T>
	class list
	{
	private:
		struct listNode
		{
			listNode *next, *prev;
			T data;
		};
		listNode *first, *last;
	public:
		list() :first(nullptr), last(nullptr)
		{

		}
		//void operator=() {}
		//Iterators :
		class iterator
		{
		public:
			iterator() :node(nullptr), parent(nullptr)
			{}
			~iterator()
			{}
			T& operator*()
			{
				return node->data;
			}
			T* operator->()
			{
				return &(node->data);
			}
		private:
			listNode* node;
			list *parent;
		protected:
			friend list;
			iterator(listNode *currentNode, list *_parent) : node(currentNode), parent(_parent)
			{}
		};
		iterator begin()
		{
			return iterator(first, this);
		}
		//Return iterator to beginning(public member function)
		void end() {}
		//Return iterator to end(public member function)
		void rbegin() {}
		//Return reverse iterator to reverse beginning(public member function)
		void rend() {}
		//Return reverse iterator to reverse end(public member function)
		void cbegin() {}
		//Return const_iterator to beginning(public member function)
		void cend() {}
		//Return const_iterator to end(public member function)
		void crbegin() {}
		//Return const_reverse_iterator to reverse beginning(public member function)
		void crend() {}
		//Return const_reverse_iterator to reverse end(public member function)
		//Capacity:
		void empty() {}
		//Test whether container is empty(public member function)
		void size() {}
		//Return size(public member function)
		void max_size() {}
		//Return maximum size(public member function)
		//Element access :
		//Access first element(public member function)
		T& front()
		{
			return first->data;
		}

		//Access first element(public member function)
		const T& front() const
		{
			return first->data;
		}

		void back() {}
		//Access last element(public member function)
		//Modifiers:
		void assign() {}
		//Assign new content to container(public member function)
		void emplace_front() {}
		//Construct and insert element at beginning(public member function)

		//Insert element at beginning(public member function)
		void push_front(const T &t)
		{
			listNode* tmp = new listNode();
			tmp->data = t;
			tmp->prev = nullptr;
			tmp->next = first;
			if (!first)
			{
				last = tmp;
			}
			else
			{
				first->prev = tmp;
			}
			first = tmp;
		}
		//Delete first element(public member function)
		void pop_front()
		{
			auto tmp = first;
			first = first->next;
			first->prev = nullptr;
			delete tmp;
		}
		void emplace_back() {}
		//Construct and insert element at the end(public member function)
		void push_back() {}
		//Add element at the end(public member function)
		void pop_back() {}
		//Delete last element(public member function)
		void emplace() {}
		//Construct and insert element(public member function)
		void insert() {}
		//Insert elements(public member function)
		void erase() {}
		//Erase elements(public member function)
		void swap() {}
		//Swap content(public member function)
		void resize() {}
		//Change size(public member function)
		void clear() {}
		//Clear content(public member function)
		//Operations:
		void splice() {}
		//	Transfer elements from list to list(public member function)
		void remove() {}
		//Remove elements with specific value(public member function)
		void remove_if() {}
		//Remove elements fulfilling condition(public member function template)
		void unique() {}
		//Remove duplicate values(public member function)
		void merge() {}
		//Merge sorted lists(public member function)
		void sort() {}
		//Sort elements in container(public member function)
		void reverse() {}
		//Reverse the order of elements(public member function)
	};
};
#endif // !_LIST_H_
