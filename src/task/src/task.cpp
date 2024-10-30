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
const char* filename = "tasks.bin";




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
            viewTask();  // Görevleri görüntüleme
            enterToContinue();
            break;
        case 3:
            categorizeTask();  // Kategoriye göre listeleme
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
        return 0;
    }

    Task newTask;
    newTask.id = *taskCount + 1;

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

    // Görevi listeye ekle ve sayacı artır
    taskList[*taskCount] = newTask;
    (*taskCount)++;

    // Görevleri kaydet
    saveTasks(taskList, *taskCount);

    printf("Task added and saved successfully!\n");
    return 1;
}



void viewTask() {
    FILE* file = fopen("tasks.bin", "rb");  // Dosyayı okuma modunda aç
    if (file == NULL) {
        printf("No tasks found. The task list is empty.\n");
        enterToContinue();
        return;
    }

    Task task;
    int taskCount = 0;

    printf("\n--- List of Tasks ---\n");
    // Dosyadaki tüm görevleri sırayla okuyalım
    while (fread(&task, sizeof(Task), 1, file)) {
        printf("ID: %d\n", task.id);
        printf("Name: %s\n", task.name);
        printf("Description: %s\n", task.description);
        printf("Category: %s\n", task.category);
        printf("Due Date: %s\n", task.dueDate);
        printf("---------------------------\n");
        taskCount++;
    }

    if (taskCount == 0) {
        printf("No tasks available.\n");
    }

    fclose(file);  // Dosyayı kapatmayı unutmayın
    enterToContinue();  // Kullanıcının devam etmesi için bekle
}



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

void viewDeadlines() {
    FILE* tasksFile = fopen("tasks.bin", "rb");  // Görev adlarını okuma
    FILE* deadlinesFile = fopen("deadlines.bin", "rb");  // Son teslim tarihlerini okuma

    if (!tasksFile || !deadlinesFile) {
        printf("Error: Could not open tasks or deadlines file.\n");
        if (tasksFile) fclose(tasksFile);
        if (deadlinesFile) fclose(deadlinesFile);
        return;
    }

    Task task;
    Assignment deadline;
    int taskCount = 0;

    printf("\n--- Upcoming Deadlines ---\n");
    printf("----------------------------\n");

    // Her iki dosyadan sırayla görev ve deadline bilgilerini okuyalım
    while (fread(&task, sizeof(Task), 1, tasksFile) == 1 &&
        fread(&deadline, sizeof(Assignment), 1, deadlinesFile) == 1) {
        printf("%d. Task: %s - Deadline: %02d/%02d/%04d\n",
            ++taskCount,
            task.name,
            deadline.day,
            deadline.month,
            deadline.year);
    }

    if (taskCount == 0) {
        printf("No assignments to display.\n");
    }

    printf("----------------------------\n");

    fclose(tasksFile);  // Dosyaları kapat
    fclose(deadlinesFile);
    printf("\n");
    enterToContinue();  // Kullanıcıdan devam etmesini bekle
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

int findTaskByName(const char* name) {
    for (int i = 0; i < taskCount; i++) {
        if (strcmp(tasks[i].taskName, name) == 0) {
            return i;
        }
    }
    return -1; // Görev bulunamazsa -1 döner
}

void markTaskImportance() {
    char taskName[50];
    int taskIndex, importance;

    if (taskCount == 0) {
        printf("The task list is empty.\n");
        enterToContinue();
        return;
    }

    clearScreen();
    printf("Select the task you want to set importance for:\n");
    for (int i = 0; i < taskCount; i++) {
        const char* importanceStr =
            tasks[i].importance == 1 ? "Low" :
            tasks[i].importance == 2 ? "Medium" : "High";

        printf("%d. %s (Importance: %s)\n", i + 1, tasks[i].taskName, importanceStr);
    }

    while (1) {
        printf("Enter the name of the task: ");
        scanf(" %[^\n]", taskName);  // Görev ismini okuma

        taskIndex = findTaskByName(taskName);
        if (taskIndex != -1) break;  // Geçerli görev bulunduysa döngüden çık

        printf("Task not found! Please enter a valid task name.\n");
    }

    while (1) {
        printf("Set the new importance (1: Low, 2: Medium, 3: High): ");
        importance = getInput();

        if (importance >= 1 && importance <= 3) break;  // Geçerli önem seviyesi girildiyse çık

        printf("Invalid importance value! Please enter 1, 2, or 3.\n");
    }

    tasks[taskIndex].importance = importance;
    printf("Importance level updated successfully!\n");

    enterToContinue();
}

void reorderTasks() {
    char fromTaskName[50], toTaskName[50];
    int fromIndex, toIndex;

    if (taskCount < 2) {
        printf("You need at least two tasks to reorder.\n");
        enterToContinue();
        return;
    }

    clearScreen();
    printf("Reorder your tasks:\n");
    for (int i = 0; i < taskCount; i++) {
        const char* importanceStr =
            tasks[i].importance == 1 ? "Low" :
            tasks[i].importance == 2 ? "Medium" : "High";

        printf("%d. %s (Importance: %s)\n", i + 1, tasks[i].taskName, importanceStr);
    }

    while (1) {
        printf("Enter the name of the task you want to move: ");
        scanf(" %[^\n]", fromTaskName);

        fromIndex = findTaskByName(fromTaskName);
        if (fromIndex != -1) break;  // Geçerli görev bulunduysa döngüden çık

        printf("Task not found! Please enter a valid task name.\n");
    }

    while (1) {
        printf("Enter the name of the task to move it before: ");
        scanf(" %[^\n]", toTaskName);

        toIndex = findTaskByName(toTaskName);
        if (toIndex != -1) break;  // Geçerli görev bulunduysa döngüden çık

        printf("Task not found! Please enter a valid task name.\n");
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