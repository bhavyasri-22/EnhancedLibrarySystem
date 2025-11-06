#include "AuthSystem.h"
#include <fstream>
#include <sstream>
#include <iostream>

const char SEP = '|';

AuthSystem::AuthSystem() : currentUser(nullptr) {
    loadUsers();
    
    // Create default librarian if no users exist
    if (users.empty()) {
        User admin("admin", "admin123", LIBRARIAN, "");
        users[admin.username] = admin;
        cout << "Default librarian account created (admin/admin123)\n";
        saveUsers();
    }
}

AuthSystem::~AuthSystem() {
    saveUsers();
    delete currentUser;
}

bool AuthSystem::login(const string& username, const string& password) {
    auto it = users.find(username);
    if (it != users.end() && it->second.authenticate(password)) {
        currentUser = new User(it->second);
        currentUser->updateLastLogin();
        users[username] = *currentUser; // Update last login in map
        saveUsers();
        return true;
    }
    return false;
}

void AuthSystem::logout() {
    delete currentUser;
    currentUser = nullptr;
}

bool AuthSystem::registerUser(const User& user) {
    if (users.find(user.username) != users.end()) {
        return false; // Username already exists
    }
    users[user.username] = user;
    saveUsers();
    return true;
}

bool AuthSystem::isLibrarian() const {
    return currentUser && currentUser->role == LIBRARIAN;
}

void AuthSystem::saveUsers() {
    ofstream file("users.txt");
    if (!file) return;
    
    for (const auto& p : users) {
        const User& u = p.second;
        file << u.username << SEP 
             << u.password << SEP 
             << (int)u.role << SEP 
             << u.linkedStudentID << SEP 
             << u.lastLogin << "\n";
    }
    file.close();
}

void AuthSystem::loadUsers() {
    ifstream file("users.txt");
    if (!file) return;
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string username, password, roleStr, studentID, loginStr;
        
        getline(ss, username, SEP);
        getline(ss, password, SEP);
        getline(ss, roleStr, SEP);
        getline(ss, studentID, SEP);
        getline(ss, loginStr, SEP);
        
        UserRole role = (roleStr == "0") ? LIBRARIAN : STUDENT;
        time_t lastLogin = 0;
        try { lastLogin = stoll(loginStr); } catch(...) {}
        
        User u(username, password, role, studentID);
        u.lastLogin = lastLogin;
        users[username] = u;
    }
    file.close();
}