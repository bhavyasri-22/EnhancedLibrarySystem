#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "colors.h"
#include "Library.h"

using namespace std;

int readIntOption() {
    int choice;
    while(!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a valid number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear rest of line
    return choice;
}
void printMenu() {
    cout << CYAN << "\n========================================\n";
    cout << BOLD << "        📚 LIBRARY MANAGEMENT SYSTEM\n";
    cout << CYAN << "========================================" << RESET << "\n";

    cout << YELLOW << "1." << RESET << " Add Book\n";
    cout << YELLOW << "2." << RESET << " Remove Book\n";
    cout << YELLOW << "3." << RESET << " Display All Books\n";
    cout << YELLOW << "4." << RESET << " Register Student\n";
    cout << YELLOW << "5." << RESET << " Issue Book\n";
    cout << YELLOW << "6." << RESET << " Return Book\n";
    cout << YELLOW << "7." << RESET << " Show Issued Books\n";
    cout << YELLOW << "8." << RESET << " Search Book by Title\n";
    cout << YELLOW << "9." << RESET << " Autocomplete Title\n";
    cout << YELLOW << "10." << RESET << " Search Book by ID\n";
    cout << YELLOW << "11." << RESET << " Display Books by Genre\n";
    cout << YELLOW << "12." << RESET << " Undo Last Operation\n";
    cout << YELLOW << "13." << RESET << " Redo Last Operation\n";
    cout << YELLOW << "14." << RESET << RED << " Exit\n" << RESET;

    cout << CYAN << "========================================\n" << RESET;
    cout << GREEN << "➡️  Enter your choice: " << RESET;
}

int main() {
    Library lib;
    int choice;

    while(true) {
        printMenu();

        choice = readIntOption();

        if(choice == 1) {
            string id, title, author, genre;
            cout << "Book ID: "; getline(cin, id);
            cout << "Title: "; getline(cin, title);
            cout << "Author: "; getline(cin, author);
            cout << "Genre: "; getline(cin, genre);
            lib.addBook(Book(id, title, author, genre));
        }
        else if(choice == 2) {
            string id;
            cout << "Book ID to remove: "; getline(cin, id);
            lib.removeBook(id);
        }
        else if(choice == 3) {
            lib.displayAllBooks();
        }
        else if(choice == 4) {
            string studentID, name;
            cout << "Student ID: "; getline(cin, studentID);
            cout << "Name: "; getline(cin, name);
            lib.addStudent(Student(studentID, name));
        }
        else if(choice == 5) {
            string studentID, bookID;
            cout << "Student ID: "; getline(cin, studentID);
            cout << "Book ID: "; getline(cin, bookID);
            lib.issueBook(bookID, studentID);
        }
        else if(choice == 6) {
            string studentID, bookID;
            cout << "Student ID: "; getline(cin, studentID);
            cout << "Book ID: "; getline(cin, bookID);
            lib.returnBook(bookID, studentID);
        }
        else if(choice == 7) {
            lib.displayIssuedBooks();
        }
        else if(choice == 8) {
            string title;
            cout << "Enter title to search (substring): "; getline(cin, title);
            lib.searchBookByTitle(title);
        }
        else if(choice == 9) {
            string prefix;
            cout << "Enter prefix: "; getline(cin, prefix);
            auto results = lib.autocompleteTitle(prefix);
            if(results.empty()) cout << "No matches found.\n";
            else {
                cout << "Matches:\n";
                for(auto &t : results) cout << "  " << t << "\n";
            }
        }
        else if(choice == 10) {
            string id;
            cout << "Enter book ID: "; getline(cin, id);
            lib.searchBookByID(id);
        }
        else if(choice == 11) {
            // Option B: show list of available genres
            auto genres = lib.availableGenres();
            if(genres.empty()) {
                cout << "No genres available (no books).\n";
            } else {
                cout << "Choose a genre:\n";
                for(size_t i = 0; i < genres.size(); ++i) {
                    cout << (i+1) << ". " << genres[i] << "\n";
                }
                cout << "0. Cancel\nChoice: ";
                int gchoice = readIntOption();
                if(gchoice > 0 && gchoice <= (int)genres.size()) {
                    lib.displayBooksByGenre(genres[gchoice-1]);
                } else {
                    cout << "Cancelled or invalid selection.\n";
                }
            }
        }
        else if(choice == 12) {
            lib.undoLastOperation();
        }
        else if(choice == 13) {
            lib.redoLastOperation();
        }
        else if(choice == 14) {
            cout << "Exiting...\n";
            break;
        }
        else {
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}
