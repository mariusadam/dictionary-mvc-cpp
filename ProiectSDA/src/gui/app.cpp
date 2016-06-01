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

	mainLayout->addWidget(this->__nrCuvinteLabel, 0, 0);
	mainLayout->addWidget(this->__cuvinteListWidget, 1, 0);
	mainLayout->addWidget(this->__nrExplicatiiLabel, 0, 1);
	mainLayout->addWidget(this->__explicatiiListWidget, 1, 1);
	mainLayout->addWidget(formWidget, 2, 0);
	mainLayout->addLayout(layoutUpdateDeleteButton, 2, 1);
}

void App::__connectSignalsSlots() {
	QObject::connect(__cuvinteListWidget, &QListWidget::itemClicked, this, &App::__on_cuvinteListWidgetItem_clicked);
	QObject::connect(__explicatiiListWidget, &QListWidget::itemClicked, this, &App::__on_explicatiiListWidgetItem_clicked);
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
	for (const auto& explicatie : explicatii) {
		QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(explicatie));
		this->__explicatiiListWidget->addItem(item);
	}
}

void App::__refreshExplicatiiListWidget() {
	std::string cuvant{ this->__getSelectedCuvant() };
	this->__reloadExplicatiiListWidget(this->__ctrl.getExplicatiiPentruCuvant(cuvant));
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