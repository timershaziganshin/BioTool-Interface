#include "interface.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Interface* w = new Interface();//::instance();	
	w->show();
	return a.exec();
}
