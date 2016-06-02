#include "app.h"
#include <qlayout.h>
#include <qformlayout.h>
#include <qmessagebox.h>

void App::__initComponents() {
	QWidget* mainWidget = new QWidget(this);
	QGridLayout* mainLayout = new QGridLayout(mainWidget);
	
	mainWidget->setLayout(mainLayout);

	this->setCentralWidget(mainWidget);
	
	this->__cuvinteListWidget = new QListWidget;
	this->__nrCuvinteLabel = new QLabel;

	this->__explicatiiListWidget = new QListWidget;
	this->__nrExplicatiiLabel = new QLabel;

	QWidget* formWidget = new QWidget;
	QFormLayout* formLayout = new QFormLayout(formWidget);
	this->__cuvantLineEdit = new QLineEdit;
	formLayout->addRow(QString("Cuvant"), this->__cuvantLineEdit);
	this->__explicatieLineEdit = new QLineEdit;
	formLayout->addRow(QString("Explicatie"), this->__explicatieLineEdit);
	
	QHBoxLayout* layoutAddButtons = new QHBoxLayout;
	this->__addCuvantButton = new QPushButton("Adauga cuvant nou");
	this->__addExplicatieButton = new QPushButton("Adauga explicatie");
	layoutAddButtons->addWidget(this->__addCuvantButton);
	layoutAddButtons->addWidget(this->__addExplicatieButton);

	formLayout->addRow(layoutAddButtons);

	QVBoxLayout* layoutUpdateDeleteButton = new QVBoxLayout;
	this->__updateExplicatieButton = new QPushButton("Actualizeaza explicatie");
	this->__deleteCuvantButton = new QPushButton("Sterge cuvantul din dictionar");
	this->__deleteExplicatieButton = new QPushButton("Sterge explicatie");
	layoutUpdateDeleteButton->addWidget(this->__updateExplicatieButton);
	layoutUpdateDeleteButton->addWidget(this->__deleteCuvantButton);
	layoutUpdateDeleteButton->addWidget(this->__deleteExplicatieButton);

	QWidget* searchFormWidget = new QWidget;
	QFormLayout* searchLayout = new QFormLayout(searchFormWidget);;
	this->__searchButton = new QPushButton("Cauta cuvinte");
	this->__searchLineEdit = new QLineEdit;
	searchLayout->addRow(QString("Cauta dupa"), this->__searchLineEdit);
	searchLayout->addWidget(this->__searchButton);

	this->__refreshButton = new QPushButton("Reimprospatare lista");
	this->__quitButton = new QPushButton("Iesire");

	mainLayout->addWidget(this->__nrCuvinteLabel, 0, 0);
	mainLayout->addWidget(this->__cuvinteListWidget, 1, 0);
	mainLayout->addWidget(this->__nrExplicatiiLabel, 0, 1);
	mainLayout->addWidget(this->__explicatiiListWidget, 1, 1);
	mainLayout->addWidget(formWidget, 2, 0);
	mainLayout->addLayout(layoutUpdateDeleteButton, 2, 1);
	mainLayout->addWidget(searchFormWidget, 3, 0);
	mainLayout->addWidget(this->__refreshButton, 3, 1);
	mainLayout->addWidget(this->__quitButton, 4, 1);
}

void App::__connectSignalsSlots() {
	QObject::connect(this->__cuvinteListWidget, &QListWidget::itemClicked, this, &App::__on_cuvinteListWidgetItem_clicked);
	QObject::connect(this->__cuvinteListWidget, &QListWidget::itemSelectionChanged, this, &App::__on_cuvinteListWidgetItem_selectionChanged);
	QObject::connect(this->__explicatiiListWidget, &QListWidget::itemClicked, this, &App::__on_explicatiiListWidgetItem_clicked);
	QObject::connect(this->__addCuvantButton, &QPushButton::clicked, this, &App::__on_addCuvantButton_clicked);
	QObject::connect(this->__updateExplicatieButton, &QPushButton::clicked, this, &App::__on_updateExplicatieButton_clicked);
	QObject::connect(this->__addExplicatieButton, &QPushButton::clicked, this, &App::__on_addExplicatieButton_clicked);
	QObject::connect(this->__deleteCuvantButton, &QPushButton::clicked, this, &App::__on_deleteCuvantButton_clicked);
	QObject::connect(this->__deleteExplicatieButton, &QPushButton::clicked, this, &App::__on_deleteExplicatieButton_clicked);
	QObject::connect(this->__refreshButton, &QPushButton::clicked, this, &App::__on_refreshButton_clicked);
	QObject::connect(this->__searchButton, &QPushButton::clicked, this, &App::__on_searchButton_clicked);
	QObject::connect(this->__quitButton, &QPushButton::clicked, this, &App::__on_quitButton_Clicked);
}

