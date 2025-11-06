#ifndef USER_H
#define USER_H

#include <string>
#include <ctime>
using namespace std;

enum UserRole { LIBRARIAN, STUDENT };

class User {
public:
    string username;
    string password; // In production, use hashed passwords
    UserRole role;
    string linkedStudentID; // Only for STUDENT role
    time_t lastLogin;

    User();
    User(const string& _username, const string& _password, UserRole _role, const string& _studentID = "");
    
    bool authenticate(const string& pwd) const;
    void updateLastLogin();
};

#endif