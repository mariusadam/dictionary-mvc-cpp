#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <string>
#include <exception>
#include "../util/multimap.h"
#include "../util/vector.h"

class DictionarException : public std::exception {
public:
	DictionarException() : std::exception() {}
	DictionarException(const char* msg) : std::exception{ msg }{}
	DictionarException(std::string &msg) : std::exception{ msg.c_str() }{}
};

class Repository {
private:
	std::string __fileName;
	MultiMap<std::string, std::string> __dictionary;

	Vector<std::string> __split(const std::string &str, const char delimiter) const;
	void __loadFromFile();
	void __saveToFile();
public:
	Repository(const std::string &fileName) : __fileName{ fileName } {
		this->__loadFromFile();
	}
	~Repository() {
		this->__saveToFile();
	}
	void add(const std::string &cuvant, const std::string &explicatie);
	void del(const std::string &cuvant, const std::string &explicatie);
	void delAll(const std::string &cuvant);
	void update(const std::string &cuvant, const std::string &explicatieVeche, const std::string &explicatieNoua);
	Vector<std::string> getMultimeCuvinte() const;
	const MultiMap<std::string, std::string>& getDictionar() const;
};

#endif // !REPOSITORY_H
