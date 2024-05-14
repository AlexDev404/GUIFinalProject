#include "database.hpp"
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

void database::setDatabase(string db_name) {
	this->db_name = db_name;
}

odb::sqlite::database database::getDatabase() {
	QString roaming_app_data = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0];
	QDir dir(roaming_app_data);
	if (!dir.exists())
		dir.mkpath(".");
	odb::sqlite::database db_context(roaming_app_data.toStdString() + "\\" + this->db_name + ".sqlite", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
	return db_context;
}