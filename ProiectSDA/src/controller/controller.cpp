#include "controller.h"

void Controller::__validateEntry(const std::string & cuvant, const std::string & explicatie) {
	std::string errors{ "" };
	if (cuvant == "" || cuvant.length() > 255) {
		errors += "Cuvantul " + cuvant + " este invalid!\n";
	}
	if (explicatie == "") {
		errors += "Explicatia " + explicatie + " este invalida!\n";
	}
	if (errors.empty() == false) {
		throw DictionarException(errors);
	}
}

void Controller::addCuvantNou(const std::string cuvant, const std::string explicatie) {
	this->__validateEntry(cuvant, explicatie);
	if (this->__repo.containsKey(cuvant)) {
		throw DictionarException("Cuvantul " + cuvant + " exista deja!");
	}
	this->__repo.add(cuvant, explicatie);
}

void Controller::addExplicatiePentruCuvant(const std::string cuvant, const std::string explicatie) {
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
	return this->__repo.getExplicatiiPentruCuvant(cuvant);
}

Vector<std::string> Controller::getCuvinteFiltrate(const std::string pattern) {
	auto multimeCuvinte = this->getMultimeCuvinte();
	Vector<std::string> rez;
	for (const auto& cuvant : multimeCuvinte) {
		if (cuvant.find(pattern) != std::string::npos) {
			rez.push_back(cuvant);
		}
	}
	return rez;
}
