#include "LinkedList.h"

List* CreateList(void)
{
	return calloc(1, sizeof(List));
}

Element* CreateElement(void* _data)
{
	Element* newElement = calloc(1, sizeof(Element));
	if (newElement != NULL)
	{
		newElement->data = _data;
		newElement->prev = NULL;
		newElement->next = NULL;
	}
	return newElement;
}

Element* GetElement(const List* _list, int _index)
{
	if (_index < 0)
	{
		return NULL;
	}
	Element* actualElement = _list->first;

	int index = 0;
	while (index < _index && actualElement != NULL)
	{
		actualElement = actualElement->next;
		index++;
	}

	return actualElement;
}

void InsertElement(List* _list, Element* _element, int _index)
{
	if (_index == _list->count)
	{
		_list->last = _element;
	}

	// Ajout en début de liste
	if (_index == 0)
	{
		if (_list->first != NULL)
		{
			_list->first->prev = _element;
		}
		//L'élément précedent le premier n'existe pas
		_element->prev = NULL;
		//L'élément aprés le nouveau premier est l'ancient premier
		_element->next = _list->first;
		//Le pointeur "first" pointe vers le nouveau premier
		_list->first = _element;
		_list->count++;
	}
	else
	{
		Element* previousElement = GetElement(_list, _index - 1);
		if (previousElement != NULL)
		{
			_element->next = previousElement->next;
			_element->prev = previousElement;
			previousElement->next = _element;
			_list->count++;
		}
	}
}

void RemoveElement(List* _list, int _index)
{
	// L'index est négatif
	if (_index < 0)
	{
		return;
	}
	else if (_index == 0)
	{
		// Suppression du 1er élément de la liste
		Element* elementToDelete = _list->first;
		_list->first = elementToDelete->next;
		if (_list->first != NULL)
		{
			_list->first->prev = NULL;
		}
		free(elementToDelete);
		_list->count--;
	}
	else
	{
		// Suppression un élément de la liste
		Element* previousElement = GetElement(_list, _index - 1);
		Element* elementToDelete = previousElement->next;
		// On supprime un élément qui ne se trouve pas en dehors de la liste
		if (elementToDelete != NULL)
		{
			previousElement->next = elementToDelete->next;

			if (previousElement->next != NULL)
			{
				previousElement->next->prev = previousElement;
			}
			free(elementToDelete);
			_list->count--;
		}
	}

	if (_index == _list->count)
	{
		_list->last = GetElement(_list, _list->count - 1);
	}
}

unsigned GetSizeList(const List* _list)
{
	Element* actualElement = _list->first;

	unsigned sizeList = 0;
	while (actualElement != NULL)
	{
		actualElement = actualElement->next;
		sizeList++;
	}

	return sizeList;
}

void DeleteList(List* _list)
{
	Element* elementToDelete = _list->first;
	while (_list->first != NULL)
	{
		_list->first = _list->first->next;
		free(elementToDelete->data);
		free(elementToDelete);
		elementToDelete = _list->first;
	}
	free(_list);
}

void DrawList(const List* _list)
{
	//	Element* actualElement = _list->first;
	//	unsigned index = 0;
	//
	//	while (actualElement != NULL)
	//	{
	//		actualElement = actualElement->next;
	//		index++;
	//	}
}
//
void DrawRevertList(const List* _list)
{
	//	Element* actualElement = _list->last;
	//	int index = _list->count - 1;
	//	while (actualElement != NULL)
	//	{
	//		actualElement = actualElement->prev;
	//		index--;
	//	}
}