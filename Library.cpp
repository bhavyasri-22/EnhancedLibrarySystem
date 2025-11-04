#include "Library.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <set>

const int FINE_PER_DAY = 10;
const char SEP = '|';

Library::Library() {
    loadFromFile();
}

Library::~Library() {
    saveToFile();
}

void Library::addBook(const Book &b) {
    books[b.id] = b;
    bookTrie.insert(b.title);

    Operation op;
    op.type = ADD_BOOK;
    op.bookBackup = b;
    undoStack.push(op);
    while(!redoStack.empty()) redoStack.pop();

    cout << "Book added successfully.\n";
}

void Library::removeBook(const string &bookID) {
    auto it = books.find(bookID);
    if(it != books.end()) {
        Operation op;
        op.type = REMOVE_BOOK;
        op.bookBackup = it->second;
        undoStack.push(op);

        bookTrie.remove(it->second.title);
        books.erase(it);

        while(!redoStack.empty()) redoStack.pop();
        cout << "Book removed.\n";
    } else {
        cout << "Book not found.\n";
    }
}

void Library::searchBookByTitle(const string &title) {
    bool found = false;

    // convert search input to lowercase
    string searchKey = title;
    for(char &c : searchKey) c = tolower(c);

    for(const auto &p : books) {
        string bookTitle = p.second.title;

        // convert book title to lowercase
        string lowerTitle = bookTitle;
        for(char &c : lowerTitle) c = tolower(c);

        // check case-insensitive substring match
        if(lowerTitle.find(searchKey) != string::npos) {
            p.second.printBook();
            found = true;
        }
    }

    if(!found) cout << "No books found.\n";
}


void Library::searchBookByID(const string &bookID) {
    auto it = books.find(bookID);
    if(it != books.end()) it->second.printBook();
    else cout << "Book ID not found!\n";
}

void Library::addStudent(const Student &s) {
    students[s.id] = s;
    cout << "Student registered.\n";
    while(!redoStack.empty()) redoStack.pop();
}

void Library::performIssueWithoutRecord(const string& bookID, const string& studentID) {
    auto bIt = books.find(bookID);
    auto sIt = students.find(studentID);
    if(bIt==books.end() || sIt==students.end()) return;
    if(!bIt->second.available) return;

    bIt->second.available = false;
    IssuedBook ib; ib.bookID = bookID; ib.issueDate = time(nullptr);
    sIt->second.issuedBooks.push_back(ib);
}

void Library::performReturnWithoutRecord(const string& bookID, const string& studentID) {
    auto sIt = students.find(studentID);
    if(sIt==students.end()) return;
    auto &issued = sIt->second.issuedBooks;
    for(auto it = issued.begin(); it != issued.end(); ++it) {
        if(it->bookID == bookID) {
            issued.erase(it);
            break;
        }
    }
    auto bIt = books.find(bookID);
    if(bIt != books.end()) bIt->second.available = true;
}

void Library::issueBook(const string &bookID, const string &studentID) {
    auto sIt = students.find(studentID);
    if(sIt==students.end()) { cout << "Student not found.\n"; return; }
    auto bIt = books.find(bookID);
    if(bIt==books.end()) { cout << "Book not found.\n"; return; }

    if(!bIt->second.available) {
        waitingList.push({bookID, studentID});
        cout << "Book unavailable. Added to waiting list.\n";
        return;
    }

    bIt->second.available = false;
    IssuedBook ib; ib.bookID = bookID; ib.issueDate = time(nullptr);
    sIt->second.issuedBooks.push_back(ib);

    Operation op;
    op.type = ISSUE_BOOK;
    op.bookID = bookID;
    op.studentID = studentID;
    undoStack.push(op);
    while(!redoStack.empty()) redoStack.pop();

    cout << "Book issued successfully.\n";
}

void Library::returnBook(const string &bookID, const string &studentID) {
    auto sIt = students.find(studentID);
    if(sIt==students.end()) { cout << "Student not found.\n"; return; }

    auto &issued = sIt->second.issuedBooks;
    bool found = false;
    time_t issueTime = 0;
    for(auto it = issued.begin(); it != issued.end(); ++it) {
        if(it->bookID == bookID) {
            issueTime = it->issueDate;
            issued.erase(it);
            found = true;
            break;
        }
    }
    if(!found) { cout << "This book was not issued to this student.\n"; return; }

    time_t now = time(nullptr);
    double days = difftime(now, issueTime)/(60*60*24);
    if(days > 2.0) {
        int fine = static_cast<int>((days - 2.0) * FINE_PER_DAY);
        cout << "Overdue! Fine Rs " << fine << "\n";
    } else {
        cout << "Book returned on time.\n";
    }

    auto bIt = books.find(bookID);
    if(bIt != books.end()) bIt->second.available = true;

    Operation op;
    op.type = RETURN_BOOK;
    op.bookID = bookID;
    op.studentID = studentID;
    undoStack.push(op);
    while(!redoStack.empty()) redoStack.pop();

    cout << "Book returned successfully.\n";

    // Serve waitlist entries for this book (FIFO)
    if(!waitingList.empty()) {
        int qsize = (int)waitingList.size();
        bool served = false;
        queue<pair<string,string>> temp;
        while(qsize--) {
            auto w = waitingList.front();
            waitingList.pop();
            if(!served && w.first == bookID) {
                // w.first == bookID, w.second == studentID waiting
                issueBook(w.first, w.second);
                served = true;
            } else {
                temp.push(w);
            }
        }
       waitingList = std::move(temp);

    }
}

