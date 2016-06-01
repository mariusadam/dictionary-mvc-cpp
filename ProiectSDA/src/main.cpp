#include "gui/app.h"
#include <QtWidgets/QApplication>
#include "repository/repository.h"
#include "controller/controller.h"

int main(int argc, char *argv[]) {
	Repository repo{ "Files//dictionar.txt" };
	Controller ctrl{ repo };
	QApplication a(argc, argv);
	App w{ ctrl };
	w.show();
	return a.exec();
}
