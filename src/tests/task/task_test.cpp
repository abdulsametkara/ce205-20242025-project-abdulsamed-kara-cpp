#define ENABLE_TASK_TEST  // Bu sat�r etkinle�tirildi
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
        createTestUserFile();
        createTestTaskFile();
        createTestDeadlineFile();
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
    // Kullan�c� dizisi �rne�i ve kullan�c� say�s�n� belirle
    User users[] = {
        {1, "John", "Doe", "john@example.com", "password123", nullptr},
        {2, "Jane", "Doe", "jane@example.com", "password456", nullptr},
    };
    int userCount = 2;

    // Fonksiyonu �a��r ve beklenen de�eri test et
    int newUserId = getNewUserId(users, userCount);
    EXPECT_EQ(newUserId, 3);  // Son kullan�c� ID'si 2 oldu�undan, yeni kullan�c� ID'si 3 olmal�d�r

    // Kullan�c� say�s� s�f�r oldu�unda test et
    userCount = 0;
    newUserId = getNewUserId(users, userCount);
    EXPECT_EQ(newUserId, 1);  // Kullan�c� yoksa ID 1 ile ba�lamal�d�r
}

TEST_F(TaskAppTest, LoadUsers) {
    // Test i�in sahte kullan�c� dosyas�n� haz�rla
    const char* testFile = "test_users.bin";

    // Test kullan�c�lar� olu�tur ve dosyaya kaydet
    User testUsers[] = {
        {1, "John", "Doe", "john@example.com", "password123", nullptr},
        {2, "Jane", "Doe", "jane@example.com", "password456", nullptr}
    };
    int userCount = sizeof(testUsers) / sizeof(testUsers[0]);

    FILE* file = fopen(testFile, "wb");
    fwrite(&userCount, sizeof(int), 1, file);
    fwrite(testUsers, sizeof(User), userCount, file);
    fclose(file);

    // Test edilen fonksiyonu �a��r
    User* loadedUsers = nullptr;
    int loadedCount = loadUsers(testFile, &loadedUsers);

    // Sonu�lar� do�rula
    EXPECT_EQ(loadedCount, userCount);  // Kullan�c� say�s� do�ru mu?
    ASSERT_NE(loadedUsers, nullptr);    // Kullan�c�lar y�klendi mi?

    for (int i = 0; i < userCount; ++i) {
        EXPECT_EQ(loadedUsers[i].id, testUsers[i].id);
        EXPECT_STREQ(loadedUsers[i].name, testUsers[i].name);
        EXPECT_STREQ(loadedUsers[i].surname, testUsers[i].surname);
        EXPECT_STREQ(loadedUsers[i].email, testUsers[i].email);
        EXPECT_STREQ(loadedUsers[i].password, testUsers[i].password);
    }

    // Bellekteki kullan�c�lar� temizle
    free(loadedUsers);

    // Test dosyas�n� sil
    remove(testFile);
}

TEST_F(TaskAppTest, AddTaskToXORList) {
    // Yeni bir g�rev olu�tur
    Task task = { 3, "Task 3", "Description 3", "Category 3", "2024-12-01", 0, {2}, 1 };

    // G�revi XOR ba�lant�l� listeye ekle
    int result = addTaskToXORList(task);

    // Fonksiyonun ba�ar�yla �al��t���n� do�rula
    EXPECT_EQ(result, 1);

    // Ba� d���m�n ve kuyruk d���m�n do�ru �ekilde ayarland���n� kontrol et
    ASSERT_NE(xorHead, nullptr);
    ASSERT_NE(xorTail, nullptr);

    // �lk d���m olarak eklenen g�revin do�ru atan�p atanmad���n� kontrol et
    EXPECT_EQ(xorHead->task.id, task.id);
    EXPECT_STREQ(xorHead->task.name, task.name);
    EXPECT_STREQ(xorHead->task.description, task.description);
    EXPECT_STREQ(xorHead->task.category, task.category);

    // Eklenen d���m�n sonunda oldu�unu do�rula
    EXPECT_EQ(xorTail->task.id, task.id);
    EXPECT_EQ(xorHead, xorTail); // Bu g�rev ilk g�revse ba� ve son ayn� olmal�
}
///



TEST_F(TaskAppTest, AssignDeadline_ValidInput) {
    // Ge�erli bir g�rev ad� ve tarih girdisini sim�le et
    simulateUserInput("Sample Task\n\n10 11 2024\n");

    Assignment assignment;

    // Fonksiyonu �al��t�r ve beklenen de�eri kontrol et
    int result = assign_deadline(&assignment);
    resetStdinStdout();
    EXPECT_EQ(result, 0);  // Fonksiyonun ba�ar�yla �al��mas� bekleniyor


    // Standart giri� ve ��k��� s�f�rla
    
}

TEST_F(TaskAppTest, AssignDeadline_InvalidDate) {
    // Ge�ersiz bir tarih girdisini sim�le et
    simulateUserInput("Sample Task\n\n32 13 2024\n");

    Assignment assignment;

    // Fonksiyonu �al��t�r ve ge�ersiz tarih i�in -1 d�nd�rmesini bekle
    int result = assign_deadline(&assignment);
    resetStdinStdout();
    EXPECT_EQ(result, -1);

    // Standart giri� ve ��k��� s�f�rla
    
}

TEST_F(TaskAppTest, ViewDeadlines_NoDeadlines) {
    // Y���n� bo� hale getiriyoruz
    deadlineHeap.size = 0;

    // viewDeadlines fonksiyonunu �a��r�yoruz ve ��kt� dosyas�na y�nlendiriyoruz
    simulateUserInput(""); // Bo� girdi sim�le ediyoruz
    int result = viewDeadlines();
    resetStdinStdout();

    // Fonksiyonun -1 d�nd�rd���n� ve "No deadlines to display." yazd���n� kontrol ediyoruz
    EXPECT_EQ(result, -1);
    FILE* outputFile = fopen(outputTest, "rb");
    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 256, outputFile);
    fclose(outputFile);
    EXPECT_NE(strstr(outputBuffer, "No deadlines to display."), nullptr);
}






