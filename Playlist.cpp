// Import database schemas 
#include "Playlist.hpp"
#include "Track_Playlist.hpp"


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

// Hate these
void* Playlist::AddTrack(Track* track) {
	// Add track to playlist and increment duration
	this->duration_ = this->duration_ + track->Duration();
	// Create new playlist mapping
	Track_Playlist tp(track, this);

	// Return this so that we can save it to the database
	return &tp; // Happy now???

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
