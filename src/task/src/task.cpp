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

TaskNode* head = NULL;      // Liste başı (ilk görev)
TaskNode* tail = NULL;      // Liste sonu (son görev)
XORNode* xorHead = NULL;
XORNode* xorTail = NULL;



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
User overflowArea[OVERFLOW_SIZE];
int overflowCount = 0;



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
    printf("5. Algorithms\n");
    printf("6. Exit\n");
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

void loadTasksToXORList(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Unable to open tasks file.\n");
        return;
    }

    Task task;
    while (fread(&task, sizeof(Task), 1, file)) {
        addTaskToXORList(task); // Her bir görevi XOR Linked List'e ekle
    }

    fclose(file);
    printf("Tasks loaded into XOR Linked List successfully!\n");
}

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

// Görevlerin bağımlılıklarını DFS ile takip eden yardımcı fonksiyon
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

// Ana fonksiyon: Bir görevin tüm bağımlılıklarını yazdır
void printDependencies(Task taskList[], int taskCount, int startTaskId) {
    bool visited[MAX_TASKS] = { false };

    printf("Dependencies for Task %d:\n", startTaskId);
    printDependenciesUtil(taskList, startTaskId, visited);
}


void pushSccStack(int v) {
    AdjacencyNode* newNode = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    newNode->data = v;
    newNode->next = sccStack;
    sccStack = newNode;
}

int popSccStack() {
    if (sccStack == NULL) return -1;
    int top = sccStack->data;
    AdjacencyNode* temp = sccStack;
    sccStack = sccStack->next;
    free(temp);
    return top;
}

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


// Önek Tablosunu (Prefix Table) Oluştur
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

// KMP Arama Fonksiyonu
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

int getDateKey(int day, int month, int year) {
    return year * 10000 + month * 100 + day;  // YYYYMMDD formatında bir anahtar oluşturur
}

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

int swap(Assignment* a, Assignment* b) {
    Assignment temp = *a;
    *a = *b;
    *b = temp;
    return 1;
}

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

// Yığına görevi deadline sırasına göre ekleme
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






