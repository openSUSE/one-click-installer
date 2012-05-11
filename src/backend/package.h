#ifndef PACKAGE_H
#define PACKAGE_H

#include <QString>

class Package
{
	public:
		Package(QString name,QString summary,QString description);
		Package();
		QString name() const;
		QString summary() const;
		QString description() const;
		void setName(const QString& name);
		void setSummary(const QString& summary);
		void setDescription(const QString& description);
	private:
		QString m_name;
		QString m_summary;
		QString m_description;
};

#endif
