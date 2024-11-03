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
    struct User* next;
} User;

typedef struct Task {
    int id;
    char name[100];
    char description[255];
    char category[50];
    char dueDate[20];
    int impid;
} Task;

struct Assignment {
    char name[100];
    int day, month, year;
};

typedef struct QueueNode {
    Task task;
    struct QueueNode* next;
} QueueNode;

typedef struct StackNode {
    Task task;
    struct StackNode* next;
} StackNode;

QueueNode* front = NULL;  // Kuyruðun baþý
QueueNode* rear = NULL;   // Kuyruðun sonu
StackNode* stackTop = NULL;  // Yýðýnýn en üstü


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
void viewTask();
void categorizeTask();
int loadTasks(Task taskList[], int maxTasks);
void saveTasks(const Task taskList[], int taskCount);
void enqueue(Task task);
void push(Task task);
Task dequeue();




int assign_deadline(Assignment* assignment);
void viewDeadlines();



void setReminders();
void platformSleep(int seconds);
void showCurrentNotificationMethod();
void notificationSettings();



int findTaskByName(const char* name);
void markTaskImportance();
void saveTasksToFile();



#endif // TASK_H