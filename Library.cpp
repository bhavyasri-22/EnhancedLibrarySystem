#include "Library.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
const int FINE_PER_DAY = 10;

Library::Library() { loadFromFile(); }
Library::~Library() { saveToFile(); }

void Library::addBook(const Book &b) {
    books[b.id] = b;
    bookTrie.insert(b.title);
    Operation op; op.type = ADD_BOOK; op.bookBackup = b; undoStack.push(op);
    cout << "Book added successfully.\n";
}

void Library::removeBook(const string &bookID) {
    if(books.find(bookID) != books.end()) {
        Operation op; op.type = REMOVE_BOOK; op.bookBackup = books[bookID]; undoStack.push(op);
        books.erase(bookID);
        cout << "Book removed.\n";
    } else cout << "Book not found.\n";
}

void Library::searchBookByTitle(const string &title) {
    bool found = false;
    for(auto &b : books) if(b.second.title.find(title) != string::npos) { b.second.printBook(); found = true;}
    if(!found) cout << "No books found.\n";
}

void Library::addStudent(const Student &s) { students[s.id] = s; cout << "Student registered.\n"; }

void Library::issueBook(const string &bookID, const string &studentID) {
    if(students.find(studentID)==students.end()) { cout << "Student not found.\n"; return; }
    if(books.find(bookID)==books.end()) { cout << "Book not found.\n"; return; }
    if(!books[bookID].available) {
        waitingList.push({bookID, studentID}); cout << "Book unavailable. Added to waitlist.\n"; return;
    }
    books[bookID].available = false;
    IssuedBook ib; ib.bookID = bookID; ib.issueDate = time(nullptr);
    students[studentID].issuedBooks.push_back(ib);

    Operation op; op.type = ISSUE_BOOK; op.bookID = bookID; op.studentID = studentID; undoStack.push(op);
    cout << "Book issued successfully.\n";
}

void Library::returnBook(const string &bookID, const string &studentID) {
    if(students.find(studentID)==students.end()) { cout << "Student not found.\n"; return; }
    auto &issued = students[studentID].issuedBooks;
    bool found = false;
    for(auto it = issued.begin(); it != issued.end(); ++it) {
        if(it->bookID == bookID) {
            time_t now = time(nullptr);
            double days = difftime(now, it->issueDate)/(60*60*24);
            if(days>2) cout << "Overdue! Fine Rs " << static_cast<int>((days-2)*FINE_PER_DAY) << "\n";
            else cout << "Book returned on time.\n";
            issued.erase(it); found = true; break;
        }
    }
    if(found) {
        books[bookID].available = true;
        Operation op; op.type = RETURN_BOOK; op.bookID = bookID; op.studentID = studentID; undoStack.push(op);
        cout << "Book returned successfully.\n";
        // Serve waitlist
        if(!waitingList.empty()) {
            auto w = waitingList.front(); waitingList.pop();
            issueBook(w.second, w.first);
        }
    } else cout << "This book was not issued to this student.\n";
}

vector<string> Library::autocompleteTitle(const string &prefix) { return bookTrie.searchPrefix(prefix); }

void Library::undoLastOperation() {
    if(undoStack.empty()) { cout << "Nothing to undo.\n"; return; }
    Operation op = undoStack.top(); undoStack.pop();
    switch(op.type) {
        case ADD_BOOK: books.erase(op.bookBackup.id); break;
        case REMOVE_BOOK: books[op.bookBackup.id] = op.bookBackup; break;
        case ISSUE_BOOK: returnBook(op.bookID, op.studentID); break;
        case RETURN_BOOK: issueBook(op.bookID, op.studentID); break;
    }
    redoStack.push(op);
    cout << "Undo performed.\n";
}

void Library::redoLastOperation() {
    if(redoStack.empty()) { cout << "Nothing to redo.\n"; return; }
    Operation op = redoStack.top(); redoStack.pop();
    switch(op.type) {
        case ADD_BOOK: addBook(op.bookBackup); break;
        case REMOVE_BOOK: removeBook(op.bookBackup.id); break;
        case ISSUE_BOOK: issueBook(op.bookID, op.studentID); break;
        case RETURN_BOOK: returnBook(op.bookID, op.studentID); break;
    }
    undoStack.push(op);
    cout << "Redo performed.\n";
}

void Library::displayAllBooks() { for(auto &b : books) b.second.printBook(); }
void Library::displayIssuedBooks() { for(auto &s : students) s.second.printStudent(); }

void Library::saveToFile() {
    ofstream bookFile("books.txt");
    for(auto &b : books) bookFile << b.second.id << "," << b.second.title << "," << b.second.author << "," << b.second.available << "\n";
    bookFile.close();
    ofstream studentFile("students.txt");
    for(auto &s : students) {
        studentFile << s.second.id << "," << s.second.name;
        for(auto &ib : s.second.issuedBooks) studentFile << "," << ib.bookID << "," << ib.issueDate;
        studentFile << "\n";
    }
    studentFile.close();
}

void Library::loadFromFile() {
    ifstream bookFile("books.txt");
    if(bookFile) {
        string line;
        while(getline(bookFile, line)) {
            stringstream ss(line);
            string id, title, author, availableStr;
            getline(ss, id, ','); getline(ss, title, ','); getline(ss, author, ','); getline(ss, availableStr, ',');
            bool available = (availableStr=="1");
            books[id] = Book(id,title,author,available);
            bookTrie.insert(title);
        }
    }
    ifstream studentFile("students.txt");
    if(studentFile) {
        string line;
        while(getline(studentFile, line)) {
            stringstream ss(line);
            string id, name;
            getline(ss,id,','); getline(ss,name,',');
            Student s(id,name);
            string bookID, issueDateStr;
            while(getline(ss,bookID,',')) {
                if(getline(ss,issueDateStr,',')) {
                    IssuedBook ib; ib.bookID = bookID; ib.issueDate = stoll(issueDateStr);
                    s.issuedBooks.push_back(ib);
                    books[bookID].available=false;
                }
            }
            students[id]=s;
        }
    }
}