TEST_F(TaskAppTest, PlatformSleep_ReturnsCorrectValue) {
    // Set a test duration in seconds
    int testDuration = 1;

    // Call platformSleep and verify it returns 1 as expected
    int result = platformSleep(testDuration);
    EXPECT_EQ(result, 1);
}



TEST_F(TaskAppTest, AddNotificationToSparseMatrix) {
    SparseMatrixNode* head = NULL;

    // Test Case 1: Add a single notification
    int row1 = 1, col1 = 2, value1 = 5;
    int result = addNotification(&head, row1, col1, value1);
    EXPECT_EQ(result, 1);  // Check if the function returned success
    ASSERT_NE(head, nullptr);  // Head should not be NULL after adding

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
    // Kuyruk bo�ken viewTask fonksiyonunu test ediyoruz
    front = NULL;  // Kuyruk ba�� NULL olmal�

    // ��k��� yakalamak i�in stdout'u test dosyas�na y�nlendir
    freopen(outputTest, "wb", stdout);

    // Fonksiyonu �a��r
    int result = viewTask();

    // Stdout'u eski haline getir
    resetStdinStdout();

    // D�n�� de�erinin 0 oldu�unu do�rula
    EXPECT_EQ(result, 0);

    // ��kt�y� kontrol etmek i�in dosyadan oku
    FILE* outputFile = fopen(outputTest, "rb");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    // Hata mesaj�n�n do�ru yaz�ld���n� kontrol et
    EXPECT_NE(strstr(outputBuffer, "No tasks found. The task list is empty."), nullptr);
}


TEST_F(TaskAppTest, ViewTask_FilledQueue) {
    // Test i�in sahte g�revler olu�tur
    Task task1 = { 1, "Task 1", "Description 1", "Category 1", "2024-11-10", 0, {0}, 1 };
    Task task2 = { 2, "Task 2", "Description 2", "Category 2", "2024-11-15", 0, {0}, 1 };

    // Kuyru�a g�rev ekle
    enqueue(task1);
    enqueue(task2);

    // Fonksiyonu �al��t�rmadan �nce stdout'u test dosyas�na y�nlendir
    freopen(outputTest, "wb", stdout);

    // Fonksiyonu �al��t�r
    int result = viewTask();

    // Stdout'u eski haline getir
    resetStdinStdout();

    // D�n�� de�erinin 1 oldu�unu do�rula
    EXPECT_EQ(result, 1);

    // ��kt�y� kontrol etmek i�in dosyadan oku
    FILE* outputFile = fopen(outputTest, "rb");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[512] = { 0 };
    fread(outputBuffer, sizeof(char), 511, outputFile);
    fclose(outputFile);

    // G�revlerin do�ru yaz�ld���n� kontrol et
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
    // Kullan�c� giri�ini sim�le et
    simulateUserInput("\n");

    // ��k��� yakalamak i�in stdout'u test dosyas�na y�nlendir
    freopen(outputTest, "wb", stdout);

    // Fonksiyonu �a��r
    int result = enterToContinue();

    // Stdout'u eski haline getir
    resetStdinStdout();

    // Fonksiyonun do�ru sonucu d�nd�rd���n� kontrol et
    EXPECT_EQ(result, 1);

    // ��kt�y� kontrol etmek i�in dosyadan oku
    FILE* outputFile = fopen(outputTest, "rb");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    // Fonksiyonun do�ru mesaj� yazd���n� kontrol et
    EXPECT_NE(strstr(outputBuffer, "Press enter to continue"), nullptr);
}

TEST_F(TaskAppTest, HandleInputError) {
    // Ge�ersiz giri� sim�lasyonu yap�yoruz (�rne�in: "abc")
    simulateUserInput("abc\n");

    // ��k��� yakalamak i�in stdout'u test dosyas�na y�nlendir
    freopen(outputTest, "wb", stdout);

    // Fonksiyonu �a��r
    int result = handleInputError();

    // Stdout'u eski haline getir
    resetStdinStdout();

    // Fonksiyonun do�ru sonucu d�nd�rd���n� kontrol et
    EXPECT_EQ(result, 0);

    // ��kt�y� kontrol etmek i�in dosyadan oku
    FILE* outputFile = fopen(outputTest, "rb");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    // Fonksiyonun do�ru mesaj� yazd�rd���n� kontrol et
    EXPECT_NE(strstr(outputBuffer, "Invalid input. Please enter a number."), nullptr);
}

TEST_F(TaskAppTest, AddTaskToList_ValidTask) {
    // Ba�lang��ta listeyi temizle
    head = NULL;
    tail = NULL;

    // Yeni g�rev olu�tur
    Task newTask = { 1, "Test Task", "Test Description", "Test Category", "2024-12-15", 0, {2}, 1 };

    // Fonksiyonu �a��r ve sonucu kontrol et
    int result = addTaskToList(newTask);
    EXPECT_EQ(result, 1); // Fonksiyonun ba�ar�yla tamamland���n� kontrol et

    // Listenin g�ncellendi�ini kontrol et
    ASSERT_NE(head, nullptr); // Listenin ba�� NULL olmamal�
    ASSERT_NE(tail, nullptr); // Listenin sonu NULL olmamal�
    EXPECT_EQ(head, tail);    // Liste tek d���mden olu�mal�

    // D���m�n do�ru atand���n� kontrol et
    EXPECT_EQ(head->task.id, newTask.id);
    EXPECT_STREQ(head->task.name, newTask.name);
    EXPECT_STREQ(head->task.description, newTask.description);
    EXPECT_STREQ(head->task.category, newTask.category);
    EXPECT_STREQ(head->task.dueDate, newTask.dueDate);
}

