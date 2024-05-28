/*libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
void information();
int choice(char choice); 
int checkCharacter(char letter);
int checkWordCharacters(char word[], int length);
char resizeLetter(char letter);
void resizeWord(char word[], int length);
void refresh();
void drawHangman(int remainingAttempts);
char insertLetter();
void initializeHiddenWord(int length, char hiddenWord[]);
void insertWord(char word[]);
int isLetterAlreadyGuessed(char letter, char guessedLetters[], int length);
int checkLetter(char letter, char word[], int length);
void updateGuessedLetters(char letter, char word[], char guessedLetters[], int length);
int checkVictory(char word[], char guessedLetters[]);

/*main*/
int main() {
    char word[MAX_LETTERS];/*word to guess*/
    char guessedLetters[MAX_LETTERS];/*hidden word*/
    char previouslyGuessedLetters[26];/*repeated letters*/
    char letter;
    int remainingAttempts;
    int repeatLetterCount;
    int length, gameChoice;

    printf(ANSI_COLOR_GRAY "==Welcome to Hangman==\n==Windows-Linux==\n" ANSI_COLOR_RESET "\n\n");
    menu();
    gameChoice = choice(gameChoice);

    while (gameChoice > 0) {
        if (gameChoice == 2) {
            refresh();
            information();
            menu();
            gameChoice = choice(gameChoice);
        }

        while (gameChoice == 1) {
            refresh();
            remainingAttempts = MAX_ATTEMPTS;
            repeatLetterCount = 0;
            do {
                insertWord(word);

                length = strlen(word);
                if (checkWordCharacters(word, length) == 1) {
                    refresh();
                    printf(ANSI_UNDERLINE_RED "The word contains invalid characters. Please enter a word that contains alphabetic letters only!" ANSI_COLOR_RESET "\n");
                }
            } while (checkWordCharacters(word, length) == 1);

            resizeWord(word, length);
            initializeHiddenWord(length, guessedLetters);
            refresh();

            /*the main game loop based on remaining attempts and checking if the word is not guessed yet*/
            while (remainingAttempts > 0 && !checkVictory(word, guessedLetters)) {
                do {
                    drawHangman(remainingAttempts);
                    printf("\n" ANSI_COLOR_YELLOW_BRIGHT "Word: %s" ANSI_COLOR_RESET "\n", guessedLetters);

                    letter = insertLetter();

                    if (checkCharacter(letter) == 1) {
                        refresh();
                        printf(ANSI_UNDERLINE_RED "The entered character is not valid. Please enter an alphabetic letter!" ANSI_RESET_ALL "\n");
                    }
                } while (checkCharacter(letter) == 1);

                letter = resizeLetter(letter);

                if (isLetterAlreadyGuessed(letter, previouslyGuessedLetters, repeatLetterCount)) {
                    refresh();
                    printf(ANSI_UNDERLINE_RED "You have already guessed this letter. Please enter another letter!" ANSI_RESET_ALL "\n");
                    continue;
                }

                previouslyGuessedLetters[repeatLetterCount] = letter;
                repeatLetterCount++;

                if (checkLetter(letter, word, length)) {
                    refresh();
                    printf(ANSI_COLOR_YELLOW_BRIGHT "The letter " ANSI_COLOR_RESET ANSI_UNDERLINE_GREEN "'%c'" ANSI_RESET_ALL ANSI_COLOR_YELLOW_BRIGHT " is in the word!" ANSI_COLOR_RESET "\n", letter);
                    updateGuessedLetters(letter, word, guessedLetters, length);
                } else {
                    refresh();
                    printf(ANSI_COLOR_YELLOW_BRIGHT "The letter " ANSI_COLOR_RESET ANSI_UNDERLINE_RED "'%c'" ANSI_RESET_ALL ANSI_COLOR_YELLOW_BRIGHT " is not in the word." ANSI_COLOR_RESET "\n", letter);
                    remainingAttempts--;
                }
            }
            refresh();

            /*checking if the player won or lost and displaying the result*/
            if (checkVictory(word, guessedLetters)) {
                drawHangman(remainingAttempts);
                printf("\n" ANSI_COLOR_YELLOW_BRIGHT "Congratulations! You guessed the word: " ANSI_COLOR_RESET ANSI_UNDERLINE_GREEN "%s" ANSI_RESET_ALL "\n\n", word);
            } else {
                drawHangman(remainingAttempts);
                printf("\n" ANSI_COLOR_YELLOW_BRIGHT "Sorry, you lost. The word was: " ANSI_COLOR_RESET ANSI_UNDERLINE_RED "%s" ANSI_RESET_ALL "\n\n", word);
            }

            menu();
            gameChoice = choice(gameChoice);
        }
    }
    return 0;
}

