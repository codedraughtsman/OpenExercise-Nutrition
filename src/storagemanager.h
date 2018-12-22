#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include "src_global.h"

#include <QMap>

class SRCSHARED_EXPORT StorageManager {

  public:
	static void init();

	static void addFood( QString foodName, uint kjPer100g, uint gramsPerCup );

	static void addPortion( QString foodName, uint grams );

	static uint getKjPer100g( QString id );

  private:
	StorageManager() {}
	static QMap<QString, uint> m_kjPer100gMap;
	static void createTables();
};

#endif // STORAGEMANAGER_H
