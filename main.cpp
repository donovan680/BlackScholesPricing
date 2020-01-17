#include "stdafx.h"
#include "OptionsPricing.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OptionsPricing w;
	w.show();
	return a.exec();
}
