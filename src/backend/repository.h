#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QString>

namespace OCI{

class Repository
{
	public:
		//Repository(QString recommended,QString name,QString summary,QString description,QString url);
		Repository();
		QString name() const;
		QString description() const;
		QString summary() const;
		QString url() const;
		QString recommended() const;
		void setRecommended(const QString& recommmended);
		void setName(const QString& name);
		void setDescription(const QString& description);
		void setSummary(const QString& summary);
		void setUrl(const QString& url);
	private:
		QString m_recommended;
		QString m_name;
		QString m_description;
		QString m_summary;
		QString m_url;
};
};
#endif
