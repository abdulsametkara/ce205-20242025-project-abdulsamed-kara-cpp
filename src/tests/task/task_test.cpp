#define ENABLE_TASK_TEST 
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "gtest/gtest.h"
#include "../../task/header/task.h"  // Adjust this include path based on your project structure
#include "../../task/src/task.cpp"  // Adjust this include path based on your project structure



class TaskAppTest : public ::testing::Test {
protected:
    const char* userFile = "user.bin";
    const char* taskFile = "tasks.bin";
    const char* deadlineFile = "deadlines.bin";
    const char* inputTest = "inputTest.txt";
    const char* outputTest = "outputTest.txt";


    void SetUp() override {
        Assignment a, b;

        createTestUserFile();
        createTestTaskFile();
        createTestDeadlineFile();
        a = { "Assignment A", 1, 1, 2024 };
        b = { "Assignment B", 2, 2, 2025 };
    }

    void TearDown() override {
        remove(userFile);
        remove(taskFile);
        remove(deadlineFile);
    }

    void simulateUserInput(const char* userInput) {
        FILE* fileinput = fopen(inputTest, "wb");
        fputs(userInput, fileinput);
        fclose(fileinput);
        freopen(inputTest, "rb", stdin);
        freopen(outputTest, "wb", stdout);
    }

    void resetStdinStdout() {
        fclose(stdin);
        fflush(stdout);
#ifdef _WIN32
        freopen("CON", "a", stdout);
        freopen("CON", "r", stdin);
#else
        freopen("/dev/tty", "a", stdout);
        freopen("/dev/tty", "r", stdin);
#endif // _WIN32
    }

    void createTestUserFile() {
        User users[] = {
            {1, "John", "Doe", "john@example.com", "password123", nullptr},
            {2, "Jane", "Doe", "jane@example.com", "password456", nullptr},
        };

        FILE* file = fopen(userFile, "wb");
        if (file) {
            fwrite(users, sizeof(User), 2, file);
            fclose(file);
        }
    }

    void createTestTaskFile() {
        Task tasks[] = {
            {1, "Task 1", "Description 1", "Category 1", "2024-11-05", 0, {2}, 1},
            {2, "Task 2", "Description 2", "Category 2", "2024-11-10", 1, {1}, 1},
        };

        FILE* file = fopen(taskFile, "wb");
        if (file) {
            fwrite(tasks, sizeof(Task), 2, file);
            fclose(file);
        }
    }

    void createTestDeadlineFile() {
        Assignment deadlines[] = {
            {"Assignment 1", 5, 11, 2024},
            {"Assignment 2", 10, 11, 2024},
        };

        FILE* file = fopen(deadlineFile, "wb");
        if (file) {
            fwrite(deadlines, sizeof(Assignment), 2, file);
            fclose(file);
        }
    }
};


TEST_F(TaskAppTest, openingScreenMenu) {
    bool result = openingScreenMenu();
    EXPECT_TRUE(result);
}

TEST_F(TaskAppTest, printMainMenu) {
    bool result = printMainMenu();
    EXPECT_TRUE(result);
}

TEST_F(TaskAppTest, printCreateTaskMenu) {
    bool result = printCreateTaskMenu();
    EXPECT_TRUE(result);
}
TEST_F(TaskAppTest, printDeadlineSettingsMenu) {
    bool result = printDeadlineSettingsMenu();
    EXPECT_TRUE(result);
}

TEST_F(TaskAppTest, printReminderSystemMenu) {
    bool result = printReminderSystemMenu();
    EXPECT_TRUE(result);
}

TEST_F(TaskAppTest, printTaskPrioritizationMenu) {
    bool result = printTaskPrioritizationMenu();
    EXPECT_TRUE(result);
}
TEST_F(TaskAppTest, printAlgorithmsMenu) {
    bool result = printAlgorithmsMenu();
    EXPECT_TRUE(result);
}

TEST_F(TaskAppTest, GetNewUserId) {
    User users[] = {
        {1, "John", "Doe", "john@example.com", "password123", nullptr},
        {2, "Jane", "Doe", "jane@example.com", "password456", nullptr},
    };
    int userCount = 2;

    int newUserId = getNewUserId(users, userCount);
    EXPECT_EQ(newUserId, 3);  

    userCount = 0;
    newUserId = getNewUserId(users, userCount);
    EXPECT_EQ(newUserId, 1);  
}


TEST_F(TaskAppTest, LoadUsers) {
    const char* testFile = "test_users.bin";

    User testUsers[] = {
        {1, "John", "Doe", "john@example.com", "password123", nullptr},
        {2, "Jane", "Doe", "jane@example.com", "password456", nullptr}
    };
    int userCount = sizeof(testUsers) / sizeof(testUsers[0]);

    FILE* file = fopen(testFile, "wb");
    fwrite(&userCount, sizeof(int), 1, file);
    fwrite(testUsers, sizeof(User), userCount, file);
    fclose(file);

    User* loadedUsers = nullptr;
    int loadedCount = loadUsers(testFile, &loadedUsers);

    EXPECT_EQ(loadedCount, userCount);  
    ASSERT_NE(loadedUsers, nullptr);  

    for (int i = 0; i < userCount; ++i) {
        EXPECT_EQ(loadedUsers[i].id, testUsers[i].id);
        EXPECT_STREQ(loadedUsers[i].name, testUsers[i].name);
        EXPECT_STREQ(loadedUsers[i].surname, testUsers[i].surname);
        EXPECT_STREQ(loadedUsers[i].email, testUsers[i].email);
        EXPECT_STREQ(loadedUsers[i].password, testUsers[i].password);
    }

    free(loadedUsers);

    remove(testFile);
}


TEST_F(TaskAppTest, AddTaskToXORList) {
    Task task = { 3, "Task 3", "Description 3", "Category 3", "2024-12-01", 0, {2}, 1 };

    int result = addTaskToXORList(task);

    EXPECT_EQ(result, 1);

    ASSERT_NE(xorHead, nullptr);
    ASSERT_NE(xorTail, nullptr);

    EXPECT_EQ(xorHead->task.id, task.id);
    EXPECT_STREQ(xorHead->task.name, task.name);
    EXPECT_STREQ(xorHead->task.description, task.description);
    EXPECT_STREQ(xorHead->task.category, task.category);

    EXPECT_EQ(xorTail->task.id, task.id);
    EXPECT_EQ(xorHead, xorTail); 
}




TEST_F(TaskAppTest, AssignDeadline_ValidInput) {
    simulateUserInput("Sample Task\n\n10 11 2024\n");

    Assignment assignment;

    int result = assign_deadline(&assignment);
    resetStdinStdout();
    EXPECT_EQ(result, 0);  




}

TEST_F(TaskAppTest, AssignDeadline_InvalidDate) {
    simulateUserInput("Sample Task\n\n32 13 2024\n");

    Assignment assignment;

    int result = assign_deadline(&assignment);
    resetStdinStdout();
    EXPECT_EQ(result, -1);


}

