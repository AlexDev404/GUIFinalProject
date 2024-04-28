#pragma once
#include "Track.hpp"
#include <string>
#include <odb/core.hxx>
using std::string;

#pragma db object
class Playlist {
public:
	Playlist(string name, string year) : name_(name), year_(year) {
		this->duration_ = 0.00; // Initialize duration to 0.00 (playlist is empty)
	}

	// Getters

	/// <summary>
	/// The unique identifier of the playlist.
	/// </summary>
	/// <returns></returns>
	const int Id();
	/// <summary>
	/// The title of the playlist.
	/// </summary>
	/// <returns></returns>
	const string Name();
	/// <summary>
	/// The year the playlist was created.
	/// </summary>
	/// <returns></returns>
	const string Year();
	/// <summary>
	/// The duration of the playlist.
	/// </summary>
	/// <returns></returns>
	const double Duration();

	
	// Methods
	void* AddTrack(Track* track); // A fucking generic pointer since including Track_Playlist causes a circular dependency
	// Setters

	/// <summary>
	/// Sets the title of the playlist.
	/// </summary>
	/// <param name="title"></param>
	void SetName(string name);
	/// <summary>
	/// Sets the year the playlist was created.
	/// </summary>
	/// <param name="year"></param>
	void SetYear(string year);
	/// <summary>
	/// Sets the duration of the playlist.
	/// </summary>
	/// <param name="duration"></param>
	void SetDuration(double duration);
private:
	friend class odb::access;
	Playlist() {}

	#pragma db id auto
	int id_;
	string name_;
	string year_;
	double duration_;

};