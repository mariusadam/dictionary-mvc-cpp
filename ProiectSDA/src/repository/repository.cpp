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

bool Repository::containsEntry(const std::string & cuvant, const std::string & explicatie) const{
	return this->__dictionary.containsEntry(cuvant, explicatie);
}

void Repository::add(const std::string & cuvant, const std::string & explicatie) {
	if (this->containsEntry(cuvant, explicatie)) {
		//return;
		throw DictionarException("Perechea : " + cuvant + " => " + explicatie + " exista deja!");
	}
	try {
		this->__dictionary.add(cuvant, explicatie);
	}
	catch (MultiMapException &ex) {
		throw DictionarException("Cuvantul " + cuvant + " nu a putut fi gasit!");
	}
}

void Repository::del(const std::string & cuvant, const std::string & explicatie) {
	try {
		this->__dictionary.remove(cuvant, explicatie);
	}
	catch (MultiMapException &ex) {
		throw DictionarException("Perechea : " + cuvant + " => " + explicatie + " nu exista in dictionar!");
	}
}

void Repository::delAll(const std::string & cuvant) {
	try {
		this->__dictionary.removeAll(cuvant);
	}
	catch (MultiMapException &ex) {
		throw DictionarException("Cuvantul : " + cuvant + " nu exista in dictionar!");
	}
}

void Repository::update(const std::string & cuvant, const std::string & explicatieVeche, const std::string & explicatieNoua) {
	std::string errors{ "" };
	if (this->containsEntry(cuvant, explicatieVeche) == false) {
		errors += "Perechea : " + cuvant + " => " + explicatieVeche + " nu exista in dictionar!\n";
	}
	if (this->containsEntry(cuvant, explicatieNoua)) {
		errors += "Perechea : " + cuvant + " => " + explicatieNoua + " exista deja!\n";
	}
	if (errors.empty() == false) {
		throw DictionarException(errors);
	}
	this->del(cuvant, explicatieVeche);
	this->add(cuvant, explicatieNoua);
}

Vector<std::string> Repository::getMultimeCuvinte() const {
	return this->__dictionary.keySet();
}

Vector<std::string> Repository::getExplicatiiPentruCuvant(const std::string & cuvant) {
	try {
		return this->__dictionary[cuvant];
	}
	catch (MultiMapException& ex) {
		throw DictionarException("Cuvantul " + cuvant + " nu a putut fi gasit!");
	}
}

const MultiMap<std::string, std::string>& Repository::getDictionar() const {
	return this->__dictionary;
}

bool Repository::containsKey(const std::string & cuvant) const {
	return this->__dictionary.containsKey(cuvant);
}
