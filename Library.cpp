#include "Library.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <set>
#include "colors.h" // Assuming this file provides GREEN, RED, YELLOW, RESET

using namespace std;

const int FINE_PER_DAY = 10;
const char SEP = '|';

Library::Library()
{
    loadFromFile();
}

Library::~Library()
{
    saveToFile();
}

// ------------------------------------------------------------------
// CORE TRANSACTION HELPERS (USED BY UNDO/REDO)
// These helpers are simplified for use by undo/redo and no longer check availability
// or handle waiting lists, as the main issue/return functions will handle that.
// ------------------------------------------------------------------

void Library::performIssueWithoutRecord(const string &bookID, const string &studentID)
{
    auto bIt = books.find(bookID);
    auto sIt = students.find(studentID);
    if (bIt == books.end() || sIt == students.end())
        return;

    // CRITICAL CHANGE: Increment the issued count
    bIt->second.issuedCopies++;

    // Add the issue record to the student (using current time, which is imperfect for undo/redo)
    IssuedBook ib;
    ib.bookID = bookID;
    ib.issueDate = time(nullptr);
    sIt->second.issuedBooks.push_back(ib);
}

void Library::performReturnWithoutRecord(const string &bookID, const string &studentID)
{
    auto bIt = books.find(bookID);
    auto sIt = students.find(studentID);
    if (bIt == books.end() || sIt == students.end())
        return;

    // CRITICAL CHANGE: Decrement the issued count
    if (bIt->second.issuedCopies > 0)
    {
        bIt->second.issuedCopies--;
    }

    // Remove the issue record from the student
    auto &issued = sIt->second.issuedBooks;
    for (auto it = issued.begin(); it != issued.end(); ++it)
    {
        if (it->bookID == bookID)
        {
            issued.erase(it);
            break;
        }
    }
}

// ------------------------------------------------------------------
// BOOK MANAGEMENT
// ------------------------------------------------------------------

void Library::addBook(const Book &b)
{
    auto it = books.find(b.id);

    if (it != books.end())
    {
        // Book ID already exists: Increment copy count
        it->second.totalCopies++;
        // NOTE: We skip complex undo for copy increments to simplify.
        cout << GREEN << "Copy of book \"" << it->second.title << "\" added successfully. Total copies: " << it->second.totalCopies << RESET << endl;
    }
    else
    {
        // Brand new book ID/Title: Add it to the map and Trie
        books[b.id] = b;
        books[b.id].totalCopies = 1;
        books[b.id].issuedCopies = 0;
        bookTrie.insert(b.title);

        // Record the operation (only for new titles)
        Operation op;
        op.type = ADD_BOOK;
        op.bookBackup = books[b.id];
        undoStack.push(op);
        while (!redoStack.empty())
            redoStack.pop();

        cout << GREEN << "New book title added successfully. Total copies: 1." << RESET << "\n";
    }
}

void Library::removeBook(const string &bookID)
{
    auto it = books.find(bookID);
    if (it != books.end())
    {
        // You may want to prevent removal if issuedCopies > 0, but for now we proceed:
        if (it->second.issuedCopies > 0)
        {
            cout << YELLOW << "Warning: " << it->second.issuedCopies << " copies are currently issued. Removing anyway." << RESET << "\n";
        }

        Operation op;
        op.type = REMOVE_BOOK;
        op.bookBackup = it->second;
        undoStack.push(op);

        bookTrie.remove(it->second.title);
        books.erase(it);

        while (!redoStack.empty())
            redoStack.pop();
        cout << "Book removed.\n";
    }
    else
    {
        cout << "Book not found.\n";
    }
}

void Library::searchBookByTitle(const string &title)
{
    bool found = false;

    // convert search input to lowercase
    string searchKey = title;
    for (char &c : searchKey)
        c = tolower(c);

    for (const auto &p : books)
    {
        string bookTitle = p.second.title;

        // convert book title to lowercase
        string lowerTitle = bookTitle;
        for (char &c : lowerTitle)
            c = tolower(c);

        // check case-insensitive substring match
        if (lowerTitle.find(searchKey) != string::npos)
        {
            p.second.printBook();
            found = true;
        }
    }

    if (!found)
        cout << "No books found.\n";
}

void Library::searchBookByID(const string &bookID)
{
    auto it = books.find(bookID);
    if (it != books.end())
        it->second.printBook();
    else
        cout << "Book ID not found!\n";
}

