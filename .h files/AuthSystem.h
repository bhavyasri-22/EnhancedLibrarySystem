#ifndef AUTHSYSTEM_H
#define AUTHSYSTEM_H

#include <unordered_map>
#include <string>
#include "User.h"
using namespace std;

class AuthSystem {
private:
    unordered_map<string, User> users; // username -> User
    User* currentUser;

public:
    AuthSystem();
    ~AuthSystem();

    bool login(const string& username, const string& password);
    void logout();
    bool registerUser(const User& user);
    
    User* getCurrentUser() const { return currentUser; }
    bool isLoggedIn() const { return currentUser != nullptr; }
    bool isLibrarian() const;
    
    void saveUsers();
    void loadUsers();
};

#endif