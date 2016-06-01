#include "controller.h"

void Controller::__validateEntry(const std::string & cuvant, const std::string & explicatie) {
	std::string errors{ "" };
	if (cuvant == "" || cuvant.length() > 30) {
		errors += "Cuvantul introdus este invalid!\n";
	}
	if (explicatie == "") {
		errors += "Explicatia introdusa este invalida!\n";
	}
	if (errors.empty() == false) {
		throw DictionarException(errors);
	}
}

void Controller::add(const std::string cuvant, const std::string explicatie) {
	this->__validateEntry(cuvant, explicatie);
	this->__repo.add(cuvant, explicatie);
}

void Controller::del(const std::string cuvant, const std::string explicatie) {
	this->__validateEntry(cuvant, explicatie);
	this->__repo.del(cuvant, explicatie);
}

void Controller::delAll(const std::string cuvant) {
	this->__repo.delAll(cuvant);
}

void Controller::update(const std::string cuvant, const std::string explicatieVeche, const std::string explicatieNoua) {
	this->__validateEntry(cuvant, explicatieNoua);
	this->__repo.update(cuvant, explicatieVeche, explicatieNoua);
}

Vector<std::string> Controller::getMultimeCuvinte() {
	return this->__repo.getMultimeCuvinte();
}

Vector<std::string> Controller::getExplicatiiPentruCuvant(const std::string & cuvant) {
	const MultiMap<std::string, std::string> &map = this->__repo.getDictionar();
	return map[cuvant];
}
