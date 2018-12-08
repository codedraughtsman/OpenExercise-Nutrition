#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include "src_global.h"

class SRCSHARED_EXPORT StorageManager {

  public:
	static void init();

	static void addFood( QString foodName, uint kjPer100g, uint gramsPerCup );

	static void addPortion( QString foodName, uint grams );

  private:
	StorageManager() {}
	static void createTables();
};

#endif // STORAGEMANAGER_H
