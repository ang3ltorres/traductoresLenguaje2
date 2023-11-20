#pragma once

#include <iostream>

template <class T>
class Node
{
	T data;
	Node<T>* next;

public:
	Node();

	Node<T>* getNext();
	T getDato();
	void setDato(T dato);
	void setNext(Node<T>* sig);
	//friend class List<T>;
};

template <class T>
Node<T>::Node()
{
	this->next = nullptr;
}

template <class T>
Node<T>* Node<T>::getNext()
{
	return next;
}

template <class T>
T Node<T>::getDato()
{
	return this->data;
}

template <class T>
void Node<T>::setDato(T dato)
{
	this->data = dato;
}

template <class T>
void Node<T>::setNext(Node<T>* sig)
{
	this->next = sig;
}
