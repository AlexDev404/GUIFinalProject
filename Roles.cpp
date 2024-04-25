#include "Roles.hpp"

// Getters
int Roles::Id() {
	return this->id;
}

string Roles::Name() {
	return this->name;
}

// Setters
void Roles::SetName(string role) {
	this->name = role;
}
