﻿#define _CRT_SECURE_NO_WARNINGS
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

#define TABLE_SIZE 10  // Hash tablosunun boyutu
#define MAX_TASKS 100  // Maksimum görev sayısı
#define MAX_ASSIGNMENT_NAME 50
Assignment assignments[100];



User* hashTable[TABLE_SIZE];
Task taskList[MAX_TASKS];  // Görev listesi
int taskCount = 0;  // Mevcut görev sayısı
TaskInfo tasks[MAX_TASKS]; // Görevler burada tutulur


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

int createTaskMenu(Task taskList[], int* taskCount) {
    int maxTasks = 100;  // Lokal olarak tanımlanan ve başlangıç değeri atanan değişken
    int choice;

    while (1) {
        printCreateTaskMenu();  // Menü çıktısını ekrana basar
        choice = getInput();  // Kullanıcıdan giriş alır

        if (choice == -2) {
            handleInputError();  // Hatalı giriş
            enterToContinue();
            continue;
        }

        switch (choice) {
        case 1:
            addTask(taskList, taskCount, maxTasks);  // Görev ekleme
            break;
        case 2:
            viewTask(taskList, *taskCount);  // Görevleri görüntüleme
            enterToContinue();
            break;
        case 3:
            categorizeTask(taskList, *taskCount);  // Kategoriye göre listeleme
            break;
        case 4:
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
        return 0;  // Görev listesi dolu
    }

    Task newTask;
    newTask.id = *taskCount + 1;  // Otomatik ID atama

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

    // Görevi listeye ekle
    taskList[*taskCount] = newTask;
    (*taskCount)++;

    printf("Task added successfully!\n");
    return 1;  // Başarıyla eklendi
}


void viewTask(const Task taskList[], int taskCount) {
    if (taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }

    printf("List of Tasks:\n");
    for (int i = 0; i < taskCount; i++) {
        printf("ID: %d\n", taskList[i].id);
        printf("Name: %s\n", taskList[i].name);
        printf("Description: %s\n", taskList[i].description);
        printf("Category: %s\n", taskList[i].category);
        printf("Due Date: %s\n", taskList[i].dueDate);
        printf("---------------------------\n");
    }
}


void categorizeTask(const Task taskList[], int taskCount) {
    if (taskCount == 0) {
        printf("No tasks available to categorize.\n");
        enterToContinue();
        return;
    }

    char category[50];
    printf("Enter category to filter: ");
    fgets(category, sizeof(category), stdin);
    category[strcspn(category, "\n")] = 0;  // Yeni satırı sil

    int found = 0;
    printf("Tasks in category '%s':\n", category);
    for (int i = 0; i < taskCount; i++) {
        if (strcmp(taskList[i].category, category) == 0) {
            printf("ID: %d\n", taskList[i].id);
            printf("Name: %s\n", taskList[i].name);
            printf("Description: %s\n", taskList[i].description);
            printf("Due Date: %s\n", taskList[i].dueDate);
            printf("---------------------------\n");
            found = 1;
        }
    }

    if (!found) {
        printf("No tasks found in this category.\n");
    }

    enterToContinue();  // Kullanıcı devam etmek için bir tuşa basar
}






// Menü fonksiyonu
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
            viewDeadlines(assignments, taskCount);
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

// Kullanıcıdan görev bilgisi alıp deadline atayan fonksiyon
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

    printf("Deadline assigned successfully!\n");
    return 0;
}