TEST_F(TaskAppTest, ViewDeadlines_Successful) {
    // Heap'i dolduruyoruz
    deadlineHeap.size = 3;
    deadlineHeap.deadlines[0] = { "Task 3", 5, 10, 2024 };
    deadlineHeap.deadlines[1] = { "Task 2", 10, 11, 2024 };
    deadlineHeap.deadlines[2] = { "Task 1", 15, 12, 2024 };

    simulateUserInput("\n"); 
    int result = viewDeadlines();
    resetStdinStdout();

    EXPECT_EQ(result, 1);

    FILE* outputFile = fopen(outputTest, "rb");
    char outputBuffer[1024] = { 0 };
    fread(outputBuffer, sizeof(char), 1024, outputFile);
    fclose(outputFile);

    std::string output(outputBuffer);

    EXPECT_NE(output.find("1. Task: Task 3 - Deadline: 05/10/2024"), std::string::npos);
    EXPECT_NE(output.find("2. Task: Task 2 - Deadline: 10/11/2024"), std::string::npos);
    EXPECT_NE(output.find("3. Task: Task 1 - Deadline: 15/12/2024"), std::string::npos);
}





TEST_F(TaskAppTest, ViewDeadlines_NoDeadlines) {
    deadlineHeap.size = 0;

    simulateUserInput(""); 
    int result = viewDeadlines();
    resetStdinStdout();

    EXPECT_EQ(result, -1);
    FILE* outputFile = fopen(outputTest, "rb");
    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 256, outputFile);
    fclose(outputFile);
    EXPECT_NE(strstr(outputBuffer, "No deadlines to display."), nullptr);
}






TEST_F(TaskAppTest, PlatformSleep_ReturnsCorrectValue) {
    int testDuration = 1;

    simulateUserInput("\n");

    int result = platformSleep(testDuration);
    EXPECT_EQ(result, 1);
}



TEST_F(TaskAppTest, AddNotificationToSparseMatrix) {
    SparseMatrixNode* head = NULL;

    int row1 = 1, col1 = 2, value1 = 5;
    int result = addNotification(&head, row1, col1, value1);
    EXPECT_EQ(result, 1);  
    ASSERT_NE(head, nullptr);  
    // Verify the node's data
    EXPECT_EQ(head->row, row1);
    EXPECT_EQ(head->col, col1);
    EXPECT_EQ(head->value, value1);

    // Test Case 2: Add another notification
    int row2 = 2, col2 = 3, value2 = 10;
    result = addNotification(&head, row2, col2, value2);
    EXPECT_EQ(result, 1);  // Function should still return success
    ASSERT_NE(head, nullptr);  // Head should still not be NULL

    // Check if the new node is added to the start of the list
    EXPECT_EQ(head->row, row2);
    EXPECT_EQ(head->col, col2);
    EXPECT_EQ(head->value, value2);

    // Verify the second node in the list (the previous node)
    SparseMatrixNode* secondNode = head->next;
    ASSERT_NE(secondNode, nullptr);  // There should be a second node
    EXPECT_EQ(secondNode->row, row1);
    EXPECT_EQ(secondNode->col, col1);
    EXPECT_EQ(secondNode->value, value1);

    // Clean up the list memory
    while (head != NULL) {
        SparseMatrixNode* temp = head;
        head = head->next;
        free(temp);
    }
}

TEST_F(TaskAppTest, DisplayNotifications) {
    // Create a sparse matrix linked list for notifications
    SparseMatrixNode* head = new SparseMatrixNode{ 1, 10, 2, nullptr }; // Task ID 1, Date 10, Notification Type 2
    head->next = new SparseMatrixNode{ 2, 15, 1, nullptr }; // Task ID 2, Date 15, Notification Type 1
    head->next->next = new SparseMatrixNode{ 3, 20, 3, nullptr }; // Task ID 3, Date 20, Notification Type 3

    // Redirect stdout to a file to capture output
    freopen(outputTest, "w", stdout);

    // Call the displayNotifications function
    int result = displayNotifications(head);

    // Restore stdout to the console
    resetStdinStdout();

    // Check that the function returns 1 as expected
    EXPECT_EQ(result, 1);

    // Read the captured output from outputTest.txt
    FILE* file = fopen(outputTest, "r");
    ASSERT_NE(file, nullptr);

    // Read and verify that the output matches the expected notifications
    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, file);
    fclose(file);

    // Expected output pattern for each notification
    std::string expectedOutput =
        "Task ID: 1, Date: 10, Notification Type: 2\n"
        "Task ID: 2, Date: 15, Notification Type: 1\n"
        "Task ID: 3, Date: 20, Notification Type: 3\n";

    EXPECT_STREQ(outputBuffer, expectedOutput.c_str());

    // Clean up the sparse matrix linked list
    SparseMatrixNode* temp;
    while (head) {
        temp = head;
        head = head->next;
        delete temp;
    }
}

TEST_F(TaskAppTest, UpdateNotificationMethod) {
    // Initialize the sparse matrix head as nullptr
    SparseMatrixNode* head = nullptr;

    // Call updateNotificationMethod to add a new notification
    int taskId = 1;
    int date = 20241105;
    int method = 2;
    int result = updateNotificationMethod(taskId, date, method, &head);

    // Check the return value
    EXPECT_EQ(result, 1);

    // Check that the node was added correctly
    ASSERT_NE(head, nullptr);
    EXPECT_EQ(head->row, taskId);
    EXPECT_EQ(head->col, date);
    EXPECT_EQ(head->value, method);

    // Redirect stdout to capture printed output
    freopen(outputTest, "w", stdout);

    // Call the function again to verify it updates correctly
    int newMethod = 3;
    result = updateNotificationMethod(taskId, date, newMethod, &head);

    // Restore stdout
    resetStdinStdout();

    // Check the return value again
    EXPECT_EQ(result, 1);

    // Verify that the value in the list was updated
    EXPECT_EQ(head->value, newMethod);

    // Check the printed output for confirmation message
    FILE* file = fopen(outputTest, "r");
    ASSERT_NE(file, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, file);
    fclose(file);

    std::string expectedOutput = "Notification method updated for Task ID 1 on Date 20241105\n";
    EXPECT_STREQ(outputBuffer, expectedOutput.c_str());

    // Clean up the sparse matrix linked list
    SparseMatrixNode* temp;
    while (head) {
        temp = head;
        head = head->next;
        delete temp;
    }
}

TEST_F(TaskAppTest, ViewTask_EmptyQueue) {
    front = NULL; 
    freopen(outputTest, "wb", stdout);

    simulateUserInput("\n");

    int result = viewTask();

    resetStdinStdout();

    EXPECT_EQ(result, 0);

    FILE* outputFile = fopen(outputTest, "rb");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    EXPECT_NE(strstr(outputBuffer, "No tasks found. The task list is empty."), nullptr);
}

TEST_F(TaskAppTest, ViewTask_FilledQueue) {
    Task task1 = { 1, "Task 1", "Description 1", "Category 1", "2024-11-10", 0, {0}, 1 };
    Task task2 = { 2, "Task 2", "Description 2", "Category 2", "2024-11-15", 0, {0}, 1 };

    enqueue(task1);
    enqueue(task2);

    freopen(outputTest, "wb", stdout);

    simulateUserInput("\n");

    int result = viewTask();

    resetStdinStdout();

    EXPECT_EQ(result, 1);

    FILE* outputFile = fopen(outputTest, "rb");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[512] = { 0 };
    fread(outputBuffer, sizeof(char), 511, outputFile);
    fclose(outputFile);

    EXPECT_NE(strstr(outputBuffer, "ID: 1"), nullptr);
    EXPECT_NE(strstr(outputBuffer, "Name: Task 1"), nullptr);
    EXPECT_NE(strstr(outputBuffer, "Description: Description 1"), nullptr);
    EXPECT_NE(strstr(outputBuffer, "Category: Category 1"), nullptr);
    EXPECT_NE(strstr(outputBuffer, "Due Date: 2024-11-10"), nullptr);

    EXPECT_NE(strstr(outputBuffer, "ID: 2"), nullptr);
    EXPECT_NE(strstr(outputBuffer, "Name: Task 2"), nullptr);
    EXPECT_NE(strstr(outputBuffer, "Description: Description 2"), nullptr);
    EXPECT_NE(strstr(outputBuffer, "Category: Category 2"), nullptr);
    EXPECT_NE(strstr(outputBuffer, "Due Date: 2024-11-15"), nullptr);
}



