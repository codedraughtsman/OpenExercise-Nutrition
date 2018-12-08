#include "storagemanager.h"

#include <QDebug>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

void StorageManager::createTables() {
	QSqlQuery query;
	query.exec( "CREATE TABLE IF NOT EXISTS food "
				"(foodName TEXT PRIMARY KEY ,"
				"kjPer100g INTEGER,"
				"gramsPerCup INTEGER)" );
	query.exec( "CREATE TABLE IF NOT EXISTS portions "
				"(timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,"
				"foodName TEXT,"
				"grams INTEGER)" );
}

void addDebugFoods() {
	StorageManager::addFood( "apple", 300, 100 );
	StorageManager::addFood( "tomato", 150, 150 );
	StorageManager::addFood( "banna", 700, 300 );

	StorageManager::addPortion( "apple", 400 );
	StorageManager::addPortion( "tomato", 200 );
	StorageManager::addPortion( "banna", 1000 );

	StorageManager::addPortion( "apple", 150 );
}

void StorageManager::init() {
	// create DB path
	QString path = QDir::home().filePath( ".openExercise/nutrition.db" );
	qDebug() << "making ~/.openExercise/ "
			 << QDir::home().mkpath( ".openExercise/" );
	QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );
	db.setDatabaseName( path );
	qDebug() << "trying to open " << path << ", " << db.open();

	createTables();
	addDebugFoods();
}
void StorageManager::addFood( QString foodName, uint kjPer100g,
							  uint gramsPerCup ) {
	QSqlQuery query;
	query.prepare( "INSERT INTO food (foodName, kjPer100g, gramsPerCup) "
				   "Values ( ? , ? , ? ) " );
	query.addBindValue( foodName );
	query.addBindValue( kjPer100g );
	query.addBindValue( gramsPerCup );
	bool sucessful = query.exec();

	if ( !sucessful ) {
		qDebug() << "last query was: " << query.lastQuery();
		qDebug() << "trying to add food:" << foodName << " failed. "
				 << query.lastError().text();
	}
}
void StorageManager::addPortion( QString foodName, uint grams ) {
	QSqlQuery query;
	query.prepare( "INSERT INTO portions (foodName, grams) "
				   "Values ( ? , ?) " );
	query.addBindValue( foodName );
	query.addBindValue( grams );
	bool sucessful = query.exec();

	if ( !sucessful ) {
		qDebug() << "last query was: " << query.lastQuery();
		qDebug() << "trying to add food:" << foodName << " failed. "
				 << query.lastError().text();
	}
}
