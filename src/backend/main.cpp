#include <QApplication>
#include "xmlparse.h"

int main(int argc,char *argv[])
{
	QApplication app(argc,argv);
	XMLParse x(argv[1]);
	x.parse();
	x.printRepoList();
	x.printPackageList();
	return 0;
}
