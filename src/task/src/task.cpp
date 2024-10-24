#define _CRT_SECURE_NO_WARNINGS
#include "../header/task.h"
#include <stdexcept>
#include <regex>
#include <fstream>
#include <stdio.h>
#include <limits>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <iostream>
#include <cstring>

using namespace std;

User loggedUser;




void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/**
 * @brief Waits for the user to press enter to continue.
 *
 * This function prompts the user to press enter to continue and waits for the input.
 *
 * @return Always returns 1.
 */
int enterToContinue() {
    printf("\nPress enter to continue");
    while (getchar() != '\n');
    return 1;
}
/**
 * @brief Handles input error by clearing the input buffer.
 *
 * This function is responsible for handling input errors by clearing the input buffer
 * until a newline character or EOF is encountered. It then prints an error message
 * indicating that the input is invalid and prompts the user to enter a number.
 *
 * @return Always returns 0.
 */
int handleInputError() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    printf("Invalid input. Please enter a number.\n");
    return 0;
}
/**
 * @brief Gets user input from standard input.
 *
 * This function reads a line of input from standard input (stdin), attempts to parse
 * it as an integer, and returns the parsed integer value. If an error occurs during
 * input or parsing, appropriate error codes are returned.
 *
 * @return The integer input by the user, or error codes (-1 or -2) if an error occurs.
 */
int getInput() {
    char line[256];
    int choice, result;

    if (fgets(line, sizeof(line), stdin) == NULL) {
        return -1;
    }

    result = sscanf(line, "%d", &choice);
    if (result != 1) {
        return -2;
    }

    return choice;
}

int openingScreenMenu() {
    printf("***************************************\n");
    printf("*                                     *\n");
    printf("*      WELCOME TO TASK MANAGER!       *\n");
    printf("*                                     *\n");
    printf("***************************************\n\n");

    printf("=============== MAIN MENU ===============\n");
    printf("1. Login\n");
    printf("2. Register\n");
    printf("3. Exit Program\n");
    printf("=========================================\n");
    printf("Please enter a number : ");

    return 1;
}

int printMainMenu() {
    clearScreen();
    printf("========================================\n");
    printf("        MAIN MENU - TASK MANAGER     \n");
    printf("========================================\n");
    printf("1. Create Task\n");
    printf("2. Deadline Settings\n");
    printf("3. Reminder System\n");
    printf("4. Task Prioritization\n");
    printf("5. Exit\n");
    printf("========================================\n");
    printf("Please enter your choice: ");
    return 1;
}



int printCreateTaskMenu() {
    clearScreen();
    printf("========================================\n");
    printf("           CREATE TASK MENU          \n");
    printf("========================================\n");
    printf("1. Add Task\n");
    printf("2. Categorize Tasks\n");
    printf("3. View Tasks\n");
    printf("4. Exit\n");
    printf("========================================\n");
    printf("Please enter your choice: ");
    return 1;
}


// Son Tarih Ayarları Menüsü
int printDeadlineSettingsMenu() {
    clearScreen();
    printf("========================================\n");
    printf("       DEADLINE SETTINGS MENU        \n");
    printf("========================================\n");
    printf("1. Assign Deadline\n");
    printf("2. View Deadlines\n");
    printf("3. Exit\n");
    printf("========================================\n");
    printf("Please enter your choice : ");
    return 1;
}


// Hatırlatma Sistemi Menüsü
int printReminderSystemMenu() {
    clearScreen();
    printf("========================================\n");
    printf("        REMINDER SYSTEM MENU         \n");
    printf("========================================\n");
    printf("1. Set Reminders\n");
    printf("2. Notification Settings\n");
    printf("3. Exit\n");
    printf("========================================\n");
    printf("Please enter your choice: ");
    return 1;
}


// Görev Önceliği Menüsü
int printTaskPrioritizationMenu() {
    clearScreen();
    printf("========================================\n");
    printf("       TASK PRIORITIZATION MENU       \n");
    printf("========================================\n");
    printf("1. Mark Task Importance\n");
    printf("2. Reorder Tasks\n");
    printf("3. Exit\n");
    printf("========================================\n");
    printf("Please enter your choice : ");
    return 1;
}


