
/**
 * @file task.cpp
 * @brief Contains task definitions and operations for the Task Scheduler project.
 *
 * This file contains functions and classes related to task management in the Task Scheduler project.
 * The functions include task addition, editing, and various user operations.
 */

 /**
  * @def _CRT_SECURE_NO_WARNINGS
  * @brief Disables warnings related to the use of unsafe C runtime functions.
  *
  * This macro is used to suppress warnings that are generated when using certain functions
  * such as `strcpy`, `sprintf`, etc., which are considered unsafe by modern compilers.
  * Defining this macro allows the use of these functions without triggering security warnings.
  */
#define _CRT_SECURE_NO_WARNINGS


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

#ifdef _WIN32
#include <windows.h>  // Windows için Sleep()
#else
#include <unistd.h>   // Linux/macOS için sleep()
#endif

  /**
   * @brief Pointer to the head of the task list.
   *
   * This pointer represents the beginning of the linked list that stores all the tasks.
   * It is used to traverse and manage the list of tasks effectively.
   */
TaskNode* head = NULL;  // Liste başı (ilk görev)

/**
 * @brief Pointer to the tail of the task list.
 *
 * This pointer represents the end of the linked list that stores all the tasks.
 * It is used to quickly add new tasks to the end of the list.
 */
TaskNode* tail = NULL;  // Liste sonu (son görev)

/**
 * @brief Pointer to the head of the XOR linked list.
 *
 * This pointer represents the beginning of an XOR linked list, which is used for storing tasks.
 * The XOR linked list provides efficient traversal using XOR-based memory representation.
 */
XORNode* xorHead = NULL;

/**
 * @brief Pointer to the tail of the XOR linked list.
 *
 * This pointer represents the end of the XOR linked list.
 * It is used to add new tasks to the end and manage the list efficiently.
 */
XORNode* xorTail = NULL;

/**
 * @brief Size of the hash table.
 *
 * This constant defines the number of buckets in the hash table used for storing user information.
 * The value of TABLE_SIZE is set to 10, which determines the capacity of the hash table.
 */
#define TABLE_SIZE 10  // Hash tablosunun boyutu

 /**
  * @brief Maximum number of tasks that can be stored.
  *
  * This constant defines the maximum number of tasks that can be managed by the application.
  * The value of MAX_TASKS is set to 100, limiting the number of tasks that can be created and stored.
  */
#define MAX_TASKS 100  // Maksimum görev sayısı

  /**
   * @brief Maximum length of assignment names.
   *
   * This constant defines the maximum length of an assignment name that can be stored in the application.
   * The value of MAX_ASSIGNMENT_NAME is set to 50, ensuring assignment names do not exceed this length.
   */
#define MAX_ASSIGNMENT_NAME 50

   /**
    * @brief Array to store assignments.
    *
    * This array holds the assignments related to tasks. It can store up to 100 assignments, and each assignment
    * is identified by a unique name with a maximum length of 50 characters.
    */
Assignment assignments[100];

/**
 * @brief File name for storing tasks.
 *
 * This constant represents the name of the binary file used to store task information persistently.
 * The tasks are saved to and loaded from this file to maintain data across application sessions.
 */
const char* filename = "tasks.bin";


/**
 * @brief Array to store tasks.
 *
 * This array holds the tasks that users create. It has a fixed size of 100, meaning it can store up to 100 tasks.
 */
Task tasks[100];

/**
 * @brief Hash table for storing user information.
 *
 * This hash table is used to store pointers to User objects. The size of the hash table is defined by TABLE_SIZE.
 * It is used for efficient lookup, insertion, and management of user data.
 */
User* hashTable[TABLE_SIZE];

/**
 * @brief List of tasks for task management.
 *
 * This array is used to manage the tasks created by users. It can store up to MAX_TASKS tasks and is used
 * throughout the application for task-related operations such as creation, modification, and deletion.
 */
Task taskList[MAX_TASKS];  // Görev listesi

/**
 * @brief Counter for the current number of tasks.
 *
 * This variable keeps track of the number of tasks that have been created and are currently stored in the taskList.
 * It is used to manage task operations and to determine the next available position in the taskList array.
 */
int taskCount = 0;  // Mevcut görev sayısı

/**
 * @brief Current notification method setting.
 *
 * This variable stores the current notification method selected by the user. It can take values corresponding to
 * different notification methods such as SMS, Email, or Phone Call. The default value is 0, indicating no method selected.
 */
int notificationMethod = 0;

/**
 * @brief Overflow area for handling user data collisions.
 *
 * This array is used as an overflow area to handle collisions in the hash table. When a collision occurs and cannot
 * be resolved within the main hash table, user data is stored in this overflow area. The size of the overflow area
 * is defined by OVERFLOW_SIZE.
 */
User overflowArea[OVERFLOW_SIZE];

/**
 * @brief Counter for the number of entries in the overflow area.
 *
 * This variable keeps track of how many users are currently stored in the overflow area. It is used to manage
 * the overflow mechanism and ensure that the overflow area does not exceed its capacity.
 */
int overflowCount = 0;



using namespace std;

/**
 * @brief Represents the currently logged-in user.
 *
 * The `loggedUser` variable holds the information of the user who is currently logged in.
 * It is used throughout the application to access user-specific data and preferences.
 */
User loggedUser;

/**
 * @brief Clears the console screen.
 *
 * The `clearScreen` function is used to clear the console screen, depending on the operating system.
 * On Windows, it uses the `cls` command, while on Unix-like systems (e.g., Linux, macOS), it uses the `clear` command.
 */

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

/**
 * @brief Displays the opening screen menu of the Task Manager.
 *
 * The `openingScreenMenu` function displays a welcome message and provides the main menu options for the user.
 * The user can choose to log in, register, or exit the program.
 *
 * @return Always returns 1, which can be used as a placeholder for further menu implementation.
 */

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

/**
 * @brief Displays the main menu of the Task Manager.
 *
 * The `printMainMenu` function clears the console and displays the main menu options for the user.
 * The menu includes options for creating a task, setting deadlines, managing reminders, prioritizing tasks,
 * running algorithms, and exiting the application.
 *
 * @return Always returns 1, which can be used as a placeholder for further menu implementation.
 */

int printMainMenu() {
    clearScreen();
    printf("========================================\n");
    printf("        MAIN MENU - TASK MANAGER     \n");
    printf("========================================\n");
    printf("1. Create Task\n");
    printf("2. Deadline Settings\n");
    printf("3. Reminder System\n");
    printf("4. Task Prioritization\n");
    printf("5. Algorithms\n");
    printf("6. Exit\n");
    printf("========================================\n");
    printf("Please enter your choice: ");
    return 1;
}

/**
 * @brief Displays the create task menu in the Task Manager.
 *
 * The `printCreateTaskMenu` function clears the console and presents the user with options for creating and managing tasks.
 * Options include adding a new task, viewing existing tasks, categorizing tasks, analyzing dependencies, and more advanced
 * functions such as analyzing strongly connected components (SCC) or working with linked lists.
 *
 * @return Always returns 1, which can be used as a placeholder for further menu implementation.
 */

int printCreateTaskMenu() {
    clearScreen();
    printf("========================================\n");
    printf("           CREATE TASK MENU          \n");
    printf("========================================\n");
    printf("1. Add Task\n");
    printf("2. View Tasks\n");
    printf("3. Categorize Tasks\n");
    printf("4. Dependencies of Functions\n");
    printf("5. Analyze SCC\n");
    printf("6. Search By Keyword\n");
    printf("7. Double Linked List\n");
    printf("8. XOR Linked List\n");
    printf("9. Exit\n");
    printf("========================================\n");
    printf("Please enter your choice: ");
    return 1;
}


/**
 * @brief Displays the deadline settings menu in the Task Manager.
 *
 * The `printDeadlineSettingsMenu` function clears the console and presents the user with options to manage task deadlines.
 * The menu allows the user to assign deadlines to tasks, view existing deadlines, or exit the menu.
 *
 * @return Always returns 1, which can be used as a placeholder for further menu implementation.
 */

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


/**
 * @brief Displays the reminder system menu in the Task Manager.
 *
 * The `printReminderSystemMenu` function clears the console and presents the user with options for managing reminders.
 * Options include setting reminders for tasks, configuring notification settings, or exiting the menu.
 *
 * @return Always returns 1, which can be used as a placeholder for further menu implementation.
 */

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

/**
 * @brief Displays the task prioritization menu in the Task Manager.
 *
 * The `printTaskPrioritizationMenu` function clears the console and presents the user with options to manage task prioritization.
 * The menu allows the user to mark a task as important, reorder tasks based on their importance, or exit the menu.
 *
 * @return Always returns 1, which can be used as a placeholder for further menu implementation.
 */

int printTaskPrioritizationMenu() {
    clearScreen();
    printf("========================================\n");
    printf("       TASK PRIORITIZATION MENU       \n");
    printf("========================================\n");
    printf("1. Mark Task Importance\n");
    printf("2. Importance Ordering\n");
    printf("3. Exit\n");
    printf("========================================\n");
    printf("Please enter your choice : ");
    return 1;
}

/**
 * @brief Displays the algorithms menu in the Task Manager.
 *
 * The `printAlgorithmsMenu` function clears the console and presents the user with various algorithms
 * that can be explored within the Task Manager project. These include techniques for collision resolution in hashing
 * such as progressive overflow, linear probing, quadratic probing, double hashing, and more.
 * Users can also return to the main menu by selecting the appropriate option.
 *
 * @return Always returns 1, which can be used as a placeholder for further menu implementation.
 */

int printAlgorithmsMenu() {
    clearScreen();
    printf("========================================\n");
    printf("              ALGORITHMS MENU           \n");
    printf("========================================\n");
    printf("1. Progressive Overflow\n");
    printf("2. Linear Probing\n");
    printf("3. Quadratic Probing\n");
    printf("4. Double Hashing\n");
    printf("5. Use of Buckets\n");
    printf("6. Linear Quotient\n");
    printf("7. Brent's Method\n");
    printf("8. Exit to Main Menu\n");
    printf("========================================\n");
    printf("Please enter your choice: ");
    return 1;
}

/**
 * @brief Generates a new user ID based on existing user data.
 *
 * The `getNewUserId` function calculates a new unique user ID by finding the maximum existing user ID
 * and incrementing it by one. If no users exist, it returns 1 as the first user ID.
 *
 * @param users Array of users.
 * @param userCount The number of users currently in the system.
 * @return The newly generated user ID.
 */

int getNewUserId(User users[], int userCount) {
    if (userCount == 0)
        return 1;

    int maxId = users[0].id;
    for (int i = 1; i < userCount; ++i) {
        if (users[i].id > maxId) {
            maxId = users[i].id;
        }
    }

    return maxId + 1;
}

/**
 * @brief Loads user data from a binary file.
 *
 * The `loadUsers` function loads user data from the specified binary file and stores it in a dynamically
 * allocated array. If the file cannot be opened, it returns 0 and sets the `users` pointer to NULL.
 *
 * @param pathFileUsers Path to the binary file containing user data.
 * @param users Pointer to the array where user data will be stored.
 * @return The number of users loaded from the file.
 */

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


/**
 * @brief Displays and handles the task creation menu for the Task Manager.
 *
 * The `createTaskMenu` function provides an interactive menu that allows users to manage tasks.
 * Users can create tasks, view and categorize tasks, analyze dependencies, and explore more advanced operations
 * such as searching tasks by keyword, navigating tasks using a double linked list, and using an XOR linked list.
 * The function runs in a loop until the user chooses to exit.
 *
 * @param taskList Array of tasks to be managed.
 * @param taskCount Pointer to the current number of tasks in the system.
 * @return This function does not return, as it runs indefinitely until an exit condition is met.
 */


