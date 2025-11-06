#ifndef STUDENT_INTERFACE_H
#define STUDENT_INTERFACE_H

#include "Library.h"
#include "AuthSystem.h"

class StudentInterface {
private:
    Library* library;
    AuthSystem* auth;
    string studentID;
    
    void showMenu();
    void handleSearchBooks();
    void handleViewMyBooks();
    void handleViewBooksByGenre();
    void handleAutocomplete();
    void handleViewProfile();
    void handleRequestBook(); // Request librarian to issue

public:
    StudentInterface(Library* lib, AuthSystem* authSys);
    void run();
};

#endif