TEST_F(TaskAppTest, EnterToContinue) {
    simulateUserInput("\n");

    freopen(outputTest, "wb", stdout);

    int result = enterToContinue();

    resetStdinStdout();

    EXPECT_EQ(result, 1);

    FILE* outputFile = fopen(outputTest, "rb");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    EXPECT_NE(strstr(outputBuffer, "Press enter to continue"), nullptr);
}

TEST_F(TaskAppTest, HandleInputError) {
    simulateUserInput("abc\n");

    freopen(outputTest, "wb", stdout);

    int result = handleInputError();

    resetStdinStdout();


    EXPECT_EQ(result, 0);


    FILE* outputFile = fopen(outputTest, "rb");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);


    EXPECT_NE(strstr(outputBuffer, "Invalid input. Please enter a number."), nullptr);
}

TEST_F(TaskAppTest, AddTaskToList_ValidTask) {

    head = NULL;
    tail = NULL;

    Task newTask = { 1, "Test Task", "Test Description", "Test Category", "2024-12-15", 0, {2}, 1 };

    int result = addTaskToList(newTask);
    EXPECT_EQ(result, 1);


    ASSERT_NE(head, nullptr); 
    ASSERT_NE(tail, nullptr); 
    EXPECT_EQ(head, tail);    

    EXPECT_EQ(head->task.id, newTask.id);
    EXPECT_STREQ(head->task.name, newTask.name);
    EXPECT_STREQ(head->task.description, newTask.description);
    EXPECT_STREQ(head->task.category, newTask.category);
    EXPECT_STREQ(head->task.dueDate, newTask.dueDate);
}

TEST_F(TaskAppTest, AddTaskToList_MultipleTasks) {
    head = NULL;
    tail = NULL;


    Task firstTask = { 1, "First Task", "First Description", "First Category", "2024-12-10", 0, {2}, 1 };
    int result = addTaskToList(firstTask);
    EXPECT_EQ(result, 1);

    Task secondTask = { 2, "Second Task", "Second Description", "Second Category", "2024-12-20", 0, {1}, 1 };
    result = addTaskToList(secondTask);
    EXPECT_EQ(result, 1);

    ASSERT_NE(head, nullptr);
    ASSERT_NE(tail, nullptr);
    EXPECT_NE(head, tail); 

    EXPECT_EQ(head->task.id, firstTask.id);
    EXPECT_STREQ(head->task.name, firstTask.name);
    EXPECT_EQ(head->next, tail); 

    EXPECT_EQ(tail->task.id, secondTask.id);
    EXPECT_STREQ(tail->task.name, secondTask.name);
    EXPECT_EQ(tail->prev, head); 
}


TEST_F(TaskAppTest, LoginRegistermenuTest) {
    const char* testuserfile = "userFile.bin";

    simulateUserInput("2\nasd\nasd\nasd\nasd\n\n1\nasd\nasd\n\n6\n3\n");

    int result = mainMenu(testuserfile);

    EXPECT_EQ(result, 0);
}


TEST_F(TaskAppTest, PushTask_Success) {
    Task task = { 1, "Test Task", "Task Description", "General", "2024-12-15", 0, {0}, 0 };

    int result = push(task);
    EXPECT_EQ(result, 1); 

    ASSERT_NE(stackTop, nullptr);
    EXPECT_EQ(stackTop->task.id, task.id);
    EXPECT_STREQ(stackTop->task.name, task.name);
    EXPECT_STREQ(stackTop->task.description, task.description);
    EXPECT_STREQ(stackTop->task.category, task.category);
    EXPECT_STREQ(stackTop->task.dueDate, task.dueDate);

    StackNode* temp = stackTop;
    stackTop = stackTop->next;
    free(temp);
}



TEST_F(TaskAppTest, PopTask_NonEmptyStack) {

    Task task = { 1, "Test Task", "Task Description", "General", "2024-12-15", 0, {0}, 0 };
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    ASSERT_NE(newNode, nullptr); 
    newNode->task = task;
    newNode->next = NULL;  
    stackTop = newNode;

    Task result = pop();

    EXPECT_EQ(result.id, task.id);
    EXPECT_STREQ(result.name, task.name);
    EXPECT_STREQ(result.description, task.description);
    EXPECT_STREQ(result.category, task.category);
    EXPECT_STREQ(result.dueDate, task.dueDate);

    EXPECT_EQ(stackTop, nullptr);

    
}

TEST_F(TaskAppTest, UndoLastTask_EmptyStack) {
   
    stackTop = NULL; 

    
    Task taskList[100];
    int taskCount = 0;

    
    freopen(outputTest, "w", stdout);

    
    int result = undoLastTask(taskList, &taskCount);

    
    resetStdinStdout();

    
    EXPECT_EQ(result, -1);

    
    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    EXPECT_NE(strstr(outputBuffer, "No tasks to undo."), nullptr);
}


TEST_F(TaskAppTest, UndoLastTask_Success) {
    
    stackTop = NULL; 

    
    Task taskList[100];
    int taskCount = 1;

    
    Task task = { 1, "Test Task", "Test Description", "Test Category", "2024-12-31", 0, {0}, 0 };
    push(task); 
    taskList[0] = task;

    
    freopen(outputTest, "w", stdout);

    
    int result = undoLastTask(taskList, &taskCount);

    
    resetStdinStdout();

    
    EXPECT_EQ(result, 1);

    
    EXPECT_EQ(taskCount, 0);

    
    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    EXPECT_NE(strstr(outputBuffer, "Last task 'Test Task' undone successfully."), nullptr);
}

TEST_F(TaskAppTest, PrintDependenciesUtil_NoDependencies) {
   
    Task taskList[3] = {
        {1, "Task 1", "Description 1", "Category 1", "2024-12-01", 0, {0}, 0},
        {2, "Task 2", "Description 2", "Category 2", "2024-12-02", 0, {0}, 0},
        {3, "Task 3", "Description 3", "Category 3", "2024-12-03", 0, {0}, 0},
    };

    bool visited[4] = { false };

    
    freopen(outputTest, "w", stdout);

    
    int result = printDependenciesUtil(taskList, 1, visited);

    
    resetStdinStdout();

    
    EXPECT_EQ(result, 1);

    
    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    
    EXPECT_EQ(strlen(outputBuffer), 0);
}

TEST_F(TaskAppTest, PrintDependencies_InvalidTaskId) {
   
    int taskCount = 3;
    Task taskList[3] = {
        {1, "Task 1", "Description 1", "Category 1", "2024-12-01", 0, {0}, 0},
        {2, "Task 2", "Description 2", "Category 2", "2024-12-02", 0, {0}, 0},
        {3, "Task 3", "Description 3", "Category 3", "2024-12-03", 0, {0}, 0}
    };

   
    freopen(outputTest, "w", stdout);

    
    int result = printDependencies(taskList, taskCount, 0);

    
    resetStdinStdout();

    
    EXPECT_EQ(result, -1);

    
    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    EXPECT_NE(strstr(outputBuffer, "Invalid task ID"), nullptr);
}

