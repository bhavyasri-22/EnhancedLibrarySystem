#include "Student.h"
#include <ctime>
Student::Student() : id(""), name("") {}
Student::Student(string _id, string _name) : id(_id), name(_name) {}
void Student::printStudent() const {
    cout << "ID: " << id << " | Name: " << name << "\n";
    if(!issuedBooks.empty()) {
        cout << "Issued Books:\n";
        for(auto &ib : issuedBooks) {
            cout << "  " << ib.bookID << " | Issue date: " << ctime(&ib.issueDate);
        }
    }
}