void viewDeadlines(Assignment assignments[], int taskCount) {
    if (taskCount == 0) {
        printf("No assignments to display.\n");
        return;
    }

    printf("Upcoming Deadlines:\n");
    printf("----------------------------\n");

    // Tüm görevleri listele
    for (int i = 0; i < taskCount; ++i) {
        printf("%d. %s - Deadline: %02d/%02d/%04d\n",
            i + 1,
            assignments[i].name,
            assignments[i].day,
            assignments[i].month,
            assignments[i].year);
    }

    printf("----------------------------\n");
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
            break;
        case 2:
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
            markTaskImportance();
            break;
        case 2:
            reorderTasks();
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

void markTaskImportance() {
    int taskIndex, importance;

    if (taskCount == 0) {
        printf("The task list is empty.\n");
        enterToContinue();
        return;
    }

    clearScreen();
    printf("Select the task you want to set importance for:\n");
    for (int i = 0; i < taskCount; i++) {
        printf("%d. %s (Importance: %s)\n",
            i + 1,
            tasks[i].taskName,
            tasks[i].importance ? "High" : "Low");
    }

    printf("Enter your choice (1-%d): ", taskCount);
    taskIndex = getInput() - 1;

    if (taskIndex < 0 || taskIndex >= taskCount) {
        printf("Invalid selection!\n");
            enterToContinue();
        return;
    }

    printf("Set the new importance (0: Low, 1: High): ");
    importance = getInput();

    if (importance != 0 && importance != 1) {
        printf("Invalid importance value!\n");
    }
    else {
        tasks[taskIndex].importance = importance;
        printf("Importance level updated successfully!\n");
    }

    enterToContinue();
}


void reorderTasks() {
    int fromIndex, toIndex;

    if (taskCount < 2) {
        printf("You need at least two tasks to reorder.\n");
        enterToContinue();
        return;
    }

    clearScreen();
    printf("Reorder your tasks:\n");
    for (int i = 0; i < taskCount; i++) {
        printf("%d. %s (Importance: %s)\n",
            i + 1,
            tasks[i].taskName,
            tasks[i].importance ? "High" : "Low");
    }

    printf("Which task do you want to move? (1-%d): ", taskCount);
    fromIndex = getInput() - 1;

    printf("Where do you want to move this task? (1-%d): ", taskCount);
    toIndex = getInput() - 1;

    if (fromIndex < 0 || fromIndex >= taskCount || toIndex < 0 || toIndex >= taskCount) {
        printf("Invalid selection!\n");
        enterToContinue();
        return;
    }

    // Görevi geçici olarak kaydet
    TaskInfo temp = tasks[fromIndex];

    // Taşınan görev sonrası elemanları kaydır
    if (fromIndex < toIndex) {
        for (int i = fromIndex; i < toIndex; i++) {
            tasks[i] = tasks[i + 1];
        }
    }
    else {
        for (int i = fromIndex; i > toIndex; i--) {
            tasks[i] = tasks[i - 1];
        }
    }

    // Görevi yeni yerine ekle
    tasks[toIndex] = temp;

    printf("Task moved successfully!\n");
    enterToContinue();
}


int hashFunction(const char* email) {
    int hash = 0;
    while (*email) {
        hash = (hash + *email) % TABLE_SIZE;
        email++;
    }
    return hash;
}

void insertUserToHashTable(User* user) {
    int index = hashFunction(user->email);

    // Yeni kullanıcıyı eklemek için bağlı liste başına yerleştiriyoruz
    user->next = hashTable[index];
    hashTable[index] = user;
}

User* searchUserInHashTable(const char* email, const char* password) {
    int index = hashFunction(email);
    User* current = hashTable[index];

    // Bağlı liste üzerinde arama yapıyoruz
    while (current) {
        if (strcmp(current->email, email) == 0 && strcmp(current->password, password) == 0) {
            return current;  // Kullanıcı bulundu
        }
        current = current->next;
    }
    return NULL;  // Kullanıcı bulunamadı
}


int registerUser(User user, const char* pathFileUser) {
    FILE* file = fopen(pathFileUser, "rb+");
    int userCount = 0;
    User* users = NULL;

    if (file) {
        fread(&userCount, sizeof(int), 1, file);
        users = (User*)malloc(sizeof(User) * userCount);
        fread(users, sizeof(User), userCount, file);

        // Hash tablosunu doldur
        for (int i = 0; i < userCount; ++i) {
            insertUserToHashTable(&users[i]);
        }

        // Kullanıcı zaten var mı kontrol et
        if (searchUserInHashTable(user.email, user.password)) {
            printf("User already exists.\n");
            fclose(file);
            free(users);
            enterToContinue();
            return 0;
        }
    }
    else {
        file = fopen(pathFileUser, "wb");
    }

    user.id = userCount + 1;  // Yeni kullanıcı ID'si
    insertUserToHashTable(&user);  // Hash tablosuna ekle

    // Kullanıcıyı dosyaya ekle
    userCount++;
    users = (User*)realloc(users, sizeof(User) * userCount);
    users[userCount - 1] = user;

    rewind(file);
    fwrite(&userCount, sizeof(int), 1, file);
    fwrite(users, sizeof(User), userCount, file);

    printf("User registered successfully: Welcome %s %s\n", user.name, user.surname);

    free(users);
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
    fread(&userCount, sizeof(int), 1, file);
    User* users = (User*)malloc(sizeof(User) * userCount);
    fread(users, sizeof(User), userCount, file);

    // Hash tablosunu doldur
    for (int i = 0; i < userCount; ++i) {
        insertUserToHashTable(&users[i]);
    }

    User* foundUser = searchUserInHashTable(loginUser.email, loginUser.password);
    if (foundUser) {
        printf("Login successful.\n");
        loggedUser = *foundUser;
        free(users);
        fclose(file);
        enterToContinue();
        return 1;
    }
    else {
        printf("Incorrect email or password.\n");
        free(users);
        fclose(file);
        enterToContinue();
        return 0;
    }
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