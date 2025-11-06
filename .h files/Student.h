#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

struct IssuedBook {
    string bookID;
    time_t issueDate;
};

class Student {
public:
    string id;
    string name;
    vector<IssuedBook> issuedBooks;

    Student();
    Student(const string& _id, const string& _name);
    void printStudent() const;
};

#endif // STUDENT_H