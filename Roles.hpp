#pragma once
#include <string>
#include <odb/core.hxx>
using std::string;

#pragma db object
class Roles {
public:
	// Constructor

	/// <summary>
	/// Creates a new role. Requires a name to be passed.
	/// </summary>
	/// <param name="role"></param>
	Roles(string name) : name(name) {}

	// Getters

	/// <summary>
	/// Returns the role ID.
	/// </summary>
	/// <returns></returns>
	int Id();

	/// <summary>
	/// Returns the role name.
	/// </summary>
	/// <returns></returns>
	string Name();

	// Setters

	/// <summary>
	/// Sets the role name.
	/// </summary>
	/// <param name="role"></param>
	void SetName(string role);

private:
	// Variables
	friend class odb::access;
	Roles() {}

#pragma db id auto
	int id;

#pragma db type("TEXT")
	string name;
};
