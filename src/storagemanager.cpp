#include "storagemanager.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

QMap<QString, uint> StorageManager::m_kjPer100gMap;

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

void StorageManager::init() {
	// create DB path
	QString path = QDir::home().filePath( ".openExercise/nutrition.db" );
	qDebug() << "making ~/.openExercise/ "
			 << QDir::home().mkpath( ".openExercise/" );
	QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );
	db.setDatabaseName( path );
	qDebug() << "trying to open " << path << ", " << db.open();

	createTables();
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
		return;
	}
}
void StorageManager::addPortion( QString foodName, uint grams ) {
	QSqlQuery query;
	query.prepare( "INSERT INTO portions (timestamp,foodName, grams) "
				   "Values (?, ? , ?) " );
	query.addBindValue(
		QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss" ) );
	query.addBindValue( foodName );
	query.addBindValue( grams );
	bool sucessful = query.exec();

	if ( !sucessful ) {
		qDebug() << "last query was: " << query.lastQuery();
		qDebug() << "trying to add food:" << foodName << " failed. "
				 << query.lastError().text();
		return;
	}
	emit dataChanged();
	qDebug() << "emiting datachanged " << this;
}

uint StorageManager::getKjPer100g( QString id ) {
	if ( !m_kjPer100gMap.contains( id ) ) {
		QSqlQuery query;

		query.prepare(
			"select foodName,kjPer100g from  food where foodName =?" );
		query.addBindValue( id );
		query.exec();
		query.first(); // select the first valid record.

		m_kjPer100gMap[ id ] = query.value( 1 ).toUInt();
	}

	return m_kjPer100gMap[ id ];
}

StorageManager &StorageManager::instance() {
	static StorageManager *inst = new StorageManager();
	return *inst;
}
