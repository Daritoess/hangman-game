/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Colors */
#define ANSI_COLOR_PINK "\x1b[95m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_GRAY "\x1b[90m"
#define ANSI_COLOR_YELLOW_BRIGHT "\x1b[93m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_UNDERLINE_RED "\x1b[4m\x1b[31m"
#define ANSI_UNDERLINE_GREEN "\x1b[4m\x1b[32m"
#define ANSI_RESET_ALL "\x1b[0m"

/* Constants */
#define MAX_ATTEMPTS 6
#define MAX_LETTERS 20

/* Function declarations */
void menu();
void information();
int choice(char choice); 
int checkCharacter(char letter);
int checkWordCharacters(char word[], int length);
char resizeLetter(char letter);
void resizeWord(char word[], int length);
void refresh();
void hangman(int attemptsLeft);
char enterLetter();
void initializeHiddenWord(int length, char hiddenWord[]);
void enterWord(char word[]);
int letterAlreadyPresent(char letter, char guessedLetters[], int length);
int checkLetter(char letter, char word[], int length);
void updateGuessedLetters(char letter, char word[], char guessedLetters[], int length);
int checkWin(char word[], char guessedLetters[]);

/* Main */
int main() {
    char word[MAX_LETTERS];/*parola da indovinare*/
    char guessedLetters[MAX_LETTERS];/*parola nascosta*/
    char previouslyGuessedLetters[26];/*lettere ripetute*/
    char letter;
    int attemptsLeft;
    int repeatedLetterCount;
    int length, gameChoice;

    printf(ANSI_COLOR_GRAY "==Welcome to the Hangman Program==\n==Windows-Linux==\n" ANSI_COLOR_RESET "\n\n");
    menu();
    gameChoice = choice(gameChoice);

    while(gameChoice > 0){
        if(gameChoice==2){
          refresh();
          information();
          menu();
          gameChoice = choice(gameChoice);
        }

        while(gameChoice==1){
            refresh();
            attemptsLeft = MAX_ATTEMPTS;
            repeatedLetterCount = 0;
            do{
                enterWord(word);

                length = strlen(word);
                if(checkWordCharacters(word, length)==1){
                    refresh;
                    printf(ANSI_UNDERLINE_RED "The word contains characters not suitable for the game. Enter a word containing alphabet letters!" ANSI_COLOR_RESET "\n");
                }
            }while(checkWordCharacters(word, length)==1);

            resizeWord(word,length);
            initializeHiddenWord(length, guessedLetters);
            refresh();

            /* the loop that determines the actual game based on remaining attempts and a check if you have not guessed the word */
            while (attemptsLeft > 0 && !checkWin(word, guessedLetters)) {
                do{
                    hangman(attemptsLeft);
                    printf("\n" ANSI_COLOR_YELLOW_BRIGHT "Word: %s" ANSI_COLOR_RESET "\n", guessedLetters);
                    letter = enterLetter();

                    if(checkCharacter(letter)==1){
                        refresh();
                        printf(ANSI_UNDERLINE_RED "The character entered is not suitable for the program. Enter an alphabet letter!" ANSI_RESET_ALL "\n");
                    }
                }while(checkCharacter(letter)==1);

                letter=resizeLetter(letter);

                if (letterAlreadyPresent(letter, previouslyGuessedLetters, repeatedLetterCount)) {
                    refresh();
                    printf(ANSI_UNDERLINE_RED "You have already entered this letter previously. Enter another letter!" ANSI_RESET_ALL "\n");
                    continue;
                }

                previouslyGuessedLetters[repeatedLetterCount] = letter;
                repeatedLetterCount++;

                if (checkLetter(letter, word, length)) {
                    refresh();
                    printf(ANSI_COLOR_YELLOW_BRIGHT "The letter " ANSI_COLOR_RESET ANSI_UNDERLINE_GREEN "'%c'" ANSI_RESET_ALL ANSI_COLOR_YELLOW_BRIGHT " is present!" ANSI_COLOR_RESET "\n", letter);
                    updateGuessedLetters(letter, word, guessedLetters, length);
                } else {
                    refresh();
                    printf(ANSI_COLOR_YELLOW_BRIGHT "The letter " ANSI_COLOR_RESET ANSI_UNDERLINE_RED "'%c'" ANSI_RESET_ALL ANSI_COLOR_YELLOW_BRIGHT " is not present." ANSI_COLOR_RESET "\n", letter);
                    attemptsLeft--;
                }
            }
            refresh();

            /* check if you win based on the function if you win or lose also printing how many attempts you made */
            if (checkWin(word, guessedLetters)) {
                hangman(attemptsLeft);
                printf("\n" ANSI_COLOR_YELLOW_BRIGHT "Congratulations! You guessed the word: " ANSI_COLOR_RESET ANSI_UNDERLINE_GREEN "%s" ANSI_RESET_ALL "\n\n", word);
            } else {
                hangman(attemptsLeft);
                printf("\n" ANSI_COLOR_YELLOW_BRIGHT "I'm sorry you lost. The word was: " ANSI_COLOR_RESET ANSI_UNDERLINE_RED "%s" ANSI_RESET_ALL "\n\n", word);
            }

            menu();
            gameChoice = choice(gameChoice);
        }
    }
    return 0;
}

