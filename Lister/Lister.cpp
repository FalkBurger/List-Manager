#define NOMINMAX

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <cstdint>
#include <windows.h>
#include <lmcons.h>
#include <tchar.h>
#include <stdio.h>
#include <ShlObj.h>
#include <limits>

using namespace std;

bool itExists;
string filePath = "C:/ProgramData/Lister/list.txt", folderPath = "C:/ProgramData/Lister";
int fileLength;
string arrayList[255];

void readFile(string path, string dir)
{
    ifstream readFile(path);
    string outputString;
    fileLength = 0;

    bool exists(readFile);
    itExists = exists;

    if (!itExists)
    {
        filesystem::create_directory(dir);
        cout << "No data has been found. New File has been created to prevent any further problems. If this resulted in any data loss contact the developer for assistance.\n";
    }
    else
    {
        cout << "\nThe List: \n";
        while (!readFile.eof())
        {
            getline(readFile, outputString);
            arrayList[fileLength] = outputString;
            cout << to_string(fileLength + 1) << ". " << outputString << "\n";
            fileLength++;
        }
    }
    
    readFile.close();
}

void createFile(int length, string path)
{
    ofstream createFile(path);
    string sInput;


    for (int i = 0; i < length; i++)
    {
        cout << "\nEnter entry " << i + 1 << "\n";
        getline(cin, sInput);

        /*if (i == 0)
        {
            getline(cin, sInput);
        }*/

        if (i < length - 1)
        {
            createFile << sInput << "\n";
        }
        else
            createFile << sInput;
    }

    createFile.close();
}

int validIntAnswer()
{
    bool validAnswer = false;
    int iInput;
    string sInput;

    while (!validAnswer)
    {
        getline(cin, sInput);

        try
        {
            iInput = stoi(sInput);

            if ((iInput < 1) or (iInput > fileLength))
            {
                cout << "Please enter a valid number less than " << fileLength << "\n";
            }
            else
            {
                validAnswer = true;
                return iInput;
            }
        }

        catch (invalid_argument const& e)
        {
            cout << "Please enter a number.\n";
        }
    }
}

void arrayToFile()
{
    ofstream writeFile(filePath);
    string output;

    for (int i = 0; i < fileLength; i++)
    {
        output = arrayList[i];

        if (i < fileLength - 1)
        {
            writeFile << output << "\n";
        }
        else
            writeFile << output;
    }

    writeFile.close();
}

void moveEntry(int selection)
{
    int target;
    string temp, carry, carry2;

    cout << "Please specify to where you wish to move this entry.\n";
    target = validIntAnswer() - 1;

    temp = arrayList[target];
    arrayList[target] = arrayList[selection];
    arrayList[selection] = temp;

    arrayToFile();
}

void modifyFile(int selection)
{
    int entry;

    switch (selection)
    {
    case 1 :
        cout << "Please specify which entry you wish to move.\n";

        moveEntry(validIntAnswer() - 1);
        //readFile(filePath, folderPath);
        break;

    case 2 :
        break;

    default :
        cout << "Now that shouldn't be possible. Try again.\n";
        break;
    }
}

void checkFirstTimeStartup()
{
    int listLength;
    string sInput;

    if (!itExists)
    {
        bool exitClause = false;

        while (!exitClause)
        {
            cout << "\nHow many items do you want to list?\n";
            getline(cin, sInput);

            try
            {
                listLength = stoi(sInput);

                if (listLength < 1)
                {
                    cout << "Please enter valid number.\n";
                }
                else
                {
                    exitClause = true;
                }
            }

            catch (invalid_argument const& e)
            {
                cout << "Please enter a number.\n";
            }
        }

        createFile(listLength, filePath);
        readFile(filePath, folderPath);
    }
}

void addEntry()
{
    string sInput;
    fstream addEntry(filePath, ios::app);

    cout << "Enter new entry.\n";
    getline(cin, sInput);

    addEntry << "\n" << sInput;

    addEntry.close();
}

void deleteEntry()
{
    int iDelete;
    string temp;
    cout << "Which entry to you wish to delete?\n";
    iDelete = validIntAnswer() - 1;
    
    /*for (int i = fileLength - 1; i > iDelete - 1; i--)
    {
        temp = arrayList[i];
        arrayList[i] = arrayList[i + 1];
    }*/

    for (int i = iDelete; i < fileLength; i++)
    {
        arrayList[i] = arrayList[i + 1];
    }

    fileLength--;

    arrayToFile();
}

int main()
{
    bool loopExit = false;
    string sInput;
    int iInput;
    while (!loopExit)
    {
        readFile(filePath, folderPath);
        checkFirstTimeStartup();
        bool correctEntry = false;

        while (!correctEntry)
        {
            cout << "\nDo you wish to: \n";
            cout << "1. Change the position of an entry?\n";
            cout << "2. Add a new entry?\n";
            cout << "3. Delete an entry?\n";
            cout << "4. Clear the List?\n";
            cout << "5. Exit?\n\n";

            getline(cin, sInput);

            try
            {
                iInput = stoi(sInput);

                switch (iInput)
                {
                case 1:
                    modifyFile(iInput);
                    correctEntry = true;
                    break;
                case 2:
                    addEntry();
                    correctEntry = true;
                    break;
                case 3:
                    deleteEntry();
                    correctEntry = true;
                    break;
                case 4:
                    cout << "Are you sure?(Yes/No)\n";
                    getline(cin, sInput);

                    if (sInput == "Yes")
                    {
                        itExists = false;
                        checkFirstTimeStartup();
                        correctEntry = true;
                    }
                    break;
                case 5:
                    loopExit = true;
                    correctEntry = true;
                    break;
                default:
                    cout << "Please enter a valid number.\n";
                }

            }

            catch (invalid_argument const& e)
            {
                cout << "Please enter a number.\n";
            }
        }
    }
}
