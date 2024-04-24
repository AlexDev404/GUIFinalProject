#include "WindowsAccount.hpp"

// Getters
const int Windows_Account::Id() {
	return _id;
}

const string Windows_Account::Name() {
	return name_;
}

const string Windows_Account::AccessLevel() {
	return access_level_;
}

// Setters
void Windows_Account::SetName(string name) {
	name_ = name;
}

void Windows_Account::SetAccessLevel(string access_level) {
	access_level_ = access_level;
}