// ------------------------------------------------------------------
// STUDENT MANAGEMENT
// ------------------------------------------------------------------

void Library::addStudent(const Student &s)
{
    students[s.id] = s;
    cout << "Student registered.\n";
    while (!redoStack.empty())
        redoStack.pop();
}

// ------------------------------------------------------------------
// TRANSACTIONS
// ------------------------------------------------------------------

void Library::issueBook(const string &bookID, const string &studentID)
{
    auto sIt = students.find(studentID);
    if (sIt == students.end())
    {
        cout << RED << "Student not found." << RESET << "\n";
        return;
    }
    auto bIt = books.find(bookID);
    if (bIt == books.end())
    {
        cout << RED << "Book not found." << RESET << "\n";
        return;
    }

    int availableCopies = bIt->second.totalCopies - bIt->second.issuedCopies;

    // CRITICAL CHANGE: Check availability using the copy counts
    if (availableCopies <= 0)
    {
        waitingList.push({bookID, studentID});
        cout << YELLOW << "Book unavailable. All " << bIt->second.totalCopies << " copies are issued. Added to waiting list." << RESET << "\n";
        return;
    }

    // Check if student already has this book (good practice)
    for (const auto &ib : sIt->second.issuedBooks)
    {
        if (ib.bookID == bookID)
        {
            cout << YELLOW << "Warning: Student already has a copy of this book. Cannot issue another." << RESET << "\n";
            return;
        }
    }

    // 1. Update the Book's counter
    bIt->second.issuedCopies++;

    // 2. Add the record to the Student
    IssuedBook ib;
    ib.bookID = bookID;
    ib.issueDate = time(nullptr);
    sIt->second.issuedBooks.push_back(ib);

    // 3. Record for Undo/Redo
    Operation op;
    op.type = ISSUE_BOOK;
    op.bookID = bookID;
    op.studentID = studentID;
    undoStack.push(op);
    while (!redoStack.empty())
        redoStack.pop();

    cout << GREEN << "Book issued successfully. Copies remaining: " << (bIt->second.totalCopies - bIt->second.issuedCopies) << RESET << "\n";
}

void Library::returnBook(const string &bookID, const string &studentID)
{
    // Check if student exists
    auto sIt = students.find(studentID);
    if (sIt == students.end())
    {
        cout << RED << "Student not found." << RESET << "\n";
        return;
    }
    // Check if book exists
    auto bIt = books.find(bookID);
    if (bIt == books.end())
    {
        cout << RED << "Book not found." << RESET << "\n";
        return;
    }

    // Check if the book was issued to this student (and find issue time)
    auto &issued = sIt->second.issuedBooks;
    bool found = false;
    time_t issueTime = 0;
    for (auto it = issued.begin(); it != issued.end(); ++it)
    {
        if (it->bookID == bookID)
        {
            issueTime = it->issueDate;
            issued.erase(it); // Remove the book from student's list
            found = true;
            break;
        }
    }
    if (!found)
    {
        cout << YELLOW << "Warning: This book was not issued to this student." << RESET << "\n";
        return;
    }

    // CRITICAL CHANGE: Decrement Issued Counter
    if (bIt->second.issuedCopies > 0)
    {
        bIt->second.issuedCopies--;
    }
    else
    {
        cout << RED << "System Error: Book count was inconsistent (issuedCopies already 0)." << RESET << "\n";
    }

    // Calculate overdue fine (logic remains the same)
    time_t now = time(nullptr);
    double days = difftime(now, issueTime) / (60 * 60 * 24);
    int fine = 0;
    if (days > 2.0)
    {
        fine = static_cast<int>((days - 2.0) * FINE_PER_DAY);
        cout << RED << "Overdue! Fine Rs " << fine << RESET << "\n";
    }
    else
    {
        cout << "Book returned on time.\n";
    }

    // Log operation for undo/redo
    Operation op;
    op.type = RETURN_BOOK;
    op.bookID = bookID;
    op.studentID = studentID;
    undoStack.push(op);
    while (!redoStack.empty())
        redoStack.pop();

    cout << GREEN << "Book '" << bIt->second.title << "' returned by student: " << studentID << RESET << "\n";

    // Serve waiting list (FIFO) for this book
    bool reissued = false;
    queue<pair<string, string>> temp;

    // CRITICAL CHANGE: The book is now available if totalCopies > issuedCopies
    if (bIt->second.totalCopies > bIt->second.issuedCopies)
    {
        while (!waitingList.empty())
        {
            auto w = waitingList.front();
            waitingList.pop();
            if (!reissued && w.first == bookID)
            {
                // Reissue to waiting student (call the main function, which will increment issuedCopies)
                issueBook(w.first, w.second);
                reissued = true;
            }
            else
            {
                temp.push(w);
            }
        }
    }
    waitingList = std::move(temp);

    if (!reissued)
    {
        cout << "No pending requests for this book served.\n";
    }
}

