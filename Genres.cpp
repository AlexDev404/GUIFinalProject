#include "Genres.hpp"

// Getters
int Genres::Id() {
	return this->id;
}

string Genres::Title() {
	return this->title;
}

// Setters
void Genres::SetTitle(string name) {
	this->title = name;
}