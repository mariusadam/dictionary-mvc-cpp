#ifndef VECTOR_H
#define VECTOR_H

#include <exception>
#include <cmath>

class VectorException : public std::exception
{
public:
	VectorException() : std::exception() {}
	VectorException(const char* msg) : std::exception{ msg } {}
};

template<typename Type> class IteratorVector;

/* Vector declaration */
template<typename Type> class Vector
{
private:
	int __size;
	int __capacity;
	Type* __elements;

	void __resize();
	void __ensureCapacity();
public:
	static const int DEFAULT_CAPACITY;
	static const double RESIZE_FACTOR;

	Vector();
	Vector(const int& dim);
	Vector(const Vector& other);
	~Vector();
	const int size() const;
	void push_back(const Type& element);
	bool has(const Type& element);
	Type& at(const int& offset) const;
	Type& pop(const int& offset);
	Type& pop();
	Type& remove(const Type& element);
	Type& operator[](const int& offset) const;
	IteratorVector<Type> begin() const;
	IteratorVector<Type> end() const;
};

/* IteratorVector  declaration */
template<typename Type> class IteratorVector
{
private:
	const Vector<Type>& __vec;
	int __current;
public:
	IteratorVector(const Vector<Type>& vec) : __vec{ vec }, __current{ 0 } {}
	IteratorVector(const Vector<Type>& vec, const int& poz) : __vec{ vec }, __current{ poz } {}
	~IteratorVector() {}
	Type& element() const;
	void next();
	bool valid() const;
	IteratorVector& operator++();
	Type& operator*() const;
	bool operator==(const IteratorVector<Type>& other) const;
	bool operator!=(const IteratorVector<Type>& other) const;
};

/* Vector implementation */
template<typename Type>const int Vector<Type>::DEFAULT_CAPACITY = 1;

template<typename Type>const double Vector<Type>::RESIZE_FACTOR = 1.5;

template<typename Type>inline Vector<Type>::Vector() {
	this->__size = 0;
	this->__capacity = DEFAULT_CAPACITY;
	this->__elements = new Type[DEFAULT_CAPACITY];
}

template<typename Type>inline Vector<Type>::Vector(const int& dim) {
	if (dim < 0) {
		throw VectorException("Invalid capacity!");
	}
	this->__size = 0;
	this->__capacity = dim;
	this->__elements = new Type[dim];
}

template<typename Type>inline  Vector<Type>::Vector(const Vector& other) {
	this->__elements = new Type[other.__size];
	for (int i = 0; i < other.__size; i++) {
		this->__elements[i] = other.__elements[i];
	}
	this->__size = other.__size;
	this->__capacity = other.__capacity;
}

template<typename Type>inline Vector<Type>::~Vector() {
	delete[] this->__elements;
}

template<typename Type>inline const int Vector<Type>::size() const {
	return this->__size;
}

template<typename Type>inline void Vector<Type>::push_back(const Type& elem) {
	this->__ensureCapacity();
	this->__elements[this->__size++] = elem;
}

template<typename Type>inline bool Vector<Type>::has(const Type & element)
{
	for (int i = 0; i < this->__size; i++) {
		if (this->__elements[i] == element) {
			return true;
		}
	}
	return false;
}

template<typename Type>inline Type& Vector<Type>::at(const int& offset) const {
	if (offset < 0 || offset >= this->__size) {
		throw VectorException("Index out of range!");
	}
	return this->__elements[offset];
}

template<typename Type>inline Type& Vector<Type>::pop(const int& offset) {
	if (offset < 0 || offset >= this->__size) {
		throw VectorException("Index out of range!");
	}
	Type elem = this->__elements[offset];
	for (int i = offset; i < this->__size - 1; i++) {
		this->__elements[i] = this->__elements[i + 1];
	}
	this->__size--;
	return elem;
}

template<typename Type>inline Type& Vector<Type>::pop() {
	if (this->__size == 0) {
		throw VectorException("The vector is empty!");
	}
	return this->pop(this->__size - 1)
}

template<typename Type>inline Type& Vector<Type>::remove(const Type& elem) {
	for (int i = 0; i < this->__size; i++) {
		if (this->__elements[i] == elem) {
			return this->pop(i);
		}
	}
	throw VectorException("Element not found!");
}

template<typename Type>inline Type& Vector<Type>::operator[](const int& offset) const {
	return this->at(offset);
}

template<typename Type>inline IteratorVector<Type> Vector<Type>::begin() const {
	return IteratorVector<Type> {*this};
}

template<typename Type>inline IteratorVector<Type> Vector<Type>::end() const {
	return IteratorVector<Type> {*this, this->__size};
}

template<typename Type>inline void Vector<Type>::__ensureCapacity() {
	if (this->__size == this->__capacity) {
		this->__resize();
	}
}

template<typename Type>inline void Vector<Type>::__resize() {
	int newCapacity = this->__capacity + (int)std::round(this->__capacity * this->RESIZE_FACTOR) + 1;
	Type* newElements = new Type[newCapacity];
	if (newElements == NULL) {
		throw VectorException("Could not resize, you ran out of memory!");
	}
	for (int i = 0; i < this->__size; i++) {
		newElements[i] = this->__elements[i];
	}
	delete[] this->__elements;
	this->__elements = newElements;
	this->__capacity = newCapacity;
}

/* IteratorVector implementation */
template<typename Type>inline Type& IteratorVector<Type>::element() const {
	return this->__vec[this->__current];
}

template<typename Type>inline void IteratorVector<Type>::next() {
	this->__current++;
}

template<typename Type>inline bool IteratorVector<Type>::valid() const {
	return this->__current != this->__vec.size();
}

template<typename Type>inline IteratorVector<Type>& IteratorVector<Type>::operator++() {
	this->next();
	return *this;
}

template<typename Type>inline bool IteratorVector<Type>::operator==(const IteratorVector<Type>& other) const {
	return this->__current == other.__current;
}

template<typename Type>inline bool IteratorVector<Type>::operator!=(const IteratorVector<Type>& other) const {
	return !(*this == other);
}

template<typename Type>inline Type& IteratorVector<Type>::operator*() const {
	return this->element();
}

#endif // !VECTOR_H


