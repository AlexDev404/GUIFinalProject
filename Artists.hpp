#pragma once
#include <string>
#include <odb/core.hxx>
using std::string;

#pragma db object
class Artists {
public:
	Artists(string name) : name_(name) {}

	// Getters

	/// <summary>
	/// The unique identifier of the artist.
	/// </summary>
	/// <returns></returns>
	const int Id();
	/// <summary>
	/// The name of the artist.
	/// </summary>
	/// <returns></returns>
	const string Name();
	
	// Setters

	/// <summary>
	/// Sets the name of the artist.
	/// </summary>
	/// <param name="name"></param>
	void SetName(string name);
	
private:
	friend class odb::access;
	Artists() {}

	#pragma db id auto
	int id_;
	string name_;
};