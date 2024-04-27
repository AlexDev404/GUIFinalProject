#pragma once
#include <string>
#include <odb/core.hxx>
using std::string;

#pragma db object
class Albums {
public:
	// Constructor

	/// <summary>
	/// Creates a new Album. Requires a name to be passed.
	/// </summary>
	/// <param name="album"></param>
	Albums(string title, string release_date) : title(title), release_date(release_date) {}

	// Getters

	/// <summary>
	/// Returns the album ID.
	/// </summary>
	/// <returns></returns>
	int Id();

	/// <summary>
	/// Returns the album name.
	/// </summary>
	/// <returns></returns>
	string Title();

	/// <summary>
	/// Returns the album releae date.
	/// </summary>
	/// <returns></returns>
	string ReleaseDate();

	// Setters

	/// <summary>
	/// Sets the title.
	/// </summary>
	/// <returns></returns>
	void SetTitle(string name);

	/// <summary>
	/// Sets the release date.
	/// </summary>
	/// <returns></returns>
	void SetReleaseDate(string rdate);

private:
	friend class odb::access;
	Albums() {}

#pragma db id auto
	int id_;

#pragma db type("TEXT")
	string title;
	string release_date;

};