TEST_F(TaskAppTest, AddTaskToList_MultipleTasks) {
    // Ba�lang��ta listeyi temizle
    head = NULL;
    tail = NULL;

    // �lk g�revi ekle
    Task firstTask = { 1, "First Task", "First Description", "First Category", "2024-12-10", 0, {2}, 1 };
    int result = addTaskToList(firstTask);
    EXPECT_EQ(result, 1);

    // �kinci g�revi ekle
    Task secondTask = { 2, "Second Task", "Second Description", "Second Category", "2024-12-20", 0, {1}, 1 };
    result = addTaskToList(secondTask);
    EXPECT_EQ(result, 1);

    // Listenin do�ru �ekilde g�ncellendi�ini kontrol et
    ASSERT_NE(head, nullptr);
    ASSERT_NE(tail, nullptr);
    EXPECT_NE(head, tail); // Liste birden fazla d���mden olu�mal�

    // �lk d���m� kontrol et
    EXPECT_EQ(head->task.id, firstTask.id);
    EXPECT_STREQ(head->task.name, firstTask.name);
    EXPECT_EQ(head->next, tail); // �lk d���m�n next i�aret�isi kuyru�u g�stermeli

    // �kinci d���m� kontrol et
    EXPECT_EQ(tail->task.id, secondTask.id);
    EXPECT_STREQ(tail->task.name, secondTask.name);
    EXPECT_EQ(tail->prev, head); // Kuyru�un prev i�aret�isi ba�� g�stermeli
}
//TEST_F(TaskAppTest, LoginUserMenu_SuccessfulLogin) {
//    const char* mockUserFile = "mock_users.bin";
//
//    // Kullan�c� giri�ini sim�le et
//    simulateUserInput("john@example.com\npassword123\n");
//
//    // Fonksiyonu �a��r
//    int result = loginUserMenu(mockUserFile);
//
//    // Do�ru sonucu kontrol et
//    EXPECT_EQ(result, 1);
//}

// Yanl�� e-posta ile giri� denendi�inde ba�ar�s�z d�nmesi beklenir
//TEST_F(TaskAppTest, LoginUserMenu_WrongEmail) {
//    const char* mockUserFile = "mock_users.bin";
//
//    // Yanl�� e-posta girdisini sim�le et
//    simulateUserInput("wrong@example.com\npassword123\n");
//
//    // Fonksiyonu �a��r
//    int result = loginUserMenu(mockUserFile);
//
//    // Yanl�� giri� oldu�u i�in 0 d�nmesi beklenir
//    EXPECT_EQ(result, 0);
//}
//
//// Yanl�� �ifre ile giri� denendi�inde ba�ar�s�z d�nmesi beklenir
//TEST_F(TaskAppTest, LoginUserMenu_WrongPassword) {
//    const char* mockUserFile = "mock_users.bin";
//
//    // Yanl�� �ifre girdisini sim�le et
//    simulateUserInput("john@example.com\nwrongpassword\n");
//
//    // Fonksiyonu �a��r
//    int result = loginUserMenu(mockUserFile);
//
//    // Yanl�� giri� oldu�u i�in 0 d�nmesi beklenir
//    EXPECT_EQ(result, 0);
//}
//
//// Eksik dosya ile giri� denendi�inde ba�ar�s�z d�nmesi beklenir
//TEST_F(TaskAppTest, LoginUserMenu_MissingFile) {
//    const char* invalidFile = "nonexistent_users.bin";
//
//    // Kullan�c� giri�ini sim�le et
//    simulateUserInput("john@example.com\npassword123\n");
//
//    // Fonksiyonu �a��r
//    int result = loginUserMenu(invalidFile);
//
//    // Dosya bulunmad��� i�in 0 d�nmesi beklenir
//    EXPECT_EQ(result, 0);
//}

TEST_F(TaskAppTest, LoginRegistermenuTest) {
    const char* testuserfile = "userFile.bin";

    simulateUserInput("2\nasd\nasd\nasd\nasd\n\n1\nasd\nasd\n\n6\n3\n");

    int result = mainMenu(testuserfile);

    EXPECT_EQ(result, 0);
}

//TEST_F(TaskAppTest, taskprenotaskfail) {
//    const char* testuserfile = "taskFile.bin";
//
//    simulateUserInput("4\n1\n\n3\n");
//
//    int result = userOptionsMenu();
//
//    EXPECT_EQ(result, 0);
//}

TEST_F(TaskAppTest, taskprecorrectwittasks) {
    const char* testuserfile = "taskFile.bin";
    createTestUserFile();

    simulateUserInput("4\n1\nTask 1\n2\n\n3\n6\n3\n");

    int result = userOptionsMenu();

    EXPECT_EQ(result, 0);
}