/*function definitions*/

/*this function clears the terminal checking if it's windows or open source*/
void refresh() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/*this function prints the hangman based on remaining attempts*/
void drawHangman(int remainingAttempts) {
    switch (remainingAttempts) {
        case 6:
            printf(ANSI_COLOR_YELLOW "   _____" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |     " ANSI_COLOR_RESET ANSI_COLOR_RED "|" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "__|__" ANSI_COLOR_RESET "\n");
            break;
        case 5:
            printf(ANSI_COLOR_YELLOW "   _____" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |     " ANSI_COLOR_RESET ANSI_COLOR_RED "|" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |     " ANSI_COLOR_RESET ANSI_COLOR_PINK "O" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "__|__" ANSI_COLOR_RESET "\n");
            break;
        case 4:
            printf(ANSI_COLOR_YELLOW "   _____" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |     " ANSI_COLOR_RESET ANSI_COLOR_RED "|" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |     " ANSI_COLOR_RESET ANSI_COLOR_PINK "O" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |     " ANSI_COLOR_RESET ANSI_COLOR_PINK "|" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "__|__" ANSI_COLOR_RESET "\n");
            break;
        case 3:
            printf(ANSI_COLOR_YELLOW "   _____" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |     " ANSI_COLOR_RESET ANSI_COLOR_RED "|" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |     " ANSI_COLOR_RESET ANSI_COLOR_PINK "O" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |    " ANSI_COLOR_RESET ANSI_COLOR_PINK "/|" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "__|__" ANSI_COLOR_RESET "\n");
            break;
        case 2:
            printf(ANSI_COLOR_YELLOW "   _____" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |     " ANSI_COLOR_RESET ANSI_COLOR_RED "|" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |     " ANSI_COLOR_RESET ANSI_COLOR_PINK "O" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |    " ANSI_COLOR_RESET ANSI_COLOR_PINK "/|\\" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "__|__" ANSI_COLOR_RESET "\n");
            break;
        case 1:
            printf(ANSI_COLOR_YELLOW "   _____" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |     " ANSI_COLOR_RESET ANSI_COLOR_RED "|" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |     " ANSI_COLOR_RESET ANSI_COLOR_PINK "O" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |    " ANSI_COLOR_RESET ANSI_COLOR_PINK "/|\\" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |    " ANSI_COLOR_RESET ANSI_COLOR_PINK "/" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "__|__" ANSI_COLOR_RESET "\n");
            break;
        case 0:
            printf(ANSI_COLOR_YELLOW "   _____" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |     " ANSI_COLOR_RESET ANSI_COLOR_RED "|" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |     " ANSI_COLOR_RESET ANSI_COLOR_PINK "O" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |    " ANSI_COLOR_RESET ANSI_COLOR_PINK "/|\\" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "  |    " ANSI_COLOR_RESET ANSI_COLOR_PINK "/ \\" ANSI_COLOR_RESET "\n");
            printf(ANSI_COLOR_YELLOW "__|__" ANSI_COLOR_RESET "\n");
            break;
    }
}

/*this function displays the menu*/
void menu() {
    printf(ANSI_COLOR_GRAY "[2]" ANSI_COLOR_RESET ANSI_COLOR_PINK " Information" ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_GRAY "[1]" ANSI_COLOR_RESET ANSI_COLOR_YELLOW " Start Game" ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_GRAY "[0]" ANSI_COLOR_RESET ANSI_COLOR_RED " Exit" ANSI_COLOR_RESET "\n\n");
    printf(ANSI_COLOR_GRAY "Enter your choice: " ANSI_COLOR_RESET);
}

/*this function displays the information about the game*/
void information() {
    printf(ANSI_COLOR_GRAY
           "***************************************\n"
           "* Creator:\n"
           "  Dario Falcone\n"
           "\n"
           "* The Program:\n"
           "  Simulates a game of Hangman.\n"
           "\n"
           "* Rules:\n"
           "  1. Do not use made-up words.\n"
           "  2. Avoid using offensive words.\n"
           "  3. Be respectful to the other player.\n"
           "  4. Declare only one letter per turn.\n"
           "  5. To win, find all the letters in the word.\n"
           "  6. For each incorrect guess, a part of the hangman will be drawn.\n"
           "  7. If the hangman is fully drawn, you lose.\n"
           "\n"
           "* Have Fun!\n"
           "***************************************\n" ANSI_COLOR_RESET);
}

/*this function processes the player's choice*/
int choice(char choice) {
    int c;
    char newline;
    while (1) {
        printf(ANSI_COLOR_YELLOW_BRIGHT "Enter your choice: " ANSI_COLOR_RESET);
        if (scanf("%d%c", &c, &newline) != 2 || newline != '\n') {
            while (getchar() != '\n'); // clear input buffer
            printf(ANSI_UNDERLINE_RED "Invalid input. Please enter a number between 0 and 2." ANSI_RESET_ALL "\n");
        } else if (c < 0 || c > 2) {
            printf(ANSI_UNDERLINE_RED "Invalid choice. Please enter a number between 0 and 2." ANSI_RESET_ALL "\n");
        } else {
            return c;
        }
    }
}

/*this function checks if the letter is alphabetic*/
int checkCharacter(char letter) {
    if (!isalpha(letter)) {
        return 1;
    }
    return 0;
}

/*this function checks if the word contains only alphabetic letters*/
int checkWordCharacters(char word[], int length) {
    for (int i = 0; i < length; i++) {
        if (!isalpha(word[i])) {
            return 1;
        }
    }
    return 0;
}

/*this function converts a letter to uppercase*/
char resizeLetter(char letter) {
    return toupper(letter);
}

/*this function converts a word to uppercase*/
void resizeWord(char word[], int length) {
    for (int i = 0; i < length; i++) {
        word[i] = toupper(word[i]);
    }
}

/*this function initializes the hidden word with underscores*/
void initializeHiddenWord(int length, char hiddenWord[]) {
    for (int i = 0; i < length; i++) {
        hiddenWord[i] = '_';
    }
    hiddenWord[length] = '\0';
}

/*this function prompts the user to insert a word*/
void insertWord(char word[]) {
    printf(ANSI_COLOR_YELLOW_BRIGHT "Enter the word to guess: " ANSI_COLOR_RESET);
    scanf("%s", word);
    while (getchar() != '\n'); // clear input buffer
}

/*this function prompts the user to insert a letter*/
char insertLetter() {
    char letter;
    printf(ANSI_COLOR_YELLOW_BRIGHT "Enter a letter: " ANSI_COLOR_RESET);
    scanf(" %c", &letter);
    while (getchar() != '\n'); // clear input buffer
    return letter;
}

/*this function checks if the letter is already guessed*/
int isLetterAlreadyGuessed(char letter, char guessedLetters[], int length) {
    for (int i = 0; i < length; i++) {
        if (guessedLetters[i] == letter) {
            return 1;
        }
    }
    return 0;
}

/*this function checks if the letter is in the word*/
int checkLetter(char letter, char word[], int length) {
    for (int i = 0; i < length; i++) {
        if (word[i] == letter) {
            return 1;
        }
    }
    return 0;
}

/*this function updates the guessed letters*/
void updateGuessedLetters(char letter, char word[], char guessedLetters[], int length) {
    for (int i = 0; i < length; i++) {
        if (word[i] == letter) {
            guessedLetters[i] = letter;
        }
    }
}

/*this function checks if the player has guessed the word*/
int checkVictory(char word[], char guessedLetters[]) {
    return strcmp(word, guessedLetters) == 0;
}
