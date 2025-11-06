#!/bin/bash

echo "🔨 Compiling Library Management System..."

# Check if source files exist in subdirectories
if [ -d ".cpp files" ] && [ -d ".h files" ]; then
    echo "📁 Found subdirectories - compiling from .cpp files/ and .h files/"
    
    g++ -std=c++11 -o library_system \
        ".cpp files/main.cpp" \
        ".cpp files/Library.cpp" \
        ".cpp files/Book.cpp" \
        ".cpp files/Student.cpp" \
        ".cpp files/Trie.cpp" \
        ".cpp files/User.cpp" \
        ".cpp files/AuthSystem.cpp" \
        ".cpp files/LibrarianInterface.cpp" \
        ".cpp files/StudentInterface.cpp" \
        -I ".h files"

elif [ -f "main.cpp" ]; then
    echo "📁 Compiling from current directory"
    g++ -std=c++11 -o library_system \
        main.cpp \
        Library.cpp \
        Book.cpp \
        Student.cpp \
        Trie.cpp \
        User.cpp \
        AuthSystem.cpp \
        LibrarianInterface.cpp \
        StudentInterface.cpp

else
    echo "❌ No source files found!"
    echo "Please make sure your .cpp files are in the current directory or in 'cpp files' subdirectory"
    exit 1
fi

if [ $? -eq 0 ]; then
    echo "✅ Compilation successful!"
    echo "🚀 Run with: ./library_system"
else
    echo "❌ Compilation failed. Please check errors above."
    exit 1
fi