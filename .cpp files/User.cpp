#include "User.h"
#include <ctime>

User::User() : username(""), password(""), role(STUDENT), linkedStudentID(""), lastLogin(0) {}

User::User(const string& _username, const string& _password, UserRole _role, const string& _studentID)
    : username(_username), password(_password), role(_role), linkedStudentID(_studentID) {
    lastLogin = time(nullptr);
}

bool User::authenticate(const string& pwd) const {
    return password == pwd; // Simple comparison (use bcrypt in production)
}

void User::updateLastLogin() {
    lastLogin = time(nullptr);
}