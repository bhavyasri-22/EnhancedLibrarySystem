#include <iostream>
#include <string>
#include "Library.h"

using namespace std;

int main() {
    Library lib;
    int choice;
    string id, title, author, studentID, bookID, name;

    while(true) {
        cout << "\n---- Library Menu ----\n"
             << "1. Add Book\n2. Remove Book\n3. Display All Books\n4. Register Student\n"
             << "5. Issue Book\n6. Return Book\n7. Show Issued Books\n"
             << "8. Search Book by Title\n9. Autocomplete Title\n"
             << "10. Undo Last Operation\n11. Redo Last Operation\n12. Exit\nChoice: ";
        cin >> choice;
        cin.ignore(); // clear newline

        switch(choice) {
            case 1:
                cout << "Book ID: "; getline(cin, id);
                cout << "Title: "; getline(cin, title);
                cout << "Author: "; getline(cin, author);
                lib.addBook(Book(id,title,author));
                break;
            case 2:
                cout << "Book ID to remove: "; getline(cin, id);
                lib.removeBook(id);
                break;
            case 3:
                lib.displayAllBooks();
                break;
            case 4:
                cout << "Student ID: "; getline(cin, studentID);
                cout << "Name: "; getline(cin, name);
                lib.addStudent(Student(studentID,name));
                break;
            case 5:
                cout << "Student ID: "; getline(cin, studentID);
                cout << "Book ID: "; getline(cin, bookID);
                lib.issueBook(bookID, studentID);
                break;
            case 6:
                cout << "Student ID: "; getline(cin, studentID);
                cout << "Book ID: "; getline(cin, bookID);
                lib.returnBook(bookID, studentID);
                break;
            case 7:
                lib.displayIssuedBooks();
                break;
            case 8:
                cout << "Enter title to search: "; getline(cin, title);
                lib.searchBookByTitle(title);
                break;
            case 9:
                cout << "Enter prefix: "; getline(cin, title);
                {
                    auto results = lib.autocompleteTitle(title);
                    if(results.empty()) cout << "No matches found.\n";
                    else {
                        cout << "Matches:\n";
                        for(auto &t : results) cout << t << "\n";
                    }
                }
                break;
            case 10:
                lib.undoLastOperation();
                break;
            case 11:
                lib.redoLastOperation();
                break;
            case 12:
                cout << "Exiting...\n"; return 0;
            default:
                cout << "Invalid choice!\n";
        }
    }

    return 0;
}