//TEST_F(TaskAppTest, LoadTasksToXORList_ValidFile) {
//    // Test i�in ge�ici bir g�rev dosyas� olu�tur
//    const char* testFilename = "test_tasks.bin";
//
//    // Test g�revlerini olu�tur ve dosyaya kaydet
//    Task testTasks[] = {
//        {1, "Task 1", "Description 1", "Category 1", "2024-11-05", 0, {2}, 1},
//        {2, "Task 2", "Description 2", "Category 2", "2024-11-10", 1, {1}, 1}
//    };
//    int taskCount = sizeof(testTasks) / sizeof(testTasks[0]);
//
//    FILE* file = fopen(testFilename, "wb");
//    ASSERT_NE(file, nullptr) << "Error: Unable to create test file.";
//    fwrite(testTasks, sizeof(Task), taskCount, file);
//    fclose(file);
//
//    // Fonksiyonu �a��r ve do�ru �al���p �al��mad���n� kontrol et
//    int result = loadTasksToXORList(testFilename);
//    EXPECT_EQ(result, 1);  // Ba�ar�yla tamamland���n� kontrol et
//
//    // XOR listesine eklenen g�revleri kontrol et
//    XORNode* current = xorHead;
//    for (int i = 0; i < taskCount; ++i) {
//        ASSERT_NE(current, nullptr) << "XOR list is missing nodes.";
//        EXPECT_EQ(current->task.id, testTasks[i].id);
//        EXPECT_STREQ(current->task.name, testTasks[i].name);
//        EXPECT_STREQ(current->task.description, testTasks[i].description);
//        EXPECT_STREQ(current->task.category, testTasks[i].category);
//        EXPECT_STREQ(current->task.dueDate, testTasks[i].dueDate);
//
//        // Bir sonraki d���me ge�
//        XORNode* nextNode = (XORNode*)((uintptr_t)current->xorPtr ^ (uintptr_t)nullptr);
//        current = nextNode;
//    }
//
//    // Test dosyas�n� sil
//    remove(testFilename);
//}
//
// TEST_F(TaskAppTest, LoadTasksToXORList_InvalidFile) {
//    // Ge�ersiz dosya ad�yla fonksiyonu �a��r
//    const char* invalidFilename = "nonexistent_tasks.bin";
//    int result = loadTasksToXORList(invalidFilename);
//
//    // D�n�� de�erinin -1 oldu�unu kontrol et
//    EXPECT_EQ(result, -1);
//
//    // ��kt�y� kontrol etmek i�in dosyadan oku
//    FILE* outputFile = fopen(outputTest, "rb");
//    ASSERT_NE(outputFile, nullptr);
//
//    char outputBuffer[256] = { 0 };
//    fread(outputBuffer, sizeof(char), 255, outputFile);
//    fclose(outputFile);
//
//    // Hata mesaj�n�n do�ru yaz�ld���n� kontrol et
//    EXPECT_NE(strstr(outputBuffer, "Error: Unable to open tasks file."), nullptr);
//}

TEST_F(TaskAppTest, PushTask_Success) {
    // Test i�in bir g�rev olu�tur
    Task task = { 1, "Test Task", "Task Description", "General", "2024-12-15", 0, {0}, 0 };

    // push fonksiyonunu �a��r ve ba�ar� durumunu kontrol et
    int result = push(task);
    EXPECT_EQ(result, 1); // Ba�ar�l� olmal�

    // stackTop'un NULL olmad���n� ve do�ru �ekilde ayarland���n� kontrol et
    ASSERT_NE(stackTop, nullptr);
    EXPECT_EQ(stackTop->task.id, task.id);
    EXPECT_STREQ(stackTop->task.name, task.name);
    EXPECT_STREQ(stackTop->task.description, task.description);
    EXPECT_STREQ(stackTop->task.category, task.category);
    EXPECT_STREQ(stackTop->task.dueDate, task.dueDate);

    // Belle�i temizle
    StackNode* temp = stackTop;
    stackTop = stackTop->next;
    free(temp);
}

/*TEST_F(TaskAppTest, PopTask_EmptyStack) {
    // Bo� bir y���n durumunda pop fonksiyonunu test et
    stackTop = NULL;  // Y���n bo� oldu�undan stackTop'u NULL yap

    Task result = pop();

    // Task id'sinin -1 d�nd���n� kontrol et (bo� y���n i�areti)
    

    // ��kt�y� kontrol etmek i�in dosyadan oku
    FILE* outputFile = fopen(outputTest, "rb");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    // Hata mesaj�n�n do�ru yaz�ld���n� kontrol et
    EXPECT_NE(strstr(outputBuffer, "Stack is empty"), nullptr);

    // ��kt�y� debug i�in yazd�r
    std::cout << "Captured Output: " << outputBuffer << std::endl;
    EXPECT_EQ(result.id, -1);
}*/

TEST_F(TaskAppTest, PopTask_NonEmptyStack) {
    // Bir y���n olu�tur ve bir g�rev ekle
    Task task = { 1, "Test Task", "Task Description", "General", "2024-12-15", 0, {0}, 0 };
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    ASSERT_NE(newNode, nullptr); // Bellek tahsisini kontrol et
    newNode->task = task;
    newNode->next = NULL;  // Bu tek ��e olacak
    stackTop = newNode;

    // Pop fonksiyonunu �a��r
    Task result = pop();

    // ��kar�lan g�revin do�ru oldu�unu kontrol et
    EXPECT_EQ(result.id, task.id);
    EXPECT_STREQ(result.name, task.name);
    EXPECT_STREQ(result.description, task.description);
    EXPECT_STREQ(result.category, task.category);
    EXPECT_STREQ(result.dueDate, task.dueDate);

    // Y���n�n bo� oldu�undan emin ol
    EXPECT_EQ(stackTop, nullptr);

    // Pop edilen ��enin bellekte serbest b�rak�ld���n� kontrol et
    // Bunun i�in �zel bir bellek testi gerekir (manuel do�rulama yap�labilir)
}

TEST_F(TaskAppTest, UndoLastTask_EmptyStack) {
    // Test: Y���n bo�ken `undoLastTask` fonksiyonu
    stackTop = NULL; // Y���n� bo� yap

    // G�rev listesi ve g�rev say�s� sim�le ediliyor
    Task taskList[100];
    int taskCount = 0;

    // Standart ��kt�y� yakalamak i�in y�nlendir
    freopen(outputTest, "w", stdout);

    // Fonksiyonu �a��r
    int result = undoLastTask(taskList, &taskCount);

    // Standart ��kt�y� s�f�rla
    resetStdinStdout();

    // Fonksiyonun d�n�� de�erini kontrol et
    EXPECT_EQ(result, -1);

    // ��kt�y� kontrol etmek i�in dosyadan oku
    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    // ��kt�n�n do�ru hata mesaj�n� i�erdi�ini kontrol et
    EXPECT_NE(strstr(outputBuffer, "No tasks to undo."), nullptr);
}

