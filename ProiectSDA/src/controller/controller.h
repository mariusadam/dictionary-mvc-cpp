#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../repository/repository.h"
#include <string>

class Controller {
private:
	Repository& __repo;

	void __validateEntry(const std::string &cuvant, const std::string &explicatie);
public:
	Controller(Repository& repo) : __repo{ repo } {}
	~Controller() {}
	void addExplicatiePentruCuvant(const std::string cuvant, const std::string explicatie);
	void addCuvantNou(const std::string cuvant, const std::string explicatie);
	void del(const std::string cuvant, const std::string explicatie);
	void delAll(const std::string cuvant);
	void update(const std::string cuvant, const std::string explicatieVeche, const std::string explicatieNoua);
	bool existaCuvant(const std::string &cuvant) {
		return this->__repo.containsKey(cuvant);
	}
	Vector<std::string> getMultimeCuvinte();
	Vector<std::string> getExplicatiiPentruCuvant(const std::string &cuvant);
	Vector<std::string> getCuvinteFiltrate(const std::string pattern);
};

#endif // !CONTROLLER_H
