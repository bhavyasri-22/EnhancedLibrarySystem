#include "StudentInterface.h"
#include "colors.h"
#include <iostream>
#include <limits>
#include <ctime>

using namespace std;

extern int readIntOption();

StudentInterface::StudentInterface(Library* lib, AuthSystem* authSys) 
    : library(lib), auth(authSys) {
    studentID = auth->getCurrentUser()->linkedStudentID;
}

void StudentInterface::showMenu() {
    system("clear");
    
    cout << CYAN << "\nSTUDENT PORTAL\n";
    cout << "==============\n" << RESET;
    cout << "Student ID: " << studentID << "\n\n";
    
    cout << "BROWSE BOOKS:\n";
    cout << "1. Search Books by Title\n";
    cout << "2. Browse Books by Genre\n";
    cout << "3. View All Available Books\n";
    cout << "4. Autocomplete Title Search\n";
    
    cout << "\nMY ACCOUNT:\n";
    cout << "5. View My Issued Books\n";
    cout << "6. View My Profile\n";
    cout << "7. Request Book Issue\n";
    
    cout << RED << "\n0. Logout\n" << RESET;
    
    cout << GREEN << "\nEnter your choice: " << RESET;
}

void StudentInterface::handleSearchBooks() {
    string title;
    cout << "\n" << CYAN << "SEARCH BOOKS\n" << RESET;
    cout << "Enter title to search: "; 
    getline(cin, title);
    library->searchBookByTitle(title);
}

void StudentInterface::handleViewMyBooks() {
    cout << "\n" << CYAN << "MY ISSUED BOOKS\n" << RESET;
    library->displayIssuedBooks();
    
    cout << YELLOW << "\nNote: Showing all issued books. Filtered view coming soon!\n" << RESET;
}

void StudentInterface::handleViewBooksByGenre() {
    auto genres = library->availableGenres();
    if(genres.empty()) {
        cout << RED << "No genres available.\n" << RESET;
        return;
    }
    
    cout << "\n" << CYAN << "BROWSE BY GENRE\n" << RESET;
    for(size_t i = 0; i < genres.size(); ++i) {
        cout << (i+1) << ". " << genres[i] << "\n";
    }
    cout << "0. Cancel\n";
    cout << GREEN << "\nChoice: " << RESET;
    
    int choice = readIntOption();
    if(choice > 0 && choice <= (int)genres.size()) {
        cout << "\n" << CYAN << genres[choice-1] << " BOOKS\n" << RESET;
        library->displayBooksByGenre(genres[choice-1]);
    } else if (choice != 0) {
        cout << RED << "Invalid selection!\n" << RESET;
    }
}

void StudentInterface::handleAutocomplete() {
    string prefix;
    cout << "\n" << CYAN << "AUTOCOMPLETE SEARCH\n" << RESET;
    cout << "Enter book title prefix: "; 
    getline(cin, prefix);
    
    auto results = library->autocompleteTitle(prefix);
    
    if(results.empty()) {
        cout << YELLOW << "No matches found.\n" << RESET;
    } else {
        cout << GREEN << "\nFound " << results.size() << " matching titles:\n" << RESET;
        for(size_t i = 0; i < results.size(); ++i) {
            cout << "  " << (i+1) << ". " << results[i] << "\n";
        }
    }
}

void StudentInterface::handleViewProfile() {
    cout << "\n" << CYAN << "MY PROFILE\n" << RESET;
    
    User* currentUser = auth->getCurrentUser();
    
    cout << "Username: " << currentUser->username << "\n";
    cout << "Student ID: " << studentID << "\n";
    cout << "Role: Student\n";
    
    char timeStr[100];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&currentUser->lastLogin));
    cout << "Last Login: " << timeStr << "\n";
}

void StudentInterface::handleRequestBook() {
    string bookID;
    cout << "\n" << CYAN << "REQUEST BOOK ISSUE\n" << RESET;
    
    cout << "Enter Book ID you want to borrow: ";
    getline(cin, bookID);
    
    library->searchBookByID(bookID);
    
    cout << YELLOW << "\nTo borrow this book:\n" << RESET;
    cout << "1. Note down the Book ID: " << GREEN << bookID << RESET << "\n";
    cout << "2. Visit the library desk\n";
    cout << "3. Present your Student ID: " << GREEN << studentID << RESET << "\n";
    cout << "4. The librarian will issue the book\n\n";
    
    cout << GREEN << "Request noted! Please visit the library.\n" << RESET;
}

void StudentInterface::run() {
    while (true) {
        showMenu();
        int choice = readIntOption();
        
        switch(choice) {
            case 1: 
                handleSearchBooks(); 
                break;
            case 2: 
                handleViewBooksByGenre(); 
                break;
            case 3:
                cout << "\n" << CYAN << "ALL AVAILABLE BOOKS\n" << RESET;
                library->displayAllBooks();
                break;
            case 4: 
                handleAutocomplete(); 
                break;
            case 5: 
                handleViewMyBooks(); 
                break;
            case 6: 
                handleViewProfile(); 
                break;
            case 7: 
                handleRequestBook(); 
                break;
            case 0:
                cout << GREEN << "\nLogging out...\n" << RESET;
                cout << "Thank you for using the library system!\n";
                return;
            default:
                cout << RED << "Invalid choice! Please try again.\n" << RESET;
        }
        
        cout << "\n" << YELLOW << "Press Enter to continue..." << RESET;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}