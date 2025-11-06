#include <iostream>
#include <string>
#include <limits>
#include "colors.h"
#include "Library.h"
#include "AuthSystem.h"
#include "LibrarianInterface.h"
#include "StudentInterface.h"

using namespace std;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void printWelcomeBanner() {
    cout << BLUE << "\n========================================\n";
    cout << "      LIBRARY MANAGEMENT SYSTEM\n";
    cout << "========================================\n" << RESET;
}

bool loginScreen(AuthSystem& auth) {
    clearScreen();
    printWelcomeBanner();
    
    cout << GREEN << "\nPlease login to continue\n";
    cout << "-------------------------\n" << RESET;
    
    string username, password;
    cout << "Username: ";
    getline(cin, username);
    
    cout << "Password: ";
    getline(cin, password);
    
    if (auth.login(username, password)) {
        cout << GREEN << "\n✓ Login successful!\n" << RESET;
        cout << "Welcome, " << username << "!\n";
        
        if (auth.isLibrarian()) {
            cout << BLUE << "Role: Librarian\n" << RESET;
        } else {
            cout << CYAN << "Role: Student\n" << RESET;
        }
        
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return true;
    } else {
        cout << RED << "\n✗ Invalid credentials!\n" << RESET;
        cout << "Press Enter to try again...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
}

bool directLibrarianLogin(AuthSystem& auth) {
    clearScreen();
    printWelcomeBanner();
    
    cout << BLUE << "\nLibrarian Direct Access\n";
    cout << "-----------------------\n" << RESET;
    
    string password;
    cout << "Enter Librarian Password: ";
    getline(cin, password);
    
    if (password == "lib123") {
        auth.login("admin", "admin123");
        cout << GREEN << "\n✓ Librarian access granted!\n" << RESET;
        cout << BLUE << "Role: Librarian\n" << RESET;
        
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return true;
    } else {
        cout << RED << "\n✗ Invalid librarian password!\n" << RESET;
        cout << "Press Enter to return to main menu...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
}

void registerScreen(AuthSystem& auth, Library& lib) {
    clearScreen();
    cout << CYAN << "\n==============================\n";
    cout << "     STUDENT REGISTRATION\n";
    cout << "==============================\n" << RESET;
    
    string username, password, name, studentID;
    
    cout << "\nEnter registration details:\n";
    cout << "---------------------------\n";
    
    cout << "Student ID: ";
    getline(cin, studentID);
    
    cout << "Full Name: ";
    getline(cin, name);
    
    cout << "Username: ";
    getline(cin, username);
    
    cout << "Password: ";
    getline(cin, password);
    
    Student s(studentID, name);
    lib.addStudent(s);
    
    User newUser(username, password, STUDENT, studentID);
    
    if (auth.registerUser(newUser)) {
        cout << GREEN << "\n✓ Registration successful!\n" << RESET;
        cout << "You can now login with your credentials.\n";
    } else {
        cout << RED << "\n✗ Username already exists!\n" << RESET;
    }
    
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void mainMenu(AuthSystem& auth, Library& lib) {
    while (true) {
        clearScreen();
        printWelcomeBanner();
        
        cout << "\nMAIN MENU:\n";
        cout << "---------\n";
        cout << "1. Student Login\n";
        cout << "2. Student Registration\n";
        cout << "3. Librarian Access\n";
        cout << "4. Exit\n";
        
        cout << GREEN << "\nEnter your choice: " << RESET;
        
        int choice;
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Invalid input! Enter a number: " << RESET;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (choice == 1) {
            if (loginScreen(auth)) {
                clearScreen();
                
                if (auth.isLibrarian()) {
                    LibrarianInterface libInterface(&lib, &auth);
                    libInterface.run();
                } else {
                    StudentInterface studentInterface(&lib, &auth);
                    studentInterface.run();
                }
                
                auth.logout();
            }
        } else if (choice == 2) {
            registerScreen(auth, lib);
        } else if (choice == 3) {
            if (directLibrarianLogin(auth)) {
                clearScreen();
                LibrarianInterface libInterface(&lib, &auth);
                libInterface.run();
                auth.logout();
            }
        } else if (choice == 4) {
            cout << BLUE << "\nThank you for using Library Management System!\n" << RESET;
            cout << "Goodbye!\n\n";
            break;
        } else {
            cout << RED << "Invalid choice!\n" << RESET;
            cout << "Press Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

int main() {
    Library lib;
    AuthSystem auth;
    
    mainMenu(auth, lib);
    
    return 0;
}