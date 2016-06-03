#ifndef MULTIMAP_H
#define MULTIMAP_H

#include "vector.h"
#include "entry.h"
#include <exception>
#include <iostream>

class MultiMapException : public std::exception {
public:
	MultiMapException() : std::exception() {}
	MultiMapException(const char* msg) : std::exception{ msg }{}
};

/* Iterator pentru MultiDictionar */
template<typename TKey, typename TValue>class IteratorMultiMap;

/* Clasa responsabila pentru container-ul MultiDictionar */
template<typename TKey, typename TValue>class MultiMap {
private:
	int __size;
	int __capacity;
	Entry<TKey, TValue>* __entries;

	int __dispersion(const Entry<TKey, TValue> &entry, int i) const{
		int hash = entry.hashCode();
		//verificare liniara
		return (hash + i) % this->__capacity;
	}
	void __redispersion();
	bool __put(const TKey &key, const TValue &value);
public:
	template<typename TKey, typename TValue>friend class IteratorMultiMap;
	static const int DEFAULT_CAPACITY;

	MultiMap();
	~MultiMap();
	void add(const TKey &key, const TValue &value);
	bool containsEntry(const TKey &key, const TValue &value) const;
	bool containsKey(const TKey &key) const;
	Vector<Entry<TKey, TValue>> entries() const;
	Vector<TValue> get(const TKey &key) const;
	Vector<TKey> keys() const;
	Vector<TValue> values() const;
	Vector<TKey> keySet() const;
	void remove(const TKey &key, const TValue &value);
	void removeAll(const TKey &key);
	bool empty() const;
	Vector<TValue> operator[](const TKey &key) const;
	const int capacity() const;
	const int size() const;
	IteratorMultiMap<TKey, TValue> begin() const;
	IteratorMultiMap<TKey, TValue> end() const;

};

/* Start of MultiMap implementation */

/*
	Dimensiunea initiala a tabelei de dispersie
*/
template<typename TKey, typename TValue>const int MultiMap<TKey, TValue>::DEFAULT_CAPACITY = 2;

/*
	Constructorul implicit pentru TAD MultiDictionar
*/
template<typename TKey, typename TValue>MultiMap<TKey, TValue>::MultiMap() {
	this->__size = 0;
	this->__capacity = this->DEFAULT_CAPACITY;
	this->__entries = new Entry<TKey, TValue>[this->__capacity];
}

/*
	Destructor
*/
template<typename TKey, typename TValue>MultiMap<TKey, TValue>::~MultiMap() {
	delete[] this->__entries;
}

/*
	@input  : true
	@output : realoca o noua tabela si adauga elementele din tabela veche
		      arunca MultiMapException daca nu s-a putut realoca tabela in caz de depasire,
					 multidictionarul ramane intr-o stare valida
	Complexitate : C.F. = C.M. = C.D. : theta(n)
*/
template<typename TKey, typename TValue>void MultiMap<TKey, TValue>::__redispersion() {
	Entry<TKey, TValue>* oldEntries = this->__entries;
	int newCapacity = this->__capacity * 2 + 1;
	this->__entries = new Entry<TKey, TValue>[newCapacity];
	if (this->__entries == NULL) {
		//restore the values if could not allocate
		this->__entries = oldEntries;
		throw DictionarException("Could not resize, you ran out of memory!");
	}
	int oldCapacity = this->__capacity;
	this->__capacity = newCapacity;
	this->__size = 0;
	for (int i = 0; i < oldCapacity; i++) {
		if (oldEntries[i].isValid()) {
			this->add(oldEntries[i].getKey(), oldEntries[i].getValue());
		}
	}
	delete[] oldEntries;
}

/*
	@input  : key  - TKey	
			  value- TValue
			  garanteaza adaugarea cheii key, value, daca exista spatiu in dictionar
	@output : true, daca s-a putut adauga pereche
			  false, altfel
	Complexitate : C.F. = C.M. : O(1), daca s-a putut adauga perechea
				   C.D. : O(dim), daca nu mai este spatiu in tabela
*/
template<typename TKey, typename TValue>bool MultiMap<TKey, TValue>::__put(const TKey &key, const TValue &value) {
	int i, j;
	Entry<TKey, TValue> newEntry{ key, value };
	bool added = false;
	/*
		i - numarul de verificare
		j - locatia de verificat
	*/
	for (i = 0; i < this->__capacity && !added; i++) {
		j = this->__dispersion(newEntry, i);
		if (this->__entries[j].isValid() == false) {
			//am gasit o pozitie libera
			this->__entries[j] = newEntry;
			this->__size++;
			added = true;
		}
	}
	return added;
}

/*
	@input  : key  - TKey	
			  value- TValue
	@output : adauga o noua pereche cheie, valoare in multi dictionar
			  arunca MultiMapException daca nu s-a putut realoca tabela in caz de depasire,
					 multidictionarul ramane intr-o stare valida
	Complexitate : C.F. = C.M. = C.D. : O(1) (realocarea este amortizata in timp)
*/
template<typename TKey, typename TValue>void MultiMap<TKey, TValue>::add(const TKey &key, const TValue &value) {
	if (this->__put(key, value) == false) {
		//nu mai este spatiu --> realocam, redispersam si adaugam din nou
		this->__redispersion();
		this->__put(key, value);
	}
}

