#include "Artists.hpp"

// Getters
const int Artists::Id() {
	return id_;
}

const string Artists::Name() {
	return name_;
}

// Setters
void Artists::SetName(string name) {
		name_ = name;
}