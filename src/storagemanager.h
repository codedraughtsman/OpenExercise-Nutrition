#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include "src_global.h"

#include <QColor>
#include <QDateTime>
#include <QMap>
#include <QObject>

class SRCSHARED_EXPORT StorageManager : public QObject {
	Q_OBJECT
	QMap<QString, QColor> m_portionColors;

  public:
	static void init();

	void addFood( QString foodName, uint kjPer100g, uint gramsPerCup );

	void addPortion( QString foodName, uint grams,
					 QDateTime dateTime = QDateTime::currentDateTime() );

	static uint getKjPer100g( QString id );
	static StorageManager &instance();

	void addWeight( double weight, QDateTime dateTime );

	QColor getColor( QString id );

  private:
	StorageManager() {}
	static QMap<QString, uint> m_kjPer100gMap;
	static void createTables();

  signals:
	void dataChanged();
};

#endif // STORAGEMANAGER_H
