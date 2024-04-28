// Import database schemas 
#include "Track_Playlist.hpp"
#include "Playlist.hpp"
#include "Track.hpp"

// Import database mappings
#include "Track-odb.hxx"
#include "Playlist-odb.hxx"
#include "Track_Playlist-odb.hxx"

const int Playlist::Id() {
	return id_;
}

const string Playlist::Name() {
	return name_;
}

const string Playlist::Year() {
	return year_;
}

const double Playlist::Duration() {
	return duration_;
}

void Playlist::AddTrack(Track* track) {
	// Add track to playlist and increment duration
	this->duration_ = this->duration_ + track->Duration();
	// Create a new track_playlist object to map the track to the playlist
	Track_Playlist tp(track, this);
}

void Playlist::SetName(string name) {
	name_ = name;
}

void Playlist::SetYear(string year) {
	year_ = year;
}

void Playlist::SetDuration(double duration) {
	duration_ = duration;
}
