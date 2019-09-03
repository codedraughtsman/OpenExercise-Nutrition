#include "storagemanager.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QRandomGenerator>
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
QColor StorageManager::getColor( QString id ) {
	if ( !m_portionColors.contains( id ) ) {
		m_portionColors[ id ] =
			QColor::fromRgb( QRandomGenerator::global()->generate() );
	}

	return m_portionColors[ id ];
}
void StorageManager::addFood( QString foodName, uint kjPer100g,
							  uint gramsPerCup ) {
	QSqlQuery query;
	query.prepare( "INSERT INTO foodData (ShortFoodName, Energy) "
				   "Values ( ? , ?  ) " );
	query.addBindValue( foodName );
	query.addBindValue( kjPer100g );
	bool sucessful = query.exec();

	if ( !sucessful ) {
		qDebug() << "last query was: " << query.lastQuery();
		qDebug() << "trying to add food:" << foodName << " failed. "
				 << query.lastError().text();
		return;
	}
}

void StorageManager::addWeight( double weight, QDateTime dateTime ) {
	QSqlQuery query;
	query.prepare( "INSERT INTO weight (timestamp, date, time, weight) "
				   "Values (?, ?, ?, ?) " );

	query.addBindValue( dateTime.toString( "yyyy-MM-dd hh:mm:ss" ) );
	query.addBindValue( dateTime.toString( "yyyy-MM-dd" ) );
	query.addBindValue( dateTime.toString( "hh:mm:ss" ) );

	query.addBindValue( weight );

	bool sucessful = query.exec();

	if ( !sucessful ) {
		qDebug() << "last query was: " << query.lastQuery();
		qDebug() << "trying to add weight:" << weight << " on "
				 << "date failed. "
				 << dateTime.toString( "yyyy-MM-dd hh:mm:ss" )
				 << query.lastError().text();
		return;
	}
	emit dataChanged();
}

void StorageManager::addPortion( QString foodName, uint grams,
								 QDateTime dateTime ) {
	QSqlQuery query;
	query.prepare(
		"INSERT INTO portions (timestamp, date, time, foodName, grams) "
		"Values (?, ?, ?, ? , ?) " );

	query.addBindValue( dateTime.toString( "yyyy-MM-dd hh:mm:ss" ) );
	query.addBindValue( dateTime.toString( "yyyy-MM-dd" ) );
	query.addBindValue( dateTime.toString( "hh:mm:ss" ) );

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
