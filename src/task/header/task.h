#ifndef TASK_H
#define TASK_H
#include "../../utility/header/commonTypes.h"
#include <iostream>

using namespace std;

typedef struct {
	int id;
	char name[100];
	char surname[100];
	char email[100];
	char password[100];
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