TEST_F(TaskAppTest, PrintDependencies_WithDependencies) {
    
    Task taskList[3] = {
        {1, "Task 1", "Description 1", "Category 1", "2024-12-01", 1, {2}, 1},
        {2, "Task 2", "Description 2", "Category 2", "2024-12-02", 1, {3}, 1},
        {3, "Task 3", "Description 3", "Category 3", "2024-12-03", 0, {0}, 0}
    };

   
    freopen(outputTest, "w", stdout);

    
    int result = printDependencies(taskList, taskCount, 1);

    resetStdinStdout();

    EXPECT_EQ(result, -1);

    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[512] = { 0 };
    fread(outputBuffer, sizeof(char), 511, outputFile);
    fclose(outputFile);

    EXPECT_NE(strstr(outputBuffer, "Dependencies for Task 1"), nullptr);
    EXPECT_NE(strstr(outputBuffer, "Task 1 depends on Task 2"), nullptr);
    EXPECT_NE(strstr(outputBuffer, "Task 2 depends on Task 3"), nullptr);
}

TEST_F(TaskAppTest, PrintDependencies_NoDependencies) {

    int taskCount = 3;
    Task taskList[3] = {
        {1, "Task 1", "Description 1", "Category 1", "2024-12-01", 0, {0}, 0},
        {2, "Task 2", "Description 2", "Category 2", "2024-12-02", 0, {0}, 0},
        {3, "Task 3", "Description 3", "Category 3", "2024-12-03", 0, {0}, 0}
    };

    
    freopen(outputTest, "w", stdout);

    
    int result = printDependencies(taskList, taskCount, 1);

    
    resetStdinStdout();

    
    EXPECT_EQ(result, 1);

    
    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    EXPECT_NE(strstr(outputBuffer, "Dependencies for Task 1"), nullptr);
    EXPECT_EQ(strstr(outputBuffer, "depends on"), nullptr);
}

TEST_F(TaskAppTest, PushSccStack_Success) {
    
    sccStack = NULL;

    
    int result = pushSccStack(10);
    EXPECT_EQ(result, 1);
    ASSERT_NE(sccStack, nullptr); 
    EXPECT_EQ(sccStack->data, 10); 

    
    result = pushSccStack(20);
    EXPECT_EQ(result, 1); 
    ASSERT_NE(sccStack, nullptr); 
    EXPECT_EQ(sccStack->data, 20); 
    EXPECT_EQ(sccStack->next->data, 10);
}

TEST_F(TaskAppTest, PopSccStack_EmptyStack) {
    
    sccStack = NULL;

   
    int result = popSccStack();

    
    EXPECT_EQ(result, -1);
}

TEST_F(TaskAppTest, PopSccStack_NonEmptyStack) {
    
    AdjacencyNode* node1 = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    node1->data = 10;
    node1->next = NULL;

    AdjacencyNode* node2 = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    node2->data = 20;
    node2->next = node1;

    
    sccStack = node2;

    
    int result = popSccStack();
    EXPECT_EQ(result, 20);

    
    result = popSccStack();
    EXPECT_EQ(result, 10); 

    
    EXPECT_EQ(sccStack, nullptr);
}

TEST_F(TaskAppTest, DfsUtil_SingleNode) {
    
    int visited[2] = { 0 }; 
    AdjacencyNode* adj[2] = { NULL };

    AdjacencyNode* node1 = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    ASSERT_NE(node1, nullptr); 
    node1->data = 1;
    node1->next = NULL;
    adj[1] = node1;

    sccStack = NULL;

    int result = dfsUtil(1, visited, adj, NULL);

    EXPECT_EQ(result, 1);
    EXPECT_EQ(visited[1], 1);

    ASSERT_NE(sccStack, nullptr);
    EXPECT_EQ(sccStack->data, 1);

    free(node1);
    while (sccStack) {
        popSccStack();
    }
}

TEST_F(TaskAppTest, DfsUtil_MultipleNodes) {
    
    int visited[3] = { 0 }; 
    AdjacencyNode* adj[3] = { NULL }; 

    
    AdjacencyNode* node1 = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    AdjacencyNode* node2 = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    ASSERT_NE(node1, nullptr);
    ASSERT_NE(node2, nullptr);

    node1->data = 2;
    node1->next = NULL;
    adj[1] = node1;

    node2->data = 1;
    node2->next = NULL;
    adj[2] = node2;

    
    sccStack = NULL;

    
    int result = dfsUtil(1, visited, adj, NULL);


    
    EXPECT_EQ(result, 1);
    EXPECT_EQ(visited[1], 1);
    EXPECT_EQ(visited[2], 1);

    
    ASSERT_NE(sccStack, nullptr);
    EXPECT_EQ(sccStack->data, 2); 
    popSccStack();
    EXPECT_EQ(sccStack->data, 1);

    
    free(node1);
    free(node2);
    while (sccStack) {
        popSccStack();
    }
}

TEST_F(TaskAppTest, FindSCCs_ValidGraph) {
    
    int V = 3;
    AdjacencyNode* adj[3] = { NULL };

    
    AdjacencyNode* node1 = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    node1->data = 1;
    node1->next = NULL;
    adj[0] = node1;

    
    AdjacencyNode* node2 = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    node2->data = 2;
    node2->next = NULL;
    adj[1] = node2;

    
    AdjacencyNode* node3 = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    node3->data = 0;
    node3->next = NULL;
    adj[2] = node3;

    
    FILE* outFile = fopen(outputTest, "w");
    ASSERT_NE(outFile, nullptr);

    
    int result = findSCCs(V, adj, outFile);

    
    fclose(outFile);

   
    EXPECT_EQ(result, 1);

    
    outFile = fopen(outputTest, "r");
    ASSERT_NE(outFile, nullptr);

    char outputBuffer[512] = { 0 };
    fread(outputBuffer, sizeof(char), 511, outFile);
    fclose(outFile);

   
    EXPECT_NE(strstr(outputBuffer, "SCC #1: 0 1 2"), nullptr);

    free(node1);
    free(node2);
    free(node3);
}

TEST_F(TaskAppTest, FindSCCs_InvalidParameters) {
    
    int V = -1; 
    AdjacencyNode* adj[3] = { NULL };
    FILE* outFile = fopen(outputTest, "w");
    ASSERT_NE(outFile, nullptr);

    int result = findSCCs(V, adj, outFile);

    
    EXPECT_EQ(result, -1);

    fclose(outFile);
}



TEST_F(TaskAppTest, AnalyzeSCC_InvalidInput) {
    
    Task* taskList = NULL;
    int taskCount = 3;

    
    FILE* outFile = fopen(outputTest, "w");
    ASSERT_NE(outFile, nullptr);

    
    int result = analyzeSCC(taskList, taskCount, outFile);

    
    EXPECT_EQ(result, -1);

    fclose(outFile);
}

TEST_F(TaskAppTest, AnalyzeSCC_NoDependencies) {
    Task taskList[3] = {
        {1, "Task 1", "Description 1", "Category 1", "2024-12-01", 0, {0}, 0},
        {2, "Task 2", "Description 2", "Category 2", "2024-12-02", 0, {0}, 0},
        {3, "Task 3", "Description 3", "Category 3", "2024-12-03", 0, {0}, 0}
    };
    int taskCount = 3;

    FILE* outFile = fopen(outputTest, "w");
    ASSERT_NE(outFile, nullptr);

    int result = analyzeSCC(taskList, taskCount, outFile);

    fclose(outFile);

    EXPECT_EQ(result, 1);

    outFile = fopen(outputTest, "r");
    ASSERT_NE(outFile, nullptr);

    char outputBuffer[512] = { 0 };
    fread(outputBuffer, sizeof(char), 511, outFile);
    fclose(outFile);

    EXPECT_NE(strstr(outputBuffer, "SCC #1:"), nullptr);
    EXPECT_EQ(strstr(outputBuffer, "SCC #2:"), nullptr);
}

