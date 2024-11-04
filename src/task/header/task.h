#ifndef TASK_H
#define TASK_H
#include "../../utility/header/commonTypes.h"
#include <iostream>

using namespace std;

#define MAX_ASSIGNMENT_NAME 50
#define MAX_HEAP_SIZE 100  // Maksimum görev sayısı
#define MAX_KEYS 3  // Her düğümde maksimum anahtar sayısı (basit bir örnek için)
#define MAX_CHILDREN (MAX_KEYS + 1)
#define MAX_TASKS 100
#define MAX_DEPENDENCIES 10

#define TABLE_SIZE 10  // Hash tablosunun boyutu
#define OVERFLOW_SIZE 10  // Taşma alanı boyutu
#define TABLE_SIZE 10  // Hash tablosunun boyutu
#define Q_STEP 3       // Sabit adım boyutu (q)




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
    int dependencies[MAX_DEPENDENCIES]; // Görevin bağlı olduğu diğer görevlerin ID’leri
    int dependencyCount;
} Task;

struct Assignment {
    char name[100];
    int day, month, year;
};

// Sparse Matrix d���m yap�s�
typedef struct SparseMatrixNode {
    int row;  // G�rev ID
    int col;  // Tarih
    int value;  // Bildirim t�r� (0: Yok, 1: SMS, 2: E-Mail, 3: Notification)
    struct SparseMatrixNode* next;
} SparseMatrixNode;


typedef struct QueueNode {
    Task task;
    struct QueueNode* next;
} QueueNode;

typedef struct StackNode {
    Task task;
    struct StackNode* next;
} StackNode;

typedef struct {
    Assignment deadlines[MAX_HEAP_SIZE];
    int size;
} MinHeap;

typedef struct AdjacencyNode {
    int data;
    struct AdjacencyNode* next;
} AdjacencyNode;

AdjacencyNode* sccStack = NULL;



QueueNode* front = NULL;  // Kuyru�un ba��
QueueNode* rear = NULL;   // Kuyru�un sonu
StackNode* stackTop = NULL;  // Y���n�n en �st�

typedef struct ScheduledTask {
    char name[50];
    int day, month, year;
    char description[100];
    char category[50];
} ScheduledTask;

typedef struct BPlusTreeNode {
    bool isLeaf;
    int numKeys;
    int keys[MAX_KEYS];
    ScheduledTask* tasks[MAX_KEYS];  // Burada Task yerine ScheduledTask kullanıyoruz
    struct BPlusTreeNode* children[MAX_CHILDREN];
    struct BPlusTreeNode* next;  // Yaprak düğümler arasında geçiş için
} BPlusTreeNode;

typedef struct BPlusTree {
    BPlusTreeNode* root;
} BPlusTree;




BPlusTreeNode* createNode(bool isLeaf) {
    BPlusTreeNode* node = (BPlusTreeNode*)malloc(sizeof(BPlusTreeNode));
    node->isLeaf = isLeaf;
    node->numKeys = 0;
    node->next = NULL;
    for (int i = 0; i < MAX_CHILDREN; i++) {
        node->children[i] = NULL;
    }
    return node;
}

BPlusTree* createBPlusTree() {
    BPlusTree* tree = (BPlusTree*)malloc(sizeof(BPlusTree));
    tree->root = createNode(true);  // Başlangıçta tek bir yaprak düğümü var
    return tree;
}


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
void algorithmsMenu();





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
void printDependencies(Task taskList[], int taskCount, int startTaskId);
int analyzeSCC(Task taskList[], int taskCount, FILE* out);
void searchTasksByKeyword();




int assign_deadline(Assignment* assignment);
void viewDeadlines();
void insertMinHeap(MinHeap* heap, Assignment deadline);
Assignment extractMin(MinHeap* heap);



void setReminders();
void platformSleep(int seconds);
void showCurrentNotificationMethod();
void notificationSettings();



int findTaskByName(const char* name);
void markTaskImportance();
void saveTasksToFile();



#endif // TASK_H