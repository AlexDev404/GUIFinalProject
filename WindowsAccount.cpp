#include "WindowsAccount.hpp"

// Getters
const int Windows_Account::Id() {
	return _id;
}

const string Windows_Account::Name() {
	return username_;
}

const Roles* Windows_Account::AccessLevel() {
	return role_id_;
}

// Setters
void Windows_Account::SetName(string username) {
	username_ = username;
}

void Windows_Account::SetAccessLevel(Roles* access_level) {
	this->role_id_ = access_level;
}
