

/**
 * @file TASK_H
 * @brief Header guard to prevent multiple inclusions of the same header file.
 *
 * The `#ifndef TASK_H` directive is used as a header guard to ensure that the contents of this header file
 * are not included more than once in a single compilation. This helps to avoid redefinition errors and ensures
 * that the compiler processes the header file only once.
 */
#ifndef TASK_H
#define TASK_H
#include "../../utility/header/commonTypes.h"
#include <iostream>

 /**
  * @brief Namespace usage for standard library.
  *
  * The `using namespace std` directive allows the use of all the names in the standard library without
  * qualifying them with the `std::` prefix. This can make the code shorter and more readable, but it is
  * recommended to use this directive carefully to avoid name conflicts.
  */
using namespace std;

/**
 * @brief Namespace usage for standard library.
 *
 * The `using namespace std` directive allows the use of all the names in the standard library without
 * qualifying them with the `std::` prefix. This can make the code shorter and more readable, but it is
 * recommended to use this directive carefully to avoid name conflicts.
 */
using namespace std;

/**
 * @brief Maximum length of an assignment name.
 *
 * This macro defines the maximum number of characters allowed for an assignment name.
 * The value is set to 50 to ensure consistency in assignment naming.
 */
#define MAX_ASSIGNMENT_NAME 50

 /**
  * @brief Maximum size of the heap.
  *
  * This macro defines the maximum number of tasks that can be stored in the heap.
  * The value is set to 100, which represents the limit for heap management.
  */
#define MAX_HEAP_SIZE 100  

  /**
   * @brief Maximum number of keys in a node.
   *
   * This macro defines the maximum number of keys that each node can have.
   * The value is set to 3, which is used for simplifying operations.
   */
#define MAX_KEYS 3  

   /**
    * @brief Maximum number of children for each node.
    *
    * This macro calculates the maximum number of children that a node can have based on the number of keys.
    * The value is `MAX_KEYS + 1` to ensure proper structure in tree-based data structures.
    */
#define MAX_CHILDREN (MAX_KEYS + 1)

    /**
     * @brief Maximum number of tasks allowed.
     *
     * This macro defines the limit for the number of tasks that can be created or managed in the application.
     * The value is set to 100, which limits the number of concurrent tasks.
     */
#define MAX_TASKS 100

     /**
      * @brief Maximum number of dependencies per task.
      *
      * This macro defines the maximum number of dependencies that a task can have.
      * The value is set to 10, ensuring tasks can have multiple dependencies.
      */
#define MAX_DEPENDENCIES 10

      /**
       * @brief Size of the hash table.
       *
       * This macro defines the number of slots in the hash table used for managing user data.
       * The value is set to 10, which defines the capacity of the table.
       */
#define TABLE_SIZE 10  

       /**
        * @brief Size of the overflow area for hash table.
        *
        * This macro defines the size of the overflow area for the hash table to manage collisions.
        * The value is set to 10, allowing for additional entries when primary hash table slots are full.
        */
#define OVERFLOW_SIZE 10  

        /**
         * @brief Step size for linear quotient hashing.
         *
         * This macro defines the fixed step size used in linear quotient hashing.
         * The value is set to 3, representing the constant increment for resolving collisions.
         */
#define Q_STEP 3       


         /**
          * @brief Structure representing a user.
          *
          * This structure contains information about a user, including their ID, name, surname, email, and password.
          * It also includes a pointer to the next user in the linked list, allowing for efficient user management.
          */
typedef struct User {
    int id;  ///< Unique identifier for the user.
    char name[50];  ///< User's first name.
    char surname[50];  ///< User's surname.
    char email[50];  ///< User's email address.
    char password[50];  ///< User's password.
    struct User* next;  ///< Pointer to the next user in the linked list.
} User;

/**
 * @brief Structure representing a task.
 *
 * This structure contains information about a task, including its ID, name, description, category, and due date.
 * It also contains information about the task's importance and any dependencies on other tasks.
 */
typedef struct Task {
    int id;  ///< Unique identifier for the task.
    char name[100];  ///< Name of the task.
    char description[255];  ///< Description of the task.
    char category[50];  ///< Category of the task (e.g., work, personal).
    char dueDate[20];  ///< Due date of the task in string format.
    int impid;  ///< Importance level of the task (e.g., high, medium, low).
    int dependencies[MAX_DEPENDENCIES];  ///< Array of IDs for tasks that this task depends on.
    int dependencyCount;  ///< Number of dependencies for this task.
} Task;

