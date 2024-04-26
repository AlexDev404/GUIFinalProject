#include "Albums.hpp"

// Getters
int Albums::Id() {
	return this->id_;
}

string Albums::Title() {
	return this->title;
}

string Albums::Release_Date() {
	return this->release_date;
}
// Setters
void Albums::SetTitle(string name) {
	this->title = name;
}

void Albums::SetReleaseDate(string rdate) {
	this->release_date = rdate;
}
