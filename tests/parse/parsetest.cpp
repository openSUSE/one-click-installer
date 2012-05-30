#include <QtTest/QTest>
#include "parsetest.h"

void ParseTest::parse()
{
	OCI::YmpParser parser("/home/saurabh/workspace/oneclick/tests/parse/geany.ymp");
	parser.parse();
	QList<OCI::Package*> packageList = parser.packages();
	QList<OCI::Repository*> repositoryList = parser.repositories();

	QCOMPARE(packageList.at(0)->name(),QString("geany"));
	QCOMPARE(repositoryList.at(0)->name(),QString("openSUSE:12.1"));
}

QTEST_MAIN(ParseTest)
