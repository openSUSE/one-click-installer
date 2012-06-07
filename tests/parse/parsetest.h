#ifndef PARSETEST_H
#define PARSETEST_H

#include "../../src/backend/YmpParser.h"
#include "../../src/backend/repository.h"
#include "../../src/backend/package.h"

class ParseTest : public QObject
{
	Q_OBJECT
	
	private slots:
		void parse();

};

#endif