TEST_F(TaskAppTest, UndoLastTask_Success) {
    // Test: Y���n doluyken `undoLastTask` fonksiyonu
    stackTop = NULL; // Y���n� temizle

    // G�rev listesi ve g�rev say�s� sim�le ediliyor
    Task taskList[100];
    int taskCount = 1;

    // �rnek bir g�rev ekle
    Task task = { 1, "Test Task", "Test Description", "Test Category", "2024-12-31", 0, {0}, 0 };
    push(task); // G�revi y���na ekle
    taskList[0] = task;

    // Standart ��kt�y� yakalamak i�in y�nlendir
    freopen(outputTest, "w", stdout);

    // Fonksiyonu �a��r
    int result = undoLastTask(taskList, &taskCount);

    // Standart ��kt�y� s�f�rla
    resetStdinStdout();

    // Fonksiyonun d�n�� de�erini kontrol et
    EXPECT_EQ(result, 1);

    // G�rev say�s�n�n g�ncellendi�ini kontrol et
    EXPECT_EQ(taskCount, 0);

    // ��kt�y� kontrol etmek i�in dosyadan oku
    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    // ��kt�n�n do�ru ba�ar� mesaj�n� i�erdi�ini kontrol et
    EXPECT_NE(strstr(outputBuffer, "Last task 'Test Task' undone successfully."), nullptr);
}
TEST_F(TaskAppTest, PrintDependenciesUtil_NoDependencies) {
    // Test i�in sahte g�rev dizisi olu�tur
    Task taskList[3] = {
        {1, "Task 1", "Description 1", "Category 1", "2024-12-01", 0, {0}, 0},
        {2, "Task 2", "Description 2", "Category 2", "2024-12-02", 0, {0}, 0},
        {3, "Task 3", "Description 3", "Category 3", "2024-12-03", 0, {0}, 0},
    };

    bool visited[4] = { false };

    // Standart ��kt�y� yakalamak i�in y�nlendir
    freopen(outputTest, "w", stdout);

    // Fonksiyonu �a��r
    int result = printDependenciesUtil(taskList, 1, visited);

    // Standart ��kt�y� s�f�rla
    resetStdinStdout();

    // Fonksiyonun do�ru de�er d�nd�rd���n� kontrol et
    EXPECT_EQ(result, 1);

    // ��kt�y� kontrol etmek i�in dosyadan oku
    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    // ��k���n do�ru oldu�unu do�rula (ba��ml�l�k olmad���ndan ��kt� olmamal�)
    EXPECT_EQ(strlen(outputBuffer), 0);
}

/*TEST_F(TaskAppTest, PrintDependenciesUtil_WithDependencies) {
    // Test i�in sahte g�rev dizisi olu�tur
    Task taskList[3] = {
        {1, "Task 1", "Description 1", "Category 1", "2024-12-01", 2, {2, 3}, 0},
        {2, "Task 2", "Description 2", "Category 2", "2024-12-02", 0, {0}, 0},
        {3, "Task 3", "Description 3", "Category 3", "2024-12-03", 0, {0}, 0},
    };

    bool visited[4] = { false };

    // Standart ��kt�y� yakalamak i�in y�nlendir
    freopen(outputTest, "w", stdout);

    // Fonksiyonu �a��r
    int result = printDependenciesUtil(taskList, 1, visited);

    // Standart ��kt�y� s�f�rla
    resetStdinStdout();

    // Fonksiyonun do�ru de�er d�nd�rd���n� kontrol et
    EXPECT_EQ(result, 1);

    // ��kt�y� kontrol etmek i�in dosyadan oku
    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    // ��k���n do�ru oldu�unu do�rula
    EXPECT_NE(strstr(outputBuffer, "Task 1 depends on Task 2\n"), nullptr);
    EXPECT_NE(strstr(outputBuffer, "Task 1 depends on Task 3\n"), nullptr);
   
}*/

TEST_F(TaskAppTest, PrintDependencies_InvalidTaskId) {
    // Ge�ersiz task ID ile �a��r
    int taskCount = 3;
    Task taskList[3] = {
        {1, "Task 1", "Description 1", "Category 1", "2024-12-01", 0, {0}, 0},
        {2, "Task 2", "Description 2", "Category 2", "2024-12-02", 0, {0}, 0},
        {3, "Task 3", "Description 3", "Category 3", "2024-12-03", 0, {0}, 0}
    };

    // Standart ��kt�y� dosyaya y�nlendir
    freopen(outputTest, "w", stdout);

    // Fonksiyonu �a��r
    int result = printDependencies(taskList, taskCount, 0);

    // Standart ��kt�y� s�f�rla
    resetStdinStdout();

    // D�n�� de�erinin -1 oldu�unu do�rula
    EXPECT_EQ(result, -1);

    // ��kt�y� kontrol et
    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    EXPECT_NE(strstr(outputBuffer, "Invalid task ID"), nullptr);
}

TEST_F(TaskAppTest, PrintDependencies_WithDependencies) {
    // Ba��ml�l�klar� olan g�revler i�in test
    int taskCount = 3;
    Task taskList[3] = {
        {1, "Task 1", "Description 1", "Category 1", "2024-12-01", 1, {2}, 1},
        {2, "Task 2", "Description 2", "Category 2", "2024-12-02", 1, {3}, 1},
        {3, "Task 3", "Description 3", "Category 3", "2024-12-03", 0, {0}, 0}
    };

    // Standart ��kt�y� dosyaya y�nlendir
    freopen(outputTest, "w", stdout);

    // Fonksiyonu �a��r
    int result = printDependencies(taskList, taskCount, 1);

    // Standart ��kt�y� s�f�rla
    resetStdinStdout();

    // D�n�� de�erinin 1 oldu�unu do�rula
    EXPECT_EQ(result, 1);

    // ��kt�y� kontrol et
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
    // Hi� ba��ml�l��� olmayan g�rev i�in test
    int taskCount = 3;
    Task taskList[3] = {
        {1, "Task 1", "Description 1", "Category 1", "2024-12-01", 0, {0}, 0},
        {2, "Task 2", "Description 2", "Category 2", "2024-12-02", 0, {0}, 0},
        {3, "Task 3", "Description 3", "Category 3", "2024-12-03", 0, {0}, 0}
    };

    // Standart ��kt�y� dosyaya y�nlendir
    freopen(outputTest, "w", stdout);

    // Fonksiyonu �a��r
    int result = printDependencies(taskList, taskCount, 1);

    // Standart ��kt�y� s�f�rla
    resetStdinStdout();

    // D�n�� de�erinin 1 oldu�unu do�rula
    EXPECT_EQ(result, 1);

    // ��kt�y� kontrol et
    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    EXPECT_NE(strstr(outputBuffer, "Dependencies for Task 1"), nullptr);
    EXPECT_EQ(strstr(outputBuffer, "depends on"), nullptr);
}