vector<string> Library::autocompleteTitle(const string &prefix)
{
    return bookTrie.searchPrefix(prefix, 15);
}

// ------------------------------------------------------------------
// UNDO/REDO
// ------------------------------------------------------------------

void Library::undoLastOperation()
{
    if (undoStack.empty())
    {
        cout << "Nothing to undo.\n";
        return;
    }
    Operation op = undoStack.top();
    undoStack.pop();

    switch (op.type)
    {
    case ADD_BOOK:
    {
        auto it = books.find(op.bookBackup.id);
        if (it != books.end() && it->second.totalCopies == 1 && it->second.issuedCopies == 0)
        {
            // Only undo if it was a new title (totalCopies=1)
            bookTrie.remove(it->second.title);
            books.erase(it);
        }
        else if (it != books.end() && it->second.totalCopies > 1)
        {
            // If it was an incremented copy, we decrement the count instead of full removal
            it->second.totalCopies--;
        }
        break;
    }
    case REMOVE_BOOK:
    {
        books[op.bookBackup.id] = op.bookBackup;
        bookTrie.insert(op.bookBackup.title);
        break;
    }
    case ISSUE_BOOK:
    {
        // Revert the issue
        performReturnWithoutRecord(op.bookID, op.studentID);
        break;
    }
    case RETURN_BOOK:
    {
        // Revert the return
        performIssueWithoutRecord(op.bookID, op.studentID);
        break;
    }
    }

    redoStack.push(op);
    cout << "Undo performed.\n";
}

void Library::redoLastOperation()
{
    if (redoStack.empty())
    {
        cout << "Nothing to redo.\n";
        return;
    }
    Operation op = redoStack.top();
    redoStack.pop();

    switch (op.type)
    {
    case ADD_BOOK:
        addBook(op.bookBackup);
        break;
    case REMOVE_BOOK:
        removeBook(op.bookBackup.id);
        break;
    // NOTE: Redo for issue/return will not re-check availability or waiting list,
    // it just reverses the effect of the undo
    case ISSUE_BOOK:
        performIssueWithoutRecord(op.bookID, op.studentID);
        break;
    case RETURN_BOOK:
        performReturnWithoutRecord(op.bookID, op.studentID);
        break;
    }

    // The original code uses issueBook/returnBook which leads to recursive calls and re-checking
    // of availability and undo/redo stack pushes. The corrected code uses the perform*WithoutRecord
    // to apply the state change directly. I will keep the original implementation style for now
    // but note that the original code for redo was recursive. The code provided here follows the original style.
    // The original logic here was flawed in its use of the main functions which re-push to undoStack.
    // Keeping the original structure for the user:
    // case ISSUE_BOOK: issueBook(op.bookID, op.studentID); break;
    // case RETURN_BOOK: returnBook(op.bookID, op.studentID); break;

    // Re-do the state change using the core function for simplicity, accepting the re-push to the undo stack
    // (a clean implementation would use the perform*WithoutRecord helpers here too)
    // case ISSUE_BOOK: issueBook(op.bookID, op.studentID); break;
    // case RETURN_BOOK: returnBook(op.bookID, op.studentID); break;

    cout << "Redo performed.\n";
}

// ------------------------------------------------------------------
// DISPLAY / UTILITY
// ------------------------------------------------------------------

void Library::displayAllBooks()
{
    if (books.empty())
    {
        cout << "No books available.\n";
        return;
    }
    for (const auto &p : books)
        p.second.printBook();
}

void Library::displayIssuedBooks()
{
    bool any = false;
    for (const auto &s : students)
    {
        if (!s.second.issuedBooks.empty())
        {
            s.second.printStudent();
            any = true;
        }
    }
    if (!any)
        cout << "No issued books currently.\n";
}

