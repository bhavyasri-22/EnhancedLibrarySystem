#include "LibrarianInterface.h"
#include "colors.h"
#include <iostream>
#include <limits>

using namespace std;

int readIntOption() {
    int choice;
    while(!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "Invalid input! Enter a number: " << RESET;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

LibrarianInterface::LibrarianInterface(Library* lib, AuthSystem* authSys) 
    : library(lib), auth(authSys) {}

void LibrarianInterface::showMenu() {
    system("clear");
    
    cout << BLUE << "\nLIBRARIAN DASHBOARD\n";
    cout << "===================\n" << RESET;
    
    cout << "\nBOOK MANAGEMENT:\n";
    cout << "1.  Add Book\n";
    cout << "2.  Remove Book\n";
    cout << "3.  Display All Books\n";
    cout << "4.  Search Book by Title\n";
    cout << "5.  Search Book by ID\n";
    cout << "6.  Display Books by Genre\n";
    
    cout << "\nSTUDENT MANAGEMENT:\n";
    cout << "7.  Register New Student\n";
    cout << "8.  View All Students\n";
    
    cout << "\nTRANSACTIONS:\n";
    cout << "9.  Issue Book to Student\n";
    cout << "10. Process Book Return\n";
    cout << "11. Show All Issued Books\n";
    
    cout << "\nSYSTEM TOOLS:\n";
    cout << "12. Undo Last Operation\n";
    cout << "13. Redo Last Operation\n";
    cout << "14. View Statistics\n";
    cout << "15. Manage User Accounts\n";
    
    cout << RED << "\n0.  Logout\n" << RESET;
    
    cout << GREEN << "\nEnter your choice: " << RESET;
}

void LibrarianInterface::handleAddBook() {
    string id, title, author, genre;
    cout << "\n" << CYAN << "ADD NEW BOOK\n" << RESET;
    cout << "Book ID: "; getline(cin, id);
    cout << "Title: "; getline(cin, title);
    cout << "Author: "; getline(cin, author);
    cout << "Genre: "; getline(cin, genre);
    library->addBook(Book(id, title, author, genre));
}

void LibrarianInterface::handleRemoveBook() {
    string id;
    cout << "\n" << CYAN << "REMOVE BOOK\n" << RESET;
    cout << "Book ID to remove: "; getline(cin, id);
    library->removeBook(id);
}

void LibrarianInterface::handleRegisterStudent() {
    string studentID, name;
    cout << "\n" << CYAN << "REGISTER STUDENT\n" << RESET;
    cout << "Student ID: "; getline(cin, studentID);
    cout << "Full Name: "; getline(cin, name);
    library->addStudent(Student(studentID, name));
}

void LibrarianInterface::handleIssueBook() {
    string studentID, bookID;
    cout << "\n" << CYAN << "ISSUE BOOK\n" << RESET;
    cout << "Student ID: "; getline(cin, studentID);
    cout << "Book ID: "; getline(cin, bookID);
    library->issueBook(bookID, studentID);
}

void LibrarianInterface::handleReturnBook() {
    string studentID, bookID;
    cout << "\n" << CYAN << "RETURN BOOK\n" << RESET;
    cout << "Student ID: "; getline(cin, studentID);
    cout << "Book ID: "; getline(cin, bookID);
    library->returnBook(bookID, studentID);
}

void LibrarianInterface::handleViewAllBooks() {
    cout << "\n" << CYAN << "ALL BOOKS\n" << RESET;
    library->displayAllBooks();
}

void LibrarianInterface::handleViewIssuedBooks() {
    cout << "\n" << CYAN << "CURRENTLY ISSUED BOOKS\n" << RESET;
    library->displayIssuedBooks();
}

void LibrarianInterface::handleSearchBooks() {
    string title;
    cout << "\n" << CYAN << "SEARCH BOOKS\n" << RESET;
    cout << "Enter title (substring search): "; 
    getline(cin, title);
    library->searchBookByTitle(title);
}

void LibrarianInterface::handleUndo() {
    cout << "\n" << CYAN << "UNDO LAST OPERATION\n" << RESET;
    library->undoLastOperation();
}

void LibrarianInterface::handleRedo() {
    cout << "\n" << CYAN << "REDO LAST OPERATION\n" << RESET;
    library->redoLastOperation();
}

void LibrarianInterface::handleViewStatistics() {
    cout << "\n" << CYAN << "LIBRARY STATISTICS\n" << RESET;
    cout << GREEN << "Total Books: " << RESET << "TODO\n";
    cout << GREEN << "Total Students: " << RESET << "TODO\n";
    cout << GREEN << "Books Issued: " << RESET << "TODO\n";
    cout << GREEN << "Books Available: " << RESET << "TODO\n";
    cout << GREEN << "Overdue Books: " << RESET << "TODO\n";
}

void LibrarianInterface::handleManageUsers() {
    cout << "\n" << CYAN << "USER ACCOUNT MANAGEMENT\n" << RESET;
    cout << YELLOW << "Feature coming soon!\n" << RESET;
    cout << "This will allow you to:\n";
    cout << "  - View all user accounts\n";
    cout << "  - Reset passwords\n";
    cout << "  - Disable/Enable accounts\n";
}

void LibrarianInterface::run() {
    while (true) {
        showMenu();
        int choice = readIntOption();
        
        switch(choice) {
            case 1: handleAddBook(); break;
            case 2: handleRemoveBook(); break;
            case 3: handleViewAllBooks(); break;
            case 4: handleSearchBooks(); break;
            case 5: {
                string id;
                cout << "Enter book ID: "; getline(cin, id);
                library->searchBookByID(id);
                break;
            }
            case 6: {
                auto genres = library->availableGenres();
                if(genres.empty()) {
                    cout << "No genres available.\n";
                } else {
                    cout << "\nAvailable Genres:\n";
                    for(size_t i = 0; i < genres.size(); ++i) {
                        cout << (i+1) << ". " << genres[i] << "\n";
                    }
                    cout << "0. Cancel\nChoice: ";
                    int gchoice = readIntOption();
                    if(gchoice > 0 && gchoice <= (int)genres.size()) {
                        library->displayBooksByGenre(genres[gchoice-1]);
                    }
                }
                break;
            }
            case 7: handleRegisterStudent(); break;
            case 8: {
                cout << "\n" << CYAN << "ALL STUDENTS\n" << RESET;
                library->displayIssuedBooks();
                break;
            }
            case 9: handleIssueBook(); break;
            case 10: handleReturnBook(); break;
            case 11: handleViewIssuedBooks(); break;
            case 12: handleUndo(); break;
            case 13: handleRedo(); break;
            case 14: handleViewStatistics(); break;
            case 15: handleManageUsers(); break;
            case 0:
                cout << GREEN << "\nLogging out...\n" << RESET;
                return;
            default:
                cout << RED << "Invalid choice!\n" << RESET;
        }
        
        cout << "\n" << YELLOW << "Press Enter to continue..." << RESET;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}