TEST_F(TaskAppTest, PushSccStack_Success) {
    // SCC y���n� bo� durumda ba�las�n
    sccStack = NULL;

    // �lk veri ekleme
    int result = pushSccStack(10);
    EXPECT_EQ(result, 1); // Fonksiyonun ba�ar� d�nd�rmesi beklenir
    ASSERT_NE(sccStack, nullptr); // Yeni d���m�n eklenmi� olmas� gerekir
    EXPECT_EQ(sccStack->data, 10); // Y���n�n �st�ndeki veri kontrol edilir

    // �kinci veri ekleme
    result = pushSccStack(20);
    EXPECT_EQ(result, 1); // �kinci eklemenin de ba�ar�l� olmas� beklenir
    ASSERT_NE(sccStack, nullptr); // Y���n h�l� dolu olmal�
    EXPECT_EQ(sccStack->data, 20); // En �stteki veri kontrol edilir
    EXPECT_EQ(sccStack->next->data, 10); // Bir alt�ndaki d���m kontrol edilir
}

/*TEST_F(TaskAppTest, PushSccStack_MemoryAllocationFailure) {
    // malloc'u ge�ici olarak ba�ar�s�z olacak �ekilde ayarla

// ��k��� dosyaya y�nlendir
    freopen(outputTest, "w", stdout);

    // pushSccStack �a�r�s�
    int result = pushSccStack(-1);

    // Stdout'u s�f�rla
    resetStdinStdout();

    // Fonksiyonun -1 d�nd�rd���n� do�rula
    EXPECT_EQ(result, 1);

    // ��kt�y� kontrol et
    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    // Hata mesaj�n� kontrol et
    EXPECT_NE(strstr(outputBuffer, "Memory allocation failed"), nullptr);

}*/

TEST_F(TaskAppTest, PopSccStack_EmptyStack) {
    // Bo� bir SCC y���n� durumunda popSccStack fonksiyonunu test et
    sccStack = NULL; // SCC y���n� bo�

    // Fonksiyonu �a��r
    int result = popSccStack();

    // Sonucun -1 oldu�unu kontrol et
    EXPECT_EQ(result, -1);
}

TEST_F(TaskAppTest, PopSccStack_NonEmptyStack) {
    // SCC y���n�na birka� eleman ekle
    AdjacencyNode* node1 = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    node1->data = 10;
    node1->next = NULL;

    AdjacencyNode* node2 = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    node2->data = 20;
    node2->next = node1;

    // Y���n� ba�lat
    sccStack = node2;

    // �lk eleman� ��kar ve kontrol et
    int result = popSccStack();
    EXPECT_EQ(result, 20); // En �stteki eleman 20 olmal�

    // �kinci eleman� ��kar ve kontrol et
    result = popSccStack();
    EXPECT_EQ(result, 10); // En �stteki eleman 10 olmal�

    // Y���n�n tamamen bo� oldu�unu kontrol et
    EXPECT_EQ(sccStack, nullptr);
}

TEST_F(TaskAppTest, DfsUtil_SingleNode) {
    // Bir d���ml� bir grafik olu�tur
    int visited[2] = { 0 }; // D���m 1 i�in yer ay�r
    AdjacencyNode* adj[2] = { NULL }; // Adjacency list

    // Adjacency list'i doldur (tek d���m)
    AdjacencyNode* node1 = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    ASSERT_NE(node1, nullptr); // Bellek tahsisini kontrol et
    node1->data = 1;
    node1->next = NULL;
    adj[1] = node1;

    // SCC y���n�n� temizle
    sccStack = NULL;

    // Fonksiyonu �a��r
    int result = dfsUtil(1, visited, adj, NULL);

    // Ziyaret durumunu kontrol et
    EXPECT_EQ(result, 1);
    EXPECT_EQ(visited[1], 1);

    // SCC y���n�n� kontrol et
    ASSERT_NE(sccStack, nullptr);
    EXPECT_EQ(sccStack->data, 1);

    // Belle�i temizle
    free(node1);
    while (sccStack) {
        popSccStack();
    }
}

TEST_F(TaskAppTest, DfsUtil_MultipleNodes) {
    // �ki d���ml� bir grafik olu�tur
    int visited[3] = { 0 }; // D���mler 1 ve 2 i�in yer ay�r
    AdjacencyNode* adj[3] = { NULL }; // Adjacency list

    // Adjacency list'i doldur
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

    // SCC y���n�n� temizle
    sccStack = NULL;

    // Fonksiyonu �a��r
    int result = dfsUtil(1, visited, adj, NULL);

    // Ziyaret durumlar�n� kontrol et
    EXPECT_EQ(result, 1);
    EXPECT_EQ(visited[1], 1);
    EXPECT_EQ(visited[2], 1);

    // SCC y���n�n� kontrol et
    ASSERT_NE(sccStack, nullptr);
    EXPECT_EQ(sccStack->data, 2); // �lk eklenen
    popSccStack();
    EXPECT_EQ(sccStack->data, 1); // Son eklenen

    // Belle�i temizle
    free(node1);
    free(node2);
    while (sccStack) {
        popSccStack();
    }
}

