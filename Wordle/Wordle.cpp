// Wordle.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
using namespace std;

//global consts so we avoid slower dynamic arrays
const size_t MAX_COUNT_USERS = 100;
const size_t MAX_USERNAME_LEN = 12;
const size_t MAX_PASS_LEN = 16;


char usernames[MAX_COUNT_USERS][MAX_USERNAME_LEN];
char passwords[MAX_COUNT_USERS][MAX_PASS_LEN];
int gamesPlayed[MAX_COUNT_USERS];
int gamesWon[MAX_COUNT_USERS];
int isAdmin[MAX_COUNT_USERS];
int usersCount = 0;

void loadUsers() {
    ifstream usersFile("users.txt");
    if (!usersFile.is_open()) {
        // file may not exist on first opening
        return;
    }


    while (usersCount < MAX_COUNT_USERS &&
        usersFile >> usernames[usersCount]
        >> passwords[usersCount]
        >> gamesPlayed[usersCount]
        >> gamesWon[usersCount]
        >> isAdmin[usersCount]) {

        usersCount++;
    }

    usersFile.close();
}

void handleSignUp() {
    loadUsers(); 

    if (usersCount >= MAX_COUNT_USERS) {
        cout << "Maximum number of users reached.\n";
        return;
    }

    char newUsername[MAX_USERNAME_LEN];
    char newPassword[MAX_PASS_LEN];

    cout << "Enter username (max " << MAX_USERNAME_LEN - 1 << " symbols): ";
    cin >> newUsername;

    // Is username taken
    for (int i = 0; i < usersCount; i++) {
        if (strcmp(newUsername, usernames[i]) == 0) {
            cout << "Username already exists.\n";
            return;
        }
    }

    cout << "Enter password (max " << MAX_PASS_LEN - 1 << " symbols): ";
    cin >> newPassword;

    // Insert new user in file
    ofstream file("users.txt", ios::app);
    if (!file.is_open()) {
        cout << "Error opening users.txt\n";
        return;
    }

    file << newUsername << " "
        << newPassword << " "
        << 0 << " "   // gamesPlayed
        << 0 << " "   // gamesWon
        << 0 << "\n"; // isAdmin

    file.close();

    cout << "Registration successful!\n";
}

int main()
{
    loadUsers();

    while (true) {
        cout << "Choose an option:" << endl << "1: Login of existing user" << endl << "2: SignUp" << endl << "3: Exit" <<endl;
        int command;
        cin >> command;
        switch (command) {
        case 1:
            cout << "Login";
            //handleLoginLogicWithFunction
            break;
        case 2:
            cout << "SignUp";
            handleSignUp();
            break;
        case 3:
            cout << "Exit. Thanks for playing!";
            return 0;
            break;
        default:
            cout << "Please pick a number from the menu" <<endl;
                break;
        }
    }

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
