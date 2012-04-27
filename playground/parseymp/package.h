#ifndef PACKAGE_H
#define PACKAGE

#include <QString>

class Package
{
	private:
		QString m_name;
		QString m_summary;
		QString m_description;
	public:
		Package(QString name,QString summary,QString description);
		Package();
		QString name();
		QString summary();
		QString description();
		void setName(QString name);
		void setSummary(QString summary);
		void setDescription(QString description);
};

#endif