void Library::displayBooksByGenre(const string &genre)
{
    bool found = false;
    for (const auto &p : books)
    {
        if (p.second.genre == genre)
        {
            p.second.printBook();
            found = true;
        }
    }
    if (!found)
        cout << "No books found in this genre!\n";
}

vector<string> Library::availableGenres() const
{
    set<string> genres;
    for (const auto &p : books)
    {
        if (!p.second.genre.empty())
            genres.insert(p.second.genre);
    }
    return vector<string>(genres.begin(), genres.end());
}

// ------------------------------------------------------------------
// FILE PERSISTENCE (SAVE / LOAD)
// ------------------------------------------------------------------

void Library::saveToFile()
{
    // --- Book Saving (NEW FORMAT) ---
    ofstream bookFile("books.txt");
    if (!bookFile)
    {
        cerr << "Failed to open books.txt for writing\n";
        return;
    }
    for (const auto &p : books)
    {
        const Book &b = p.second;
        bookFile << b.id << SEP
                 << b.title << SEP
                 << b.author << SEP
                 << b.genre << SEP
                 // CRITICAL CHANGE: Save total copies and issued copies
                 << b.totalCopies << SEP
                 << b.issuedCopies << "\n";
    }
    bookFile.close();

    // --- Student Saving (Same Format) ---
    ofstream studentFile("students.txt");
    if (!studentFile)
    {
        cerr << "Failed to open students.txt for writing\n";
        return;
    }

    for (const auto &p : students)
    {
        const Student &s = p.second;
        studentFile << s.id << SEP << s.name;
        for (const auto &ib : s.issuedBooks)
        {
            studentFile << SEP << ib.bookID << SEP << ib.issueDate;
        }
        studentFile << "\n";
    }
    studentFile.close();
}

void Library::loadFromFile()
{
    books.clear();
    students.clear();
    bookTrie.clear();

    // --- Book Loading (NEW FORMAT) ---
    ifstream bookFile("books.txt");
    if (bookFile)
    {
        string line;
        while (getline(bookFile, line))
        {
            if (line.empty())
                continue;
            stringstream ss(line);
            // Changed 'avail' to 'totalStr' and 'issuedStr'
            string id, title, author, genre, totalStr, issuedStr;

            if (!getline(ss, id, SEP))
                continue;
            if (!getline(ss, title, SEP))
                title = "";
            if (!getline(ss, author, SEP))
                author = "";
            if (!getline(ss, genre, SEP))
                genre = "";

            // CRITICAL CHANGE: Read Total and Issued Counts
            if (!getline(ss, totalStr, SEP))
                totalStr = "1";
            if (!getline(ss, issuedStr, SEP))
                issuedStr = "0";

            int total = 1;
            try
            {
                total = stoi(totalStr);
            }
            catch (...)
            {
                total = 1;
            }
            int issued = 0;
            try
            {
                issued = stoi(issuedStr);
            }
            catch (...)
            {
                issued = 0;
            }

            // Use the new multi-copy constructor
            books[id] = Book(id, title, author, genre, total, issued);
            bookTrie.insert(title);
        }
        bookFile.close();
    }

    // --- Student Loading (Fix to reconcile issued book list with new Book class) ---
    ifstream studentFile("students.txt");
    if (studentFile)
    {
        string line;
        while (getline(studentFile, line))
        {
            if (line.empty())
                continue;
            stringstream ss(line);
            string id, name;
            if (!getline(ss, id, SEP))
                continue;
            if (!getline(ss, name, SEP))
                name = "";
            Student s(id, name);

            string token;
            while (getline(ss, token, SEP))
            {
                string bookID = token;
                string issueEpochStr;
                if (!getline(ss, issueEpochStr, SEP))
                    break;
                long long epoch = 0;
                try
                {
                    epoch = stoll(issueEpochStr);
                }
                catch (...)
                {
                    epoch = 0;
                }
                IssuedBook ib;
                ib.bookID = bookID;
                ib.issueDate = (time_t)epoch;
                s.issuedBooks.push_back(ib);

                // FIX: The old code incorrectly set bIt->second.available = false.
                // We now increment issuedCopies here to ensure consistency.
                auto bIt = books.find(bookID);
                if (bIt != books.end())
                {
                    // Check to prevent over-incrementing if the book count was incorrectly saved
                    if (bIt->second.issuedCopies < bIt->second.totalCopies)
                    {
                        bIt->second.issuedCopies++;
                    }
                }
            }
            students[id] = s;
        }
        studentFile.close();
    }
}