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


#define TABLE_SIZE 10  // Hash tablosunun boyutu
#define MAX_TASKS 100  // Maksimum görev sayısı
#define MAX_ASSIGNMENT_NAME 50
Assignment assignments[100];
const char* filename = "tasks.bin";





Task tasks[100];
User* hashTable[TABLE_SIZE];
Task taskList[MAX_TASKS];  // Görev listesi
int taskCount = 0;  // Mevcut görev sayısı
int notificationMethod = 0;



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
    printf("2. View Tasks\n");
    printf("3. Categorize Tasks\n");
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
    printf("2. Importance Ordering\n");
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

    // Görevi kuyruğa ekle
    enqueue(newTask);

    // Görevi geri alma için yığına ekle
    push(newTask);

    // Görevleri kaydet
    saveTasks(taskList, *taskCount);

    printf("Task added and saved successfully!\n");
    return 1;
}



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

void push(Task task) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->task = task;
    newNode->next = stackTop;
    stackTop = newNode;
}

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

MinHeap deadlineHeap = { {0}, 0 };  // İlk alan tüm görevler için sıfırlanır, ikinci alan size olarak 0 atanır

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

void viewDeadlines() {
    if (deadlineHeap.size == 0) {
        printf("No deadlines to display.\n");
        return;
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
}

int getDateKey(int day, int month, int year) {
    return year * 10000 + month * 100 + day;  // YYYYMMDD formatında bir anahtar oluşturur
}

void insertInLeaf(BPlusTreeNode* leaf, int key, ScheduledTask* task) {
    int i = leaf->numKeys - 1;
    while (i >= 0 && leaf->keys[i] > key) {
        leaf->keys[i + 1] = leaf->keys[i];
        leaf->tasks[i + 1] = leaf->tasks[i];
        i--;
    }
    leaf->keys[i + 1] = key;
    leaf->tasks[i + 1] = task;
    leaf->numKeys++;
}

void insertInBPlusTree(BPlusTree* tree, ScheduledTask* task) {
    int key = getDateKey(task->day, task->month, task->year);
    BPlusTreeNode* root = tree->root;

    if (root->numKeys < MAX_KEYS) {
        insertInLeaf(root, key, task);
    }
    else {
        printf("Node splitting required. Implement split logic here.\n");
    }
}

void searchInDateRange(BPlusTreeNode* node, int startKey, int endKey) {
    if (node == NULL) return;

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
}

void viewDeadlinesInRange(BPlusTree* tree) {
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
}

void swap(Assignment* a, Assignment* b) {
    Assignment temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(MinHeap* heap, int i) {
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
}

// Yığına görevi deadline sırasına göre ekleme
void insertMinHeap(MinHeap* heap, Assignment deadline) {
    if (heap->size == MAX_HEAP_SIZE) {
        printf("Heap is full\n");
        return;
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
}

// Yığının en üstündeki (en yakın deadline'a sahip) görevi çıkartma
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
void setReminders() {
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
        return;
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
void platformSleep(int seconds) {
#ifdef _WIN32
    Sleep(seconds * 1000);  // Windows: milisaniye cinsinden bekler
#else
    sleep(seconds);         // Linux/macOS: saniye cinsinden bekler
#endif
}





void notificationSettings() {
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
        return;
    }

    enterToContinue();
}

// Mevcut bildirim yöntemini ekranda gösterir
void showCurrentNotificationMethod() {
    if (notificationMethod == 0) {
        printf("No notification method selected yet.\n");
    }
    else {
        const char* methodStr =
            (notificationMethod == 1) ? "SMS" :
            (notificationMethod == 2) ? "E-Mail" : "Notification";

        printf("Current notification method: %s\n", methodStr);
    }
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
void addNotification(SparseMatrixNode** head, int row, int col, int value) {
    SparseMatrixNode* newNode = createNode(row, col, value);
    newNode->next = *head;
    *head = newNode;
}

// Bildirimleri gösterme
void displayNotifications(SparseMatrixNode* head) {
    SparseMatrixNode* temp = head;
    while (temp != NULL) {
        printf("Task ID: %d, Date: %d, Notification Type: %d\n", temp->row, temp->col, temp->value);
        temp = temp->next;
    }
}

void updateNotificationMethod(int taskId, int date, int method, SparseMatrixNode** head) {
    addNotification(head, taskId, date, method);
    printf("Notification method updated for Task ID %d on Date %d\n", taskId, date);
}

void showCurrentNotificationMethod(SparseMatrixNode* head, int taskId, int date) {
    SparseMatrixNode* temp = head;
    while (temp != NULL) {
        if (temp->row == taskId && temp->col == date) {
            const char* methodStr =
                (temp->value == 1) ? "SMS" :
                (temp->value == 2) ? "E-Mail" : "Notification";
            printf("Current notification method for Task ID %d on Date %d: %s\n", taskId, date, methodStr);
            return;
        }
        temp = temp->next;
    }
    printf("No notification method selected for Task ID %d on Date %d.\n", taskId, date);
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


void loadTasksFromFile() {
    FILE* file = fopen("tasks.bin", "rb");
    if (file == NULL) {
        printf("Error opening tasks.bin! No tasks loaded.\n");
        return;
    }

    taskCount = fread(tasks, sizeof(Task), 100, file);
    fclose(file);

    printf("%d tasks loaded successfully from tasks.bin.\n", taskCount);
}

// tasks.bin dosyasına görevleri kaydeden fonksiyon
void saveTasksToFile() {
    FILE* file = fopen("tasks.bin", "wb");
    if (file == NULL) {
        printf("Error opening tasks.bin for writing!\n");
        return;
    }

    fwrite(tasks, sizeof(Task), taskCount, file);
    fclose(file);

    printf("Tasks saved successfully to tasks.bin.\n");
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


char* huffmanEncode(const char* text) {
    // Metni Huffman kodlaması ile sıkıştırılmış bir dizgeye çevirir.
    // Not: Gerçek bir Huffman kodlaması için daha fazla kod gerekebilir.
    // Bu örnek işlev, sıkıştırılmış bir örnek metin döndürür.
    return "encoded_text";  // Örnek sıkıştırılmış metin (gerçek kodlamayı buraya eklemeniz gerekebilir)
}

char* huffmanDecode(const char* encodedText) {
    // Kodlanmış Huffman dizgesini orijinal metne çevirir.
    return "decoded_text";  // Örnek çözülmüş metin (gerçek kod çözme işlevini buraya ekleyin)
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

    // E-posta ve parolayı sıkıştırarak sakla
    char* encodedEmail = huffmanEncode(user.email);
    char* encodedPassword = huffmanEncode(user.password);
    strcpy(user.email, encodedEmail);
    strcpy(user.password, encodedPassword);

    // Belleği temizle
    free(encodedEmail);
    free(encodedPassword);

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

    // Kullanıcının giriş e-posta ve parolasını sıkıştır
    char* encodedEmail = huffmanEncode(loginUser.email);
    char* encodedPassword = huffmanEncode(loginUser.password);

    // Hash tablosunu doldur ve kullanıcıyı ara
    for (int i = 0; i < userCount; ++i) {
        insertUserToHashTable(&users[i]);
    }

    // Sıkıştırılmış e-posta ve parola ile kullanıcıyı arayın
    loginUser.email[0] = '\0';
    loginUser.password[0] = '\0';

    User* foundUser = NULL;
    for (int i = 0; i < userCount; ++i) {
        if (strcmp(users[i].email, encodedEmail) == 0 && strcmp(users[i].password, encodedPassword) == 0) {
            foundUser = &users[i];
            break;
        }
    }

    free(encodedEmail);
    free(encodedPassword);

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






