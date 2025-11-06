#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>   // For rand and srand
#include <ctime>     // For time
#include <algorithm> // For std::max

using namespace std;

// The separator used in books.txt
const char SEP = '|';
const string FILENAME = "books.txt";

void initializeRandomCounts()
{
    ifstream inputFile(FILENAME);
    if (!inputFile.is_open())
    {
        cerr << "Error: Could not open " << FILENAME << " for reading. Make sure it exists in the current directory." << endl;
        return;
    }

    // Use a temporary file to store the new data safely
    ofstream tempFile("books_temp.txt");
    if (!tempFile.is_open())
    {
        cerr << "Error: Could not create temporary file books_temp.txt." << endl;
        return;
    }

    // Seed the random number generator using the current time
    srand(static_cast<unsigned int>(time(0)));

    string line;
    while (getline(inputFile, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string id, title, author, genre, old_flag_or_total;

        // 1. Read the fixed fields (ID, Title, Author, Genre)
        if (!getline(ss, id, SEP) || !getline(ss, title, SEP) ||
            !getline(ss, author, SEP) || !getline(ss, genre, SEP))
        {
            // Skip or write malformed lines as is
            tempFile << line << "\n";
            continue;
        }

        // 2. Determine Issued Copies and handle the old format
        int issuedCopies = 0;
        if (getline(ss, old_flag_or_total, SEP))
        {
            // If the old availability flag was '0', it means one copy was issued.
            if (old_flag_or_total == "0")
            {
                issuedCopies = 1;
            }
        }

        // 3. Generate Random Total Copies (1 to 4)
        // The minimum total copies must be at least the number of issued copies.
        int min_total = max(issuedCopies, 1);

        // Generate a random number in the range [min_total, 4]
        // rand() % (max - min + 1) + min
        int max_range = 4;
        int range = max_range - min_total + 1;
        int totalCopies = rand() % range + min_total;

        // 4. Write the New Line (ID|Title|Author|Genre|TotalCopies|IssuedCopies)
        tempFile << id << SEP
                 << title << SEP
                 << author << SEP
                 << genre << SEP
                 << totalCopies << SEP
                 << issuedCopies << "\n";
    }

    inputFile.close();
    tempFile.close();

    // 5. Replace the original file with the temporary file
    if (remove(FILENAME.c_str()) != 0)
    {
        cerr << "Warning: Could not delete the old " << FILENAME << ". Please manually rename books_temp.txt to books.txt." << endl;
    }
    else if (rename("books_temp.txt", FILENAME.c_str()) != 0)
    {
        cerr << "Warning: Could not rename books_temp.txt to " << FILENAME << ". Please manually rename it." << endl;
    }
    else
    {
        cout << "SUCCESS: " << FILENAME << " has been successfully updated with random copy counts (1-4)." << endl;
    }
}

int main()
{
    initializeRandomCounts();
    return 0;
}