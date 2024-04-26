#pragma once
#include <string>
#include <odb/core.hxx>
using std::string;

#pragma db object
class Genres {
public:
	// Constructor

	/// <summary>
	/// Creates a new genre. Requires a title to be passed.
	/// </summary>
	/// <param name="role"></param>
	Genres(string title) : title(title) {}

	// Getters

	/// <summary>
	/// Returns the genre ID.
	/// </summary>
	/// <returns></returns>
	int Id();

	/// <summary>
	/// Returns the genre title.
	/// </summary>
	/// <returns></returns>
	string Title();

	// Setters

	/// <summary>
	/// Sets the genre title.
	/// </summary>
	/// <param name="role"></param>
	void SetTitle(string name);

private:
	// Variables
	friend class odb::access;
	Genres() {}

#pragma db id auto
	int id;

#pragma db type("TEXT")
	string title;
};
