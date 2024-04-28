#include "WindowsAccount.hpp"

// Getters
const int Windows_Account::Id() {
	return _id;
}

const string Windows_Account::Name() {
	return name_;
}

const Roles* Windows_Account::AccessLevel() {
	return access_level_;
}

// Setters
void Windows_Account::SetName(string name) {
	name_ = name;
}

void Windows_Account::SetAccessLevel(Roles* access_level) {
	this->access_level_ = access_level;
}