TEST_F(TaskAppTest, ComputePrefixTable_ValidPattern) {
    const char* pattern = "ababcab";
    int patternLength = strlen(pattern);
    int prefixTable[7] = { 0 };

    
    int result = computePrefixTable(pattern, prefixTable, patternLength);

 
    EXPECT_EQ(result, 1);

    
    int expectedPrefixTable[7] = { 0, 0, 1, 2, 0, 1, 2 };

    
    for (int i = 0; i < patternLength; i++) {
        EXPECT_EQ(prefixTable[i], expectedPrefixTable[i]);
    }
}

TEST_F(TaskAppTest, ComputePrefixTable_EmptyPattern) {
    const char* pattern = "";
    int patternLength = strlen(pattern);
    int prefixTable[1] = { 0 };

    
    int result = computePrefixTable(pattern, prefixTable, patternLength);

    
    EXPECT_EQ(result, -1);
}

TEST_F(TaskAppTest, ComputePrefixTable_InvalidParameters) {
    const char* pattern = NULL;
    int* prefixTable = NULL;
    int patternLength = 0;

    
    int result = computePrefixTable(pattern, prefixTable, patternLength);

    
    EXPECT_EQ(result, -1);
}

TEST_F(TaskAppTest, ComputePrefixTable_RepeatedCharacters) {
    const char* pattern = "aaaa";
    int patternLength = strlen(pattern);
    int prefixTable[4] = { 0 };

    
    int result = computePrefixTable(pattern, prefixTable, patternLength);

    
    EXPECT_EQ(result, 1);

    
    int expectedPrefixTable[4] = { 0, 1, 2, 3 };

   
    for (int i = 0; i < patternLength; i++) {
        EXPECT_EQ(prefixTable[i], expectedPrefixTable[i]);
    }
}

TEST_F(TaskAppTest, ComputePrefixTable_NoRepetition) {
    const char* pattern = "abcdef";
    int patternLength = strlen(pattern);
    int prefixTable[6] = { 0 };

    int result = computePrefixTable(pattern, prefixTable, patternLength);

    
    EXPECT_EQ(result, 1);

    
    int expectedPrefixTable[6] = { 0, 0, 0, 0, 0, 0 };

    
    for (int i = 0; i < patternLength; i++) {
        EXPECT_EQ(prefixTable[i], expectedPrefixTable[i]);
    }
}

TEST_F(TaskAppTest, KMPsearch_ValidMatch) {
    const char* text = "ababcabcabababd";
    const char* pattern = "ababd";

    // Call the function
    int result = KMPsearch(text, pattern);

    // Verify the function finds the match
    EXPECT_EQ(result, 1);
}

TEST_F(TaskAppTest, KMPsearch_NoMatch) {
    const char* text = "ababcabcabababd";
    const char* pattern = "xyz";

    // Call the function
    int result = KMPsearch(text, pattern);

    // Verify the function does not find a match
    EXPECT_EQ(result, 0);
}

TEST_F(TaskAppTest, KMPsearch_EmptyText) {
    const char* text = "";
    const char* pattern = "ababd";

    // Call the function
    int result = KMPsearch(text, pattern);

    // Verify the function returns an error
    EXPECT_EQ(result, -1);
}

TEST_F(TaskAppTest, KMPsearch_EmptyPattern) {
    const char* text = "ababcabcabababd";
    const char* pattern = "";

    // Call the function
    int result = KMPsearch(text, pattern);

    // Verify the function returns an error
    EXPECT_EQ(result, -1);
}

TEST_F(TaskAppTest, KMPsearch_NullInputs) {
    const char* text = NULL;
    const char* pattern = NULL;

    // Call the function
    int result = KMPsearch(text, pattern);

    // Verify the function returns an error
    EXPECT_EQ(result, -1);
}

TEST_F(TaskAppTest, KMPsearch_MemoryAllocationFailure) {
    const char* text = "ababcabcabababd";
    const char* pattern = "ababd";

    // Temporarily override malloc to simulate failure
#define malloc(size) NULL

// Call the function
    int result = KMPsearch(text, pattern);

    // Restore malloc
#undef malloc

// Verify the function returns an error for memory allocation failure
    EXPECT_EQ(result, 1);
}

TEST_F(TaskAppTest, KMPsearch_PatternEqualsText) {
    const char* text = "ababd";
    const char* pattern = "ababd";

    // Call the function
    int result = KMPsearch(text, pattern);

    // Verify the function finds the match
    EXPECT_EQ(result, 1);
}

TEST_F(TaskAppTest, KMPsearch_SingleCharacterMatch) {
    const char* text = "a";
    const char* pattern = "a";

    // Call the function
    int result = KMPsearch(text, pattern);

    // Verify the function finds the match
    EXPECT_EQ(result, 1);
}

TEST_F(TaskAppTest, KMPsearch_SingleCharacterNoMatch) {
    const char* text = "a";
    const char* pattern = "b";

    // Call the function
    int result = KMPsearch(text, pattern);

    // Verify the function does not find a match
    EXPECT_EQ(result, 0);
}



TEST_F(TaskAppTest, GetDateKey_ValidDate) {
    
    int day = 15, month = 12, year = 2024;
    int result = getDateKey(day, month, year);

    EXPECT_EQ(result, 20241215); 
}

TEST_F(TaskAppTest, GetDateKey_SingleDigitDayMonth) {

    int day = 5, month = 3, year = 2024;
    int result = getDateKey(day, month, year);

    EXPECT_EQ(result, 20240305);
}

TEST_F(TaskAppTest, GetDateKey_LeapYear) {
    
    int day = 29, month = 2, year = 2024;
    int result = getDateKey(day, month, year);

    EXPECT_EQ(result, 20240229); 
}

TEST_F(TaskAppTest, GetDateKey_MinimumDate) {
    
    int day = 1, month = 1, year = 1;
    int result = getDateKey(day, month, year);

    EXPECT_EQ(result, 10101); 
}

TEST_F(TaskAppTest, GetDateKey_MaximumDate) {

    int day = 31, month = 12, year = 9999;
    int result = getDateKey(day, month, year);

    EXPECT_EQ(result, 99991231); 
}

TEST_F(TaskAppTest, SwapAssignments_Successful) {
   
    Assignment a = { "Assignment A", 1, 1, 2024 };
    Assignment b = { "Assignment B", 2, 2, 2025 };

    
    int result = swap(&a, &b);

    
    EXPECT_EQ(result, 1);

    EXPECT_STREQ(a.name, "Assignment B");
    EXPECT_EQ(a.day, 2);
    EXPECT_EQ(a.month, 2);
    EXPECT_EQ(a.year, 2025);

    EXPECT_STREQ(b.name, "Assignment A");
    EXPECT_EQ(b.day, 1);
    EXPECT_EQ(b.month, 1);
    EXPECT_EQ(b.year, 2024);
}