TEST_F(TaskAppTest, FindSCCs_ValidGraph) {
    // Ge�erli bir grafik olu�tur
    int V = 3;
    AdjacencyNode* adj[3] = { NULL };

    // D���m 0 -> 1
    AdjacencyNode* node1 = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    node1->data = 1;
    node1->next = NULL;
    adj[0] = node1;

    // D���m 1 -> 2
    AdjacencyNode* node2 = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    node2->data = 2;
    node2->next = NULL;
    adj[1] = node2;

    // D���m 2 -> 0
    AdjacencyNode* node3 = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    node3->data = 0;
    node3->next = NULL;
    adj[2] = node3;

    // ��k��� yakalamak i�in bir dosya a�
    FILE* outFile = fopen(outputTest, "w");
    ASSERT_NE(outFile, nullptr);

    // Fonksiyonu �a��r
    int result = findSCCs(V, adj, outFile);

    // Dosyay� kapat ve sonu�lar� kontrol et
    fclose(outFile);

    // D�n�� de�erini kontrol et
    EXPECT_EQ(result, 1);

    // ��kt�y� kontrol et
    outFile = fopen(outputTest, "r");
    ASSERT_NE(outFile, nullptr);

    char outputBuffer[512] = { 0 };
    fread(outputBuffer, sizeof(char), 511, outFile);
    fclose(outFile);

    // Beklenen ��kt�: Tek bir SCC (0, 1, 2)
    EXPECT_NE(strstr(outputBuffer, "SCC #1: 0 1 2"), nullptr);

    // Belle�i temizle
    free(node1);
    free(node2);
    free(node3);
}

TEST_F(TaskAppTest, FindSCCs_InvalidParameters) {
    // Ge�ersiz giri� parametrelerini test et
    int V = -1; // Ge�ersiz d���m say�s�
    AdjacencyNode* adj[3] = { NULL };
    FILE* outFile = fopen(outputTest, "w");
    ASSERT_NE(outFile, nullptr);

    // Fonksiyonu �a��r
    int result = findSCCs(V, adj, outFile);

    // Fonksiyonun hata d�nd�rd���n� kontrol et
    EXPECT_EQ(result, -1);

    fclose(outFile);
}

/*TEST_F(TaskAppTest, AnalyzeSCC_ValidGraph) {
    Task taskList[3] = {
        {1, "Task 1", "Description 1", "Category 1", "2024-12-01", 1, {2}, 0},
        {2, "Task 2", "Description 2", "Category 2", "2024-12-02", 1, {3}, 0},
        {3, "Task 3", "Description 3", "Category 3", "2024-12-03", 1, {1}, 0}
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

    // Dinamik olarak do�ru SCC s�ralar�n� kontrol edin
    EXPECT_NE(strstr(outputBuffer, "SCC #1:"), nullptr);
    EXPECT_NE(strstr(outputBuffer, "SCC #2:"), nullptr);
}
*/

TEST_F(TaskAppTest, AnalyzeSCC_InvalidInput) {
    // Ge�ersiz parametrelerle fonksiyonu test et
    Task* taskList = NULL;
    int taskCount = 3;

    // ��k��� yakalamak i�in bir dosya a�
    FILE* outFile = fopen(outputTest, "w");
    ASSERT_NE(outFile, nullptr);

    // Ge�ersiz giri�lerle �a��r
    int result = analyzeSCC(taskList, taskCount, outFile);

    // D�n�� de�erinin hata oldu�unu do�rula
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

    // ��kt�y� do�rulay�n
    EXPECT_NE(strstr(outputBuffer, "SCC #1:"), nullptr);
    EXPECT_EQ(strstr(outputBuffer, "SCC #2:"), nullptr);
}

TEST_F(TaskAppTest, ComputePrefixTable_ValidPattern) {
    const char* pattern = "ababcab";
    int patternLength = strlen(pattern);
    int prefixTable[7] = { 0 };

    // Call the function
    int result = computePrefixTable(pattern, prefixTable, patternLength);

    // Verify the function executed successfully
    EXPECT_EQ(result, 1);

    // Expected prefix table for the given pattern
    int expectedPrefixTable[7] = { 0, 0, 1, 2, 0, 1, 2 };

    // Verify the computed prefix table
    for (int i = 0; i < patternLength; i++) {
        EXPECT_EQ(prefixTable[i], expectedPrefixTable[i]);
    }
}

TEST_F(TaskAppTest, ComputePrefixTable_EmptyPattern) {
    const char* pattern = "";
    int patternLength = strlen(pattern);
    int prefixTable[1] = { 0 };

    // Call the function
    int result = computePrefixTable(pattern, prefixTable, patternLength);

    // Verify the function returns an error for empty pattern
    EXPECT_EQ(result, -1);
}

TEST_F(TaskAppTest, ComputePrefixTable_InvalidParameters) {
    const char* pattern = NULL;
    int* prefixTable = NULL;
    int patternLength = 0;

    // Call the function with invalid parameters
    int result = computePrefixTable(pattern, prefixTable, patternLength);

    // Verify the function returns an error for invalid parameters
    EXPECT_EQ(result, -1);
}

TEST_F(TaskAppTest, ComputePrefixTable_RepeatedCharacters) {
    const char* pattern = "aaaa";
    int patternLength = strlen(pattern);
    int prefixTable[4] = { 0 };

    // Call the function
    int result = computePrefixTable(pattern, prefixTable, patternLength);

    // Verify the function executed successfully
    EXPECT_EQ(result, 1);

    // Expected prefix table for the given pattern
    int expectedPrefixTable[4] = { 0, 1, 2, 3 };

    // Verify the computed prefix table
    for (int i = 0; i < patternLength; i++) {
        EXPECT_EQ(prefixTable[i], expectedPrefixTable[i]);
    }
}

