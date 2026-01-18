/**
*
* Solution to course project # 9
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2025/2026
*
* @author Iliyan Georgiev Popov
* @idnumber 8MI0600652
* @compiler VC
*
* The wordle game 
* https://github.com/notEven1day/pb-su-wordle-project.git
*  
*/


#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

/**
 just so you know I can write the custom  
    strlen
    srtcpy_s
    strcmp
functions I just chose not to use them for the sake of maintainability 

size_t strLen(const char* arr) {
    size_t len = 0;
    for (size_t i = 0; arr[i]!='\0'; i++)
    {
        len += 1;
    }
    return len;
}

size_t strLen2(const char* arr) {
    size_t len = 0;
    while (arr[len] != '\0')
    {
        len++;
    }
    return len;
}

void srtcpy_s(char* destination, size_t size, const char* source) {
    if (!destination || !source || size == 0)
        return;

    size_t i;
    for (i = 0; i < size - 1 && source[i] != '\0'; i++) {
        destination[i] = source[i];
    }

    destination[i] = '\0';
}

bool strCmp(const char* arr1, const char* arr2) {
    size_t i = 0;

    while (arr1[i] != '\0' && arr2[i] != '\0') {
        if (arr1[i] != arr2[i]) {
            return false;
        }
        i++;
    }
    return arr1[i] == arr2[i];

}

*/


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
    usersCount = 0;
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
    //TODO: I dont know if i am allowed to use c.time for true random and I am too lazy to ask
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

    //TODO: Somehow validate
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

void saveUsers() {
    ofstream file("users.txt");

    //TODO: Find how to save user without wiping the whole fucking file
    if (!file.is_open()) {
        cout << "Error opening users.txt for writing\n";
        return;
    }

    for (int i = 0; i < usersCount; i++) {
        file << usernames[i] << " "
            << passwords[i] << " "
            << gamesPlayed[i] << " "
            << gamesWon[i] << " "
            << isAdmin[i] << endl;
    }

    file.close();
}

void updateUser(bool wonGame) {
    if (currentUserIndex == -1) {
        return;
    }

    gamesPlayed[currentUserIndex]++;

    if (wonGame) {
        gamesWon[currentUserIndex]++;
    }
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
        cout << "Attempt " << (attempts) << "/" << MAX_ATTEMPTS << ": " << endl;

        cin >> guess;
        
        //TODO: Bug found here. Stack overflows?
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
    updateUser(won);
    saveUsers();
}
void handleLogOut() {
   
     currentUserIndex = -1;
     currentUsername[MAX_USERNAME_LEN] = '!';
     currentGamesPlayed = -1;
     currentGamesWon = -1;
     currentIsAdmin = -1;
     cout << "Logged out successfully.\n" <<endl;
}
bool isWordInList(const char word[]) {
    loadWords();
    for (int i = 0; i < wordCount; i++) {
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;

}
void addWord() {
    char newWord[WORD_LEN + 1];

    cout << "Enter new word: ";
    cin >> newWord;

    if (!isValidWord(newWord)) {
        cout << "Invalid word. Use " << WORD_LEN << " lowercase letters." << endl;
    }
    else if (isWordInList(newWord)) {
        cout << "Word already added" << endl;
    }
    else {
        ofstream file("words.txt", ios::app);
        file << newWord << endl;
        file.close();
        cout << "Word added successfully.\n";
    }
    
   /* if (isValidWord(newWord) && isWordInList(newWord))
    {
        ofstream file("words.txt", ios::app);
        file << newWord << endl;
        file.close();

        cout << "Word added successfully.\n";
    }
    else
    {
        cout << "Invalid word. Use " << WORD_LEN << " lowercase letters." << endl;
    }*/
}
void sortByGamesPlayed(int order[]) {
    for (int i = 0; i < usersCount - 1; i++) {
        for (int j = 0; j < usersCount - i - 1; j++) {
            if (gamesPlayed[order[j]] < gamesPlayed[order[j + 1]]) {
                int temp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = temp;
            }
        }
    }
}

double winRate(int wins, int played) {
    if (played == 0) return 0.0;
    return (double)wins / played;
}

void sortByWinRate(int order[]) {
    for (int i = 0; i < usersCount - 1; i++) {
        for (int j = 0; j < usersCount - i - 1; j++) {
            if (winRate(gamesWon[order[j]], gamesPlayed[order[j]]) <
                winRate(gamesWon[order[j + 1]], gamesPlayed[order[j + 1]])) {

                int temp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = temp;
            }
        }
    }
}

void printLeaderboard(int order[]) {
    cout << "\n=== Leaderboard ===\n";
    cout << "Username | Played | Won | Winrate\n";
    for (int i = 0; i < usersCount; i++) {
        int idx = order[i];

        cout << usernames[idx] << " | "
            << gamesPlayed[idx] << " | "
            << gamesWon[idx] << " | "
            << winRate(gamesWon[idx], gamesPlayed[idx])
            << endl;
    }
}

void showLeaderboard() {
    loadUsers();
    cout << "\nSort leaderboard by:\n" << "1: Games played\n" << "2: Winrate\n";
    int choice;
    cin >> choice;
    int order[MAX_COUNT_USERS];
    for (int i = 0; i < usersCount; i++) {
        order[i] = i;
    }
    if (choice == 1) {
        sortByGamesPlayed(order);
    }
    else if (choice == 2) {
        sortByWinRate(order);
    }
    else {
        cout << "Invalid choice.\n";
        return;
    }

    printLeaderboard(order);


}
int main()
{
    loadUsers();
    while (true) {
        //could also add isLoggedIn for readability
        while (currentUserIndex == -1) {
            cout << "Choose an option:" << endl << "1: Login of existing user" << endl << "2: SignUp" << endl << "3: Exit" << endl <<endl;
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
                cout << "Please pick a number from the menu" << endl;
                break;
            }
        }

        while (currentUserIndex != -1) {
            cout << "\nLogged as: " << currentUsername << endl;

            cout << "1: Start game\n";
            cout << "2: Show leaderboard\n";
            cout << "3: Logout\n";

            if (currentIsAdmin) {
                cout << "4: Add word\n";
            }

            int command;
            cin >> command;

            if (command == 1) {
                cout << "Lets begin the game Mr. " << currentUsername << "!";
                playWordle();
            }
            else if (command == 2) {
                showLeaderboard();
            }
            else if (command == 3) {
                handleLogOut();
            }
            else if (currentIsAdmin && command == 4) {
                // TODO: adding word longer than 5 letters pops up a vs error
                addWord();
            }
            else {
                cout << "Invalid option.\n";
            }
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
