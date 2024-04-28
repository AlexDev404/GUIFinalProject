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

void Playlist::SetName(string name) {
	name_ = name;
}

void Playlist::SetYear(string year) {
	year_ = year;
}

void Playlist::SetDuration(double duration) {
	duration_ = duration;
}