void App::__reloadCuvinteListWidget(const Vector<std::string>& cuvinte) {
	std::string text{ "Total cuvinte: " + std::to_string(cuvinte.size()) };
	this->__nrCuvinteLabel->setText(text.c_str());
	this->__explicatieLineEdit->clear();
	this->__cuvinteListWidget->clear();
	for (const auto& cuvant : cuvinte) {
		QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(cuvant));
		this->__cuvinteListWidget->addItem(item);
	}
}

void App::__refreshCuvinteListWidget() {
	this->__reloadCuvinteListWidget(this->__ctrl.getMultimeCuvinte());
}

void App::__reloadExplicatiiListWidget(const Vector<std::string> &explicatii) {
	std::string text{ "Total explicatii: " + std::to_string(explicatii.size()) };
	this->__nrExplicatiiLabel->setText(text.c_str());
	this->__explicatiiListWidget->clear();
	if (explicatii.size() == 0) {
		return;
	}
	for (const auto& explicatie : explicatii) {
		QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(explicatie));
		this->__explicatiiListWidget->addItem(item);
	}
}

void App::__refreshExplicatiiListWidget() {
	std::string cuvant{ this->__cuvantLineEdit->text().toStdString() };
	try {
		auto explicatii = this->__ctrl.getExplicatiiPentruCuvant(cuvant);
		this->__reloadExplicatiiListWidget(explicatii);
	}
	catch (DictionarException &ex) {
		this->__reloadExplicatiiListWidget(Vector<std::string>{});
	}
}

void App::__displayError(const std::string& error) {
	QMessageBox::warning(this, "Warning", QString::fromStdString(error));
}

void App::__on_cuvinteListWidgetItem_clicked() {
	std::string cuvant{ this->__getSelectedCuvant() };
	this->__cuvantLineEdit->setText(QString::fromStdString(cuvant));
	this->__explicatieLineEdit->clear();
	auto explicatii = this->__ctrl.getExplicatiiPentruCuvant(cuvant);
	this->__reloadExplicatiiListWidget(explicatii);
}

void App::__on_explicatiiListWidgetItem_clicked() {
	std::string explicatie{ this->__getSelectedExplicatie() };
	this->__explicatieLineEdit->setText(QString::fromStdString(explicatie));
}

void App::__on_addCuvantButton_clicked() {
	std::string cuvant = this->__cuvantLineEdit->text().toStdString();
	std::string explicatie = this->__explicatieLineEdit->text().toStdString();
	try {
		this->__ctrl.addCuvantNou(cuvant, explicatie);
		this->__refreshCuvinteListWidget();
		for (int i = 0; i < this->__cuvinteListWidget->count(); i++) {
			if (this->__cuvinteListWidget->item(i)->text().toStdString() == cuvant) {
				this->__cuvinteListWidget->item(i)->setSelected(true);
				break;
			}
		}
	}
	catch (DictionarException &ex) {
		this->__displayError(ex.what());
	}
}

void App::__on_updateExplicatieButton_clicked() {
	std::string cuvant = this->__cuvantLineEdit->text().toStdString();;
	std::string expresieVeche = this->__getSelectedExplicatie();
	std::string expresieNoua = this->__explicatieLineEdit->text().toStdString();
	if (cuvant.empty()) {
		this->__displayError("Nu ati introdus sau selectat cuvantul pentru care vreti sa modificati explicatia!");
		return;
	}
	if (expresieVeche.empty()) {
		this->__displayError("Nu ati selectat explicatia pe care doriti sa o editati!");
		return;
	}
	try {
		this->__ctrl.update(cuvant, expresieVeche, expresieNoua);
		this->__refreshExplicatiiListWidget();
	}
	catch (DictionarException &ex) {
		this->__displayError(ex.what());
	}
}