/**
 * @brief Structure representing an assignment.
 *
 * This structure contains information about an assignment, including its name and due date (day, month, year).
 */
struct Assignment {
    char name[100];  ///< Name of the assignment.
    int day;  ///< Due day of the assignment.
    int month;  ///< Due month of the assignment.
    int year;  ///< Due year of the assignment.
};

/**
 * @brief Node structure for a sparse matrix.
 *
 * This structure represents a node in a sparse matrix used for managing notifications.
 * Each node stores the row (task ID), column (date), and value (notification type).
 * The value indicates the type of notification (0: None, 1: SMS, 2: E-Mail, 3: Notification).
 */
typedef struct SparseMatrixNode {
    int row;  ///< Task ID associated with this notification.
    int col;  ///< Date associated with this notification.
    int value;  ///< Notification type (0: None, 1: SMS, 2: E-Mail, 3: Notification).
    struct SparseMatrixNode* next;  ///< Pointer to the next node in the sparse matrix.
} SparseMatrixNode;



/**
 * @brief Node structure for a queue.
 *
 * This structure represents a node in a queue used for managing tasks.
 * Each node contains a task and a pointer to the next node in the queue.
 */
typedef struct QueueNode {
    Task task;  ///< Task associated with this queue node.
    struct QueueNode* next;  ///< Pointer to the next node in the queue.
} QueueNode;

/**
 * @brief Node structure for a stack.
 *
 * This structure represents a node in a stack used for managing tasks.
 * Each node contains a task and a pointer to the next node in the stack.
 */
typedef struct StackNode {
    Task task;  ///< Task associated with this stack node.
    struct StackNode* next;  ///< Pointer to the next node in the stack.
} StackNode;

/**
 * @brief Structure representing a minimum heap.
 *
 * This structure is used for managing deadlines in a min-heap data structure.
 * It contains an array of assignments and an integer representing the current size of the heap.
 */
typedef struct {
    Assignment deadlines[MAX_HEAP_SIZE];  ///< Array of assignments in the min-heap.
    int size;  ///< Current size of the min-heap.
} MinHeap;

/**
 * @brief Node structure for adjacency list.
 *
 * This structure represents a node in an adjacency list used for graph representation.
 * It contains an integer data value and a pointer to the next node in the list.
 */
typedef struct AdjacencyNode {
    int data;  ///< Data value representing a graph vertex or edge.
    struct AdjacencyNode* next;  ///< Pointer to the next node in the adjacency list.
} AdjacencyNode;

/**
 * @brief Pointer to the top of the stack for Strongly Connected Components (SCC).
 *
 * This pointer represents the top of the stack used during the SCC algorithm for graph processing.
 */
AdjacencyNode* sccStack = NULL;

/**
 * @brief Pointer to the front of the queue.
 *
 * This pointer represents the front of the queue used for managing tasks.
 */
QueueNode* front = NULL;

/**
 * @brief Pointer to the rear of the queue.
 *
 * This pointer represents the rear of the queue used for managing tasks.
 */
QueueNode* rear = NULL;

/**
 * @brief Pointer to the top of the stack.
 *
 * This pointer represents the top of the stack used for managing tasks.
 */
StackNode* stackTop = NULL;


/**
 * @brief Structure representing a scheduled task.
 *
 * This structure contains information about a scheduled task, including its name, due date,
 * description, and category. It is used to manage tasks that are planned for specific dates.
 */
typedef struct ScheduledTask {
    char name[50];  ///< Name of the scheduled task.
    int day;  ///< Due day of the scheduled task.
    int month;  ///< Due month of the scheduled task.
    int year;  ///< Due year of the scheduled task.
    char description[100];  ///< Description of the scheduled task.
    char category[50];  ///< Category of the scheduled task.
} ScheduledTask;

/**
 * @brief Node structure for B+ tree.
 *
 * This structure represents a node in a B+ tree used for managing scheduled tasks.
 * It contains information about whether the node is a leaf, the number of keys it contains,
 * an array of keys, an array of pointers to tasks, and pointers to child nodes and the next node.
 */
