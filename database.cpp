#include "database.hpp"

void database::setDatabase(string db_name) {
	this->db_name = db_name;
}

odb::sqlite::database database::getDatabase() {
	odb::sqlite::database db_context(this->db_name + ".sqlite", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
	return db_context;
}