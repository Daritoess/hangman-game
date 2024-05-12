/*libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/*colors*/
#define ANSI_COLOR_PINK "\x1b[95m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_GRAY "\x1b[90m"
#define ANSI_COLOR_YELLOW_BRIGHT "\x1b[93m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_UNDERLINE_RED "\x1b[4m\x1b[31m"
#define ANSI_UNDERLINE_GREEN "\x1b[4m\x1b[32m"
#define ANSI_RESET_ALL "\x1b[0m"

/*constants*/
#define MAX_ATTEMPTS 6
#define MAX_LETTERS 20

/*function declarations*/
void menu();
int startGame(char choice);
int checkCharacter(char letter);
int checkWordCharacters(char word[], int length);
char resizeLetter(char letter);
void resizeWord(char word[], int length);
void refresh();
void hangingMan(int remainingAttempts);
char enterLetter();
void initializeHiddenWord(int length, char hiddenWord[]);
void enterWord(char word[]);
int letterAlreadyGuessed(char letter, char guessedLetters[], int length);
int checkLetter(char letter, char word[], int length);
void updateGuessedLetters(char letter, char word[], char guessedLetters[], int length);
int checkWin(char word[], char guessedLetters[]);

/*main*/
int main() {
    char word[MAX_LETTERS]; /*word to be guessed*/
    char guessedLetters[MAX_LETTERS]; /*hidden word*/
    char previouslyGuessedLetters[26]; /*repeated letters*/
    char letter;
    int remainingAttempts = MAX_ATTEMPTS;
    int repeatedWordCount = 0;
    int length, gameChoice;

    printf(ANSI_COLOR_GRAY"Welcome to the Hangman game\n\n"ANSI_COLOR_RESET);
    menu();
    gameChoice = startGame(gameChoice);

    while (gameChoice == 1) {
        refresh();
        do {
            enterWord(word);

            length = strlen(word);
            if (checkWordCharacters(word, length) == 1) {
                refresh();
                printf(ANSI_UNDERLINE_RED"The word contains characters not suitable for the game. Enter a word that contains alphabet letters!\n"ANSI_COLOR_RESET);
            }
        } while (checkWordCharacters(word, length) == 1);

        resizeWord(word, length);
        initializeHiddenWord(length, guessedLetters);
        refresh();

        /*the loop that determines the actual game based on remaining attempts and checking if you haven't guessed the word*/
        while (remainingAttempts > 0 && !checkWin(word, guessedLetters)) {
            do {
                hangingMan(remainingAttempts);
                printf(ANSI_COLOR_YELLOW_BRIGHT"\nWord: %s\n"ANSI_COLOR_RESET, guessedLetters);

                letter = enterLetter();

                if (checkCharacter(letter) == 1) {
                    refresh();
                    printf(ANSI_UNDERLINE_RED"The character entered is not suitable for the game. Enter an alphabet letter!\n"ANSI_RESET_ALL);
                }
            } while (checkCharacter(letter) == 1);

            letter = resizeLetter(letter);

            if (letterAlreadyGuessed(letter, previouslyGuessedLetters, repeatedWordCount)) {
                refresh();
                printf(ANSI_UNDERLINE_RED"You have already entered this letter previously. Enter another letter!\n"ANSI_RESET_ALL);
                continue;
            }

            previouslyGuessedLetters[repeatedWordCount] = letter;
            repeatedWordCount++;

            if (checkLetter(letter, word, length)) {
                refresh();
                printf(ANSI_COLOR_YELLOW_BRIGHT"The letter "ANSI_COLOR_RESET ANSI_UNDERLINE_GREEN"'%c'"ANSI_RESET_ALL ANSI_COLOR_YELLOW_BRIGHT" is present!\n"ANSI_COLOR_RESET, letter);
                updateGuessedLetters(letter, word, guessedLetters, length);
            } else {
                refresh();
                printf(ANSI_COLOR_YELLOW_BRIGHT"The letter "ANSI_COLOR_RESET ANSI_UNDERLINE_RED"'%c'"ANSI_RESET_ALL ANSI_COLOR_YELLOW_BRIGHT" is not present.\n"ANSI_COLOR_RESET, letter);
                remainingAttempts--;
            }
        }
        refresh();

        /*check if win based on the function whether you win or lose also printing how many attempts you made*/
        if (checkWin(word, guessedLetters)) {
            hangingMan(remainingAttempts);
            printf(ANSI_COLOR_YELLOW_BRIGHT"\nCongratulations! You guessed the word: "ANSI_COLOR_RESET ANSI_UNDERLINE_GREEN"%s\n\n"ANSI_RESET_ALL, word);
        } else {
            hangingMan(remainingAttempts);
            printf(ANSI_COLOR_YELLOW_BRIGHT"\nSorry, you lost. The word was: "ANSI_COLOR_RESET ANSI_UNDERLINE_RED"%s\n\n"ANSI_RESET_ALL, word);
        }

        menu();
        gameChoice = startGame(gameChoice);

        if (gameChoice == 1) {
            remainingAttempts = MAX_ATTEMPTS;
            repeatedWordCount = 0;
            memset(word, 0, sizeof(word));
            memset(guessedLetters, 0, sizeof(guessedLetters));
            memset(previouslyGuessedLetters, 0, sizeof(previouslyGuessedLetters));
        }
    }
    return 0;
}

