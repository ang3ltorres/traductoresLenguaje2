#pragma once

#include <iostream>
#include "node.hpp"
#include <string>

template <class T>
class List
{
	Node<T> *head;
	std::string cadena;

public:
	List();
	~List();
	int isEmpty();
	void display();
	int length();
	void push(T);
	void reconocimiento(T);
	T pop();
	T top();
	void expresionFinal();
	void buscar(T,T);
	void removeAll();
	void setCadena();
};

template <class T>
void List<T>::setCadena()
{
	cadena = "";
}

template <class T>
List<T>::List()
{
	head = nullptr;
}

template <class T>
List<T>::~List()
{
	Node<T> *p;
	Node<T> *q;

	p = head;

	while (p != nullptr)
	{
		q = p->getNext();
		delete p;
		p = q;
	}

	delete head;
	head = nullptr;
}

template <class T>
void List<T>::removeAll()
{
	this->~List();
}

template <class T>
void List<T>::expresionFinal()
{
	int tamano = this->length();
	for (int i = 1; i <= tamano; i++)
	{
		if (top() == '(' || top() == ')')
		{
			std::cout<<"Error de sintaxis en la expresion\n";
			return;
		}
		cadena = cadena + this->pop();
	}
	//this->removeAll();
	for (int j = 0; j < int(cadena.size()); j++)
		std::cout<<cadena[j];

	cadena = "";
	return;
}

template <class T>
int List<T>::isEmpty()
{
	return (head == nullptr);
}

template <class T>
void List<T>::display()
{
	Node<T> *p;

	p = head;
	while (p != nullptr)
	{
		std::cout << p->getDato() << " ";
		p = p->getNext();
	}
	std::cout << '\n';
}

template <class T>
int List<T>::length()
{
	Node<T> *p;
	int acum = 0;
	p = head;

	while (p != nullptr)
	{
		p = p->getNext();
		acum++;
	}
	return acum;
}

template <class T>
void List<T>::reconocimiento(T obj)
{
	switch(obj)
	{
		case '(':
			this->push('(');
			break;

		case ')':
			//std::cout<<"introduciste un )"<<endl;
			while(this->top() != '(')
				cadena = cadena + this->pop();
			this->pop();
			break;

		case '+':
			//std::cout<<"introduciste un +"<<endl;
			while(this->top()=='*'|| this->top()=='/'|| this->top()=='^')
				cadena = cadena + this->pop();
			this->push('+');
			break;

		case '-':
			while(this->top()=='*'|| this->top()=='/'|| this->top()=='^')
				cadena = cadena + this->pop();
			this->push('-');
			break;

		case '*':
			while(this->top()=='^')
				cadena = cadena + this->pop();
			this->push('*');
			break;

		case '/':
			while(this->top()=='^')
				cadena = cadena + this->pop();
			this->push('/');
			break;

		case '^':
			this->push('^');
			break;

		default:
			cadena = cadena + obj;
	}
}

template <class T>
void List<T>::buscar(T caracter1, T caracter2)
{
	int i = 0;
	Node<T> *p;
	
	p = head;
	this->pop();
	// return;

	while ( i < this->length())
	{
		if (this->top() == caracter1 || this->top() == caracter2)
		{
			cadena = cadena + this->top();
			this->pop();
		}
		i++;
	}
	return;
}

template <class T>
void List<T>::push(T obj)
{
	Node<T>* nuevo;

	nuevo = new Node<T>;
	nuevo->setDato(obj);

	if(head == nullptr)
	{
		nuevo->setNext(head);
		head = nuevo;
		return;
	}

	Node<T> *p;
	p = head;

	while (p->getNext() != nullptr)
		p = p->getNext();

	nuevo->setNext(p->getNext());
	p->setNext(nuevo);
}

template <class T>
T List<T>::pop()
{
	if (isEmpty())
		return 0;

	if (this->length() == 1)
	{
		Node<T> *p;

		if (head != nullptr)
		{
			p = head;
			T valor = p->getDato();
			head = head->getNext();
			delete p;
			return valor;
		}

		return 0;
	}

	Node<T> *p;
	Node<T> *q;

	if (head != nullptr)
	{
		p = head;
		q = nullptr;

		while (p->getNext() != nullptr)
		{
			q = p;
			p = p->getNext();
		}

		T valor = p->getDato();
		q->setNext(p->getNext());

		delete p;
		return valor;
	}

	return 0;
}

template <class T>
T List<T>::top()
{
	if (isEmpty())
		return 0;
	
	Node<T> *p;

	if (head != nullptr)
	{
		p = head;

		while (p->getNext() != nullptr)
			p = p->getNext();
		
		T valor = p->getDato();
		return valor;
	}

	return 0;
}