/*
	@input  : key - TKey
	@ouput  : true, daca multidictionarul contine cheia key
			  false, altfel
	Complexitate : C.F. : O(1) la cautare cu succes
				   C.D. : O(dim) la cautare fara success 
*/
template<typename TKey, typename TValue>bool MultiMap<TKey, TValue>::containsKey(const TKey &key) const{
	int i, j;
	Entry<TKey, TValue> entry{ key };
	for (i = 0; i < this->__capacity; i++) {
		j = this->__dispersion(entry, i);
		if (this->__entries[j].isValid() && this->__entries[j].getKey() == key) {
			return true;
		}
	}
	return false;
 }

/*
	@input  : key  - TKey
			  value- TValue
	@ouput  : true, daca multidictionarul contine perechea key, value
			  false, altfel
	Complexitate : C.F. = C.M. = C.D. : O(1) la cautare cu succes
				   C.F. = C.M. = C.D. : O(dim) la cautare fara success 
*/
template<typename TKey, typename TValue>bool MultiMap<TKey, TValue>::containsEntry(const TKey &key, const TValue &value) const {
	int i, j;
	Entry<TKey, TValue> entry{ key, value };
	for (i = 0; i < this->__capacity; i++) {
		j = this->__dispersion(entry, i);
		if (this->__entries[j].isValid() && this->__entries[j] == entry) {
			return true;
		}
	}
	return false;
}

/*
	@input  : true
	@output : colectia tuturor perechilor cheie, valoare din multi dictionar
	Complexitate : C.F. = C.M. = C.D. : theta(n)
*/
template<typename TKey, typename TValue>Vector<Entry<TKey, TValue>> MultiMap<TKey, TValue>::entries() const{
	Vector<Entry<TKey, TValue>> entries;
	for (const auto& entry : *this) {
		entries.push_back(entry);
	}
	return entries;
}

/*
	@input  : key - TKey
	@output : returneaza lista valorilor asociate cheii key
			  arunca MultiMapException daca cheia key nu exista in multidictionar
*/
template<typename TKey, typename TValue>Vector<TValue> MultiMap<TKey, TValue>::get(const TKey &key) const {
	if (this->containsKey(key) == false) {
		std::string error{ "KeyError : key could not be found!" };
		throw MultiMapException(error.c_str());
	}
	Vector<TValue> values;
	for (int i = 0; i < this->__capacity; i++) {
		if (this->__entries[i].isValid() && this->__entries[i].getKey() == key) {
			values.push_back(this->__entries[i].getValue());
		}
	}
	return values;
}

/*
	@input  : true
	@output : colectia tuturor cheilor din multidictionar
	Complexitate : C.F. = C.M. = C.D. : theta(n)
*/
template<typename TKey, typename TValue>Vector<TKey> MultiMap<TKey, TValue>::keys() const {
	Vector<TKey> keys;
	for (int i = 0; i < this->__capacity; i++) {
		if (this->__entries[i].isValid()) {
			keys.push_back(this->__entries[i].getKey());
		}
	}
	return keys;
}

/*
	@input  : true
	@output : multimea tuturor cheilor din multidictionar
	Complexitate : C.F. = C.M. = C.D. : theta(n)
*/
template<typename TKey, typename TValue>Vector<TKey> MultiMap<TKey, TValue>::keySet() const {
	Vector<TKey> keys;
	for (int i = 0; i < this->__capacity; i++) {
		if (this->__entries[i].isValid() && keys.has(this->__entries[i].getKey()) == false) {
			keys.push_back(this->__entries[i].getKey());
		}
	}
	return keys;
}

/*
	@input  : true
	@output : colectia tuturor valorilor din multidictionar
	Complexitate : C.F. = C.M. = C.D. : theta(n)
*/
template<typename TKey, typename TValue>Vector<TValue> MultiMap<TKey, TValue>::values() const {
	Vector<TValue> values;
	for (int i = 0; i < this->__capacity; i++) {
		if (this->__entries[i].isValid()) {
			values.push_back(this->__entries[i].getValue());
		}
	}
	return values;
}

/*
	@input  : key  - TKey
			  value- TValue
	@output : sterge perechea key, value din multidictionar
			  arunca MultiMapException daca perechea nu a fost gasita
	Complexitate : C.F. = C.M. = C.D. : O(1)
*/
template<typename TKey, typename TValue>void MultiMap<TKey, TValue>::remove(const TKey &key, const TValue &value)  {
	if (this->containsKey(key) == false) {
		std::string error{ "KeyError : key could not be found!" };
		throw MultiMapException(error.c_str());
	}

	int i, j;
	bool foundAtLeastOneEntry = true;
	Entry<TKey, TValue> entry{ key, value };
	for (i = 0; i < this->__capacity; i++) {
		j = this->__dispersion(entry, i);
		if (this->__entries[j].isValid() && this->__entries[j] == entry) {
			this->__entries[j].makeInvalid();
			this->__size--;
			foundAtLeastOneEntry = true;
		}
	}
	if (foundAtLeastOneEntry == false) {
		std::string error{ "KeyError : entry could not be found!" };
		throw MultiMapException(error.c_str());
	}
}