int getNewUserId(User users[], int userCount) {
    if (userCount == 0)
        return 1;

    int maxId = users[0].id;

    return maxId + 1;
}

int loadUsers(const char* pathFileUsers, User** users) {
    FILE* file = fopen(pathFileUsers, "rb");
    if (!file) {
        *users = NULL;
        return 0;
    }

    int count;
    fread(&count, sizeof(int), 1, file);

    *users = (User*)malloc(sizeof(User) * count);
    fread(*users, sizeof(User), count, file);

    fclose(file);
    return count;
}

int createTaskMenu() {
    int choice;

    while (1) {
        printCreateTaskMenu();
        choice = getInput();

        if (choice == -2) {
            handleInputError();
            enterToContinue();
            continue;
        }

        switch (choice) {
        case 1:
            //addBookMenu(pathFileBooks);
            break;
        case 2:
            //deleteBookMenu(pathFileBooks);
            break;
        case 3:
            //updateBookMenu(pathFileBooks);
            break;
        case 4:
            return 0;
        default:
            clearScreen();
            printf("Invalid choice. Please try again.\n");
            enterToContinue();
            break;
        }
    }
}

int deadlineSettingsMenu() {
    int choice;

    while (1) {
        printDeadlineSettingsMenu();
        choice = getInput();

        if (choice == -2) {
            handleInputError();
            enterToContinue();
            continue;
        }

        switch (choice) {
        case 1:
            //addBookMenu(pathFileBooks);
            break;
        case 2:
            //deleteBookMenu(pathFileBooks);
            break;
        case 3:
            return 0;
        default:
            clearScreen();
            printf("Invalid choice. Please try again.\n");
            enterToContinue();
            break;
        }
    }
}

int reminderSystemMenu() {
    int choice;

    while (1) {
        printReminderSystemMenu();
        choice = getInput();

        if (choice == -2) {
            handleInputError();
            enterToContinue();
            continue;
        }

        switch (choice) {
        case 1:
            //addBookMenu(pathFileBooks);
            break;
        case 2:
            //deleteBookMenu(pathFileBooks);
            break;
        case 3:
            return 0;
        default:
            clearScreen();
            printf("Invalid choice. Please try again.\n");
            enterToContinue();
            break;
        }
    }
}

int taskPrioritizationMenu() {
    int choice;

    while (1) {
        printTaskPrioritizationMenu();
        choice = getInput();

        if (choice == -2) {
            handleInputError();
            enterToContinue();
            continue;
        }

        switch (choice) {
        case 1:
            //addBookMenu(pathFileBooks);
            break;
        case 2:
            //deleteBookMenu(pathFileBooks);
            break;
        case 3:
            return 0;
        default:
            clearScreen();
            printf("Invalid choice. Please try again.\n");
            enterToContinue();
            break;
        }
    }
}

int registerUser(User user, const char* pathFileUser) {
    FILE* file = fopen(pathFileUser, "rb+");
    int userCount = 0;
    User* users = NULL;

    if (file) {

        fread(&userCount, sizeof(int), 1, file);
        if (userCount > 0) {
            users = (User*)malloc(sizeof(User) * userCount);
            fread(users, sizeof(User), userCount, file);

            for (int i = 0; i < userCount; ++i) {
                if (strcmp(users[i].email, user.email) == 0) {
                    printf("User already exists.\n");
                    fclose(file);
                    free(users);
                    enterToContinue();
                    return 0;
                }
            }
        }
        rewind(file);
    }
    else {
        file = fopen(pathFileUser, "wb");
    }

    user.id = getNewUserId(users, userCount);
    userCount++;
    users = (User*)realloc(users, sizeof(User) * userCount);
    users[userCount - 1] = user;

    fwrite(&userCount, sizeof(int), 1, file);
    fwrite(users, sizeof(User), userCount, file);

    printf("User registered successfully: Welcome %s %s\n", user.name, user.surname);

    if (users != NULL) {
        free(users);
    }
    fclose(file);
    enterToContinue();
    return 1;
}

/**
 * @brief Displays a menu for user registration.
 *
 * This function prompts the user to input their name, surname, email, and password for registration.
 *
 * @param pathFileUsers The path to the user database file.
 * @return 1 if the registration process is completed successfully, 0 otherwise.
 */