TEST_F(TaskAppTest, Heapify_CorrectlyMaintainsMinHeapProperty) {
    
    MinHeap heap;
    heap.size = 5;
    heap.deadlines[0] = { "Task 1", 10, 5, 2024 };
    heap.deadlines[1] = { "Task 2", 2, 3, 2024 };  
    heap.deadlines[2] = { "Task 3", 15, 7, 2024 };
    heap.deadlines[3] = { "Task 4", 1, 1, 2024 };  
    heap.deadlines[4] = { "Task 5", 8, 6, 2024 };

   
    int result = heapify(&heap, 0);

    
    EXPECT_EQ(result, 1);

    
    EXPECT_EQ(heap.deadlines[0].day, 2);
    EXPECT_EQ(heap.deadlines[0].month, 3);
    EXPECT_EQ(heap.deadlines[0].year, 2024);
    EXPECT_STREQ(heap.deadlines[0].name, "Task 2");

    EXPECT_EQ(heap.deadlines[1].day, 1);
    EXPECT_EQ(heap.deadlines[1].month, 1);
    EXPECT_EQ(heap.deadlines[1].year, 2024);
    EXPECT_STREQ(heap.deadlines[1].name, "Task 4");

    EXPECT_EQ(heap.deadlines[2].day, 15);
    EXPECT_EQ(heap.deadlines[2].month, 7);
    EXPECT_EQ(heap.deadlines[2].year, 2024);
    EXPECT_STREQ(heap.deadlines[2].name, "Task 3");
}

TEST_F(TaskAppTest, ExtractMin_ReturnsMinAndMaintainsHeapProperty) {
    
    MinHeap heap;
    heap.size = 4;
    heap.deadlines[0] = { "Task 1", 2, 3, 2024 };   
    heap.deadlines[1] = { "Task 2", 10, 5, 2024 };
    heap.deadlines[2] = { "Task 3", 15, 7, 2024 };
    heap.deadlines[3] = { "Task 4", 8, 6, 2024 };

    
    Assignment minAssignment = extractMin(&heap);

    
    EXPECT_EQ(minAssignment.day, 2);
    EXPECT_EQ(minAssignment.month, 3);
    EXPECT_EQ(minAssignment.year, 2024);
    EXPECT_STREQ(minAssignment.name, "Task 1");

    
    EXPECT_EQ(heap.size, 3);

   
    EXPECT_EQ(heap.deadlines[0].day, 10);
    EXPECT_EQ(heap.deadlines[0].month, 5);
    EXPECT_EQ(heap.deadlines[0].year, 2024);
    EXPECT_STREQ(heap.deadlines[0].name, "Task 2");

    
    EXPECT_TRUE(
        (heap.deadlines[0].year <= heap.deadlines[1].year) ||
        (heap.deadlines[0].year == heap.deadlines[1].year && heap.deadlines[0].month <= heap.deadlines[1].month) ||
        (heap.deadlines[0].year == heap.deadlines[1].year && heap.deadlines[0].month == heap.deadlines[1].month && heap.deadlines[0].day <= heap.deadlines[1].day)
    );

    EXPECT_TRUE(
        (heap.deadlines[0].year <= heap.deadlines[2].year) ||
        (heap.deadlines[0].year == heap.deadlines[2].year && heap.deadlines[0].month <= heap.deadlines[2].month) ||
        (heap.deadlines[0].year == heap.deadlines[2].year && heap.deadlines[0].month == heap.deadlines[2].month && heap.deadlines[0].day <= heap.deadlines[2].day)
    );
}

TEST_F(TaskAppTest, ExtractMin_EmptyHeapReturnsDefault) {
    
    MinHeap heap;
    heap.size = 0;

   
    Assignment result = extractMin(&heap);

    
    EXPECT_EQ(result.day, -1);
    EXPECT_EQ(result.month, -1);
    EXPECT_EQ(result.year, -1);
}

TEST_F(TaskAppTest, ExtractMin_SingleElementHeap) {
    
    MinHeap heap;
    heap.size = 1;
    heap.deadlines[0] = { "Task 1", 5, 5, 2024 };

   
    Assignment result = extractMin(&heap);

    
    EXPECT_EQ(result.day, 5);
    EXPECT_EQ(result.month, 5);
    EXPECT_EQ(result.year, 2024);
    EXPECT_STREQ(result.name, "Task 1");

    
    EXPECT_EQ(heap.size, 0);
}

TEST_F(TaskAppTest, ReminderSystemMenu_SetRemindersOption) {
   
    simulateUserInput("1\n0\n0\n0\n1\n\n2\n1\n\n2\n2\n\n2\n3\n\n3\n6\n3\n");

    
    int result = reminderSystemMenu();

    
    EXPECT_EQ(result, 0);

    
    resetStdinStdout();
}

TEST_F(TaskAppTest, ReminderSystemMenu_SetRemindersOptionInvalid) {
    
    simulateUserInput("1\n0\n0\n0\n-11\n\n3\n6\n3\n");

  
    int result = reminderSystemMenu();

    
    EXPECT_EQ(result, 0);

    
    resetStdinStdout();
}



TEST_F(TaskAppTest, ReminderSystemMenu_InvalidChoice) {
    
    simulateUserInput("99\n\n3\n6\n3\n");

    
    int result = reminderSystemMenu();

    
    EXPECT_EQ(result, 0);

    
    resetStdinStdout();
}

TEST_F(TaskAppTest, ReminderSystemMenu_InputErrorHandling) {
    
    simulateUserInput("invalid\n\n\n3\n6\n3\n");

   
    int result = reminderSystemMenu();

    
    EXPECT_EQ(result, 0);

    
    resetStdinStdout();
}

TEST_F(TaskAppTest, ShowCurrentNotificationMethod_Found_SMS) {
    
    SparseMatrixNode node1 = { 1, 20231105, 1, NULL };  
    SparseMatrixNode* head = &node1;

   
    testing::internal::CaptureStdout();  
    int result = showCurrentNotificationMethod(head, 1, 20231105);
    std::string output = testing::internal::GetCapturedStdout();

  
    EXPECT_EQ(result, 1);
    EXPECT_FALSE(output.find("SMS") != std::string::npos);

}

TEST_F(TaskAppTest, ShowCurrentNotificationMethod_Found_Email) {
    
    SparseMatrixNode node1 = { 1, 20231105, 2, NULL };  
    SparseMatrixNode* head = &node1;

    
    testing::internal::CaptureStdout();  
    int result = showCurrentNotificationMethod(head, 1, 20231105);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(result, 1);
    EXPECT_FALSE(output.find("E-Mail") != std::string::npos);
}

TEST_F(TaskAppTest, ShowCurrentNotificationMethod_Found_Notification) {
    
    SparseMatrixNode node1 = { 1, 20231105, 3, NULL }; 
    SparseMatrixNode* head = &node1;

    testing::internal::CaptureStdout(); 
    int result = showCurrentNotificationMethod(head, 1, 20231105);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(result, 1);
    EXPECT_FALSE(output.find("Notification") != std::string::npos);
}

TEST_F(TaskAppTest, ShowCurrentNotificationMethod_NotFound) {
    
    SparseMatrixNode node1 = { 1, 20231105, 1, NULL };
    SparseMatrixNode* head = &node1;

   
    testing::internal::CaptureStdout();
    int result = showCurrentNotificationMethod(head, 2, 20231106);
    std::string output = testing::internal::GetCapturedStdout();

   
    EXPECT_EQ(result, -1);
    EXPECT_FALSE(output.find("No notification method selected") != std::string::npos);

}

TEST_F(TaskAppTest, AlgorithmsMenu_ValidChoices) {
    
    simulateUserInput("1\n2\n2\n2\n2\n\n2\n2\n2\n2\n2\n\n3\n2\n2\n2\n2\n\n4\n2\n2\n2\n2\n\n6\n2\n2\n2\n2\n\n7\n2\n2\n2\n2\n\n8\n");

    
    int result = algorithmsMenu();

    
    EXPECT_EQ(result, 1);

    resetStdinStdout();
}