int createTaskMenu(Task taskList[], int* taskCount) {
    int maxTasks = 100;
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
            addTask(taskList, taskCount, maxTasks);
            break;
        case 2:
            viewTask();
            enterToContinue();
            break;
        case 3:
            categorizeTask();
            enterToContinue();
            break;
        case 4: {
            int taskId;
            printf("Enter the task ID to view its dependencies: ");
            taskId = getInput();

            if (taskId > 0 && taskId <= *taskCount) {
                printDependencies(taskList, *taskCount, taskId);
            }
            else {
                printf("Invalid task ID.\n");
            }
            enterToContinue();
            break;
        }
        case 5:
            analyzeSCC(taskList, *taskCount, stdout);
            enterToContinue();
            break;
        case 6:
            searchTasksByKeyword(); 
            enterToContinue();
            break;
        case 7:
            navigateTasks();  
            enterToContinue();
            break;
        case 8:
            loadTasksToXORList("tasks.bin");  
            navigateXORList();  
            enterToContinue();
            break;
        case 9:
            return 0;
        default:
            clearScreen();
            printf("Invalid choice. Please try again.\n");
            enterToContinue();
            break;
        }
    }
}

/**
 * @brief Adds a task to the XOR linked list.
 *
 * The `addTaskToXORList` function creates a new node for the given task and adds it to the XOR linked list.
 * If the list is empty, the new node becomes both the head and the tail of the list. If not, the function
 * updates the existing tail's XOR pointer and adds the new node at the end.
 *
 * @param task The task to be added to the XOR linked list.
 * @return Returns 1 after successfully adding the task.
 */

int addTaskToXORList(Task task) {
    XORNode* newNode = (XORNode*)malloc(sizeof(XORNode));
    newNode->task = task;
    newNode->xorPtr = xorTail; 

    if (xorHead == NULL) {
        xorHead = xorTail = newNode; 
    }
    else {
        xorTail->xorPtr = (XORNode*)((uintptr_t)(xorTail->xorPtr) ^ (uintptr_t)newNode); 
        xorTail = newNode; 
    }
    return 1;
}

/**
 * @brief Loads tasks from a file into the XOR linked list.
 *
 * The `loadTasksToXORList` function reads tasks from the specified binary file and adds each task
 * to the XOR linked list using the `addTaskToXORList` function.
 *
 * @param filename The name of the binary file containing tasks to be loaded.
 */

int loadTasksToXORList(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Unable to open tasks file.\n");
        return -1;
    }

    Task task;
    while (fread(&task, sizeof(Task), 1, file)) {
        addTaskToXORList(task); 
    }

    fclose(file);
    printf("Tasks loaded into XOR Linked List successfully!\n");
    return 1;
}

/**
 * @brief Navigates through the XOR linked list of tasks.
 *
 * The `navigateXORList` function allows the user to navigate through the XOR linked list of tasks.
 * The user can move forward or backward through the list to view tasks or exit navigation at any time.
 * Navigation is based on XOR pointers to efficiently traverse in both directions.
 */

int navigateXORList() {
    XORNode* current = xorHead;
    XORNode* prev = NULL;
    XORNode* next;

    if (current == NULL) {
        printf("No tasks found in the list.\n");
        return -1; // Hata: XOR listesi boş
    }

    int choice;
    do {
        printf("ID: %d\n", current->task.id);
        printf("Name: %s\n", current->task.name);
        printf("Description: %s\n", current->task.description);
        printf("Category: %s\n", current->task.category);
        printf("Due Date: %s\n", current->task.dueDate);
        printf("---------------------------\n");

        printf("Press 1 to go forward, 2 to go backward, or 0 to exit: ");
        choice = getInput();

        if (choice == -2) { // Hatalı giriş
            handleInputError();
            continue;
        }

        if (choice == 1) {
            next = (XORNode*)((uintptr_t)prev ^ (uintptr_t)current->xorPtr);
            prev = current;
            current = next;
        }
        else if (choice == 2) {
            next = (XORNode*)((uintptr_t)current->xorPtr ^ (uintptr_t)prev);
            prev = current;
            current = next;
        }

        if (current == NULL) {
            printf("No more tasks in this direction.\n");
            break;
        }
    } while (choice != 0);

    printf("Exiting navigation.\n");
    return 1; // Başarı durumu
}


/**
 * @brief Adds a new task to the doubly linked list of tasks.
 *
 * The `addTaskToList` function creates a new node for the given task and adds it to the end of the doubly linked list.
 * If the list is empty, the new node becomes both the head and the tail of the list. Otherwise, it is appended after the current tail.
 *
 * @param newTask The task to be added to the doubly linked list.
 */

int addTaskToList(Task newTask) {
    TaskNode* newNode = (TaskNode*)malloc(sizeof(TaskNode));
    newNode->task = newTask;
    newNode->next = NULL;
    newNode->prev = tail;

    if (tail) {
        tail->next = newNode;
    }
    else {
        head = newNode;
    }
    tail = newNode;
    return 1;
}


/**
 * @brief Kullanıcıdan bir görev bilgisi alır ve görev listesine ekler.
 *
 * @param taskList Görev listesinin tutulduğu dizi.
 * @param taskCount Mevcut görev sayısı (pointer ile güncellenecek).
 * @param maxTasks Maksimum görev sayısı.
 * @return 1: Başarıyla eklendi, 0: Liste dolu.
 */
int addTask(Task taskList[], int* taskCount, int maxTasks) {
    if (*taskCount >= maxTasks) {
        printf("Task list is full. Cannot add more tasks.\n");
        return 0;
    }

    Task newTask;
    newTask.id = *taskCount + 1;
    newTask.dependencyCount = 0;

    printf("Enter Task Name: ");
    fgets(newTask.name, sizeof(newTask.name), stdin);
    newTask.name[strcspn(newTask.name, "\n")] = 0;

    printf("Enter Task Description: ");
    fgets(newTask.description, sizeof(newTask.description), stdin);
    newTask.description[strcspn(newTask.description, "\n")] = 0;

    printf("Enter Task Category: ");
    fgets(newTask.category, sizeof(newTask.category), stdin);
    newTask.category[strcspn(newTask.category, "\n")] = 0;

    printf("Enter Due Date (YYYY-MM-DD): ");
    fgets(newTask.dueDate, sizeof(newTask.dueDate), stdin);
    newTask.dueDate[strcspn(newTask.dueDate, "\n")] = 0;

    printf("Enter number of dependencies: ");
    scanf("%d", &newTask.dependencyCount);
    getchar();

    for (int i = 0; i < newTask.dependencyCount; i++) {
        printf("Enter dependency task ID for dependency %d: ", i + 1);
        scanf("%d", &newTask.dependencies[i]);
        getchar();
    }

    taskList[*taskCount] = newTask;
    (*taskCount)++;

    enqueue(newTask);
    push(newTask);
    saveTasks(taskList, *taskCount);

    addTaskToList(newTask);

    printf("Task added and saved successfully!\n");
    return 1;
}

/**
 * @brief Navigates through tasks in the doubly linked list.
 *
 * The `navigateTasks` function allows the user to navigate through the tasks in the doubly linked list.
 * The user can move to the next or previous task or exit the navigation at any time.
 * It provides a convenient way to view the details of each task sequentially.
 */
void navigateTasks() {
    if (!head) {
        printf("No tasks available for navigation.\n");
        return;
    }

    TaskNode* current = head;
    int choice;

    while (1) {
        printf("\nCurrent Task:\n");
        printf("ID: %d\n", current->task.id);
        printf("Name: %s\n", current->task.name);
        printf("Description: %s\n", current->task.description);
        printf("Category: %s\n", current->task.category);
        printf("Due Date: %s\n", current->task.dueDate);

        printf("\n1. Next\n2. Previous\n3. Exit\nChoose an option: ");
        choice = getInput();

        if (choice == 1) {
            if (current->next) {
                current = current->next;
            }
            else {
                printf("This is the last task.\n");
            }
        }
        else if (choice == 2) {
            if (current->prev) {
                current = current->prev;
            }
            else {
                printf("This is the first task.\n");
            }
        }
        else if (choice == 3) {
            break;
        }
        else {
            printf("Invalid choice. Try again.\n");
        }
    }
}

/**
 * @brief Displays all tasks in the queue.
 *
 * The `viewTask` function displays all tasks currently stored in the queue by dequeuing them one by one.
 * If no tasks are found, it notifies the user that the task list is empty.
 */
int viewTask() {
    if (front == NULL) {
        printf("No tasks found. The task list is empty.\n");
        enterToContinue();
        return 0; // Hata durumu: Kuyruk boş
    }

    printf("\n--- List of Tasks ---\n");
    while (front != NULL) {
        Task task = dequeue();
        printf("ID: %d\n", task.id);
        printf("Name: %s\n", task.name);
        printf("Description: %s\n", task.description);
        printf("Category: %s\n", task.category);
        printf("Due Date: %s\n", task.dueDate);
        printf("---------------------------\n");
    }
    enterToContinue();
    return 1; // Başarı durumu
}


/**
 * @brief Displays tasks filtered by a specific category.
 *
 * The `categorizeTask` function reads tasks from a binary file and displays those that match the user's chosen category.
 * If the file cannot be opened or no tasks match the specified category, appropriate messages are displayed.
 *
 * @note The function prompts the user to enter the category name to filter tasks.
 */
void categorizeTask() {
    FILE* file = fopen("tasks.bin", "rb");  
    if (!file) {
        printf("Error: Could not open tasks file or no tasks found.\n");
        enterToContinue();
        return;
    }

    char category[50];
    printf("Enter category to filter: ");
    fgets(category, sizeof(category), stdin);
    category[strcspn(category, "\n")] = 0;  

    Task task;
    int found = 0;  

    printf("\n--- Tasks in Category '%s' ---\n", category);
    while (fread(&task, sizeof(Task), 1, file)) {
        if (strcmp(task.category, category) == 0) {
            printf("ID: %d\n", task.id);
            printf("Name: %s\n", task.name);
            printf("Description: %s\n", task.description);
            printf("Due Date: %s\n", task.dueDate);
            printf("---------------------------\n");
            found = 1;
        }
    }

    if (!found) {
        printf("No tasks found in this category.\n");
    }

    fclose(file);  
    enterToContinue();  
}


/**
 * @brief Saves all tasks to a binary file.
 *
 * The `saveTasks` function writes the tasks from the given task list to a binary file, ensuring persistent storage.
 * If the file cannot be opened, an error message is displayed.
 *
 * @param taskList Array of tasks to be saved.
 * @param taskCount The number of tasks in the task list.
 */
void saveTasks(const Task taskList[], int taskCount) {
    FILE* file = fopen("tasks.bin", "wb");
    if (file == NULL) {
        printf("Error opening file for saving tasks.\n");
        return;
    }

    fwrite(taskList, sizeof(Task), taskCount, file);
    fclose(file);
    printf("Tasks saved successfully!\n");
}




/**
 * @brief Loads tasks from a binary file into the task list.
 *
 * The `loadTasks` function reads tasks from a specified binary file ("tasks.bin") and loads them into the provided task list.
 * If the file does not exist, it notifies the user and returns 0 tasks loaded.
 *
 * @param taskList The array where the tasks will be loaded.
 * @param maxTasks The maximum number of tasks that can be loaded into the task list.
 * @return The number of tasks successfully loaded.
 */
int loadTasks(Task taskList[], int maxTasks) {
    FILE* file = fopen("tasks.bin", "rb");  
    if (file == NULL) {
        printf("No previous tasks found.\n");
        return 0;  
    }

    int taskCount = fread(taskList, sizeof(Task), maxTasks, file);
    fclose(file);

    printf("%d tasks loaded successfully!\n", taskCount);
    return taskCount;  
}

/**
 * @brief Adds a task to the queue.
 *
 * The `enqueue` function adds a new task to the end of the queue. If the queue is empty, the new task
 * becomes both the front and rear of the queue.
 *
 * @param task The task to be added to the queue.
 */
void enqueue(Task task) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->task = task;
    newNode->next = NULL;

    if (rear == NULL) {
        front = rear = newNode;
        return;
    }
    rear->next = newNode;
    rear = newNode;
}

/**
 * @brief Removes and returns the task at the front of the queue.
 *
 * The `dequeue` function removes the task at the front of the queue and returns it.
 * If the queue is empty, it returns a task with an ID of -1 to indicate an error.
 *
 * @return The task removed from the front of the queue.
 */
Task dequeue() {
    if (front == NULL) {
        printf("Queue is empty\n");
        Task emptyTask;
        emptyTask.id = -1; 
        return emptyTask;
    }
    QueueNode* temp = front;
    Task task = temp->task;
    front = front->next;

    if (front == NULL) {
        rear = NULL;
    }
    free(temp);
    return task;
}

