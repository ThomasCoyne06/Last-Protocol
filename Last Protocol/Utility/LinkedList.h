#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "Common.h"

typedef struct Element Element;
struct Element
{
	void* data;
	Element* prev;
	Element* next;
};

typedef struct
{
	Element* first;
	Element* last;
	unsigned count;
}List;

List* CreateList(void);
Element* CreateElement(void* _data);
Element* GetElement(const List* _list, int _index);
void InsertElement(List* _list, Element* _element, int _index);
void RemoveElement(List* _list, int _index);
unsigned int GetSizeList(const List* _list);
void DeleteList(List* _list);
#endif