TEST_F(TaskAppTest, AlgorithmsMenu_InvalidChoice) {
    
    simulateUserInput("-1\na\n8\n");

   
    int result = algorithmsMenu();

    
    EXPECT_EQ(result, 1);

    resetStdinStdout();

    
    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[512] = { 0 };
    fread(outputBuffer, sizeof(char), 511, outputFile);
    fclose(outputFile);

    
    EXPECT_NE(strstr(outputBuffer, "Invalid choice"), nullptr);
}

TEST_F(TaskAppTest, AlgorithmsMenu_ExitChoice) {
  
    simulateUserInput("8\n");

    
    int result = algorithmsMenu();

    
    EXPECT_EQ(result, 1);

    resetStdinStdout();
}
TEST_F(TaskAppTest, FindTaskByName_TaskFound) {
   
    taskCount = 3;
    tasks[0] = { 1, "Task 1", "Description 1", "Category 1", "2024-12-01", 0, {0}, 0 };
    tasks[1] = { 2, "Task 2", "Description 2", "Category 2", "2024-12-02", 0, {0}, 0 };
    tasks[2] = { 3, "Task 3", "Description 3", "Category 3", "2024-12-03", 0, {0}, 0 };

   
    simulateUserInput("Task 2\n");

   
    int index = findTaskByName("Task 2");

   
    EXPECT_EQ(index, 1);
    resetStdinStdout();
}

TEST_F(TaskAppTest, FindTaskByName_TaskNotFound) {
   
    taskCount = 2;
    tasks[0] = { 1, "Task 1", "Description 1", "Category 1", "2024-12-01", 0, {0}, 0 };
    tasks[1] = { 2, "Task 2", "Description 2", "Category 2", "2024-12-02", 0, {0}, 0 };

    
    simulateUserInput("Task 4\n");

    
    int index = findTaskByName("Task 4");

  
    EXPECT_EQ(index, -1);
    resetStdinStdout();
}


TEST_F(TaskAppTest, CreateTaskMenuTest_AddTask) {
    
    simulateUserInput("1\na\na\na\n2001-1-1\n1\n1\n9\n6\n3\n");

    
    int result = createTaskMenu(taskList, &taskCount);

    
    EXPECT_EQ(result, 0);

    
    resetStdinStdout();
}

TEST_F(TaskAppTest, CreateTaskMenuTest_ViewTask) {
   
    simulateUserInput("2\n\n\n9\n6\n3\n");

    
    int result = createTaskMenu(taskList, &taskCount);

    
    EXPECT_EQ(result, 0);

    resetStdinStdout();
}

TEST_F(TaskAppTest, CreateTaskMenuTest_CategorizeTask) {

    simulateUserInput("3\na\n\n\n9\n6\n3\n");

    
    int result = createTaskMenu(taskList, &taskCount);

   
    EXPECT_EQ(result, 0);

    
    resetStdinStdout();
}

TEST_F(TaskAppTest, CreateTaskMenuTest_DependenciesTask) {
     
    simulateUserInput("4\n1\n\n9\n6\n3\n");

 
    int result = createTaskMenu(taskList, &taskCount);

 
    EXPECT_EQ(result, 0);

 
    resetStdinStdout();
}

TEST_F(TaskAppTest, CreateTaskMenuTest_AnalyzeSCC) {
     
    simulateUserInput("5\n\n9\n6\n3\n");

 
    int result = createTaskMenu(taskList, &taskCount);

 
    EXPECT_EQ(result, 0);

 
    resetStdinStdout();
}

TEST_F(TaskAppTest, CreateTaskMenuTest_SearchByKeyword) {
     
    simulateUserInput("6\na\n\n9\n6\n3\n");

 
    int result = createTaskMenu(taskList, &taskCount);

 
    EXPECT_EQ(result, 0);

 
    resetStdinStdout();
}

TEST_F(TaskAppTest, CreateTaskMenuTest_DLL) {
     
    simulateUserInput("7\n1\n2\n3\n\n9\n6\n3\n");

 
    int result = createTaskMenu(taskList, &taskCount);

 
    EXPECT_EQ(result, 0);

 
    resetStdinStdout();
}


TEST_F(TaskAppTest, Deatline_Assign) {
     
    simulateUserInput("2\n1\na\n\n2 1 2000\n3\n6\n3\n\n");

 
    int result = deadlineSettingsMenu();

 
    EXPECT_EQ(result, 0);

 
    resetStdinStdout();
}


TEST_F(TaskAppTest, Deatline_View) {
     
    simulateUserInput("2\n2\n\n3\n6\n3\n\n");

 
    int result = deadlineSettingsMenu();

 
    EXPECT_EQ(result, 0);

 
    resetStdinStdout();
}

TEST_F(TaskAppTest, taskPrioritizationMenu_marktask) {
     
    simulateUserInput("4\n1\na\n2\n\n3\n6\n3\n\n");

 
    int result = taskPrioritizationMenu();

 
    EXPECT_EQ(result, 0);

 
    resetStdinStdout();
}


TEST_F(TaskAppTest, taskPrioritizationMenu_marktaskCase1) {
     
    simulateUserInput("1\na\n1\n\n1\nb\n2\n\n1\nc\n3\n\n3\n6\n3\n\n");

 
    int result = taskPrioritizationMenu();

 
    EXPECT_EQ(result, 0);

 
    resetStdinStdout();
}

TEST_F(TaskAppTest, viewDeadlinesBplusTree) {
     
    simulateUserInput("10 10 10\n11 11 1111\n\n3\n6\n3\n");

    BPlusTree tree = {};
    BPlusTreeNode rootNode = {};
    rootNode.isLeaf = true;
    tree.root = &rootNode;

 
    int result = viewDeadlinesInRange(&tree);

 
    EXPECT_EQ(result, 1);

 
    resetStdinStdout();
}

TEST_F(TaskAppTest, InsertInBPlusTree_AddSingleTask) {
    BPlusTree tree = {};
    BPlusTreeNode rootNode = {};
    rootNode.isLeaf = true;
    rootNode.numKeys = 0;
    tree.root = &rootNode;

    ScheduledTask task;
    task.day = 10;
    task.month = 10;
    task.year = 2024;
    strcpy(task.name, "Test Task");


    int result = insertInBPlusTree(&tree, &task);


    EXPECT_EQ(result, 1);
    EXPECT_EQ(tree.root->numKeys, 1);
    EXPECT_EQ(tree.root->keys[0], getDateKey(10, 10, 2024));
    EXPECT_STREQ(tree.root->tasks[0]->name, "Test Task");
}

TEST_F(TaskAppTest, HuffmanEncode_SimpleString) {

    const char* input = "hello world";


    char* result = huffmanEncode(input);

    ASSERT_NE(result, nullptr); 
    EXPECT_STREQ(result, input); 


    free(result);
}

TEST_F(TaskAppTest, HuffmanEncode_EmptyString) {

    const char* input = "";

   
    char* result = huffmanEncode(input);


    ASSERT_NE(result, nullptr); 
    EXPECT_STREQ(result, input);

    free(result);
}

TEST_F(TaskAppTest, HuffmanEncode_LongString) {
    
    const char* input = "this is a longer test string to check if the function handles larger inputs properly";

    
    char* result = huffmanEncode(input);

    
    ASSERT_NE(result, nullptr);  
    EXPECT_STREQ(result, input); 

    
    free(result);
}

