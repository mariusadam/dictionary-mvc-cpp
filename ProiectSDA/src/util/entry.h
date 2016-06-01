#ifndef ENTRY_H
#define ENTRY_H

#include <string>
#include "multimap.h"
#include <iostream>

template<typename TKey, typename TValue> class Entry {
private:
	TKey __key;
	TValue __value;
	bool __isValid;

	int hashCode() const {
		size_t key_hash = std::hash<TKey>()(this->__key);
		int hashInt = (int)key_hash;
		return hashInt < 0 ? (0 - hashInt) : hashInt;
	}

	bool isValid() {
		return this->__isValid;
	}

	void makeInvalid() {
		this->__isValid = false;
	}
public:
	template<typename TKey, typename TValue>friend class MultiMap;
	template<typename TKey, typename TValue>friend class IteratorMultiMap;
	Entry(const TKey &key) : __key{ key }, __value{}, __isValid{ false } {}

	/**
	*	The default constructor
	*/
	Entry() : __key{}, __value{}, __isValid{ false } {}

	/**
	*	The explicit constructor
	*/
	Entry(const TKey &key, const TValue &value) {
		this->__key = key;
		this->__value = value;
		this->__isValid = true;
	}

	TKey getKey() const {
		return this->__key;
	}

	TValue getValue() const {
		return this->__value;
	}

	Entry& operator=(const Entry& other) {
		this->__key = other.__key;
		this->__value = other.__value;
		this->__isValid = other.__isValid;
		return *this;
	}

	bool operator<(const Entry &other) const {
		return this->__key < other.__key;
	}

	bool operator<=(const Entry &other) const {
		return this->__key <= other.__key;
	}

	bool operator>(const Entry &other) const {
		return this->__key > other.__key;
	}

	bool operator>=(const Entry &other) const {
		return this->__key >= other.__key;
	}

	bool operator==(const Entry &other) const {
		return this->__key == other.__key && this->__value == other.__value && this->__isValid == other.__isValid;
	}

	bool operator!=(const Entry &other) const {
		return this->__key != other.__key;
	}

	friend std::ostream &operator<<(std::ostream& out, const Entry& pair) {
		return out << pair.__key << " = " << pair.__value;
	}
};

#endif /* ENTRY_H */