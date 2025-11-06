#include "Student.h"
#include <iostream>
#include <ctime>

Student::Student() : id(""), name("") {}
Student::Student(const string& _id, const string& _name) : id(_id), name(_name) {}

void Student::printStudent() const {
    cout << "ID: " << id << " | Name: " << name << "\n";
    if(!issuedBooks.empty()) {
        cout << "Issued Books:\n";
        for(const auto &ib : issuedBooks) {
            cout << "  " << ib.bookID << " | Issue date: " << ctime(&ib.issueDate);
        }
    }
}