/**
 * @brief Adds a task to the stack.
 *
 * The `push` function adds a new task to the top of the stack.
 *
 * @param task The task to be added to the stack.
 */
int push(Task task) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (newNode == NULL) {
        printf("Memory allocation failed for StackNode.\n");
        return -1; // Bellek tahsisi başarısız
    }
    newNode->task = task;
    newNode->next = stackTop;
    stackTop = newNode;
    return 1; // Başarılı
}


/**
 * @brief Removes and returns the task at the top of the stack.
 *
 * The `pop` function removes the task at the top of the stack and returns it.
 * If the stack is empty, it returns a task with an ID of -1 to indicate an error.
 *
 * @return The task removed from the top of the stack.
 */
Task pop() {
    if (stackTop == NULL) {
        printf("Stack is empty\n");
        Task emptyTask;
        emptyTask.id = -1; 
        return emptyTask;
    }
    StackNode* temp = stackTop;
    Task task = temp->task;
    stackTop = stackTop->next;
    free(temp);
    return task;
}


/**
 * @brief Undoes the last added task by removing it from the task list.
 *
 * The `undoLastTask` function removes the last added task from the task list by popping it from the stack.
 * If no task is available to undo, an appropriate message is displayed. The function also updates the
 * task storage file after the undo operation.
 *
 * @param taskList The list of tasks.
 * @param taskCount Pointer to the current count of tasks.
 */
int undoLastTask(Task taskList[], int* taskCount) {
    Task lastTask = pop(); // Son görevi yığından çıkar
    if (lastTask.id == -1) { // Eğer yığın boşsa
        printf("No tasks to undo.\n");
        return -1; // Hata: Undo işlemi yapılamadı
    }

    (*taskCount)--; // Görev sayısını azalt
    printf("Last task '%s' undone successfully.\n", lastTask.name);

    saveTasks(taskList, *taskCount); // Güncellenmiş görevleri kaydet
    return 1; // Başarı durumu
}


/**
 * @brief Utility function to print the dependencies of a given task using DFS.
 *
 * The `printDependenciesUtil` function is a helper function that performs a Depth-First Search (DFS) to print
 * all dependencies for a given task. It uses a recursive approach to find and display all direct and indirect
 * dependencies.
 *
 * @param taskList The list of tasks.
 * @param taskId The ID of the task for which dependencies are being printed.
 * @param visited Array to keep track of visited tasks to avoid cycles.
 */
int printDependenciesUtil(Task taskList[], int taskId, bool visited[]) {
    if (visited[taskId]) {
        return 1; // Daha önce ziyaret edildi, işleme devam etmeyin
    }
    visited[taskId] = true;

    Task task = taskList[taskId - 1]; // Görevi al

    if (task.dependencyCount == 0) {
        return 1; // Hiç bağımlılığı yok, işleme devam etmeyin
    }

    for (int i = 0; i < task.dependencyCount; i++) {
        int dependencyId = task.dependencies[i];
        printf("Task %d depends on Task %d\n", task.id, dependencyId);

        // Rekürsif olarak bağımlılıkları kontrol et
        int result = printDependenciesUtil(taskList, dependencyId, visited);
        if (result == -1) {
            return -1; // Alt bağımlılıklarda bir hata oluştu
        }
    }

    return 1; // Başarıyla tamamlandı
}


/**
 * @brief Prints all dependencies of a given task.
 *
 * The `printDependencies` function prints all direct and indirect dependencies of the given task ID.
 * It initializes a visited array to keep track of the tasks that have been visited during the traversal.
 *
 * @param taskList The list of tasks.
 * @param taskCount The number of tasks in the list.
 * @param startTaskId The ID of the task for which dependencies are to be printed.
 */

int printDependencies(Task taskList[], int taskCount, int startTaskId) {
    if (startTaskId <= 0 || startTaskId > taskCount) {
        printf("Invalid task ID: %d. Task ID must be between 1 and %d.\n", startTaskId, taskCount);
        return -1; // Hata: Geçersiz task ID
    }

    bool visited[MAX_TASKS] = { false };
    printf("Dependencies for Task %d:\n", startTaskId);

    // Bağımlılıkları yazdır
    printDependenciesUtil(taskList, startTaskId, visited);

    return 1; // Başarı durumu
}

/**
 * @brief Pushes a vertex to the SCC stack.
 *
 * The `pushSccStack` function adds a vertex to the stack used during the Strongly Connected Components (SCC) analysis.
 * This function is typically used during the Tarjan's or Kosaraju's algorithm implementation.
 *
 * @param v The vertex to be added to the SCC stack.
 */
int pushSccStack(int v) {
    AdjacencyNode* newNode = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    if (newNode == NULL) {
        printf("Memory allocation failed for AdjacencyNode.\n");
        return -1;
    }
    newNode->data = v;
    newNode->next = sccStack;
    sccStack = newNode;
    return 1;
}

/**
 * @brief Pops the top element from the SCC stack.
 *
 * The `popSccStack` function removes and returns the top element from the stack used for Strongly Connected Components (SCC)
 * analysis. If the stack is empty, it returns -1.
 *
 * @return The data value of the top element of the SCC stack, or -1 if the stack is empty.
 */
int popSccStack() {
    if (sccStack == NULL) return -1;
    int top = sccStack->data;
    AdjacencyNode* temp = sccStack;
    sccStack = sccStack->next;
    free(temp);
    return top;
}

/**
 * @brief Depth-First Search (DFS) utility function for SCC analysis.
 *
 * The `dfsUtil` function performs a Depth-First Search (DFS) on the graph to explore and mark all reachable vertices from
 * a given starting vertex. It uses recursion and is utilized to find SCCs and their components.
 *
 * @param v The starting vertex for the DFS traversal.
 * @param visited Array to track visited vertices.
 * @param adj The adjacency list representing the graph.
 * @param component Pointer to the SCC stack for storing vertices in the current component.
 */
int dfsUtil(int v, int visited[], AdjacencyNode* adj[], AdjacencyNode** component) {
    if (visited[v]) {
        return 0; // Düğüm zaten ziyaret edilmiş
    }

    visited[v] = 1;
    pushSccStack(v); // SCC yığınına ekle

    AdjacencyNode* temp = adj[v];
    while (temp != NULL) {
        if (!visited[temp->data]) {
            dfsUtil(temp->data, visited, adj, component);
        }
        temp = temp->next;
    }

    return 1; // DFS başarıyla tamamlandı
}

/**
 * @brief Finds and prints all Strongly Connected Components (SCCs) in the graph.
 *
 * The `findSCCs` function finds all SCCs in a directed graph represented by an adjacency list. It first performs a DFS
 * to fill the stack with vertices based on their finishing time. Then it transposes the graph, and finally,
 * performs another DFS to discover and print all SCCs.
 *
 * @param V The number of vertices in the graph.
 * @param adj The adjacency list representing the graph.
 * @param out The output file where SCCs are printed.
 */
int findSCCs(int V, AdjacencyNode* adj[], FILE* out) {
    if (V <= 0 || adj == NULL || out == NULL) {
        fprintf(stderr, "Invalid input parameters.\n");
        return -1; // Hata durumu
    }

    int visited[MAX_TASKS] = { 0 };

    // Orijinal graf üzerinde DFS
    for (int i = 0; i < V; i++) {
        if (!visited[i]) {
            dfsUtil(i, visited, adj, NULL);
        }
    }

    // Transpoz graf oluşturma
    AdjacencyNode* transpose[MAX_TASKS] = { NULL };
    for (int v = 0; v < V; v++) {
        AdjacencyNode* temp = adj[v];
        while (temp != NULL) {
            int u = temp->data;
            AdjacencyNode* newNode = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
            if (!newNode) {
                fprintf(stderr, "Memory allocation failed.\n");
                return -1; // Hata durumu
            }
            newNode->data = v;
            newNode->next = transpose[u];
            transpose[u] = newNode;
            temp = temp->next;
        }
    }

    memset(visited, 0, sizeof(visited));
    int numSCC = 0;

    // Transpoz graf üzerinde DFS
    while (sccStack != NULL) {
        int v = popSccStack();

        if (!visited[v]) {
            fprintf(out, "SCC #%d: ", ++numSCC);
            dfsUtil(v, visited, transpose, &sccStack);
            while (sccStack != NULL) {
                fprintf(out, "%d ", popSccStack());
            }
            fprintf(out, "\n");
        }
    }

    // Transpoz grafı temizle
    for (int i = 0; i < V; i++) {
        while (transpose[i] != NULL) {
            AdjacencyNode* temp = transpose[i];
            transpose[i] = transpose[i]->next;
            free(temp);
        }
    }

    return 1; // Başarılı durum
}


/**
 * @brief Analyzes Strongly Connected Components (SCCs) in the task dependency graph.
 *
 * The `analyzeSCC` function creates an adjacency list representing the dependencies between tasks
 * and finds all Strongly Connected Components (SCCs) in the graph. It outputs the SCCs to the specified file.
 * After processing, it deallocates any dynamically allocated memory for the adjacency list.
 *
 * @param taskList The list of tasks containing dependencies.
 * @param taskCount The number of tasks in the list.
 * @param out The output file where the SCCs will be printed.
 * @return Always returns 1 to indicate successful analysis.
 */
int analyzeSCC(Task taskList[], int taskCount, FILE* out) {
    if (taskList == NULL || taskCount <= 0 || out == NULL) {
        fprintf(stderr, "Invalid input parameters.\n");
        return -1; // Hata durumu
    }

    AdjacencyNode* adj[MAX_TASKS] = { NULL };

    // Görevlerin bağımlılıklarını oluştur
    for (int i = 0; i < taskCount; i++) {
        Task task = taskList[i];
        for (int j = 0; j < task.dependencyCount; j++) {
            int dep = task.dependencies[j];
            AdjacencyNode* newNode = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
            if (!newNode) {
                fprintf(stderr, "Memory allocation failed.\n");
                return -1; // Bellek tahsisi hatası
            }
            newNode->data = dep - 1;
            newNode->next = adj[task.id - 1];
            adj[task.id - 1] = newNode;
        }
    }

    // SCC'leri analiz et
    int result = findSCCs(taskCount, adj, out);
    if (result != 1) {
        fprintf(stderr, "Failed to find SCCs.\n");
        return -1; // SCC analiz hatası
    }

    // Bağımlılık listesini temizle
    for (int i = 0; i < taskCount; i++) {
        while (adj[i] != NULL) {
            AdjacencyNode* temp = adj[i];
            adj[i] = adj[i]->next;
            free(temp);
        }
    }

    return 1; // Başarı durumu
}



/**
 * @brief Constructs the prefix table for the Knuth-Morris-Pratt (KMP) algorithm.
 *
 * The `computePrefixTable` function constructs the prefix table used by the KMP string matching algorithm.
 * This table helps in determining the next positions to be compared during mismatches in the pattern.
 *
 * @param pattern The pattern to be searched.
 * @param prefixTable The prefix table to be filled.
 * @param patternLength The length of the pattern.
 */
int computePrefixTable(const char* pattern, int* prefixTable, int patternLength) {
    if (pattern == NULL || prefixTable == NULL || patternLength <= 0) {
        return -1; // Error: Invalid input parameters
    }

    int length = 0;
    prefixTable[0] = 0;

    for (int i = 1; i < patternLength; i++) {
        while (length > 0 && pattern[i] != pattern[length]) {
            length = prefixTable[length - 1];
        }

        if (pattern[i] == pattern[length]) {
            length++;
        }
        prefixTable[i] = length;
    }
    return 1; // Success
}


/**
 * @brief Searches for a pattern in the given text using the Knuth-Morris-Pratt (KMP) algorithm.
 *
 * The `KMPsearch` function searches for the given pattern in the provided text using the KMP algorithm.
 * If the pattern is found, the function returns 1, otherwise it returns 0.
 *
 * @param text The text in which the pattern is to be searched.
 * @param pattern The pattern to be searched in the text.
 * @return Returns 1 if the pattern is found, otherwise returns 0.
 */
