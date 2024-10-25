#ifndef TASK_H
#define TASK_H
#include "../../utility/header/commonTypes.h"
#include <iostream>

using namespace std;

typedef struct User {
    int id;
    char name[50];
    char surname[50];
    char email[50];
    char password[50];
    struct User* next;  // Zincirleme için baðlý liste yapýsý
} User;

void clearScreen();

int enterToContinue();

int handleInputError();

int getInput();


int openingScreenMenu();

int printMainMenu();

int printCreateTaskMenu();

int printDeadlineSettingsMenu();

int printTaskPrioritizationMenu();

int createTaskMenu();

int deadlineSettingsMenu();

int reminderSystemMenu();

int taskPrioritizationMenu();

int userOptionsMenu();

int mainMenu(const char* pathFileUsers);



int registerUser(User user, const char* pathFileUser);

int registerUserMenu(const char* pathFileUsers);



int loginUser(User loginUser, const char* pathFileUsers);

int loginUserMenu(const char* pathFileUsers);



#endif // TASK_H