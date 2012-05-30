#ifndef PARSETEST_H
#define PARSETEST_H

#include "YmpParser.h"
#include "repository.h"
#include "package.h"

class ParseTest : public QObject
{
	Q_OBJECT
	
	private slots:
		void parse();

};

#endif