int registerUserMenu(const char* pathFileUsers) {
    clearScreen();
    User newUser;

    printf("Enter Name: ");
    fgets(newUser.name, sizeof(newUser.name), stdin);
    newUser.name[strcspn(newUser.name, "\n")] = 0;

    printf("Enter Surname: ");
    fgets(newUser.surname, sizeof(newUser.surname), stdin);
    newUser.surname[strcspn(newUser.surname, "\n")] = 0;

    printf("Enter email: ");
    fgets(newUser.email, sizeof(newUser.email), stdin);
    newUser.email[strcspn(newUser.email, "\n")] = 0;

    printf("Enter password: ");
    fgets(newUser.password, sizeof(newUser.password), stdin);
    newUser.password[strcspn(newUser.password, "\n")] = 0;

    if (registerUser(newUser, pathFileUsers)) {
        return 1;
    }
    else {
        printf("Registration failed.\n");
    }

    enterToContinue();
    return 1;
}

/**
 * @brief Logs in a user.
 *
 * This function verifies the user's email and password against the user database file for login.
 *
 * @param loginUser The user attempting to log in.
 * @param pathFileUsers The path to the user database file.
 * @return 1 if the login is successful, 0 otherwise.
 */
int loginUser(User loginUser, const char* pathFileUsers) {
    FILE* file = fopen(pathFileUsers, "rb");
    if (!file) {
        printf("Failed to open user file.\n");
        return 0;
    }

    int userCount = 0;
    fread(&userCount, sizeof(int), 1, file);
    if (userCount == 0) {
        printf("No users registered.\n");
        fclose(file);
        enterToContinue();
        return 0;
    }

    User userFromFile;
    for (int i = 0; i < userCount; i++) {
        fread(&userFromFile, sizeof(User), 1, file);
        if (strcmp(userFromFile.email, loginUser.email) == 0 && strcmp(userFromFile.password, loginUser.password) == 0) {
            printf("Login successful.\n");
            fclose(file);
            enterToContinue();
            loggedUser = userFromFile;
            return 1;
        }
    }

    printf("Incorrect email or password.\n");
    fclose(file);
    enterToContinue();
    return 0;
}

/**
 * @brief Displays a menu for user login.
 *
 * This function prompts the user to input their email and password for login.
 *
 * @param pathFileUsers The path to the user database file.
 * @return 1 if the login process is completed successfully, 0 otherwise.
 */
int loginUserMenu(const char* pathFileUsers) {
    clearScreen();
    User loginUsers;

    printf("Enter email: ");
    fgets(loginUsers.email, sizeof(loginUsers.email), stdin);
    loginUsers.email[strcspn(loginUsers.email, "\n")] = 0;

    printf("Enter password: ");
    fgets(loginUsers.password, sizeof(loginUsers.password), stdin);
    loginUsers.password[strcspn(loginUsers.password, "\n")] = 0;

    return loginUser(loginUsers, pathFileUsers);
}

int userOptionsMenu() {
    int choice;

    while (1) {
        printMainMenu();
        choice = getInput();

        if (choice == -2) {
            handleInputError();
            enterToContinue();
            continue;
        }

        switch (choice) {
        case 1:
            createTaskMenu();
            break;
        case 2:
            deadlineSettingsMenu();
            break;
        case 3:
            reminderSystemMenu();
            break;
        case 4:
            taskPrioritizationMenu();
        case 5:
            return 0;
        default:
            clearScreen();
            printf("Invalid choice. Please try again.\n");
            enterToContinue();
            break;
        }
    }
}

int mainMenu(const char* pathFileUsers) {
    int choice;

    while (1) {
        clearScreen();
        openingScreenMenu();
        choice = getInput();

        if (choice == -2) {
            handleInputError();
            enterToContinue();
            continue;
        }

        switch (choice) {
        case 1:
            clearScreen();
            if (loginUserMenu(pathFileUsers)) userOptionsMenu();
            break;

        case 2:
            clearScreen();
            registerUserMenu(pathFileUsers);
            break;

        case 3:
            printf("Exit Program\n");
            return 0;

        default:
            printf("Invalid choice. Please try again.\n");
            enterToContinue();
            break;
        }
    }
}