int KMPsearch(const char* text, const char* pattern) {
    if (text == NULL || pattern == NULL) {
        return -1; // Error: Invalid input
    }

    int textLength = strlen(text);
    int patternLength = strlen(pattern);

    if (patternLength == 0 || textLength == 0) {
        return -1; // Error: Invalid input (empty text or pattern)
    }

    int* prefixTable = (int*)malloc(sizeof(int) * patternLength);
    if (!prefixTable) {
        return -2; // Error: Memory allocation failed
    }

    computePrefixTable(pattern, prefixTable, patternLength);

    int i = 0;
    int j = 0;
    while (i < textLength) {
        if (pattern[j] == text[i]) {
            j++;
            i++;
        }
        if (j == patternLength) {
            free(prefixTable);
            return 1; // Match found
        }
        else if (i < textLength && pattern[j] != text[i]) {
            if (j != 0) {
                j = prefixTable[j - 1];
            }
            else {
                i++;
            }
        }
    }
    free(prefixTable);
    return 0; // Match not found
}



/**
 * @brief Searches tasks for a specific keyword in their descriptions.
 *
 * The `searchTasksByKeyword` function searches for a given keyword in the descriptions of tasks stored in a binary file.
 * It uses the KMP search algorithm to find the keyword in each task description. If tasks are found that contain the keyword,
 * their details are displayed. If no tasks match, an appropriate message is shown.
 *
 * @note The keyword is case-sensitive.
 */
void searchTasksByKeyword() {
    FILE* file = fopen("tasks.bin", "rb");
    if (!file) {
        printf("Error opening tasks file.\n");
        return;
    }

    char keyword[256];
    printf("Enter the keyword to search in task descriptions: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0;  

    Task task;
    int found = 0;

    printf("\nTasks containing the keyword '%s' in their descriptions:\n", keyword);
    printf("----------------------------------------------------\n");

    while (fread(&task, sizeof(Task), 1, file)) {
        if (KMPsearch(task.description, keyword)) {
            printf("ID: %d\n", task.id);
            printf("Name: %s\n", task.name);
            printf("Description: %s\n", task.description);
            printf("Category: %s\n", task.category);
            printf("Due Date: %s\n", task.dueDate);
            printf("----------------------------------------------------\n");
            found = 1;
        }
    }

    if (!found) {
        printf("No tasks found with the keyword '%s'.\n", keyword);
    }

    fclose(file);
}




/**
 * @brief Displays the deadline settings menu for managing deadlines.
 *
 * The `deadlineSettingsMenu` function presents the user with options to manage deadlines. The options include:
 * assigning a deadline, viewing all deadlines, and exiting the menu.
 * The menu runs in a loop until the user chooses to exit.
 *
 * @return Returns 0 when the user chooses to exit the menu.
 */
int deadlineSettingsMenu() {
    int choice;
    Assignment assignment;  

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
            assign_deadline(&assignment);
            enterToContinue();
            break;
        case 2:
            viewDeadlines();
            enterToContinue();
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

/**
 * @brief Initializes a MinHeap for managing deadlines.
 *
 * The `deadlineHeap` structure is initialized with an array of tasks, all set to zero initially.
 * The second field represents the size of the heap, which is initially set to 0.
 */
MinHeap deadlineHeap = { {0}, 0 };

/**
 * @brief Assigns a deadline to a given assignment.
 *
 * The `assign_deadline` function prompts the user to enter the task name and the deadline (day, month, year).
 * It checks the validity of the input date and assigns the deadline to the given assignment.
 * The assignment is then added to a MinHeap to maintain a list of deadlines and saved to a binary file.
 *
 * @param assignment Pointer to the assignment to which the deadline will be assigned.
 * @return Returns 0 if the deadline is assigned and saved successfully, or -1 if an error occurs.
 */
int assign_deadline(Assignment* assignment) {
    char taskName[MAX_ASSIGNMENT_NAME];
    int day, month, year;

    printf("Enter Task Name: ");
    while (getchar() != '\n');  
    if (fgets(taskName, MAX_ASSIGNMENT_NAME, stdin) == NULL) {
        printf("Error reading task name.\n");
        return -1;
    }
    taskName[strcspn(taskName, "\n")] = '\0';  

    printf("Enter Deadline (day month year): ");
    if (scanf("%d %d %d", &day, &month, &year) != 3) {
        printf("Invalid input! Please try again.\n");
        while (getchar() != '\n');  
        return -1;
    }

    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900) {
        printf("Invalid date! Please enter a valid date.\n");
        return -1;
    }

    strncpy(assignment->name, taskName, MAX_ASSIGNMENT_NAME - 1);
    assignment->name[MAX_ASSIGNMENT_NAME - 1] = '\0';  

    assignment->day = day;
    assignment->month = month;
    assignment->year = year;

    insertMinHeap(&deadlineHeap, *assignment);

    FILE* file = fopen("deadlines.bin", "ab");
    if (!file) {
        printf("Error: Could not open deadlines file for writing.\n");
        return -1;
    }

    fwrite(assignment, sizeof(Assignment), 1, file);
    fclose(file);

    printf("Deadline assigned and saved successfully!\n");
    return 0;
}


/**
 * @brief Displays upcoming deadlines in sorted order.
 *
 * The `viewDeadlines` function displays all deadlines stored in a MinHeap. It uses a copy of the heap
 * to maintain the original heap order. If no deadlines are present, it notifies the user.
 *
 * @return Returns 1 if deadlines are successfully displayed, or -1 if no deadlines are found.
 */
int viewDeadlines() {
    if (deadlineHeap.size == 0) {
        printf("No deadlines to display.\n");
        return -1;
    }

    printf("\n--- Upcoming Deadlines (Sorted by Date) ---\n");
    printf("-------------------------------------------\n");

    MinHeap tempHeap = deadlineHeap;  

    int taskCount = 0;
    while (tempHeap.size > 0) {
        Assignment deadline = extractMin(&tempHeap);
        printf("%d. Task: %s - Deadline: %02d/%02d/%04d\n",
            ++taskCount,
            deadline.name,
            deadline.day,
            deadline.month,
            deadline.year);
    }

    if (taskCount == 0) {
        printf("No deadlines to display.\n");
    }

    printf("-------------------------------------------\n");
    enterToContinue();  
    return 1;
}


/**
 * @brief Generates a key for a given date in YYYYMMDD format.
 *
 * The `getDateKey` function creates an integer key based on the given day, month, and year in the format YYYYMMDD.
 * This key is useful for indexing and searching tasks by date.
 *
 * @param day The day of the date.
 * @param month The month of the date.
 * @param year The year of the date.
 * @return Returns an integer representing the date in YYYYMMDD format.
 */
int getDateKey(int day, int month, int year) {
    return year * 10000 + month * 100 + day;  
}


/**
 * @brief Inserts a key and task into a leaf node of the B+ Tree.
 *
 * The `insertInLeaf` function inserts a given key and corresponding task into a specified leaf node
 * of the B+ Tree, maintaining the order of keys.
 *
 * @param leaf Pointer to the leaf node where the key and task are to be inserted.
 * @param key The key to be inserted in the leaf.
 * @param task Pointer to the task to be inserted.
 * @return Returns 1 if the insertion is successful.
 */
int insertInLeaf(BPlusTreeNode* leaf, int key, ScheduledTask* task) {
    int i = leaf->numKeys - 1;
    while (i >= 0 && leaf->keys[i] > key) {
        leaf->keys[i + 1] = leaf->keys[i];
        leaf->tasks[i + 1] = leaf->tasks[i];
        i--;
    }
    leaf->keys[i + 1] = key;
    leaf->tasks[i + 1] = task;
    leaf->numKeys++;
    return 1;
}

/**
 * @brief Inserts a task into the B+ Tree.
 *
 * The `insertInBPlusTree` function inserts a given task into the B+ Tree. If the root node has space, the task is added directly.
 * Otherwise, a node split is required, and further logic for splitting should be implemented.
 *
 * @param tree Pointer to the B+ Tree where the task is to be inserted.
 * @param task Pointer to the task to be inserted.
 * @return Returns 1 if the insertion is successful.
 */
int insertInBPlusTree(BPlusTree* tree, ScheduledTask* task) {
    int key = getDateKey(task->day, task->month, task->year);
    BPlusTreeNode* root = tree->root;

    if (root->numKeys < MAX_KEYS) {
        insertInLeaf(root, key, task);
    }
    else {
        printf("Node splitting required. Implement split logic here.\n");
    }
    return 1;
}

/**
 * @brief Searches for tasks within a specific date range in the B+ Tree.
 *
 * The `searchInDateRange` function searches for tasks with keys within the given date range in the B+ Tree.
 * It traverses the B+ Tree, printing tasks that fall within the specified start and end keys.
 *
 * @param node Pointer to the current B+ Tree node being searched.
 * @param startKey The starting key of the date range.
 * @param endKey The ending key of the date range.
 * @return Returns 1 if the search is successful, or -1 if the node is NULL.
 */
int searchInDateRange(BPlusTreeNode* node, int startKey, int endKey) {
    if (node == NULL) return -1;

    int i = 0;
    while (i < node->numKeys && node->keys[i] < startKey) i++;

    if (node->isLeaf) {
        while (i < node->numKeys && node->keys[i] <= endKey) {
            printf("Task: %s, Deadline: %02d/%02d/%04d\n",
                node->tasks[i]->name,
                node->tasks[i]->day,
                node->tasks[i]->month,
                node->tasks[i]->year);
            i++;
        }
        if (node->next != NULL && node->keys[node->numKeys - 1] <= endKey) {
            searchInDateRange(node->next, startKey, endKey);
        }
    }
    else {
        while (i <= node->numKeys) {
            searchInDateRange(node->children[i], startKey, endKey);
            i++;
        }
    }
    return 1;
}

/**
 * @brief Displays tasks with deadlines in a specific date range.
 *
 * The `viewDeadlinesInRange` function prompts the user to enter a start and end date.
 * It then searches for and displays all tasks with deadlines within that date range.
 *
 * @param tree Pointer to the B+ Tree that stores tasks.
 * @return Returns 1 if the operation is successful.
 */
int viewDeadlinesInRange(BPlusTree* tree) {
    int startDay, startMonth, startYear;
    int endDay, endMonth, endYear;

    printf("Enter start date (day month year): ");
    scanf("%d %d %d", &startDay, &startMonth, &startYear);
    printf("Enter end date (day month year): ");
    scanf("%d %d %d", &endDay, &endMonth, &endYear);

    int startKey = getDateKey(startDay, startMonth, startYear);
    int endKey = getDateKey(endDay, endMonth, endYear);

    printf("\n--- Tasks between %02d/%02d/%04d and %02d/%02d/%04d ---\n",
        startDay, startMonth, startYear, endDay, endMonth, endYear);

    searchInDateRange(tree->root, startKey, endKey);
    return 1;
}

/**
 * @brief Swaps two assignments.
 *
 * The `swap` function exchanges the data of two given assignments.
 *
 * @param a Pointer to the first assignment.
 * @param b Pointer to the second assignment.
 * @return Returns 1 after successfully swapping the assignments.
 */
int swap(Assignment* a, Assignment* b) {
    Assignment temp = *a;
    *a = *b;
    *b = temp;
    return 1;
}

/**
 * @brief Maintains the min-heap property by adjusting the subtree rooted at a given index.
 *
 * The `heapify` function ensures that the min-heap property is maintained for the subtree rooted at index `i`.
 * It compares the parent node with its children and swaps nodes as needed to ensure the minimum value is at the root.
 *
 * @param heap Pointer to the MinHeap.
 * @param i The index of the subtree root to heapify.
 * @return Returns 1 if the operation is successful.
 */
int heapify(MinHeap* heap, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heap->size &&
        (heap->deadlines[left].year < heap->deadlines[smallest].year ||
            (heap->deadlines[left].year == heap->deadlines[smallest].year && heap->deadlines[left].month < heap->deadlines[smallest].month) ||
            (heap->deadlines[left].year == heap->deadlines[smallest].year && heap->deadlines[left].month == heap->deadlines[smallest].month && heap->deadlines[left].day < heap->deadlines[smallest].day))) {
        smallest = left;
    }

    if (right < heap->size &&
        (heap->deadlines[right].year < heap->deadlines[smallest].year ||
            (heap->deadlines[right].year == heap->deadlines[smallest].year && heap->deadlines[right].month < heap->deadlines[smallest].month) ||
            (heap->deadlines[right].year == heap->deadlines[smallest].year && heap->deadlines[right].month == heap->deadlines[smallest].month && heap->deadlines[right].day < heap->deadlines[smallest].day))) {
        smallest = right;
    }

    if (smallest != i) {
        swap(&heap->deadlines[i], &heap->deadlines[smallest]);
        heapify(heap, smallest);
    }
    return 1;
}