void App::__on_addExplicatieButton_clicked() {
	std::string cuvant = this->__cuvantLineEdit->text().toStdString();
	if (cuvant.empty()) {
		this->__displayError("Nu ati introdus cuvantul pentru care vreti sa adugati explicatia!");
		return;
	}
	std::string explicatie = this->__explicatieLineEdit->text().toStdString();
	try {
		this->__ctrl.addExplicatiePentruCuvant(cuvant, explicatie);
		this->__refreshExplicatiiListWidget();
	}
	catch (DictionarException &ex) {
		this->__displayError(ex.what());
	}
}

void App::__on_deleteExplicatieButton_clicked() {
	std::string cuvant = this->__cuvantLineEdit->text().toStdString();
	if (cuvant.empty()) {
		this->__displayError("Nu ati introdus cuvantul pentru care vreti sa stergeti explicatia!");
		return;
	}
	std::string explicatie = this->__explicatieLineEdit->text().toStdString();
	if (explicatie.empty()) {
		this->__displayError("Nu ati selectat sau introdus explicatia pe care vreit sa o stergeti!");
		return;
	}
	try {
		this->__ctrl.del(cuvant, explicatie);
		if (this->__ctrl.existaCuvant(cuvant) == false) {
			this->__refreshCuvinteListWidget();
		}
		this->__refreshExplicatiiListWidget();
		this->__explicatieLineEdit->clear();
	}
	catch (DictionarException &ex) {
		this->__displayError(ex.what());
	}
}

void App::__on_deleteCuvantButton_clicked() {
	std::string cuvant = this->__cuvantLineEdit->text().toStdString();
	if (cuvant.empty()) {
		this->__displayError("Nu ati selectat sau introdus cuvantul pe care vreti sa il stergeti!");
		return;
	}
	try {
		this->__ctrl.delAll(cuvant);
		this->__refreshCuvinteListWidget();
		this->__cuvantLineEdit->clear();
		this->__on_searchButton_clicked();
	}
	catch (DictionarException &ex) {
		this->__displayError(ex.what());
	}

}

void App::__on_refreshButton_clicked() {
	this->__cuvantLineEdit->clear();
	this->__explicatieLineEdit->clear();
	this->__searchLineEdit->clear();
	this->__reloadCuvinteListWidget(this->__ctrl.getMultimeCuvinte());
	this->__reloadExplicatiiListWidget(Vector<std::string>());
}

void App::__on_searchButton_clicked() {
	std::string pattern{ this->__searchLineEdit->text().toStdString() };
	try {
		auto result = this->__ctrl.getCuvinteFiltrate(pattern);
		if (result.size() == 0) {
			this->__displayError("Nu am putut gasi o potrivire");
			return;
		}
		this->__reloadCuvinteListWidget(result);
		this->__reloadExplicatiiListWidget(Vector<std::string>());
	}
	catch (DictionarException &ex) {
		this->__displayError(ex.what());
	}
}

void App::__on_quitButton_Clicked() {
	this->close();
}

void App::__on_cuvinteListWidgetItem_selectionChanged() {
	std::string cuvant{ this->__getSelectedCuvant() };
	this->__cuvantLineEdit->setText(QString::fromStdString(cuvant));
	this->__explicatieLineEdit->clear();
	try {
		auto explicatii = this->__ctrl.getExplicatiiPentruCuvant(cuvant);
		this->__reloadExplicatiiListWidget(explicatii);
	}
	catch (DictionarException &ex) {
		return;
	}
}

std::string App::__getSelectedCuvant() {
	QList<QListWidgetItem*> selectedItems = this->__cuvinteListWidget->selectedItems();
	if (selectedItems.empty()) {
		return "";
	}
	QListWidgetItem* selectedItem = selectedItems.at(0);
	return selectedItem->text().toStdString();
}

std::string App::__getSelectedExplicatie() {
	QList<QListWidgetItem*> selectedItems = this->__explicatiiListWidget->selectedItems();
	if (selectedItems.empty()) {
		return "";
	}
	QListWidgetItem* selectedItem = selectedItems.at(0);
	return selectedItem->text().toStdString();
}