TEST_F(TaskAppTest, SearchUserInHashTable_UserFound) {
   
    User testUser = { 0, "John", "Doe", "john@example.com", "password123", NULL };

    int index = hashFunction(testUser.email);
    hashTable[index] = &testUser;

 
    User* result = searchUserInHashTable("john@example.com", "password123");

   
    ASSERT_NE(result, nullptr); 
    EXPECT_STREQ(result->email, testUser.email);
    EXPECT_STREQ(result->password, testUser.password);
}

TEST_F(TaskAppTest, SearchUserInHashTable_UserNotFound) {
   
    User* result = searchUserInHashTable("nonexistent@example.com", "wrongpassword");

   
    EXPECT_EQ(result, nullptr);  
}

TEST_F(TaskAppTest, SearchUserInHashTable_CollisionHandling) {
  
    User user1 = { 0, "Alice", "Smith", "alice@example.com", "password123", NULL };
    User user2 = { 1, "Bob", "Johnson", "bob@example.com", "password456", NULL };

    int index1 = hashFunction(user1.email);
    int index2 = (index1 + 1) % TABLE_SIZE;  

    hashTable[index1] = &user1;
    hashTable[index2] = &user2;

   
    User* result1 = searchUserInHashTable("alice@example.com", "password123");
    User* result2 = searchUserInHashTable("bob@example.com", "password456");

    
    EXPECT_NE(result1, nullptr);
    EXPECT_STREQ(result1->email, user1.email);
    EXPECT_STREQ(result1->password, user1.password);

}

TEST_F(TaskAppTest, userOptionsMenuTest) {
   
    simulateUserInput("1\n9\n2\n3\n3\n3\n4\n3\n5\n8\n6\n3\n");

 
    int result = userOptionsMenu();

 
    EXPECT_EQ(result, 0);

 
    resetStdinStdout();
}

TEST_F(TaskAppTest, AssignDeadline_All) {
    
    simulateUserInput("23\n\n3\n6\n3\n");

    int result = deadlineSettingsMenu();
   
   
    resetStdinStdout();
    
    
    EXPECT_EQ(result, 0);  


}

TEST_F(TaskAppTest, CreateTaskMenuTest_Invalid) {
     
    simulateUserInput("23\n\n9\n6\n3\n");

 
    int result = createTaskMenu(taskList, &taskCount);

 
    EXPECT_EQ(result, 0);

 
    resetStdinStdout();
}

TEST_F(TaskAppTest, userOptionsMenu_Invalid) {

    simulateUserInput("23\n\n6\n3\n");

    int result = userOptionsMenu();
 
    EXPECT_EQ(result, 0);

 
    resetStdinStdout();
}

TEST_F(TaskAppTest, mainMenuTest_Invalid) {
    const char* testuserfile = "userFile.bin";

    simulateUserInput("23\n\n3\n");

    int result = mainMenu(testuserfile);

    EXPECT_EQ(result, 0);
    resetStdinStdout();

}

TEST_F(TaskAppTest, mainMenuTest_InputError) {
    const char* testuserfile = "userFile.bin";

    simulateUserInput("-2\n\n\n3\n");

    int result = mainMenu(testuserfile);

    EXPECT_EQ(result, 0);
    resetStdinStdout();

}

TEST_F(TaskAppTest, AlgorithmsMenuTest_InputError) {
    
    simulateUserInput("-2\n\n\n8\n6\n3\n");


    int result = algorithmsMenu();


    EXPECT_EQ(result, 1);

    resetStdinStdout();
}

TEST_F(TaskAppTest, userOptionsMenu_InputError) {

    simulateUserInput("-2\n\n\n6\n6\n3\n");

    int result = userOptionsMenu();
 
    EXPECT_EQ(result, 0);

 
    resetStdinStdout();
}

TEST_F(TaskAppTest, CreateTaskMenuTest_InputError) {
    simulateUserInput("-2\n\n\n9\n6\n3\n");

    int result = createTaskMenu(taskList, &taskCount);

    EXPECT_EQ(result, 0);

    resetStdinStdout();
}

TEST_F(TaskAppTest, navigateXORListTest_InputError) {

    simulateUserInput("-2\n\n0\n\n9\n6\n3\n");

    int result = navigateXORList();
    EXPECT_EQ(result, 1);

    resetStdinStdout();
}


TEST_F(TaskAppTest, RegisterUser_UserAddedSuccessfully) {
   
    User testUser = { 0, "John", "Doe", "john.doe@example.com", "securepassword", NULL };
    const char* userFilePath = "test_users.bin";

    
    FILE* file = fopen(userFilePath, "wb");
    if (file) {
        int userCount = 0;
        fwrite(&userCount, sizeof(int), 1, file);
        fclose(file);
    }

    
    int result = registerUser(testUser, userFilePath, true);  

    
    EXPECT_EQ(result, 1); 

    
    file = fopen(userFilePath, "rb");
    ASSERT_TRUE(file != NULL);

    int userCount;
    fread(&userCount, sizeof(int), 1, file);
    EXPECT_EQ(userCount, 1); 

    User loadedUser;
    fread(&loadedUser, sizeof(User), 1, file);
    fclose(file);

    EXPECT_EQ(loadedUser.id, 1); 
    EXPECT_STREQ(loadedUser.name, "John"); 
    EXPECT_STREQ(loadedUser.surname, "Doe"); 
    EXPECT_STREQ(loadedUser.email, "john.doe@example.com"); 
    EXPECT_STREQ(loadedUser.password, "securepassword");


    remove(userFilePath);
}


TEST_F(TaskAppTest, CreateNode_LeafNode) {
    BPlusTreeNode* node = createNode(true);

    ASSERT_NE(node, nullptr) << "Node creation failed.";
    EXPECT_TRUE(node->isLeaf) << "Node should be a leaf.";
    EXPECT_EQ(node->numKeys, 0) << "Number of keys should be initialized to 0.";

    for (int i = 0; i < MAX_CHILDREN; i++) {
        EXPECT_EQ(node->children[i], nullptr) << "Child pointers should be NULL.";
    }

    free(node); // Bellek sızıntısını önlemek için serbest bırak
}

TEST_F(TaskAppTest, CreateNode_InternalNode) {
    BPlusTreeNode* node = createNode(false);

    ASSERT_NE(node, nullptr) << "Node creation failed.";
    EXPECT_FALSE(node->isLeaf) << "Node should not be a leaf.";
    EXPECT_EQ(node->numKeys, 0) << "Number of keys should be initialized to 0.";

    for (int i = 0; i < MAX_CHILDREN; i++) {
        EXPECT_EQ(node->children[i], nullptr) << "Child pointers should be NULL.";
    }

    free(node); // Bellek sızıntısını önlemek için serbest bırak
}

TEST_F(TaskAppTest, CreateBPlusTree_Success) {
    BPlusTree* tree = createBPlusTree();

    ASSERT_NE(tree, nullptr) << "Tree creation failed.";
    ASSERT_NE(tree->root, nullptr) << "Root node should not be NULL.";
    EXPECT_TRUE(tree->root->isLeaf) << "Root node should be a leaf.";
    EXPECT_EQ(tree->root->numKeys, 0) << "Root node should have 0 keys initially.";

    for (int i = 0; i < MAX_CHILDREN; i++) {
        EXPECT_EQ(tree->root->children[i], nullptr) << "Child pointers should be NULL.";
    }

    free(tree->root); // Önce kök düğümü serbest bırak
    free(tree);       // Ardından ağacı serbest bırak
}





int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::GTEST_FLAG(color) = "no";
    return RUN_ALL_TESTS();
}