TEST_F(TaskAppTest, ComputePrefixTable_NoRepetition) {
    const char* pattern = "abcdef";
    int patternLength = strlen(pattern);
    int prefixTable[6] = { 0 };

    // Call the function
    int result = computePrefixTable(pattern, prefixTable, patternLength);

    // Verify the function executed successfully
    EXPECT_EQ(result, 1);

    // Expected prefix table for the given pattern
    int expectedPrefixTable[6] = { 0, 0, 0, 0, 0, 0 };

    // Verify the computed prefix table
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



//TEST_F(TaskAppTest, LoginUserMenu_SuccessfulLogin) {
//    const char* mockUserFile = "mock_users.bin";
//
//    // Kullan�c� giri�ini sim�le et
//    simulateUserInput("john@example.com\npassword123\n");
//
//    // Fonksiyonu �a��r
//    int result = loginUserMenu(mockUserFile);
//
//    // Do�ru sonucu kontrol et
//    EXPECT_EQ(result, 1);
//}

// Yanl�� e-posta ile giri� denendi�inde ba�ar�s�z d�nmesi beklenir
//TEST_F(TaskAppTest, LoginUserMenu_WrongEmail) {
//    const char* mockUserFile = "mock_users.bin";
//
//    // Yanl�� e-posta girdisini sim�le et
//    simulateUserInput("wrong@example.com\npassword123\n");
//
//    // Fonksiyonu �a��r
//    int result = loginUserMenu(mockUserFile);
//
//    // Yanl�� giri� oldu�u i�in 0 d�nmesi beklenir
//    EXPECT_EQ(result, 0);
//}
//
//// Yanl�� �ifre ile giri� denendi�inde ba�ar�s�z d�nmesi beklenir
//TEST_F(TaskAppTest, LoginUserMenu_WrongPassword) {
//    const char* mockUserFile = "mock_users.bin";
//
//    // Yanl�� �ifre girdisini sim�le et
//    simulateUserInput("john@example.com\nwrongpassword\n");
//
//    // Fonksiyonu �a��r
//    int result = loginUserMenu(mockUserFile);
//
//    // Yanl�� giri� oldu�u i�in 0 d�nmesi beklenir
//    EXPECT_EQ(result, 0);
//}
//
//// Eksik dosya ile giri� denendi�inde ba�ar�s�z d�nmesi beklenir
//TEST_F(TaskAppTest, LoginUserMenu_MissingFile) {
//    const char* invalidFile = "nonexistent_users.bin";
//
//    // Kullan�c� giri�ini sim�le et
//    simulateUserInput("john@example.com\npassword123\n");
//
//    // Fonksiyonu �a��r
//    int result = loginUserMenu(invalidFile);
//
//    // Dosya bulunmad��� i�in 0 d�nmesi beklenir
//    EXPECT_EQ(result, 0);
//}
//TEST_F(TaskAppTest, taskprenotaskfail) {
//    const char* testuserfile = "taskFile.bin";
//
//    simulateUserInput("4\n1\n\n3\n");
//
//    int result = userOptionsMenu();
//
//    EXPECT_EQ(result, 0);
//}


//TEST_F(TaskAppTest, LoadTasksToXORList_ValidFile) {
//    // Test i�in ge�ici bir g�rev dosyas� olu�tur
//    const char* testFilename = "test_tasks.bin";
//
//    // Test g�revlerini olu�tur ve dosyaya kaydet
//    Task testTasks[] = {
//        {1, "Task 1", "Description 1", "Category 1", "2024-11-05", 0, {2}, 1},
//        {2, "Task 2", "Description 2", "Category 2", "2024-11-10", 1, {1}, 1}
//    };
//    int taskCount = sizeof(testTasks) / sizeof(testTasks[0]);
//
//    FILE* file = fopen(testFilename, "wb");
//    ASSERT_NE(file, nullptr) << "Error: Unable to create test file.";
//    fwrite(testTasks, sizeof(Task), taskCount, file);
//    fclose(file);
//
//    // Fonksiyonu �a��r ve do�ru �al���p �al��mad���n� kontrol et
//    int result = loadTasksToXORList(testFilename);
//    EXPECT_EQ(result, 1);  // Ba�ar�yla tamamland���n� kontrol et
//
//    // XOR listesine eklenen g�revleri kontrol et
//    XORNode* current = xorHead;
//    for (int i = 0; i < taskCount; ++i) {
//        ASSERT_NE(current, nullptr) << "XOR list is missing nodes.";
//        EXPECT_EQ(current->task.id, testTasks[i].id);
//        EXPECT_STREQ(current->task.name, testTasks[i].name);
//        EXPECT_STREQ(current->task.description, testTasks[i].description);
//        EXPECT_STREQ(current->task.category, testTasks[i].category);
//        EXPECT_STREQ(current->task.dueDate, testTasks[i].dueDate);
//
//        // Bir sonraki d���me ge�
//        XORNode* nextNode = (XORNode*)((uintptr_t)current->xorPtr ^ (uintptr_t)nullptr);
//        current = nextNode;
//    }
//
//    // Test dosyas�n� sil
//    remove(testFilename);
//}
//
// TEST_F(TaskAppTest, LoadTasksToXORList_InvalidFile) {
//    // Ge�ersiz dosya ad�yla fonksiyonu �a��r
//    const char* invalidFilename = "nonexistent_tasks.bin";
//    int result = loadTasksToXORList(invalidFilename);
//
//    // D�n�� de�erinin -1 oldu�unu kontrol et
//    EXPECT_EQ(result, -1);
//
//    // ��kt�y� kontrol etmek i�in dosyadan oku
//    FILE* outputFile = fopen(outputTest, "rb");
//    ASSERT_NE(outputFile, nullptr);
//
//    char outputBuffer[256] = { 0 };
//    fread(outputBuffer, sizeof(char), 255, outputFile);
//    fclose(outputFile);
//
//    // Hata mesaj�n�n do�ru yaz�ld���n� kontrol et
//    EXPECT_NE(strstr(outputBuffer, "Error: Unable to open tasks file."), nullptr);
//}









int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::GTEST_FLAG(color) = "no";
    return RUN_ALL_TESTS();
}