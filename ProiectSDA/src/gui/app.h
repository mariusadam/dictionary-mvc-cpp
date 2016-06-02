#ifndef APP_H
#define APP_H

#include <QtWidgets/QMainWindow>
#include "ui_app.h"
#include "../controller/controller.h"
#include <qlistwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qpushbutton.h>

class App : public QMainWindow
{
	Q_OBJECT

private:
	Controller& __ctrl;
	QListWidget* __cuvinteListWidget;
	QLabel* __nrCuvinteLabel;
	QListWidget* __explicatiiListWidget;
	QLabel* __nrExplicatiiLabel;
	QLineEdit* __cuvantLineEdit;
	QLineEdit* __explicatieLineEdit;
	QLineEdit* __searchLineEdit;
	QPushButton* __searchButton;
	QPushButton* __addCuvantButton;
	QPushButton* __updateExplicatieButton;
	QPushButton* __addExplicatieButton;
	QPushButton* __deleteExplicatieButton;
	QPushButton* __deleteCuvantButton;
	QPushButton* __refreshButton;
	QPushButton* __quitButton;

	void __initComponents();
	void __connectSignalsSlots();
	void __reloadCuvinteListWidget(const Vector<std::string> &cuvinte);
	void __refreshCuvinteListWidget();
	void __reloadExplicatiiListWidget(const Vector<std::string> &explicatii);
	void __refreshExplicatiiListWidget();
	void __displayError(const std::string& error);
	std::string __getSelectedCuvant();
	std::string __getSelectedExplicatie();

	private slots:
	void __on_cuvinteListWidgetItem_clicked();
	void __on_explicatiiListWidgetItem_clicked();
	void __on_addCuvantButton_clicked();
	void __on_updateExplicatieButton_clicked();
	void __on_addExplicatieButton_clicked();
	void __on_deleteExplicatieButton_clicked();
	void __on_deleteCuvantButton_clicked();
	void __on_refreshButton_clicked();
	void __on_searchButton_clicked();
	void __on_quitButton_Clicked();
	void __on_cuvinteListWidgetItem_selectionChanged();

public:
	App(Controller& ctrl) : __ctrl{ ctrl } {
		this->__initComponents();
		this->__connectSignalsSlots();
		this->__refreshCuvinteListWidget();
	}
	~App() {

	}
};

#endif // APP_H
