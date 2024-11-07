
/**
 * @file task.cpp
 * @brief Contains task definitions and operations for the Task Scheduler project.
 *
 * This file contains functions and classes related to task management in the Task Scheduler project.
 * The functions include task addition, editing, and various user operations.
 */

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

 /**
  * @file task_manager.cpp
  * @brief Defines constants, data structures, and platform-specific headers for the Task Scheduler project.
  *
  * This file includes platform-specific headers, task structures, and global variables used in the Task Scheduler project.
  *
  * - Platform-Specific Operations: Includes appropriate headers (`Sleep()` for Windows and `sleep()` for Unix-like systems).
  * - Global Task Pointers: Task linked list using `head` and `tail`, and XOR linked list using `xorHead` and `xorTail`.
  * - Constant Definitions:
  *   - `TABLE_SIZE`: The size of the hash table.
  *   - `MAX_TASKS`: Maximum number of tasks that can be handled.
  *   - `MAX_ASSIGNMENT_NAME`: Maximum length for the assignment name.
  * - Global Variables: The `assignments` array to store tasks and `filename` for persistent task data storage.
  */

#ifdef _WIN32
#include <windows.h>  // Windows için Sleep()
#else
#include <unistd.h>   // Linux/macOS için sleep()
#endif

TaskNode* head = NULL;      // Liste başı (ilk görev)
TaskNode* tail = NULL;      // Liste sonu (son görev)
XORNode* xorHead = NULL;
XORNode* xorTail = NULL;



#define TABLE_SIZE 10  // Hash tablosunun boyutu
#define MAX_TASKS 100  // Maksimum görev sayısı
#define MAX_ASSIGNMENT_NAME 50
Assignment assignments[100];
const char* filename = "tasks.bin";


/**
 * @file task_manager.cpp
 * @brief Defines global variables, data structures, and namespace usage for the Task Scheduler project.
 *
 * This file contains various global variables and data structures for managing tasks, users, and notifications
 * in the Task Scheduler project. It also uses the standard namespace for convenience.
 *
 * - Task Management:
 *   - `tasks`: An array to store up to 100 tasks.
 *   - `taskList`: An array to maintain the current list of tasks in the system.
 *   - `taskCount`: Tracks the current number of tasks in the system.
 * - User Management:
 *   - `hashTable`: A hash table to store user information for quick access.
 *   - `overflowArea`: An area to manage user data that overflows the hash table.
 *   - `overflowCount`: Tracks the number of users in the overflow area.
 *   - `loggedUser`: Stores information about the currently logged-in user.
 * - Notification Settings:
 *   - `notificationMethod`: Stores the notification preference set by the user.
 * - Namespace:
 *   - `using namespace std`: Used to simplify the use of standard library features.
 */


Task tasks[100];
User* hashTable[TABLE_SIZE];
Task taskList[MAX_TASKS];  // Görev listesi
int taskCount = 0;  // Mevcut görev sayısı
int notificationMethod = 0;
User overflowArea[OVERFLOW_SIZE];
int overflowCount = 0;



using namespace std;

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
            searchTasksByKeyword();  // KMP araması ile görevleri ara
            enterToContinue();
            break;
        case 7:
            navigateTasks();  // Çift bağlantılı liste ile görevler arasında gezinme
            enterToContinue();
            break;
        case 8:
            loadTasksToXORList("tasks.bin");  // XOR Linked List'e görevleri yükle
            navigateXORList();  // XOR Linked List üzerinde gezin
            enterToContinue();
            break;
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
    newNode->xorPtr = xorTail; // Yeni düğümün XOR işaretçisini mevcut kuyruğa işaret edecek şekilde ayarla

    if (xorHead == NULL) {
        xorHead = xorTail = newNode; // Liste boşsa, baş ve son düğüm aynı olur
    }
    else {
        xorTail->xorPtr = (XORNode*)((uintptr_t)(xorTail->xorPtr) ^ (uintptr_t)newNode); // Önceki düğümün XOR işaretçisini güncelle
        xorTail = newNode; // Kuyruğu güncelle
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

void loadTasksToXORList(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Unable to open tasks file.\n");
        return -1;
    }

    Task task;
    while (fread(&task, sizeof(Task), 1, file)) {
        addTaskToXORList(task); // Her bir görevi XOR Linked List'e ekle
    }

    fclose(file);
    printf("Tasks loaded into XOR Linked List successfully!\n");
}

