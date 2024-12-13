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











int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::GTEST_FLAG(color) = "no";
    return RUN_ALL_TESTS();
}