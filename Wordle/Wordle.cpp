// Wordle.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

//global consts so we avoid slower dynamic arrays

//userConsts
const size_t MAX_COUNT_USERS = 100;
const size_t MAX_USERNAME_LEN = 12;
const size_t MAX_PASS_LEN = 16;

//wordConsts
const size_t MAX_WORDS = 1000;       
const size_t WORD_LEN = 5;

//gameConsts
const int MAX_ATTEMPTS = 6;

//usersList
char usernames[MAX_COUNT_USERS][MAX_USERNAME_LEN];
char passwords[MAX_COUNT_USERS][MAX_PASS_LEN];
int gamesPlayed[MAX_COUNT_USERS];
int gamesWon[MAX_COUNT_USERS];
int isAdmin[MAX_COUNT_USERS];
int usersCount = 0;

//currentUser
int currentUserIndex = -1;    
char currentUsername[MAX_USERNAME_LEN];
int currentGamesPlayed;
int currentGamesWon;
int currentIsAdmin;

//wordsList
char words[MAX_WORDS][WORD_LEN + 1]; // for '\0'
int wordCount = 0;





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
void loadWords() {
    ifstream file("words.txt");
    wordCount = 0;

    while (wordCount < MAX_WORDS && file >> words[wordCount]) {
        // necessary if we want to change word_len or somehow expand the code with difficulty
        if (strlen(words[wordCount]) == WORD_LEN) {
            wordCount++;
        }
    }

    file.close();
}

void pickRandomWord(char selectedWord[]) {
    srand(67);
    int index = rand() % wordCount;
    strcpy_s(selectedWord, WORD_LEN + 1, words[index]);
}

bool isValidWord(const char input[]) {
    if (strlen(input) != WORD_LEN) return false;

    for (int i = 0; i < WORD_LEN; i++) {
        if (input[i] < 'a' || input[i] > 'z') return false;
    }
    return true;
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

void handleLogin() {
    loadUsers(); 

    char username[MAX_USERNAME_LEN];
    char password[MAX_PASS_LEN];

    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    
    for (int i = 0; i < usersCount; i++) {
        if (strcmp(username, usernames[i]) == 0 &&
            strcmp(password, passwords[i]) == 0) {

            cout << "Login successful!\n";

            //Update currentUser
            currentUserIndex = i;
            strcpy_s(currentUsername, usernames[i]);
            currentGamesPlayed = gamesPlayed[i];
            currentGamesWon = gamesWon[i];
            currentIsAdmin = isAdmin[i];

            return;
        }
    }

    cout << "Login failed: wrong username or password.\n";
}

bool isInRightPlace(char guessChar, const char word[], int index) {
    return guessChar == word[index];
}

bool isInWord(char guessChar, const char word[]) {
    for (int i = 0; i < WORD_LEN; i++) {
        if (guessChar == word[i]) return true;
    }
    return false;
}

void printResult(const char guess[], const char word[]) {
    for (int i = 0; i < WORD_LEN; i++) {
        if (isInRightPlace(guess[i], word, i)) {
            cout << "\033[32m" << guess[i] << "\033[0m";  // GREEN
        }
        else if (isInWord(guess[i], word)) {
            cout << "\033[33m" << guess[i] << "\033[0m";  // YELLOW
        }
        else {
            cout << "\033[37m" << guess[i] << "\033[0m";  // WHITE
        }
    }
    cout << endl;
}


void playWordle() {
    loadWords();

    char word[WORD_LEN + 1];
    pickRandomWord(word);

    cout << "The word is:" << word << "FOR TESTING\n";
    char guess[WORD_LEN + 1];

    int attempts = 1;
    bool won = false;

    while (attempts <= MAX_ATTEMPTS) {
        cout << "Attempt " << (attempts) << "/" << MAX_ATTEMPTS << ": ";
        cin >> guess;

        if (!isValidWord(guess)) {
            cout << "Invalid word. Use " << WORD_LEN << " lowercase letters." << endl;
            continue;
        }

        printResult(guess, word);

        if (strcmp(guess, word) == 0) {
            cout << "Congratulations! You guessed the word.\n";
            won = true;
            break;
        }

        attempts++;
    }

    if (!won) {
        cout << "You lost! The word was: " << word << endl;
    }
}
int main()
{
    loadUsers();
    //could also add isLoggedIn for readability
    while (currentUserIndex == -1) {
        cout << "Choose an option:" << endl << "1: Login of existing user" << endl << "2: SignUp" << endl << "3: Exit" <<endl;
        int command;
        cin >> command;
        switch (command) {
        case 1:
            cout << "Login";
            handleLogin();
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
    cout << "Lets begin the game Mr. " << currentUsername << "!";
    playWordle();
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