/*
	@input  : key  - TKey
	@output : sterge toate perechile cheie, valoare care au cheia key
			  arunca MultiMapException daca cheia nu a fost gasita
	Complexitate : C.F. = C.M. = C.D. : O(dim)
*/
template<typename TKey, typename TValue>void MultiMap<TKey, TValue>::removeAll(const TKey &key)  {
	if (this->containsKey(key) == false) {
		std::string error{ "KeyError : key could not be found!" };
		throw MultiMapException(error.c_str());
	}

	int i, j;
	Entry<TKey, TValue> entry{ key };
	for (i = 0; i < this->__capacity; i++) {
		j = this->__dispersion(entry, i);
		if (this->__entries[j].isValid() && this->__entries[j].getKey() == key) {
			this->__entries[j].makeInvalid();
			this->__size--;
		}
	}
}

/*
	@input  : true
	@output : true, daca multidictionarul este gol
			  false, altfel
	Complexitate : C.F. = C.M. = C.D. : O(1)
*/
template<typename TKey, typename TValue>bool MultiMap<TKey, TValue>::empty() const {
	return this->__size == 0;
}

/*
	Suprascrie operatorul de indexare []
	@input  : key  TKey
	@output : returneaza lista valorilor asociate cheii key
			  arunca MultiMapException daca cheia key nu exista in multidictionar
	Complexitate : C.F. = C.M. = C.D. : O(dim)
*/
template<typename TKey, typename TValue>Vector<TValue> MultiMap<TKey, TValue>::operator[](const TKey &key) const{
	return this->get(key);
}

/*
	@input  : true
	@output : returneaza capacitatea de stocare curenta a multidictionarului
	Complexitate : C.F. = C.M. = C.D. : O(1)
*/
template<typename TKey, typename TValue>const int MultiMap<TKey, TValue>::capacity() const {
	return this->__capacity;
}

/*
	@input  : true
	@output : returneaza numarul perechilor cheie, valoare din multidictionar
	Complexitate : C.F. = C.M. = C.D. : O(1)
*/
template<typename TKey, typename TValue>const int MultiMap<TKey, TValue>::size() const {
	return this->__size;
}

/*
	@input  : true
	@output : returneaza un iterator la prima pereche cheie, valoare a multidictionarului
	Complexitate : C.F. = C.M. = C.D. : O(1)
*/
template<typename TKey, typename TValue>IteratorMultiMap<TKey, TValue> MultiMap<TKey, TValue>::begin() const{
	return IteratorMultiMap<TKey, TValue>(*this);
}

/*
	@input  : true
	@output : returneaza un iterator la dupa ultima pereche cheie, valoare a multidictionarului
	Complexitate : C.F. = C.M. = C.D. : O(1)
*/
template<typename TKey, typename TValue>IteratorMultiMap<TKey, TValue> MultiMap<TKey, TValue>::end() const {
	return IteratorMultiMap<TKey, TValue>(*this, this->__capacity);
}

/* End of MultiMap implementation */

template<typename TKey, typename TValue>class IteratorMultiMap {
private:
	const MultiMap<TKey, TValue> &__multiMap;
	int __curent;

	/*
		Muta pointerul intern pe o pozitie valida din multidictionar
	*/
	void __ensureValidPosition() {
		while (this->__curent < this->__multiMap.capacity() && this->__multiMap.__entries[this->__curent].isValid() == false) {
			this->__curent++;
		}
	}
public:
	template<typename TKey, typename TValue>friend class MultiMap;
	IteratorMultiMap(const MultiMap<TKey, TValue>& map) : __multiMap{ map } {
		this->__curent = 0;
		this->__ensureValidPosition();
	}
	IteratorMultiMap(const MultiMap<TKey, TValue>& map, int poz) : __multiMap{ map }, __curent{ poz } {}
	bool valid() const {
		return this->__curent < this->__multiMap.capacity();
	}
	Entry<TKey, TValue>& element() const {
		return this->__multiMap.__entries[this->__curent];
	}
	void next() {
		this->__curent++;
		this->__ensureValidPosition();
	}
	Entry<TKey, TValue>& operator*() const {
		return this->element();
	}
	IteratorMultiMap& operator++() {
		this->next();
		return *this;
	}
	bool operator==(const IteratorMultiMap& other) const {
		return this->__curent == other.__curent;
	}
	bool operator!=(const IteratorMultiMap& other) const {
		return !(*this == other);
	}
};

#endif /* MULTIMAP_H */