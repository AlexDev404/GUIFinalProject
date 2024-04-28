#pragma once
#include <odb/database.hxx>
#include <odb/sqlite/database.hxx>
#include <odb/schema-catalog.hxx>
#include <odb/sqlite/exceptions.hxx>
#include <odb/transaction.hxx>
#include <odb/result.hxx>
#include <string>
using std::string;

//** Usage **
// ==========
// In your code, first run setDatabase(string db_name)
// (preferably, in a startup function)
// 
// Ensure to save this object somewhere that is easily accessible.
// 
// ** Example **
// ==============
// 
// db = *new database();
// db.setDatabase("some_name"); // Will be stored in the root folder as `some_name.sqlite`
// 
// Then, when the database needs to be accessed, use:
// 
// odb::sqlite::database db_context = db.getDatabase(); // Required: If not done MSVC will raise an assertation error.
// db_context.<whatever_function>
// 
// 
// Database setup, functions, and the such?
class database {
public:
	// Sets the database name for later
	void setDatabase(string db_name);
	// Gets the database using its name
	odb::sqlite::database getDatabase();
private:
	string db_name = "default";
};