#include "ShaderLab.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ShaderLab w;
	w.run();
	return a.exec();
}
