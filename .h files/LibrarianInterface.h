#ifndef LIBRARIAN_INTERFACE_H
#define LIBRARIAN_INTERFACE_H

#include "Library.h"
#include "AuthSystem.h"

class LibrarianInterface {
private:
    Library* library;
    AuthSystem* auth;
    
    void showMenu();
    void handleAddBook();
    void handleRemoveBook();
    void handleRegisterStudent();
    void handleIssueBook();
    void handleReturnBook();
    void handleViewAllBooks();
    void handleViewIssuedBooks();
    void handleSearchBooks();
    void handleUndo();
    void handleRedo();
    void handleViewStatistics();
    void handleManageUsers();

public:
    LibrarianInterface(Library* lib, AuthSystem* authSys);
    void run();
};

#endif