typedef struct BPlusTreeNode {
    bool isLeaf;  ///< Indicates if the node is a leaf node.
    int numKeys;  ///< Number of keys in the node.
    int keys[MAX_KEYS];  ///< Array of keys in the node.
    ScheduledTask* tasks[MAX_KEYS];  ///< Array of pointers to tasks in the node.
    struct BPlusTreeNode* children[MAX_CHILDREN];  ///< Array of pointers to child nodes.
    struct BPlusTreeNode* next;  ///< Pointer to the next node (used for leaf nodes).
} BPlusTreeNode;

/**
 * @brief Structure representing a B+ tree.
 *
 * This structure represents the B+ tree used for managing and searching scheduled tasks.
 * It contains a pointer to the root node of the B+ tree.
 */
typedef struct BPlusTree {
    BPlusTreeNode* root;  ///< Pointer to the root node of the B+ tree.
} BPlusTree;

/**
 * @brief Node structure for a doubly linked list of tasks.
 *
 * This structure represents a node in a doubly linked list used for managing tasks.
 * Each node contains a task and pointers to the next and previous nodes in the list.
 */
typedef struct TaskNode {
    Task task;  ///< Task associated with this node.
    struct TaskNode* next;  ///< Pointer to the next node in the list.
    struct TaskNode* prev;  ///< Pointer to the previous node in the list.
} TaskNode;

/**
 * @brief Node structure for an XOR linked list.
 *
 * This structure represents a node in an XOR linked list used for managing tasks.
 * Each node contains a task and an XOR-based pointer that combines the addresses of the previous and next nodes.
 */
typedef struct XORNode {
    Task task;  ///< Task associated with this node.
    struct XORNode* xorPtr;  ///< XOR pointer to the previous and next nodes.
} XORNode;


/**
 * @brief Creates a new B+ tree node.
 *
 * This function allocates memory for a new B+ tree node and initializes it.
 * The node can be either a leaf or an internal node, depending on the value of the `isLeaf` parameter.
 * All child pointers are initialized to NULL, and the number of keys is set to zero.
 *
 * @param isLeaf A boolean value indicating whether the node is a leaf.
 * @return BPlusTreeNode* Pointer to the newly created B+ tree node.
 */
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

/**
 * @brief Creates a new B+ tree.
 *
 * This function allocates memory for a new B+ tree and initializes it.
 * The B+ tree starts with a single leaf node as its root.
 *
 * @return BPlusTree* Pointer to the newly created B+ tree.
 */
BPlusTree* createBPlusTree() {
    BPlusTree* tree = (BPlusTree*)malloc(sizeof(BPlusTree));
    tree->root = createNode(true);  
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
int algorithmsMenu();





int registerUser(User user, const char* pathFileUser, bool isTestMode);

int registerUserMenu(const char* pathFileUsers);





int loginUser(User loginUser, const char* pathFileUsers);

int loginUserMenu(const char* pathFileUsers);




int addTask(Task taskList[], int* taskCount, int maxTasks);
int viewTask();
void categorizeTask();
int loadTasks(Task taskList[], int maxTasks);
void saveTasks(const Task taskList[], int taskCount);
void enqueue(Task task);
int push(Task task);
Task dequeue();
int printDependencies(Task taskList[], int taskCount, int startTaskId);
int analyzeSCC(Task taskList[], int taskCount, FILE* out);
void searchTasksByKeyword();
void navigateTasks();
int navigateXORList();
int loadTasksToXORList(const char* filename);



int assign_deadline(Assignment* assignment);
int viewDeadlines();
int insertMinHeap(MinHeap* heap, Assignment deadline);
Assignment extractMin(MinHeap* heap);



int setReminders();
int platformSleep(int seconds);
int showCurrentNotificationMethod();
int notificationSettings();
int addNotification(SparseMatrixNode** head, int row, int col, int value);
int displayNotifications(SparseMatrixNode* head);
int updateNotificationMethod(int taskId, int date, int method, SparseMatrixNode** head);
int showCurrentNotificationMethod();




int findTaskByName(const char* name);
int markTaskImportance(const char* taskNameInput, int importanceIdInput);



#endif // TASK_H