/**
 * @brief Navigates through the XOR linked list of tasks.
 *
 * The `navigateXORList` function allows the user to navigate through the XOR linked list of tasks.
 * The user can move forward or backward through the list to view tasks or exit navigation at any time.
 * Navigation is based on XOR pointers to efficiently traverse in both directions.
 */

void navigateXORList() {
    XORNode* current = xorHead;
    XORNode* prev = NULL;
    XORNode* next;

    if (current == NULL) {
        printf("No tasks found in the list.\n");
        return;
    }

    int choice;
    do {
        // Mevcut görevi göster
        printf("ID: %d\n", current->task.id);
        printf("Name: %s\n", current->task.name);
        printf("Description: %s\n", current->task.description);
        printf("Category: %s\n", current->task.category);
        printf("Due Date: %s\n", current->task.dueDate);
        printf("---------------------------\n");

        // Kullanıcıdan seçim al
        printf("Press 1 to go forward, 2 to go backward, or 0 to exit: ");
        choice = getInput();

        // Seçime göre sonraki veya önceki düğüme geç
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
}

/**
 * @brief Adds a new task to the doubly linked list of tasks.
 *
 * The `addTaskToList` function creates a new node for the given task and adds it to the end of the doubly linked list.
 * If the list is empty, the new node becomes both the head and the tail of the list. Otherwise, it is appended after the current tail.
 *
 * @param newTask The task to be added to the doubly linked list.
 */

void addTaskToList(Task newTask) {
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

    // Çift bağlantılı listeye ekle
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
void viewTask() {
    if (front == NULL) {
        printf("No tasks found. The task list is empty.\n");
        enterToContinue();
        return;
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
    FILE* file = fopen("tasks.bin", "rb");  // Dosyayı okuma modunda aç
    if (!file) {
        printf("Error: Could not open tasks file or no tasks found.\n");
        enterToContinue();
        return;
    }

    char category[50];
    printf("Enter category to filter: ");
    fgets(category, sizeof(category), stdin);
    category[strcspn(category, "\n")] = 0;  // Yeni satır karakterini sil

    Task task;
    int found = 0;  // Kategoride görev bulunup bulunmadığını kontrol etmek için

    printf("\n--- Tasks in Category '%s' ---\n", category);
    // Dosyadaki görevleri sırayla okuyup kategoriye göre filtrele
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

    fclose(file);  // Dosyayı kapat
    enterToContinue();  // Kullanıcının devam etmesi için bekle
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
    FILE* file = fopen("tasks.bin", "rb");  // Dosyayı okuma modunda açıyoruz
    if (file == NULL) {
        printf("No previous tasks found.\n");
        return 0;  // Eğer dosya yoksa 0 görev yüklendi
    }

    int taskCount = fread(taskList, sizeof(Task), maxTasks, file);
    fclose(file);

    printf("%d tasks loaded successfully!\n", taskCount);
    return taskCount;  // Yüklenen görev sayısını geri döndürüyoruz
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
        emptyTask.id = -1; // Hata durumu için
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
void push(Task task) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->task = task;
    newNode->next = stackTop;
    stackTop = newNode;
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
        emptyTask.id = -1; // Hata durumu için
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
void undoLastTask(Task taskList[], int* taskCount) {
    Task lastTask = pop();
    if (lastTask.id == -1) {
        printf("No tasks to undo.\n");
        return;
    }

    // Son görevi listeden çıkar
    (*taskCount)--;
    printf("Last task '%s' undone successfully.\n", lastTask.name);

    // Dosyayı güncelle
    saveTasks(taskList, *taskCount);
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
void printDependenciesUtil(Task taskList[], int taskId, bool visited[]) {
    if (visited[taskId]) {
        return;
    }
    visited[taskId] = true;

    Task task = taskList[taskId - 1]; // taskId'den görevi al

    // Görevin bağımlılıklarını yazdır
    for (int i = 0; i < task.dependencyCount; i++) {
        int dependencyId = task.dependencies[i];
        printf("Task %d depends on Task %d\n", task.id, dependencyId);

        // Bağımlı görevi tekrar çağırarak tüm alt bağımlılıkları bul
        printDependenciesUtil(taskList, dependencyId, visited);
    }
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
void printDependencies(Task taskList[], int taskCount, int startTaskId) {
    bool visited[MAX_TASKS] = { false };

    printf("Dependencies for Task %d:\n", startTaskId);
    printDependenciesUtil(taskList, startTaskId, visited);
}

/**
 * @brief Pushes a vertex to the SCC stack.
 *
 * The `pushSccStack` function adds a vertex to the stack used during the Strongly Connected Components (SCC) analysis.
 * This function is typically used during the Tarjan's or Kosaraju's algorithm implementation.
 *
 * @param v The vertex to be added to the SCC stack.
 */
void pushSccStack(int v) {
    AdjacencyNode* newNode = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    newNode->data = v;
    newNode->next = sccStack;
    sccStack = newNode;
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
void dfsUtil(int v, int visited[], AdjacencyNode* adj[], AdjacencyNode** component) {
    visited[v] = 1;
    pushSccStack(v);
    AdjacencyNode* temp = adj[v];
    while (temp != NULL) {
        if (!visited[temp->data]) {
            dfsUtil(temp->data, visited, adj, component);
        }
        temp = temp->next;
    }
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
void findSCCs(int V, AdjacencyNode* adj[], FILE* out) {
    int visited[MAX_TASKS] = { 0 };

    for (int i = 0; i < V; i++) {
        if (!visited[i]) {
            dfsUtil(i, visited, adj, NULL);
        }
    }

    AdjacencyNode* transpose[MAX_TASKS] = { NULL };
    for (int v = 0; v < V; v++) {
        AdjacencyNode* temp = adj[v];
        while (temp != NULL) {
            int u = temp->data;
            AdjacencyNode* newNode = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
            newNode->data = v;
            newNode->next = transpose[u];
            transpose[u] = newNode;
            temp = temp->next;
        }
    }

    memset(visited, 0, sizeof(visited));
    int numSCC = 0;

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

    for (int i = 0; i < V; i++) {
        while (transpose[i] != NULL) {
            AdjacencyNode* temp = transpose[i];
            transpose[i] = transpose[i]->next;
            free(temp);
        }
    }
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
    AdjacencyNode* adj[MAX_TASKS] = { NULL };

    for (int i = 0; i < taskCount; i++) {
        Task task = taskList[i];
        for (int j = 0; j < task.dependencyCount; j++) {
            int dep = task.dependencies[j];
            AdjacencyNode* newNode = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
            newNode->data = dep - 1;
            newNode->next = adj[task.id - 1];
            adj[task.id - 1] = newNode;
        }
    }

    findSCCs(taskCount, adj, out);

    for (int i = 0; i < taskCount; i++) {
        while (adj[i] != NULL) {
            AdjacencyNode* temp = adj[i];
            adj[i] = adj[i]->next;
            free(temp);
        }
    }
    return 1;
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
void computePrefixTable(const char* pattern, int* prefixTable, int patternLength) {
    int length = 0;
    prefixTable[0] = 0;  // İlk eleman 0

    for (int i = 1; i < patternLength; i++) {
        while (length > 0 && pattern[i] != pattern[length]) {
            length = prefixTable[length - 1];
        }
        if (pattern[i] == pattern[length]) {
            length++;
        }
        prefixTable[i] = length;
    }
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
    int textLength = strlen(text);
    int patternLength = strlen(pattern);

    // Önek tablosunu oluştur
    int* prefixTable = (int*)malloc(sizeof(int) * patternLength);
    computePrefixTable(pattern, prefixTable, patternLength);

    int i = 0;  // text indeks
    int j = 0;  // pattern indeks
    while (i < textLength) {
        if (pattern[j] == text[i]) {
            j++;
            i++;
        }
        if (j == patternLength) {
            free(prefixTable);  // Belleği serbest bırak
            return 1;  // Eşleşme bulundu
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
    free(prefixTable);  // Belleği serbest bırak
    return 0;  // Eşleşme bulunamadı
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
    keyword[strcspn(keyword, "\n")] = 0;  // Yeni satır karakterini sil

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
    Assignment assignment;  // Görev yapısı

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
            // Deadline atama fonksiyonunu çağır
            assign_deadline(&assignment);
            enterToContinue();
            break;
        case 2:
            viewDeadlines();
            enterToContinue();
            break;
        case 3:
            return 0;  // Menüden çıkış
        default:
            clearScreen();
            printf("Invalid choice. Please try again.\n");
            enterToContinue();
            break;
        }
    }
}

/**
 * @brief Represents a MinHeap data structure for managing deadlines.
 *
 * The `MinHeap` structure is used to manage deadlines in the task scheduling system.
 * It helps to keep track of tasks with the closest deadlines at the top.
 *
 * @var deadlineHeap The MinHeap instance used to manage deadlines.
 */
MinHeap deadlineHeap = { {0}, 0 };  // İlk alan tüm görevler için sıfırlanır, ikinci alan size olarak 0 atanır


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

    // Görev adını kullanıcıdan al
    printf("Enter Task Name: ");
    while (getchar() != '\n');  // stdin'deki önceki newline'ı temizle
    if (fgets(taskName, MAX_ASSIGNMENT_NAME, stdin) == NULL) {
        printf("Error reading task name.\n");
        return -1;
    }
    taskName[strcspn(taskName, "\n")] = '\0';  // Yeni satırı sil

    // Deadline bilgisi al
    printf("Enter Deadline (day month year): ");
    if (scanf("%d %d %d", &day, &month, &year) != 3) {
        printf("Invalid input! Please try again.\n");
        while (getchar() != '\n');  // stdin temizle
        return -1;
    }

    // Tarih geçerliliğini kontrol et
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900) {
        printf("Invalid date! Please enter a valid date.\n");
        return -1;
    }

    // Görev adını kopyala
    strncpy(assignment->name, taskName, MAX_ASSIGNMENT_NAME - 1);
    assignment->name[MAX_ASSIGNMENT_NAME - 1] = '\0';  // Null terminator

    // Deadline tarihini ata
    assignment->day = day;
    assignment->month = month;
    assignment->year = year;

    // Deadline'ı yığına ekle
    insertMinHeap(&deadlineHeap, *assignment);

    // Deadline'ı dosyaya kaydet
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

    MinHeap tempHeap = deadlineHeap;  // Yığının bir kopyasını alarak orijinali koruyoruz

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
    enterToContinue();  // Kullanıcıdan devam etmesini bekle
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
    return year * 10000 + month * 100 + day;  // YYYYMMDD formatında bir anahtar oluşturur
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

    // Deadline'a göre karşılaştırma
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

    // Yukarı doğru yer değiştirerek minimum yığın özelliğini koruma
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



// Hatırlatıcı Ayarlama Fonksiyonu
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

    // Toplam süreyi saniyeye çevir
    int totalSeconds = seconds + minutes * 60 + hours * 3600 + days * 86400;

    if (totalSeconds <= 0) {
        printf("Invalid duration. Please enter a positive duration.\n");
        enterToContinue();
        return 0;
    }

    printf("Setting reminder for %d seconds...\n", totalSeconds);

    // Geri sayım yaparak bekleme
    for (int remaining = totalSeconds; remaining > 0; --remaining) {
        clearScreen();
        printf("Time remaining: %02d:%02d:%02d:%02d\n",
            remaining / 86400,            // Gün
            (remaining % 86400) / 3600,   // Saat
            (remaining % 3600) / 60,      // Dakika
            remaining % 60);              // Saniye

        platformSleep(1);  // 1 saniye bekle
    }

    printf("Time's up! Reminder triggered.\n");
    enterToContinue();

}

// Platforma göre uyumlu bekleme fonksiyonu
int platformSleep(int seconds) {
#ifdef _WIN32
    Sleep(seconds * 1000);  // Windows: milisaniye cinsinden bekler
#else
    sleep(seconds);         // Linux/macOS: saniye cinsinden bekler
#endif
    return 1;
}





int notificationSettings() {
    clearScreen();

    // Mevcut bildirim yöntemini göster
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
        notificationSettings();  // Geçersiz girişte tekrar çağır
        return 1;
    }

    enterToContinue();
    return 0;
}

// Mevcut bildirim yöntemini ekranda gösterir
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

SparseMatrixNode* createNode(int row, int col, int value) {
    SparseMatrixNode* newNode = (SparseMatrixNode*)malloc(sizeof(SparseMatrixNode));
    newNode->row = row;
    newNode->col = col;
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

// Sparse Matrix'e değer ekleme
int addNotification(SparseMatrixNode** head, int row, int col, int value) {
    SparseMatrixNode* newNode = createNode(row, col, value);
    newNode->next = *head;
    *head = newNode;
    return 1;
}

// Bildirimleri gösterme
int displayNotifications(SparseMatrixNode* head) {
    SparseMatrixNode* temp = head;
    while (temp != NULL) {
        printf("Task ID: %d, Date: %d, Notification Type: %d\n", temp->row, temp->col, temp->value);
        temp = temp->next;
    }
    return 1;
}

int updateNotificationMethod(int taskId, int date, int method, SparseMatrixNode** head) {
    addNotification(head, taskId, date, method);
    printf("Notification method updated for Task ID %d on Date %d\n", taskId, date);
    return 1;
}

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






int taskPrioritizationMenu() {
    int choice;

    while (1) {
        printTaskPrioritizationMenu();  // Menüyü ekrana yazdır
        choice = getInput();  // Kullanıcıdan giriş al

        if (choice == -2) {  // Hatalı giriş durumu
            handleInputError();
            enterToContinue();
            continue;
        }

        switch (choice) {
        case 1:
            markTaskImportance();  // Görevin önemini belirleme
            break;
        case 2:
            break;
        case 3:
            return 0;  // Menüden çıkış
        default:
            clearScreen();
            printf("Invalid choice. Please try again.\n");
            enterToContinue();
            break;
        }
    }
}


void markTaskImportance() {
    clearScreen();

    Task tasks[100];  // Maksimum 100 görev için yer ayırıyoruz
    int taskCount = loadTasks(tasks, 100);  // Görevleri yükle

    if (taskCount <= 0) {
        printf("No tasks available. Please add tasks first.\n");
        enterToContinue();
        return;
    }

    // Tüm görevleri göster
    printf("Tasks loaded:\n");
    for (int i = 0; i < taskCount; ++i) {
        const char* importanceStr =
            tasks[i].impid == 1 ? "Low" :
            tasks[i].impid == 2 ? "Medium" :
            tasks[i].impid == 3 ? "High" : "Unmarked";
        printf("ID: %d, Name: %s, Importance: %s\n",
            tasks[i].id, tasks[i].name, importanceStr);
    }

    // Kullanıcıdan görevin adını al
    char taskName[100];
    Task* selectedTask = NULL;

    while (1) {
        printf("\nEnter the name of the task to mark importance: ");
        scanf(" %[^\n]%*c", taskName);  // Boşluklu girişleri alır

        // Görev adını bul ve işaretle
        for (int i = 0; i < taskCount; ++i) {
            if (strcmp(tasks[i].name, taskName) == 0) {
                selectedTask = &tasks[i];
                break;
            }
        }

        if (selectedTask) {
            break;  // Geçerli bir görev bulunduysa döngüden çık
        }
        else {
            printf("Task not found! Please enter a valid task name.\n");
        }
    }

    // Kullanıcıdan önem seviyesini al
    int importanceId;
    while (1) {
        printf("Enter the importance ID (1: Low, 2: Medium, 3: High): ");
        importanceId = getInput();

        if (importanceId >= 1 && importanceId <= 3) {
            break;  // Geçerli önem seviyesi girildiyse döngüden çık
        }
        else {
            printf("Invalid importance value! Please enter 1, 2, or 3.\n");
        }
    }

    // Önem seviyesini güncelle
    selectedTask->impid = importanceId;

    // Güncellenen görev listesini dosyaya kaydet
    saveTasks(tasks, taskCount);

    printf("Importance level of '%s' marked successfully as %d.\n",
        selectedTask->name, importanceId);
    enterToContinue();
}







int findTaskByName(const char* name) {
    for (int i = 0; i < taskCount; i++) {
        if (strcmp(tasks[i].name, name) == 0) {
            return i;  // Görev bulunduysa indeksini döndür
        }
    }
    return -1;  // Görev bulunamazsa -1 döner
}




int hashFunction(const char* email) {
    int hash = 0;
    while (*email) {
        hash = (hash + *email) % TABLE_SIZE;
        email++;
    }
    return hash;
}

int hashFunction2(const char* email) {
    int hash = 0;
    while (*email) {
        hash = (hash + *email) % (TABLE_SIZE - 1);  // TABLE_SIZE - 1 ile çarpma önerilir
        email++;
    }
    return (TABLE_SIZE - 1 - hash);  // Double hashing için dönüş değeri
}
int secondHashFunction(const char* email) {
    int hash = 0;
    while (*email) {
        hash = (hash * 31 + *email) % TABLE_SIZE;  // Daha etkili çakışma çözümü için ikinci hash fonksiyonu
        email++;
    }
    return (TABLE_SIZE - 1 - hash);
}


void insertUserToHashTableWithLinearProbing(User* user) {
    int index = hashFunction(user->email);
    int originalIndex = index;

    printf("Inserting user with email: %s\n", user->email);  // Hata ayıklama için

    // Linear Probing ile boş yer arıyoruz
    while (hashTable[index] != NULL) {
        // Eğer aynı e-posta mevcutsa kullanıcı zaten kayıtlı
        if (strcmp(hashTable[index]->email, user->email) == 0) {
            printf("User already exists at index %d.\n", index);  // Hata ayıklama için
            return;
        }

        // Bir sonraki konuma geç ve mod işlemi ile dairesel dolaşım sağla
        index = (index + 1) % TABLE_SIZE;

        // Eğer tabloda döngü yapıyorsak, tablo dolu demektir
        if (index == originalIndex) {
            printf("Hash table is full, cannot add more users.\n");
            return;
        }
    }

    // Kullanıcıyı boş bulunan konuma ekle
    hashTable[index] = (User*)malloc(sizeof(User));
    *hashTable[index] = *user;

    printf("User added at index %d.\n", index);  // Kullanıcının eklendiğini doğrulamak için
}

void insertUserToHashTableWithOverflow(User* user) {
    int index = hashFunction(user->email);

    // Ana hash tablosunda boş yer arama
    if (hashTable[index] == NULL) {
        hashTable[index] = (User*)malloc(sizeof(User));
        *hashTable[index] = *user;
        printf("User added at index %d in main hash table.\n", index);
    }
    else {
        // Eğer ana tabloda çakışma varsa taşma alanına ekle
        if (overflowCount < OVERFLOW_SIZE) {
            overflowArea[overflowCount++] = *user;
            printf("User added at index %d in overflow area.\n", overflowCount - 1);
        }
        else {
            printf("Overflow area is full, cannot add more users.\n");
        }
    }
}

void insertUserToHashTableWithQuadraticProbing(User* user) {
    int index = hashFunction(user->email);
    int i = 1;

    // Quadratic Probing ile boş yer arama
    while (hashTable[index] != NULL) {
        // Eğer aynı e-posta mevcutsa kullanıcı zaten kayıtlı
        if (strcmp(hashTable[index]->email, user->email) == 0) {
            printf("User already exists at index %d.\n", index);  // Hata ayıklama için
            return;
        }

        // Çakışma olduğunda quadratik artış yaparak yeni indeks belirle
        index = (index + i * i) % TABLE_SIZE;
        i++;

        // Eğer döngü oluşursa, tablo dolu demektir
        if (i >= TABLE_SIZE) {
            printf("Hash table is full, cannot add more users.\n");
            return;
        }
    }

    // Kullanıcıyı boş bulunan konuma ekle
    hashTable[index] = (User*)malloc(sizeof(User));
    *hashTable[index] = *user;

    printf("User added at index %d using Quadratic Probing.\n", index);
}

void insertUserToHashTableWithDoubleHashing(User* user) {
    int index = hashFunction(user->email);
    int stepSize = hashFunction2(user->email);
    int i = 0;

    // Double Hashing ile boş yer arama
    while (hashTable[index] != NULL) {
        // Eğer aynı e-posta mevcutsa kullanıcı zaten kayıtlı
        if (strcmp(hashTable[index]->email, user->email) == 0) {
            printf("User already exists at index %d.\n", index);  // Hata ayıklama için
            return;
        }

        // Çakışma durumunda ikinci hash fonksiyonunu kullanarak yeni konuma geçiş yap
        index = (index + stepSize) % TABLE_SIZE;
        i++;

        // Eğer döngü oluşursa, tablo dolu demektir
        if (i >= TABLE_SIZE) {
            printf("Hash table is full, cannot add more users.\n");
            return;
        }
    }

    // Kullanıcıyı boş bulunan konuma ekle
    hashTable[index] = (User*)malloc(sizeof(User));
    *hashTable[index] = *user;

    printf("User added at index %d using Double Hashing.\n", index);
}

void insertUserToHashTableWithLinearQuotient(User* user) {
    int index = hashFunction(user->email);
    int stepSize = Q_STEP;

    // Linear Quotient ile boş yer arama
    while (hashTable[index] != NULL) {
        // Eğer aynı e-posta mevcutsa, kullanıcı zaten kayıtlı
        if (strcmp(hashTable[index]->email, user->email) == 0) {
            printf("User already exists at index %d.\n", index);  // Hata ayıklama için
            return;
        }

        // Çakışma olduğunda sabit bir adım boyutu kadar ilerle
        index = (index + stepSize) % TABLE_SIZE;

        // Eğer tablo tamamen doluysa, kullanıcı eklenemez
        if (index == hashFunction(user->email)) {
            printf("Hash table is full, cannot add more users.\n");
            return;
        }
    }

    // Kullanıcıyı boş bulunan konuma ekle
    hashTable[index] = (User*)malloc(sizeof(User));
    *hashTable[index] = *user;

    printf("User added at index %d using Linear Quotient.\n", index);
}

void insertUserToHashTableWithBrentsMethod(User* user) {
    int index = hashFunction(user->email);
    int stepSize = secondHashFunction(user->email);

    if (hashTable[index] == NULL) {
        // Eğer ilk pozisyon boşsa doğrudan ekle
        hashTable[index] = (User*)malloc(sizeof(User));
        *hashTable[index] = *user;
        printf("User added at index %d using Brent's Method.\n", index);
    }
    else {
        // Çakışma durumunda
        int i = 1, newIndex = index;
        User* currentUser = user;

        while (hashTable[newIndex] != NULL) {
            // İki alternatif stratejiyi değerlendir: mevcut konumdan devam etmek veya yer değiştirmek
            int trialIndex1 = (newIndex + i * stepSize) % TABLE_SIZE;  // İleri adımla yeni konum
            int trialIndex2 = (index + i) % TABLE_SIZE;                // Alternatif konum

            // En kısa adım sayısıyla uygun pozisyonu seç
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


User* searchUserInHashTable(const char* email, const char* password) {
    int index = hashFunction(email);
    int originalIndex = index;

    // Linear Probing ile kullanıcı arama
    while (hashTable[index] != NULL) {
        if (strcmp(hashTable[index]->email, email) == 0 &&
            strcmp(hashTable[index]->password, password) == 0) {
            return hashTable[index];  // Kullanıcı bulundu
        }

        // Bir sonraki konuma geç ve mod işlemi ile dairesel dolaşım sağla
        index = (index + 1) % TABLE_SIZE;

        // Eğer tabloda döngü yapıyorsak, kullanıcı yok demektir
        if (index == originalIndex) {
            break;
        }
    }
    return NULL;  // Kullanıcı bulunamadı
}




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

    insertUserToHashTableWithLinearProbing(&user);  // Linear Probing ile kullanıcı ekle

    printf("User insertion complete.\n");  // Kullanıcı eklemenin bittiğini gösteren mesaj
    enterToContinue();
}

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

    insertUserToHashTableWithOverflow(&user);  // Progressive Overflow ile kullanıcı ekle

    printf("User insertion complete.\n");  // Kullanıcı eklemenin bittiğini gösteren mesaj
    enterToContinue();
}


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

    insertUserToHashTableWithQuadraticProbing(&user);  // Quadratic Probing ile kullanıcı ekle

    printf("User insertion complete.\n");
    enterToContinue();
}

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

    insertUserToHashTableWithDoubleHashing(&user);  // Double Hashing ile kullanıcı ekle

    printf("User insertion complete.\n");
    enterToContinue();
}

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

    insertUserToHashTableWithLinearQuotient(&user);  // Linear Quotient ile kullanıcı ekle

    printf("User insertion complete.\n");
    enterToContinue();
}

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

    insertUserToHashTableWithBrentsMethod(&user);  // Brent's Method ile kullanıcı ekle

    printf("User insertion complete.\n");
    enterToContinue();
}


// Huffman ile parolayı sıkıştırarak döndüren işlev
char* huffmanEncode(const char* input) {
    // Huffman kodlaması burada gerçekleştirilir ve sıkıştırılmış dize döndürülür
    // Örnek olarak, doğrudan aynı metni döndürüyoruz (gerçek projede değiştirilmelidir).
    char* encoded = strdup(input);
    return encoded;
}

// Huffman kodlu parolayı çözen işlev
char* huffmanDecode(const char* encoded) {
    // Huffman çözme işlemi burada yapılır ve orijinal metin döndürülür.
    char* decoded = strdup(encoded);
    return decoded;
}


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

    return registerUser(newUser, pathFileUsers);
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
    if (fread(&userCount, sizeof(int), 1, file) != 1) {
        printf("Failed to read user count from file.\n");
        fclose(file);
        return 0;
    }

    int found = 0;
    User tempUser;

    for (int i = 0; i < userCount; ++i) {
        // Kullanıcı verilerini dosyadan oku
        if (fread(&tempUser, sizeof(User), 1, file) != 1) {
            printf("Error reading user data from file.\n");
            break;
        }

        // E-posta ve şifreyi Huffman kod çözme işlemiyle çöz
        char* decodedEmail = huffmanDecode(tempUser.email);
        char* decodedPassword = huffmanDecode(tempUser.password);

        // E-posta ve şifre kontrolü yap
        if (strcmp(decodedEmail, loginUser.email) == 0 && strcmp(decodedPassword, loginUser.password) == 0) {
            printf("Login successful.\n");
            loggedUser = tempUser;
            found = 1;

            // Dinamik olarak ayrılan belleği serbest bırak
            free(decodedEmail);
            free(decodedPassword);
            break;
        }

        // Dinamik olarak ayrılan belleği serbest bırak
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
 *
 * @param pathFileUsers The path to the user database file.
 * @return 1 if the login process is completed successfully, 0 otherwise.
 */
int loginUserMenu(const char* pathFileUsers) {
    clearScreen();
    User userInput;  // Değişken adını loginUser yerine userInput yaptık.

    printf("Enter email: ");
    fgets(userInput.email, sizeof(userInput.email), stdin);
    userInput.email[strcspn(userInput.email, "\n")] = 0;

    printf("Enter password: ");
    fgets(userInput.password, sizeof(userInput.password), stdin);
    userInput.password[strcspn(userInput.password, "\n")] = 0;

    // loginUser fonksiyonunu çağırırken yeni değişkeni kullanıyoruz
    return loginUser(userInput, pathFileUsers);
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
        case 5:
            algorithmsMenu();
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
            //useOfBuckets();
            break;
        case 6:
            linearQuotientDemo();
            break;
        case 7:
            brentsMethodDemo();
            break;
        case 8:
            return;  // Ana menüye dön
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