/*function definitions*/

/*this function clears the terminal checking if you have windows or an open source*/
void refresh() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/*this function prints the hangman based on the remaining attempts*/
void hangingMan(int remainingAttempts) {
    switch (remainingAttempts) {
        case 6:
            printf(ANSI_COLOR_YELLOW"   _____\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |     "ANSI_COLOR_RESET ANSI_COLOR_RED"|\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"__|__\n"ANSI_COLOR_RESET);
            break;
        case 5:
            printf(ANSI_COLOR_YELLOW"   _____\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |     "ANSI_COLOR_RESET ANSI_COLOR_RED"|\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |     "ANSI_COLOR_RESET ANSI_COLOR_PINK"O\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"__|__\n"ANSI_COLOR_RESET);
            break;
        case 4:
            printf(ANSI_COLOR_YELLOW"   _____\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |     "ANSI_COLOR_RESET ANSI_COLOR_RED"|\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |     "ANSI_COLOR_RESET ANSI_COLOR_PINK"O\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |     "ANSI_COLOR_RESET ANSI_COLOR_PINK"|\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"__|__\n"ANSI_COLOR_RESET);
            break;
        case 3:
            printf(ANSI_COLOR_YELLOW"   _____\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |     "ANSI_COLOR_RESET ANSI_COLOR_RED"|\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |     "ANSI_COLOR_RESET ANSI_COLOR_PINK"O\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |    "ANSI_COLOR_RESET ANSI_COLOR_PINK"/|\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"__|__\n"ANSI_COLOR_RESET);
            break;
        case 2:
            printf(ANSI_COLOR_YELLOW"   _____\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |     "ANSI_COLOR_RESET ANSI_COLOR_RED"|\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |     "ANSI_COLOR_RESET ANSI_COLOR_PINK"O\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |    "ANSI_COLOR_RESET ANSI_COLOR_PINK"/|\\\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"__|__\n"ANSI_COLOR_RESET);
            break;
        case 1:
            printf(ANSI_COLOR_YELLOW"   _____\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |     "ANSI_COLOR_RESET ANSI_COLOR_RED"|\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |     "ANSI_COLOR_RESET ANSI_COLOR_PINK"O\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |    "ANSI_COLOR_RESET ANSI_COLOR_PINK"/|\\\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |    "ANSI_COLOR_RESET ANSI_COLOR_PINK"/ \n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"__|__\n"ANSI_COLOR_RESET);
            break;
        case 0:
            printf(ANSI_COLOR_YELLOW"   _____\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |     "ANSI_COLOR_RESET ANSI_COLOR_RED"|\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |     "ANSI_COLOR_RESET ANSI_COLOR_PINK"O\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |    "ANSI_COLOR_RESET ANSI_COLOR_PINK"/|\\\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"  |    "ANSI_COLOR_RESET ANSI_COLOR_PINK"/ \\\n"ANSI_COLOR_RESET);
            printf(ANSI_COLOR_YELLOW"__|__\n"ANSI_COLOR_RESET);
            break;
    }
}

/*this function enters a letter*/
char enterLetter() {
    char l;
    printf(ANSI_COLOR_GRAY"Guess a letter: "ANSI_COLOR_RESET);
    scanf(" %c", &l);
    while (getchar() != '\n');  
    return l;
}

/*this function initializes the hidden word that we will see with this character '-'*/
void initializeHiddenWord(int length, char hiddenWord[]) {
    for (int i = 0; i < length; i++) {
        hiddenWord[i] = '-';
    }
    hiddenWord[length] = '\0'; 
}

/*this function enters a word and checks that it does not exceed the maximum characters*/
void enterWord(char word[]) {
    do {
        printf(ANSI_COLOR_GRAY"Enter the word to guess "ANSI_COLOR_RESET ANSI_UNDERLINE_RED"(max %d characters)"ANSI_RESET_ALL ANSI_COLOR_GRAY": "ANSI_COLOR_RESET, MAX_LETTERS);
        scanf("%s", word);
        refresh();
        if (strlen(word) > MAX_LETTERS) {
            printf(ANSI_UNDERLINE_RED"The entered word exceeds the maximum allowed length!\n"ANSI_RESET_ALL);
        }
    } while (strlen(word) > MAX_LETTERS);
}

/*this function checks if the letter has been repeated*/
int letterAlreadyGuessed(char letter, char guessedLetters[], int length) {
    for (int i = 0; i < length; i++) {
        if (guessedLetters[i] == letter) {
            return 1; 
        }
    }
    return 0; 
}

/*this function checks if the letter is present in the word*/
int checkLetter(char letter, char word[], int length) {
    for (int i = 0; i < length; i++) {
        if (word[i] == letter) {
            return 1; 
        }
    }
    return 0; 
}

/*this function updates the vector containing the letters used previously*/
void updateGuessedLetters(char letter, char word[], char guessedLetters[], int length) {
    for (int i = 0; i < length; i++) {
        if (word[i] == letter) {
            guessedLetters[i] = letter;
        }
    }
}

/*this function checks if you have won or lost by comparing the words*/
int checkWin(char word[], char guessedLetters[]) {
    if (strcmp(word, guessedLetters) == 0) {
        return 1; 
    }
    return 0;
}

/*this function resizes the letter from uppercase to lowercase*/
char resizeLetter(char letter) {
    return letter = tolower(letter);
}

/*this function resizes the word from uppercase to lowercase*/
void resizeWord(char word[], int length) {
    for (int i = 0; i < length; i++) {
        word[i] = tolower(word[i]);
    }
}

/*this function checks that characters outside of the uppercase and lowercase alphabet are not entered*/
int checkCharacter(char letter) {
    if ((letter < 65 || letter > 90) && (letter < 97 || letter > 122)) {
        return 1;
    }else{
        return 0;
    }
}

/*this function checks that characters outside of the uppercase and lowercase alphabet of a word are not entered*/
int checkWordCharacters(char word[], int length) {
    for (int i = 0; i < length; i++) {
        if ((word[i] < 65 || word[i] > 90) && (word[i] < 97 || word[i] > 122)) {
            return 1;
        }
    }
    return 0;
}

/*this function starts a game*/
int startGame(char choice) {
    do {
        scanf("%c", &choice);
        while (getchar() != '\n');
        refresh();
        if (choice < 48 || choice > 49) {
            printf(ANSI_UNDERLINE_RED "Invalid choice entered. Enter 1 or 0!\n\n" ANSI_RESET_ALL);
            menu();
        }
    } while (choice < 48 || choice > 49);
    return choice - 48;
}

/*print game menu*/
void menu() {
    printf(ANSI_COLOR_GRAY"[1]"ANSI_COLOR_RESET ANSI_COLOR_YELLOW" Start Game\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GRAY"[0]"ANSI_COLOR_RESET ANSI_COLOR_RED" Exit\n\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GRAY"Enter choice: "ANSI_COLOR_RESET);
}
