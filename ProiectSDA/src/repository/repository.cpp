#include "repository.h"
#include <sstream>
#include <fstream>

Vector<std::string> Repository::__split(const std::string & str, const char delimiter) const {
	Vector<std::string> rez;
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delimiter)) {
		rez.push_back(token);
	}
	return rez;
}

void Repository::__loadFromFile() {
	std::ifstream fin(this->__fileName);
	if (fin.is_open() == false) {
		throw DictionarException("Could not open file: " + __fileName);
	}
	std::string line;
	Vector<std::string> attrs;
	while (std::getline(fin, line)) {
		attrs = this->__split(line, '=');
		this->add(attrs[0], attrs[1]);
	}
	fin.close();
}

void Repository::__saveToFile() {
	std::ofstream fout(this->__fileName, std::ios::trunc);
	if (fout.is_open() == false) {
		throw DictionarException("Could not open file: " + this->__fileName);
	}
	for (const auto& entry : this->__dictionary) {
		fout << entry.getKey() << "=" << entry.getValue() << "\n";
	}
	fout.close();
}

void Repository::add(const std::string & cuvant, const std::string & explicatie) {
	if (this->__dictionary.containsEntry(cuvant, explicatie)) {
		throw DictionarException("The pair : " + cuvant + " => " + explicatie + " is already added!");
	}
	this->__dictionary.add(cuvant, explicatie);
}

void Repository::del(const std::string & cuvant, const std::string & explicatie) {
	try {
		this->__dictionary.remove(cuvant, explicatie);
	}
	catch (MultiMapException &ex) {
		throw DictionarException("The pair : " + cuvant + " => " + explicatie + " does not exists!");
	}
}

void Repository::delAll(const std::string & cuvant) {
	try {
		this->__dictionary.removeAll(cuvant);
	}
	catch (MultiMapException &ex) {
		throw DictionarException("The key : " + cuvant + " does not exists!");
	}
}

void Repository::update(const std::string & cuvant, const std::string & explicatieVeche, const std::string & explicatieNoua) {
	if (this->__dictionary.containsEntry(cuvant, explicatieNoua)) {
		throw DictionarException("The pair : " + cuvant + " => " + explicatieNoua + " is already added!");
	}
	this->del(cuvant, explicatieNoua);
	this->add(cuvant, explicatieNoua);
}

Vector<std::string> Repository::getMultimeCuvinte() const {
	return this->__dictionary.keySet();
}

const MultiMap<std::string, std::string>& Repository::getDictionar() const {
	return this->__dictionary;
}