/**
 * @brief Inserts a new task into the min-heap based on its deadline.
 *
 * The `insertMinHeap` function adds a new task to the min-heap while maintaining the min-heap property.
 * It compares the deadline of the new task with its parent and moves it up if needed.
 *
 * @param heap Pointer to the MinHeap.
 * @param deadline The new task to be inserted into the heap.
 * @return Returns 1 if the insertion is successful, or -1 if the heap is full.
 */
int insertMinHeap(MinHeap* heap, Assignment deadline) {
    if (heap->size == MAX_HEAP_SIZE) {
        printf("Heap is full\n");
        return -1;
    }

    heap->size++;
    int i = heap->size - 1;
    heap->deadlines[i] = deadline;

    while (i != 0 &&
        (heap->deadlines[(i - 1) / 2].year > heap->deadlines[i].year ||
            (heap->deadlines[(i - 1) / 2].year == heap->deadlines[i].year && heap->deadlines[(i - 1) / 2].month > heap->deadlines[i].month) ||
            (heap->deadlines[(i - 1) / 2].year == heap->deadlines[i].year && heap->deadlines[(i - 1) / 2].month == heap->deadlines[i].month && heap->deadlines[(i - 1) / 2].day > heap->deadlines[i].day))) {
        swap(&heap->deadlines[i], &heap->deadlines[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
    return 1;
}

/**
 * @brief Extracts the task with the earliest deadline from the min-heap.
 *
 * The `extractMin` function removes and returns the task with the earliest deadline from the min-heap.
 * It then re-heapifies the heap to maintain the min-heap property.
 *
 * @param heap Pointer to the MinHeap.
 * @return The task with the earliest deadline, or an empty task if the heap is empty.
 */
Assignment extractMin(MinHeap* heap) {
    if (heap->size <= 0) {
        printf("Heap is empty\n");
        Assignment emptyAssignment;
        emptyAssignment.day = emptyAssignment.month = emptyAssignment.year = -1;
        return emptyAssignment;
    }
    if (heap->size == 1) {
        heap->size--;
        return heap->deadlines[0];
    }

    Assignment root = heap->deadlines[0];
    heap->deadlines[0] = heap->deadlines[heap->size - 1];
    heap->size--;
    heapify(heap, 0);

    return root;
}

/**
 * @brief Displays the reminder system menu and handles user input.
 *
 * The `reminderSystemMenu` function displays a menu for the reminder system,
 * allowing the user to set reminders or adjust notification settings.
 * The menu runs in a loop until the user chooses to exit.
 *
 * @return Returns 0 when the user chooses to exit the menu.
 */
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
            setReminders();
            break;
        case 2:
            notificationSettings();
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



/**
 * @brief Sets a reminder by prompting the user for a specific duration and displaying a countdown.
 *
 * The function prompts the user to input a reminder duration in days, hours, minutes, and seconds.
 * It then calculates the total duration in seconds and starts a countdown timer to display the time
 * remaining in the format "Days:Hours:Minutes:Seconds".
 *
 * If the entered duration is invalid (i.e., less than or equal to zero), the user is prompted to enter
 * a valid duration and the function returns without setting the reminder.
 *
 * @return int Returns 0 if the duration is invalid, otherwise continues with the countdown.
 *
 * @details
 * - Uses `getInput()` to get input values for days, hours, minutes, and seconds.
 * - Uses `clearScreen()` to clear the console between countdown displays.
 * - The countdown is handled by a for loop that decreases the total number of seconds by 1.
 * - Each second, `platformSleep(1)` is called to make the program wait for 1 second.
 * - After the countdown reaches zero, a message is displayed informing the user that the reminder is triggered.
 *
 * @note The time remaining is displayed in the format Days:Hours:Minutes:Seconds.
 *
 * Example Usage:
 * @code
 *     int result = setReminders();
 *     if (result == 0) {
 *         // Handle invalid duration
 *     }
 * @endcode
 *
 * @todo Implement `getInput()`, `clearScreen()`, `enterToContinue()`, and `platformSleep(int seconds)` functions to work across different platforms.
 */
int setReminders() {
    clearScreen();

    int seconds, minutes, hours, days;

    printf("Enter the reminder duration:\n");
    printf("Days: ");
    days = getInput();
    printf("Hours: ");
    hours = getInput();
    printf("Minutes: ");
    minutes = getInput();
    printf("Seconds: ");
    seconds = getInput();

    // Convert total duration to seconds
    int totalSeconds = seconds + minutes * 60 + hours * 3600 + days * 86400;

    if (totalSeconds <= 0) {
        printf("Invalid duration. Please enter a positive duration.\n");
        enterToContinue();
        return 0;
    }

    printf("Setting reminder for %d seconds...\n", totalSeconds);

    // Countdown loop
    for (int remaining = totalSeconds; remaining > 0; --remaining) {
        clearScreen();
        printf("Time remaining: %02d:%02d:%02d:%02d\n",
            remaining / 86400,            // Days
            (remaining % 86400) / 3600,   // Hours
            (remaining % 3600) / 60,      // Minutes
            remaining % 60);              // Seconds

        platformSleep(1);  // Wait for 1 second
    }

    printf("Time's up! Reminder triggered.\n");
    enterToContinue();
}


/**
 * @brief Platform-independent sleep function.
 *
 * This function pauses the execution of the program for the specified number of seconds.
 * It uses `Sleep()` on Windows and `sleep()` on Linux/macOS.
 *
 * @param seconds The number of seconds to sleep.
 *
 * @return int Returns 1 after sleeping for the given duration.
 */
int platformSleep(int seconds) {
#ifdef _WIN32
    Sleep(seconds * 1000);  // Windows: wait in milliseconds
#else
    sleep(seconds);         // Linux/macOS: wait in seconds
#endif
    return 1;
}






/**
 * @brief Configures notification settings by allowing the user to select a notification method.
 *
 * The function displays the current notification method and prompts the user to select a new notification method
 * from the available options: SMS, E-Mail, or Phone Call.
 * If an invalid choice is made, the function is called recursively until a valid choice is entered.
 *
 * @return int Returns 0 if the notification method is successfully set, otherwise returns 1 for invalid input.
 */
int notificationSettings() {
    clearScreen();

    // Display current notification method
    showCurrentNotificationMethod();

    int choice;
    printf("Select notification method:\n");
    printf("1. SMS\n");
    printf("2. E-Mail\n");
    printf("3. Phone Call\n");
    printf("Enter your choice: ");
    choice = getInput();

    switch (choice) {
    case 1:
        notificationMethod = 1;
        printf("Your reminder has been set to SMS.\n");
        break;
    case 2:
        notificationMethod = 2;
        printf("Your reminder has been set to E-Mail.\n");
        break;
    case 3:
        notificationMethod = 3;
        printf("Your reminder has been set to Phone Call.\n");
        break;
    default:
        printf("Invalid choice. Please try again.\n");
        enterToContinue();
        notificationSettings();  // Recursively call for invalid input
        return 1;
    }

    enterToContinue();
    return 0;
}

/**
 * @brief Displays the current notification method.
 *
 * This function displays the currently selected notification method.
 * If no method is selected, it informs the user accordingly.
 *
 * @return int Returns 1 after displaying the current notification method.
 */
int showCurrentNotificationMethod() {
    if (notificationMethod == 0) {
        printf("No notification method selected yet.\n");
    }
    else {
        const char* methodStr =
            (notificationMethod == 1) ? "SMS" :
            (notificationMethod == 2) ? "E-Mail" : "Notification";

        printf("Current notification method: %s\n", methodStr);
    }
    return 1;
}

/**
 * @brief Creates a new Sparse Matrix node.
 *
 * This function allocates memory for a new node in a sparse matrix with the given row, column, and value.
 * The `next` pointer is set to NULL.
 *
 * @param row The row index of the new node.
 * @param col The column index of the new node.
 * @param value The value to be stored in the new node.
 *
 * @return SparseMatrixNode* A pointer to the newly created Sparse Matrix node.
 */
SparseMatrixNode* createNode(int row, int col, int value) {
    SparseMatrixNode* newNode = (SparseMatrixNode*)malloc(sizeof(SparseMatrixNode));
    newNode->row = row;
    newNode->col = col;
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

/**
 * @brief Adds a value to the Sparse Matrix.
 *
 * This function adds a new node with the specified row, column, and value to the sparse matrix.
 * The new node is added to the head of the linked list representing the sparse matrix.
 *
 * @param head A pointer to the head of the sparse matrix linked list.
 * @param row The row index of the new value.
 * @param col The column index of the new value.
 * @param value The value to be added to the sparse matrix.
 *
 * @return int Returns 1 after successfully adding the new value.
 */
int addNotification(SparseMatrixNode** head, int row, int col, int value) {
    SparseMatrixNode* newNode = createNode(row, col, value);
    newNode->next = *head;
    *head = newNode;
    return 1;
}

/**
 * @brief Displays all notifications in the sparse matrix.
 *
 * This function traverses the linked list representing the sparse matrix and displays the task ID, date,
 * and notification type for each node.
 *
 * @param head A pointer to the head of the sparse matrix linked list.
 *
 * @return int Returns 1 after successfully displaying all notifications.
 */
int displayNotifications(SparseMatrixNode* head) {
    SparseMatrixNode* temp = head;
    while (temp != NULL) {
        printf("Task ID: %d, Date: %d, Notification Type: %d\n", temp->row, temp->col, temp->value);
        temp = temp->next;
    }
    return 1;
}

/**
 * @brief Updates the notification method for a specific task on a specific date.
 *
 * This function adds or updates a node in the sparse matrix representing a notification for a specific task ID
 * and date with a new notification method.
 *
 * @param taskId The task ID for which the notification method is to be updated.
 * @param date The date for which the notification method is to be updated.
 * @param method The new notification method (e.g., SMS, E-Mail).
 * @param head A pointer to the head of the sparse matrix linked list.
 *
 * @return int Returns 1 after successfully updating the notification method.
 */
int updateNotificationMethod(int taskId, int date, int method, SparseMatrixNode** head) {
    addNotification(head, taskId, date, method);
    printf("Notification method updated for Task ID %d on Date %d\n", taskId, date);
    return 1;
}

/**
 * @brief Displays the current notification method for a specific task on a specific date.
 *
 * This function traverses the linked list representing the sparse matrix and finds the notification method
 * for a given task ID and date. If found, it displays the notification method; otherwise, it informs the user
 * that no notification method is selected.
 *
 * @param head A pointer to the head of the sparse matrix linked list.
 * @param taskId The task ID for which the notification method is to be displayed.
 * @param date The date for which the notification method is to be displayed.
 *
 * @return int Returns 1 if the notification method is found, otherwise returns -1.
 */
int showCurrentNotificationMethod(SparseMatrixNode* head, int taskId, int date) {
    SparseMatrixNode* temp = head;
    while (temp != NULL) {
        if (temp->row == taskId && temp->col == date) {
            const char* methodStr =
                (temp->value == 1) ? "SMS" :
                (temp->value == 2) ? "E-Mail" : "Notification";
            printf("Current notification method for Task ID %d on Date %d: %s\n", taskId, date, methodStr);
            return 1;
        }
        temp = temp->next;
    }
    printf("No notification method selected for Task ID %d on Date %d.\n", taskId, date);
    return -1;
}



/**
 * @brief Task prioritization menu allowing the user to mark task importance.
 *
 * This function displays a menu for the user to prioritize tasks by marking their importance level.
 * It provides options for marking task importance, viewing current tasks, or exiting the menu. The user
 * can select a task and assign an importance level of Low, Medium, or High.
 *
 * The menu is displayed in a loop until the user chooses to exit. If invalid input is provided, the user
 * is prompted to enter a valid option. The function handles errors and invalid choices gracefully by
 * re-prompting the user.
 *
 * @return int Returns 0 upon exiting the menu successfully.
 *
 * @details
 * - Uses `printTaskPrioritizationMenu()` to display the available options.
 * - Uses `getInput()` to read the user's choice.
 * - Allows the user to mark the importance of tasks by calling `markTaskImportance()`.
 * - Handles invalid inputs by displaying an error message and re-prompting the user.
 *
 * Example Usage:
 * @code
 *     int result = taskPrioritizationMenu();
 *     if (result == 0) {
 *         // Menu exited successfully
 *     }
 * @endcode
 */
int taskPrioritizationMenu() {
    int choice;

    while (1) {
        printTaskPrioritizationMenu();  // Display the task prioritization menu
        choice = getInput();  // Get user input

        if (choice == -2) {  // Handle invalid input case
            handleInputError();
            enterToContinue();
            continue;
        }

        switch (choice) {
        case 1:
            markTaskImportance();  // Mark the importance of a task
            break;
        case 2:
            break;
        case 3:
            return 0;  // Exit the menu
        default:
            clearScreen();
            printf("Invalid choice. Please try again.\n");
            enterToContinue();
            break;
        }
    }
}

/**
 * @brief Marks the importance level of a selected task.
 *
 * This function allows the user to select a task from the list of available tasks and mark its importance level.
 * The importance level can be set to Low, Medium, or High. The user is prompted to select a task by entering
 * the task name, and then choose the importance level by entering the corresponding importance ID.
 *
 * The updated task list is saved to ensure the importance levels are preserved for future reference.
 *
 * @details
 * - Uses `loadTasks()` to load tasks from storage.
 * - Displays all tasks along with their current importance level.
 * - Prompts the user to enter the name of the task they wish to mark.
 * - Uses `getInput()` to read the importance level from the user.
 * - Updates the importance level of the selected task and saves the updated list using `saveTasks()`.
 *
 * @note If no tasks are available, the user is informed and the function returns without making changes.
 *
 * Example Usage:
 * @code
 *     markTaskImportance();
 * @endcode
 */
void markTaskImportance() {
    clearScreen();

    Task tasks[100];  
    int taskCount = loadTasks(tasks, 100);  

    if (taskCount <= 0) {
        printf("No tasks available. Please add tasks first.\n");
        enterToContinue();
        return;
    }

    printf("Tasks loaded:\n");
    for (int i = 0; i < taskCount; ++i) {
        const char* importanceStr =
            tasks[i].impid == 1 ? "Low" :
            tasks[i].impid == 2 ? "Medium" :
            tasks[i].impid == 3 ? "High" : "Unmarked";
        printf("ID: %d, Name: %s, Importance: %s\n",
            tasks[i].id, tasks[i].name, importanceStr);
    }

    char taskName[100];
    Task* selectedTask = NULL;

    while (1) {
        printf("\nEnter the name of the task to mark importance: ");
        scanf(" %[^\n]%*c", taskName);  

        for (int i = 0; i < taskCount; ++i) {
            if (strcmp(tasks[i].name, taskName) == 0) {
                selectedTask = &tasks[i];
                break;
            }
        }

        if (selectedTask) {
            break;  
        }
        else {
            printf("Task not found! Please enter a valid task name.\n");
        }
    }

    int importanceId;
    while (1) {
        printf("Enter the importance ID (1: Low, 2: Medium, 3: High): ");
        importanceId = getInput();

        if (importanceId >= 1 && importanceId <= 3) {
            break; 
        }
        else {
            printf("Invalid importance value! Please enter 1, 2, or 3.\n");
        }
    }

    selectedTask->impid = importanceId;

    saveTasks(tasks, taskCount);

    printf("Importance level of '%s' marked successfully as %d.\n",
        selectedTask->name, importanceId);
    enterToContinue();
}



/**
 * @brief Finds a task by its name.
 *
 * This function searches for a task in the list of tasks by comparing the given name with each task's name.
 * If a task with the specified name is found, the index of the task is returned; otherwise, -1 is returned.
 *
 * @param name The name of the task to be searched.
 *
 * @return int The index of the found task, or -1 if no task is found.
 *
 * Example Usage:
 * @code
 *     int index = findTaskByName("Task A");
 *     if (index != -1) {
 *         // Task found, proceed with the index
 *     }
 * @endcode
 */
int findTaskByName(const char* name) {
    for (int i = 0; i < taskCount; i++) {
        if (strcmp(tasks[i].name, name) == 0) {
            return i;  // Task found, return index
        }
    }
    return -1;  // Task not found
}

/**
 * @brief Hash function for generating an index based on an email.
 *
 * This function calculates a hash value for the given email by adding the ASCII values of each character
 * and taking the modulus with the table size. The resulting hash value is used to determine the index
 * for storing the email in a hash table.
 *
 * @param email The email string to be hashed.
 *
 * @return int The hash value (index) for the given email.
 *
 * Example Usage:
 * @code
 *     int index = hashFunction("example@example.com");
 * @endcode
 */
int hashFunction(const char* email) {
    int hash = 0;
    while (*email) {
        hash = (hash + *email) % TABLE_SIZE;
        email++;
    }
    return hash;
}

/**
 * @brief Secondary hash function for double hashing.
 *
 * This function calculates a secondary hash value for the given email by adding the ASCII values of each character
 * and taking the modulus with (TABLE_SIZE - 1). This secondary hash value is used in double hashing for better
 * collision resolution.
 *
 * @param email The email string to be hashed.
 *
 * @return int The secondary hash value for the given email.
 *
 * Example Usage:
 * @code
 *     int index = hashFunction2("example@example.com");
 * @endcode
 */
int hashFunction2(const char* email) {
    int hash = 0;
    while (*email) {
        hash = (hash + *email) % (TABLE_SIZE - 1);
        email++;
    }
    return (TABLE_SIZE - 1 - hash);
}

/**
 * @brief Alternative secondary hash function for double hashing.
 *
 * This function calculates an alternative secondary hash value for the given email by multiplying the current hash
 * value by 31 and adding the ASCII value of each character. The modulus with TABLE_SIZE ensures the hash value
 * remains within the appropriate bounds.
 *
 * @param email The email string to be hashed.
 *
 * @return int The secondary hash value for the given email.
 *
 * Example Usage:
 * @code
 *     int index = secondHashFunction("example@example.com");
 * @endcode
 */
int secondHashFunction(const char* email) {
    int hash = 0;
    while (*email) {
        hash = (hash * 31 + *email) % TABLE_SIZE;
        email++;
    }
    return (TABLE_SIZE - 1 - hash);
}


/**
 * @brief Inserts a user into the hash table using linear probing for collision resolution.
 *
 * This function inserts a user into the hash table based on the email address. If a collision occurs,
 * linear probing is used to find the next available slot. If the hash table is full, the function will
 * display an error message.
 *
 * @param user A pointer to the user to be inserted.
 *
 * @details
 * - Uses `hashFunction()` to calculate the initial index.
 * - If a collision occurs, the function searches for the next available slot using linear probing.
 * - If the user with the given email already exists, the function returns without adding the user.
 * - If the hash table is full, an appropriate message is displayed.
 *
 * Example Usage:
 * @code
 *     User user = {"John Doe", "john@example.com"};
 *     insertUserToHashTableWithLinearProbing(&user);
 * @endcode
 */
void insertUserToHashTableWithLinearProbing(User* user) {
    int index = hashFunction(user->email);
    int originalIndex = index;

    printf("Inserting user with email: %s\n", user->email);  // For debugging purposes

    // Linear probing to find an empty spot
    while (hashTable[index] != NULL) {
        // If the same email already exists, the user is already registered
        if (strcmp(hashTable[index]->email, user->email) == 0) {
            printf("User already exists at index %d.\n", index);  // For debugging purposes
            return;
        }

        // Move to the next index and wrap around if necessary
        index = (index + 1) % TABLE_SIZE;

        // If we loop back to the original index, the table is full
        if (index == originalIndex) {
            printf("Hash table is full, cannot add more users.\n");
            return;
        }
    }

    // Add the user to the empty spot
    hashTable[index] = (User*)malloc(sizeof(User));
    *hashTable[index] = *user;

    printf("User added at index %d.\n", index);  // Confirm user addition
}

/**
 * @brief Inserts a user into the hash table with overflow area for collision resolution.
 *
 * This function inserts a user into the hash table. If a collision occurs, the user is added to an overflow
 * area. If both the hash table and the overflow area are full, the function displays an error message.
 *
 * @param user A pointer to the user to be inserted.
 *
 * @details
 * - Uses `hashFunction()` to calculate the initial index.
 * - If a collision occurs, the user is added to an overflow area if space is available.
 * - If the overflow area is full, an appropriate message is displayed.
 *
 * Example Usage:
 * @code
 *     User user = {"Jane Doe", "jane@example.com"};
 *     insertUserToHashTableWithOverflow(&user);
 * @endcode
 */
void insertUserToHashTableWithOverflow(User* user) {
    int index = hashFunction(user->email);

    // Check if the main hash table slot is empty
    if (hashTable[index] == NULL) {
        hashTable[index] = (User*)malloc(sizeof(User));
        *hashTable[index] = *user;
        printf("User added at index %d in main hash table.\n", index);
    }
    else {
        // If there is a collision, add to the overflow area
        if (overflowCount < OVERFLOW_SIZE) {
            overflowArea[overflowCount++] = *user;
            printf("User added at index %d in overflow area.\n", overflowCount - 1);
        }
        else {
            printf("Overflow area is full, cannot add more users.\n");
        }
    }
}

/**
 * @brief Inserts a user into the hash table using quadratic probing for collision resolution.
 *
 * This function inserts a user into the hash table based on the email address. If a collision occurs,
 * quadratic probing is used to find the next available slot. If the hash table is full, the function
 * displays an error message.
 *
 * @param user A pointer to the user to be inserted.
 *
 * @details
 * - Uses `hashFunction()` to calculate the initial index.
 * - If a collision occurs, the function searches for the next available slot using quadratic probing.
 * - If the user with the given email already exists, the function returns without adding the user.
 * - If the hash table is full, an appropriate message is displayed.
 *
 * Example Usage:
 * @code
 *     User user = {"Alice Smith", "alice@example.com"};
 *     insertUserToHashTableWithQuadraticProbing(&user);
 * @endcode
 */
void insertUserToHashTableWithQuadraticProbing(User* user) {
    int index = hashFunction(user->email);
    int i = 1;

    // Quadratic probing to find an empty spot
    while (hashTable[index] != NULL) {
        // If the same email already exists, the user is already registered
        if (strcmp(hashTable[index]->email, user->email) == 0) {
            printf("User already exists at index %d.\n", index);  // For debugging purposes
            return;
        }

        // Calculate the next index using quadratic probing
        index = (index + i * i) % TABLE_SIZE;
        i++;

        // If we exceed the table size, the table is full
        if (i >= TABLE_SIZE) {
            printf("Hash table is full, cannot add more users.\n");
            return;
        }
    }

    // Add the user to the empty spot
    hashTable[index] = (User*)malloc(sizeof(User));
    *hashTable[index] = *user;

    printf("User added at index %d using Quadratic Probing.\n", index);
}


/**
 * @brief Inserts a user into the hash table using double hashing for collision resolution.
 *
 * This function inserts a user into the hash table based on the email address. If a collision occurs,
 * double hashing is used to find the next available slot. If the hash table is full, the function displays
 * an error message.
 *
 * @param user A pointer to the user to be inserted.
 *
 * @details
 * - Uses `hashFunction()` to calculate the initial index.
 * - Uses `hashFunction2()` to calculate the step size for double hashing.
 * - If a collision occurs, the function searches for the next available slot using double hashing.
 * - If the user with the given email already exists, the function returns without adding the user.
 * - If the hash table is full, an appropriate message is displayed.
 *
 * Example Usage:
 * @code
 *     User user = {"Bob Johnson", "bob@example.com"};
 *     insertUserToHashTableWithDoubleHashing(&user);
 * @endcode
 */
void insertUserToHashTableWithDoubleHashing(User* user) {
    int index = hashFunction(user->email);
    int stepSize = hashFunction2(user->email);
    int i = 0;

    // Double hashing to find an empty spot
    while (hashTable[index] != NULL) {
        // If the same email already exists, the user is already registered
        if (strcmp(hashTable[index]->email, user->email) == 0) {
            printf("User already exists at index %d.\n", index);  // For debugging purposes
            return;
        }

        // Use the secondary hash function to calculate the next index
        index = (index + stepSize) % TABLE_SIZE;
        i++;

        // If we exceed the table size, the table is full
        if (i >= TABLE_SIZE) {
            printf("Hash table is full, cannot add more users.\n");
            return;
        }
    }

    // Add the user to the empty spot
    hashTable[index] = (User*)malloc(sizeof(User));
    *hashTable[index] = *user;

    printf("User added at index %d using Double Hashing.\n", index);
}

/**
 * @brief Inserts a user into the hash table using linear quotient for collision resolution.
 *
 * This function inserts a user into the hash table based on the email address. If a collision occurs,
 * linear quotient is used to find the next available slot. If the hash table is full, the function displays
 * an error message.
 *
 * @param user A pointer to the user to be inserted.
 *
 * @details
 * - Uses `hashFunction()` to calculate the initial index.
 * - If a collision occurs, the function searches for the next available slot using a fixed step size.
 * - If the user with the given email already exists, the function returns without adding the user.
 * - If the hash table is full, an appropriate message is displayed.
 *
 * Example Usage:
 * @code
 *     User user = {"Chris Evans", "chris@example.com"};
 *     insertUserToHashTableWithLinearQuotient(&user);
 * @endcode
 */
void insertUserToHashTableWithLinearQuotient(User* user) {
    int index = hashFunction(user->email);
    int stepSize = Q_STEP;

    // Linear quotient to find an empty spot
    while (hashTable[index] != NULL) {
        // If the same email already exists, the user is already registered
        if (strcmp(hashTable[index]->email, user->email) == 0) {
            printf("User already exists at index %d.\n", index);  // For debugging purposes
            return;
        }

        // Move to the next index using a fixed step size
        index = (index + stepSize) % TABLE_SIZE;

        // If we return to the original index, the table is full
        if (index == hashFunction(user->email)) {
            printf("Hash table is full, cannot add more users.\n");
            return;
        }
    }

    // Add the user to the empty spot
    hashTable[index] = (User*)malloc(sizeof(User));
    *hashTable[index] = *user;

    printf("User added at index %d using Linear Quotient.\n", index);
}

/**
 * @brief Inserts a user into the hash table using Brent's method for collision resolution.
 *
 * This function inserts a user into the hash table based on the email address. If a collision occurs,
 * Brent's method is used to minimize the total number of probes required to insert the user. If the hash
 * table is full, the function displays an error message.
 *
 * @param user A pointer to the user to be inserted.
 *
 * @details
 * - Uses `hashFunction()` to calculate the initial index.
 * - Uses `secondHashFunction()` to calculate the step size.
 * - If a collision occurs, the function evaluates two alternative strategies to minimize the total number of probes.
 * - The user is inserted into the position that minimizes the probing sequence.
 * - If the hash table is full, an appropriate message is displayed.
 *
 * Example Usage:
 * @code
 *     User user = {"Diana Prince", "diana@example.com"};
 *     insertUserToHashTableWithBrentsMethod(&user);
 * @endcode
 */
void insertUserToHashTableWithBrentsMethod(User* user) {
    int index = hashFunction(user->email);
    int stepSize = secondHashFunction(user->email);

    if (hashTable[index] == NULL) {
        // If the initial position is empty, insert directly
        hashTable[index] = (User*)malloc(sizeof(User));
        *hashTable[index] = *user;
        printf("User added at index %d using Brent's Method.\n", index);
    }
    else {
        // Handle collision with Brent's method
        int i = 1, newIndex = index;
        User* currentUser = user;

        while (hashTable[newIndex] != NULL) {
            // Evaluate two alternative strategies: continue from the current position or relocate
            int trialIndex1 = (newIndex + i * stepSize) % TABLE_SIZE;  // Move forward with step size
            int trialIndex2 = (index + i) % TABLE_SIZE;                // Alternative position

            // Choose the position with the shortest probing sequence
            if (hashTable[trialIndex1] == NULL) {
                newIndex = trialIndex1;
                break;
            }
            else if (hashTable[trialIndex2] == NULL) {
                newIndex = trialIndex2;
                break;
            }

            i++;
        }

        hashTable[newIndex] = (User*)malloc(sizeof(User));
        *hashTable[newIndex] = *user;
        printf("User placed at index %d using Brent's Method after collision.\n", newIndex);
    }
}


/**
 * @brief Searches for a user in the hash table using linear probing.
 *
 * This function searches for a user in the hash table by comparing the given email and password.
 * If the user is found, a pointer to the user is returned; otherwise, NULL is returned.
 * Linear probing is used to resolve collisions during the search.
 *
 * @param email The email of the user to be searched.
 * @param password The password of the user to be searched.
 *
 * @return User* A pointer to the found user, or NULL if the user is not found.
 *
 * Example Usage:
 * @code
 *     User* user = searchUserInHashTable("john@example.com", "password123");
 *     if (user != NULL) {
 *         // User found, proceed with user data
 *     }
 * @endcode
 */
User* searchUserInHashTable(const char* email, const char* password) {
    int index = hashFunction(email);
    int originalIndex = index;

    // Linear probing to search for the user
    while (hashTable[index] != NULL) {
        if (strcmp(hashTable[index]->email, email) == 0 &&
            strcmp(hashTable[index]->password, password) == 0) {
            return hashTable[index];  // User found
        }

        // Move to the next index and wrap around if necessary
        index = (index + 1) % TABLE_SIZE;

        // If we loop back to the original index, the user is not in the table
        if (index == originalIndex) {
            break;
        }
    }
    return NULL;  // User not found
}

/**
 * @brief Demonstrates user insertion using linear probing for collision resolution.
 *
 * This function demonstrates the process of inserting a user into the hash table using linear probing.
 * The user details are collected from the user, and then `insertUserToHashTableWithLinearProbing()` is called
 * to insert the user into the hash table.
 *
 * @details
 * - Prompts the user for their name, surname, email, and password.
 * - Calls `insertUserToHashTableWithLinearProbing()` to insert the user.
 *
 * Example Usage:
 * @code
 *     linearProbingDemo();
 * @endcode
 */
void linearProbingDemo() {
    printf("Linear Probing is active for user hash table management.\n");

    User user;
    printf("Enter user details:\n");

    printf("Name: ");
    fgets(user.name, sizeof(user.name), stdin);
    user.name[strcspn(user.name, "\n")] = 0;

    printf("Surname: ");
    fgets(user.surname, sizeof(user.surname), stdin);
    user.surname[strcspn(user.surname, "\n")] = 0;

    printf("Email: ");
    fgets(user.email, sizeof(user.email), stdin);
    user.email[strcspn(user.email, "\n")] = 0;

    printf("Password: ");
    fgets(user.password, sizeof(user.password), stdin);
    user.password[strcspn(user.password, "\n")] = 0;

    insertUserToHashTableWithLinearProbing(&user);  // Insert user with linear probing

    printf("User insertion complete.\n");  // Indicate completion of user insertion
    enterToContinue();
}

/**
 * @brief Demonstrates user insertion using progressive overflow for collision resolution.
 *
 * This function demonstrates the process of inserting a user into the hash table using progressive overflow.
 * The user details are collected from the user, and then `insertUserToHashTableWithOverflow()` is called
 * to insert the user into the hash table or the overflow area if necessary.
 *
 * @details
 * - Prompts the user for their name, surname, email, and password.
 * - Calls `insertUserToHashTableWithOverflow()` to insert the user.
 *
 * Example Usage:
 * @code
 *     progressiveOverflowDemo();
 * @endcode
 */
void progressiveOverflowDemo() {
    printf("Progressive Overflow is active for user hash table management.\n");

    User user;
    printf("Enter user details:\n");

    printf("Name: ");
    fgets(user.name, sizeof(user.name), stdin);
    user.name[strcspn(user.name, "\n")] = 0;

    printf("Surname: ");
    fgets(user.surname, sizeof(user.surname), stdin);
    user.surname[strcspn(user.surname, "\n")] = 0;

    printf("Email: ");
    fgets(user.email, sizeof(user.email), stdin);
    user.email[strcspn(user.email, "\n")] = 0;

    printf("Password: ");
    fgets(user.password, sizeof(user.password), stdin);
    user.password[strcspn(user.password, "\n")] = 0;

    insertUserToHashTableWithOverflow(&user);  // Insert user with progressive overflow

    printf("User insertion complete.\n");  // Indicate completion of user insertion
    enterToContinue();
}



/**
 * @brief Demonstrates user insertion using quadratic probing for collision resolution.
 *
 * This function demonstrates the process of inserting a user into the hash table using quadratic probing.
 * The user details are collected from the user, and then `insertUserToHashTableWithQuadraticProbing()` is called
 * to insert the user into the hash table.
 *
 * @details
 * - Prompts the user for their name, surname, email, and password.
 * - Calls `insertUserToHashTableWithQuadraticProbing()` to insert the user.
 *
 * Example Usage:
 * @code
 *     quadraticProbingDemo();
 * @endcode
 */
void quadraticProbingDemo() {
    printf("Quadratic Probing is active for user hash table management.\n");

    User user;
    printf("Enter user details:\n");

    printf("Name: ");
    fgets(user.name, sizeof(user.name), stdin);
    user.name[strcspn(user.name, "\n")] = 0;

    printf("Surname: ");
    fgets(user.surname, sizeof(user.surname), stdin);
    user.surname[strcspn(user.surname, "\n")] = 0;

    printf("Email: ");
    fgets(user.email, sizeof(user.email), stdin);
    user.email[strcspn(user.email, "\n")] = 0;

    printf("Password: ");
    fgets(user.password, sizeof(user.password), stdin);
    user.password[strcspn(user.password, "\n")] = 0;

    insertUserToHashTableWithQuadraticProbing(&user);  // Insert user with quadratic probing

    printf("User insertion complete.\n");  // Indicate completion of user insertion
    enterToContinue();
}

/**
 * @brief Demonstrates user insertion using double hashing for collision resolution.
 *
 * This function demonstrates the process of inserting a user into the hash table using double hashing.
 * The user details are collected from the user, and then `insertUserToHashTableWithDoubleHashing()` is called
 * to insert the user into the hash table.
 *
 * @details
 * - Prompts the user for their name, surname, email, and password.
 * - Calls `insertUserToHashTableWithDoubleHashing()` to insert the user.
 *
 * Example Usage:
 * @code
 *     doubleHashingDemo();
 * @endcode
 */
void doubleHashingDemo() {
    printf("Double Hashing is active for user hash table management.\n");

    User user;
    printf("Enter user details:\n");

    printf("Name: ");
    fgets(user.name, sizeof(user.name), stdin);
    user.name[strcspn(user.name, "\n")] = 0;

    printf("Surname: ");
    fgets(user.surname, sizeof(user.surname), stdin);
    user.surname[strcspn(user.surname, "\n")] = 0;

    printf("Email: ");
    fgets(user.email, sizeof(user.email), stdin);
    user.email[strcspn(user.email, "\n")] = 0;

    printf("Password: ");
    fgets(user.password, sizeof(user.password), stdin);
    user.password[strcspn(user.password, "\n")] = 0;

    insertUserToHashTableWithDoubleHashing(&user);  // Insert user with double hashing

    printf("User insertion complete.\n");  // Indicate completion of user insertion
    enterToContinue();
}

/**
 * @brief Demonstrates user insertion using linear quotient for collision resolution.
 *
 * This function demonstrates the process of inserting a user into the hash table using linear quotient.
 * The user details are collected from the user, and then `insertUserToHashTableWithLinearQuotient()` is called
 * to insert the user into the hash table.
 *
 * @details
 * - Prompts the user for their name, surname, email, and password.
 * - Calls `insertUserToHashTableWithLinearQuotient()` to insert the user.
 *
 * Example Usage:
 * @code
 *     linearQuotientDemo();
 * @endcode
 */
void linearQuotientDemo() {
    printf("Linear Quotient is active for user hash table management.\n");

    User user;
    printf("Enter user details:\n");

    printf("Name: ");
    fgets(user.name, sizeof(user.name), stdin);
    user.name[strcspn(user.name, "\n")] = 0;

    printf("Surname: ");
    fgets(user.surname, sizeof(user.surname), stdin);
    user.surname[strcspn(user.surname, "\n")] = 0;

    printf("Email: ");
    fgets(user.email, sizeof(user.email), stdin);
    user.email[strcspn(user.email, "\n")] = 0;

    printf("Password: ");
    fgets(user.password, sizeof(user.password), stdin);
    user.password[strcspn(user.password, "\n")] = 0;

    insertUserToHashTableWithLinearQuotient(&user);  // Insert user with linear quotient

    printf("User insertion complete.\n");  // Indicate completion of user insertion
    enterToContinue();
}


/**
 * @brief Demonstrates user insertion using Brent's method for collision resolution.
 *
 * This function demonstrates the process of inserting a user into the hash table using Brent's method.
 * The user details are collected from the user, and then `insertUserToHashTableWithBrentsMethod()` is called
 * to insert the user into the hash table.
 *
 * @details
 * - Prompts the user for their name, surname, email, and password.
 * - Calls `insertUserToHashTableWithBrentsMethod()` to insert the user.
 *
 * Example Usage:
 * @code
 *     brentsMethodDemo();
 * @endcode
 */
void brentsMethodDemo() {
    printf("Brent's Method is active for user hash table management.\n");

    User user;
    printf("Enter user details:\n");

    printf("Name: ");
    fgets(user.name, sizeof(user.name), stdin);
    user.name[strcspn(user.name, "\n")] = 0;

    printf("Surname: ");
    fgets(user.surname, sizeof(user.surname), stdin);
    user.surname[strcspn(user.surname, "\n")] = 0;

    printf("Email: ");
    fgets(user.email, sizeof(user.email), stdin);
    user.email[strcspn(user.email, "\n")] = 0;

    printf("Password: ");
    fgets(user.password, sizeof(user.password), stdin);
    user.password[strcspn(user.password, "\n")] = 0;

    insertUserToHashTableWithBrentsMethod(&user);  // Insert user with Brent's method

    printf("User insertion complete.\n");  // Indicate completion of user insertion
    enterToContinue();
}



/**
 * @brief Encodes a given input string using Huffman coding.
 *
 * This function performs Huffman encoding on the provided input string and returns the encoded result.
 * Currently, it simply duplicates the input string as a placeholder for the actual Huffman encoding logic.
 *
 * @param input The string to be encoded.
 *
 * @return char* A dynamically allocated string containing the encoded result.
 *
 * Example Usage:
 * @code
 *     char* encoded = huffmanEncode("example");
 *     // Use the encoded string
 *     free(encoded);
 * @endcode
 */
char* huffmanEncode(const char* input) {
    // Huffman encoding is performed here, returning the compressed string
    // For now, we return the same string (this should be modified in a real implementation).
    char* encoded = strdup(input);
    return encoded;
}

/**
 * @brief Decodes a given Huffman encoded string.
 *
 * This function performs Huffman decoding on the provided encoded string and returns the original result.
 * Currently, it simply duplicates the encoded string as a placeholder for the actual Huffman decoding logic.
 *
 * @param encoded The Huffman encoded string to be decoded.
 *
 * @return char* A dynamically allocated string containing the decoded result.
 *
 * Example Usage:
 * @code
 *     char* decoded = huffmanDecode(encoded);
 *     // Use the decoded string
 *     free(decoded);
 * @endcode
 */
char* huffmanDecode(const char* encoded) {
    // Huffman decoding is performed here, returning the original string
    char* decoded = strdup(encoded);
    return decoded;
}



/**
 * @brief Registers a new user by writing their information to a file.
 *
 * This function registers a new user by writing their details to a binary user file. It first checks if the user
 * already exists by comparing the email. If the user does not exist, it assigns a new user ID, encodes the email
 * and password using Huffman encoding, and writes the encoded data to a separate file for Huffman encoded users.
 * Finally, it writes the user details to the binary user file.
 *
 * @param user The user structure containing the user's details (name, surname, email, and password).
 * @param pathFileUser The path to the user database file.
 *
 * @return int Returns 1 if the registration is successful, otherwise returns 0.
 *
 * Example Usage:
 * @code
 *     User newUser = {"John", "Doe", "john.doe@example.com", "password123"};
 *     int result = registerUser(newUser, "users.bin");
 *     if (result) {
 *         printf("User registered successfully.\n");
 *     }
 * @endcode
 */
int registerUser(User user, const char* pathFileUser) {
    FILE* file = fopen(pathFileUser, "rb+");
    int userCount = 0;

    if (file) {
        fread(&userCount, sizeof(int), 1, file);

        User tempUser;
        for (int i = 0; i < userCount; ++i) {
            fread(&tempUser, sizeof(User), 1, file);
            if (strcmp(tempUser.email, user.email) == 0) {
                printf("User already exists.\n");
                fclose(file);
                enterToContinue();
                return 0;
            }
        }
        fseek(file, 0, SEEK_END);
    }
    else {
        file = fopen(pathFileUser, "wb+");
        if (!file) {
            printf("Failed to open or create user file.\n");
            return 0;
        }
    }

    user.id = userCount + 1;
    userCount++;

    rewind(file);
    fwrite(&userCount, sizeof(int), 1, file);

    char* encodedEmail = huffmanEncode(user.email);
    char* encodedPassword = huffmanEncode(user.password);

    FILE* huffFile = fopen("user.huf", "ab");
    if (huffFile) {
        int emailLen = strlen(encodedEmail);
        int passwordLen = strlen(encodedPassword);

        fwrite(&user.id, sizeof(int), 1, huffFile);
        fwrite(&emailLen, sizeof(int), 1, huffFile);
        fwrite(encodedEmail, sizeof(char), emailLen, huffFile);
        fwrite(&passwordLen, sizeof(int), 1, huffFile);
        fwrite(encodedPassword, sizeof(char), passwordLen, huffFile);

        fclose(huffFile);
    }
    else {
        printf("Failed to open or create Huffman encoded user file.\n");
    }

    free(encodedEmail);
    free(encodedPassword);

    fseek(file, 0, SEEK_END);
    fwrite(&user, sizeof(User), 1, file);

    printf("User registered successfully: Welcome %s %s\n", user.name, user.surname);

    fclose(file);
    enterToContinue();
    return 1;
}

/**
 * @brief Displays a menu for user registration.
 *
 * This function prompts the user to input their name, surname, email, and password for registration.
 * It then calls `registerUser()` to complete the registration process by writing the user's details to a file.
 *
 * @param pathFileUsers The path to the user database file.
 *
 * @return int Returns 1 if the registration process is completed successfully, otherwise returns 0.
 *
 * Example Usage:
 * @code
 *     int result = registerUserMenu("users.bin");
 *     if (result) {
 *         printf("User registration successful.\n");
 *     }
 * @endcode
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

    return registerUser(newUser, pathFileUsers);
}



/**
 * @brief Logs in a user.
 *
 * This function verifies the user's email and password against the user database file for login.
 * It reads the user information from the file, decodes the email and password using Huffman decoding,
 * and checks if the provided credentials match any stored user.
 *
 * @param loginUser The user attempting to log in.
 * @param pathFileUsers The path to the user database file.
 *
 * @return int Returns 1 if the login is successful, otherwise returns 0.
 *
 * Example Usage:
 * @code
 *     User user = {"", "", "john.doe@example.com", "password123"};
 *     int result = loginUser(user, "users.bin");
 *     if (result) {
 *         printf("Login successful.\n");
 *     }
 * @endcode
 */
int loginUser(User loginUser, const char* pathFileUsers) {
    FILE* file = fopen(pathFileUsers, "rb");
    if (!file) {
        printf("Failed to open user file.\n");
        return 0;
    }

    int userCount = 0;
    if (fread(&userCount, sizeof(int), 1, file) != 1) {
        printf("Failed to read user count from file.\n");
        fclose(file);
        return 0;
    }

    int found = 0;
    User tempUser;

    for (int i = 0; i < userCount; ++i) {
        // Read user data from the file
        if (fread(&tempUser, sizeof(User), 1, file) != 1) {
            printf("Error reading user data from file.\n");
            break;
        }

        // Decode email and password using Huffman decoding
        char* decodedEmail = huffmanDecode(tempUser.email);
        char* decodedPassword = huffmanDecode(tempUser.password);

        // Check email and password
        if (strcmp(decodedEmail, loginUser.email) == 0 && strcmp(decodedPassword, loginUser.password) == 0) {
            printf("Login successful.\n");
            loggedUser = tempUser;
            found = 1;

            // Free dynamically allocated memory
            free(decodedEmail);
            free(decodedPassword);
            break;
        }

        // Free dynamically allocated memory
        free(decodedEmail);
        free(decodedPassword);
    }

    fclose(file);

    if (!found) {
        printf("Incorrect email or password.\n");
        enterToContinue();
        return 0;
    }

    enterToContinue();
    return 1;
}

/**
 * @brief Displays a menu for user login.
 *
 * This function prompts the user to input their email and password for login.
 * It then calls `loginUser()` to verify the credentials against the user database file.
 *
 * @param pathFileUsers The path to the user database file.
 *
 * @return int Returns 1 if the login process is completed successfully, otherwise returns 0.
 *
 * Example Usage:
 * @code
 *     int result = loginUserMenu("users.bin");
 *     if (result) {
 *         printf("Login successful.\n");
 *     }
 * @endcode
 */
int loginUserMenu(const char* pathFileUsers) {
    clearScreen();
    User userInput;

    printf("Enter email: ");
    fgets(userInput.email, sizeof(userInput.email), stdin);
    userInput.email[strcspn(userInput.email, "\n")] = 0;

    printf("Enter password: ");
    fgets(userInput.password, sizeof(userInput.password), stdin);
    userInput.password[strcspn(userInput.password, "\n")] = 0;

    return loginUser(userInput, pathFileUsers);
}



/**
 * @brief Displays the main user options menu.
 *
 * This function presents the main menu for the user after they have successfully logged in.
 * The menu includes options such as creating tasks, setting deadlines, setting reminders, managing task prioritization,
 * and accessing algorithm demos. Depending on the user's choice, the corresponding function is called.
 *
 * @return int Returns 0 when the user chooses to exit the menu.
 */
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
            createTaskMenu(taskList, &taskCount);
            break;
        case 2:
            deadlineSettingsMenu();
            break;
        case 3:
            reminderSystemMenu();
            break;
        case 4:
            taskPrioritizationMenu();
            break;
        case 5:
            algorithmsMenu();
            break;
        case 6:
            return 0;
        default:
            clearScreen();
            printf("Invalid choice. Please try again.\n");
            enterToContinue();
            break;
        }
    }
}

/**
 * @brief Displays a menu for algorithm demonstrations.
 *
 * This function presents a menu that allows the user to select different hash table management algorithms for demonstration purposes.
 * The menu includes options such as Progressive Overflow, Linear Probing, Quadratic Probing, Double Hashing, Linear Quotient, and Brent's Method.
 * The user can view each algorithm's behavior by selecting the respective option.
 *
 * @return void This function returns to the main menu when the user chooses to exit.
 */
void algorithmsMenu() {
    int choice;
    while (1) {
        printAlgorithmsMenu();
        choice = getInput();

        if (choice == -2) {
            handleInputError();
            enterToContinue();
            continue;
        }

        switch (choice) {
        case 1:
            progressiveOverflowDemo();
            break;
        case 2:
            linearProbingDemo();
            break;
        case 3:
            quadraticProbingDemo();
            break;
        case 4:
            doubleHashingDemo();
            break;
        case 5:
            // useOfBuckets();
            break;
        case 6:
            linearQuotientDemo();
            break;
        case 7:
            brentsMethodDemo();
            break;
        case 8:
            return;  // Return to main menu
        default:
            clearScreen();
            printf("Invalid choice. Please try again.\n");
            enterToContinue();
            break;
        }
    }
}

/**
 * @brief Displays the main menu for the application.
 *
 * This function presents the opening screen menu to the user, allowing them to log in, register, or exit the program.
 * It continuously displays the menu until the user chooses to exit.
 *
 * @param pathFileUsers The path to the user database file.
 *
 * @return int Returns 0 when the user chooses to exit the program.
 */
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