/* Function Definitions */

/* this function clears the terminal checking if you have windows or an open source */
void refresh() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/* this function prints the hangman based on the remaining attempts */
void hangman(int attemptsLeft) {
    switch (attemptsLeft) {
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
    };  
}

/* this function enters a letter */
char enterLetter() {
    char l;
    printf(ANSI_COLOR_GRAY "Guess a letter: " ANSI_COLOR_RESET);
    scanf(" %c", &l);
    while (getchar() != '\n');  
    return l;
}

/* this function initializes the hidden word that we will see with this character '-' */
void initializeHiddenWord(int length, char hiddenWord[]) {
    for (int i = 0; i < length; i++) {
        hiddenWord[i] = '-';
    }
    hiddenWord[length] = '\0'; 
}

/* this function enters a word and checks that it does not exceed the max characters */
void enterWord(char word[]) {
    do{
        word[MAX_LETTERS-1]='\0';
        printf(ANSI_COLOR_GRAY "Enter the word to guess " ANSI_COLOR_RESET ANSI_UNDERLINE_RED "(max %d characters)" ANSI_RESET_ALL ANSI_COLOR_GRAY ": " ANSI_COLOR_RESET, MAX_LETTERS);
        scanf("%s", word);
        refresh();
        if (strlen(word) > MAX_LETTERS) {
            printf(ANSI_UNDERLINE_RED "The word entered exceeds the maximum allowed length!" ANSI_RESET_ALL "\n");
        }
    }while(strlen(word) > MAX_LETTERS);
}

/* this function checks if the letter has been repeated */
int letterAlreadyPresent(char letter, char guessedLetters[], int length) {
    for (int i = 0; i < length; i++) {
        if (guessedLetters[i] == letter) {
            return 1; 
        }
    }
    return 0; 
}

/* this function checks if the letter is present in the word */
int checkLetter(char letter, char word[], int length) {
    for (int i = 0; i < length; i++) {
        if (word[i] == letter) {
            return 1; 
        }
    }
    return 0; 
}

/* this function updates the array that contains the letters already used previously */
void updateGuessedLetters(char letter, char word[], char guessedLetters[], int length) {
    for (int i = 0; i < length; i++) {
        if (word[i] == letter) {
            guessedLetters[i] = letter;
        }
    }
}

/* this function checks if you win or lose by comparing the words */
int checkWin(char word[], char guessedLetters[]) {
    if (strcmp(word, guessedLetters) == 0) {
        return 1; 
    }
    return 0;
}

/* this function resizes the letter from uppercase to lowercase */
char resizeLetter(char letter){
    return letter=tolower(letter);
}

/* this function resizes the word from uppercase to lowercase */
void resizeWord(char word[], int length){
    for(int i=0; i < length; i++){
        word[i]=tolower(word[i]);
    }
}

int checkCharacter(char letter){
    if((letter<65 || letter >90) && (letter<97 || letter>122)){
        return 1;
    }else{
        return 0;
    }
}

int checkWordCharacters(char word[], int lenght){
    for(int i=0; i < lenght; i++){
        if((word[i]<65 || word[i] >90) && (word[i]<97 || word[i]>122)){
            return 1;
        }
    }
    return 0;
}

/* this function is used to choose an option from the menu */
int choice(char choice) {
    do {
        scanf("%c", &choice);
        while (getchar() != '\n');
        refresh();
        if (choice < 48 || choice > 50) {
            printf(ANSI_UNDERLINE_RED "Invalid choice entered. Enter 1 or 0!" ANSI_RESET_ALL "\n\n");
            menu();
        }
    } while (choice < 48 || choice > 50);
    return choice - 48;
}

/* print game menu */
void menu(){
    printf(ANSI_COLOR_GRAY "[2]" ANSI_COLOR_RESET ANSI_COLOR_PINK " Information" ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_GRAY "[1]" ANSI_COLOR_RESET ANSI_COLOR_YELLOW " Start Game" ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_GRAY "[0]" ANSI_COLOR_RESET ANSI_COLOR_RED " Exit" ANSI_COLOR_RESET "\n\n");
    printf(ANSI_COLOR_GRAY "Enter choice: " ANSI_COLOR_RESET);
}

/* print information about the game */
void information(){
    printf(ANSI_COLOR_GRAY
           "*Creator:\n"
           "Dario Falcone.\n"
           "\n"
           "*The program:\n"
           "Simulates a game of Hangman.\n"
           "\n"
           "*Rules:\n"
           "1-Do not enter invented words;\n"
           "2-Try not to enter obscene words;\n"
           "3-Be respectful towards the other player;\n"
           "4-Declare only one letter per turn;\n"
           "5-To win find all the letters that make up the word;\n"
           "6-For each wrong attempt, a part of the hangman's body will be added;\n"
           "7-When the hangman is hanged, you'll know you've lost;\n"
           "\n"
           "*Have Fun.\n"
           "\n" ANSI_COLOR_RESET);
}
