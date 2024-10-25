#ifndef TASK_H
#define TASK_H
#include "../../utility/header/commonTypes.h"
#include <iostream>

using namespace std;

#define MAX_ASSIGNMENT_NAME 50


typedef struct User {
    int id;
    char name[50];
    char surname[50];
    char email[50];
    char password[50];
    struct User* next;  // Zincirleme için baðlý liste yapýsý
} User;

typedef struct Task {
    int id;
    char name[100];
    char description[255];
    char category[50];
    char dueDate[20];  // YYYY-MM-DD formatýnda son tarih
} Task;

typedef struct {
    char name[MAX_ASSIGNMENT_NAME];  // Görev adý
    int day, month, year;            // Deadline (Son teslim tarihi)
} Assignment;

void clearScreen();

int enterToContinue();

int handleInputError();

int getInput();




int openingScreenMenu();

int printMainMenu();

int printCreateTaskMenu();

int printDeadlineSettingsMenu();

int printTaskPrioritizationMenu();

int createTaskMenu(Task taskList[], int* taskCount);
int deadlineSettingsMenu();

int reminderSystemMenu();

int taskPrioritizationMenu();

int userOptionsMenu();

int mainMenu(const char* pathFileUsers);





int registerUser(User user, const char* pathFileUser);

int registerUserMenu(const char* pathFileUsers);





int loginUser(User loginUser, const char* pathFileUsers);

int loginUserMenu(const char* pathFileUsers);




int addTask(Task taskList[], int* taskCount, int maxTasks);
void viewTask(const Task taskList[], int taskCount);
void categorizeTask(const Task taskList[], int taskCount);



int assign_deadline(Assignment* assignment);


#endif // TASK_H