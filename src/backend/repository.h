#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QString>

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
		void setRecommended(QString recommmended);
		void setName(QString name);
		void setDescription(QString description);
		void setSummary(QString summary);
		void setUrl(QString url);
	private:
		QString m_recommended;
		QString m_name;
		QString m_description;
		QString m_summary;
		QString m_url;
};

#endif