vector<string> Library::autocompleteTitle(const string &prefix) {
    return bookTrie.searchPrefix(prefix, 15);
}

void Library::undoLastOperation() {
    if(undoStack.empty()) { cout << "Nothing to undo.\n"; return; }
    Operation op = undoStack.top(); undoStack.pop();

    switch(op.type) {
        case ADD_BOOK: {
            auto it = books.find(op.bookBackup.id);
            if(it != books.end()) {
                bookTrie.remove(it->second.title);
                books.erase(it);
            }
            break;
        }
        case REMOVE_BOOK: {
            books[op.bookBackup.id] = op.bookBackup;
            bookTrie.insert(op.bookBackup.title);
            break;
        }
        case ISSUE_BOOK: {
            performReturnWithoutRecord(op.bookID, op.studentID);
            break;
        }
        case RETURN_BOOK: {
            performIssueWithoutRecord(op.bookID, op.studentID);
            break;
        }
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

    cout << "Redo performed.\n";
}

void Library::displayAllBooks() {
    if(books.empty()) { cout << "No books available.\n"; return; }
    for(const auto &p : books) p.second.printBook();
}

void Library::displayIssuedBooks() {
    bool any = false;
    for(const auto &s : students) {
        if(!s.second.issuedBooks.empty()) {
            s.second.printStudent();
            any = true;
        }
    }
    if(!any) cout << "No issued books currently.\n";
}

void Library::displayBooksByGenre(const string &genre) {
    bool found = false;
    for(const auto &p : books) {
        if(p.second.genre == genre) {
            p.second.printBook();
            found = true;
        }
    }
    if(!found) cout << "No books found in this genre!\n";
}

vector<string> Library::availableGenres() const {
    set<string> genres;
    for(const auto &p : books) {
        if(!p.second.genre.empty()) genres.insert(p.second.genre);
    }
    return vector<string>(genres.begin(), genres.end());
}

void Library::saveToFile() {
    ofstream bookFile("books.txt");
    if(!bookFile) {
        cerr << "Failed to open books.txt for writing\n";
        return;
    }
    for(const auto &p : books) {
        const Book &b = p.second;
        bookFile << b.id << SEP
                 << b.title << SEP
                 << b.author << SEP
                 << b.genre << SEP
                 << (b.available ? "1" : "0") << "\n";
    }
    bookFile.close();

    ofstream studentFile("students.txt");
    if(!studentFile) {
        cerr << "Failed to open students.txt for writing\n";
        return;
    }

    for(const auto &p : students) {
        const Student &s = p.second;
        studentFile << s.id << SEP << s.name;
        for(const auto &ib : s.issuedBooks) {
            studentFile << SEP << ib.bookID << SEP << ib.issueDate;
        }
        studentFile << "\n";
    }
    studentFile.close();
}

void Library::loadFromFile() {
    books.clear();
    students.clear();
    bookTrie.clear();

    ifstream bookFile("books.txt");
    if(bookFile) {
        string line;
        while(getline(bookFile, line)) {
            if(line.empty()) continue;
            stringstream ss(line);
            string id, title, author, genre, avail;
            if(!getline(ss, id, SEP)) continue;
            if(!getline(ss, title, SEP)) title = "";
            if(!getline(ss, author, SEP)) author = "";
            if(!getline(ss, genre, SEP)) genre = "";
            if(!getline(ss, avail, SEP)) avail = "1";

            bool av = (avail == "1");
            books[id] = Book(id, title, author, genre, av);
            bookTrie.insert(title);
        }
        bookFile.close();
    }

    ifstream studentFile("students.txt");
    if(studentFile) {
        string line;
        while(getline(studentFile, line)) {
            if(line.empty()) continue;
            stringstream ss(line);
            string id, name;
            if(!getline(ss, id, SEP)) continue;
            if(!getline(ss, name, SEP)) name = "";
            Student s(id, name);

            string token;
            while(getline(ss, token, SEP)) {
                string bookID = token;
                string issueEpochStr;
                if(!getline(ss, issueEpochStr, SEP)) break;
                long long epoch = 0;
                try { epoch = stoll(issueEpochStr); } catch(...) { epoch = 0; }
                IssuedBook ib; ib.bookID = bookID; ib.issueDate = (time_t)epoch;
                s.issuedBooks.push_back(ib);

                auto bIt = books.find(bookID);
                if(bIt != books.end()) bIt->second.available = false;
            }
            students[id] = s;
        }
        studentFile.close();
    }
}
