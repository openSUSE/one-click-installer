#include <QApplication>
#include "YmpParser.h"

int main(int argc,char *argv[])
{
	QApplication app(argc,argv);
	YmpParser x(argv[1]);
	x.parse();
	x.printRepoList();
	x.printPackageList();
	return 0;
}
