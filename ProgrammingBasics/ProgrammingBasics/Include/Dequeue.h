#pragma once

template<class Type> class Deck
{
private:
	class Element
	{
	public:
		Element * Next = nullptr;
		Element* Prev = nullptr;
		Type value;
	};

	Element* Head = nullptr;
	Element* Tail = nullptr;
	int Size = 0;
public:
	Deck() {}
	int GetSize() {
		return Size;
	}

	void PushHead(Type newValue) {
		Element* NewElement = new Element;
		NewElement->value = newValue;
		if (Size != 0) {
			Head->Prev = NewElement;
			NewElement->Next = Head;
		}
		else
		{
			Tail = NewElement;
		}
		Head = NewElement;
		++Size;
		return;
	}

	void PushTail(Type newValue) {
		if (Size == 0) {
			PushHead(newValue);
			return;
		}
		Element* NewElement = new Element;
		NewElement->value = newValue;
		NewElement->Prev = Tail;
		Tail->Next = NewElement;
		Tail = NewElement;
		++Size;
		return;
	}

	Type PopHead() {
		if (Size == 0) {
			return 0;
		}
		Type head = Head->value;
		Element* temp = Head;
		Head = Head->Next;
		--Size;
		delete temp;
		return head;
	}

	Type PopTail() {
		if (Size == 0) {
			return 0;
		}
		Type tail = Tail->value;
		Element* temp = Tail;
		Tail = Tail->Prev;
		--Size;
		delete temp;
		return tail;
	}

	void DeleteDeck() {
		while (Size != 0)
		{
			PopHead();
		}
		return;
	}
};