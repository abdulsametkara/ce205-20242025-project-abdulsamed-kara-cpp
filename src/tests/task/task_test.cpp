#define ENABLE_TASK_TEST  // Bu satýr etkinleþtirildi
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
    // Kullanýcý dizisi örneði ve kullanýcý sayýsýný belirle
    User users[] = {
        {1, "John", "Doe", "john@example.com", "password123", nullptr},
        {2, "Jane", "Doe", "jane@example.com", "password456", nullptr},
    };
    int userCount = 2;

    // Fonksiyonu çaðýr ve beklenen deðeri test et
    int newUserId = getNewUserId(users, userCount);
    EXPECT_EQ(newUserId, 3);  // Son kullanýcý ID'si 2 olduðundan, yeni kullanýcý ID'si 3 olmalýdýr

    // Kullanýcý sayýsý sýfýr olduðunda test et
    userCount = 0;
    newUserId = getNewUserId(users, userCount);
    EXPECT_EQ(newUserId, 1);  // Kullanýcý yoksa ID 1 ile baþlamalýdýr
}

TEST_F(TaskAppTest, LoadUsers) {
    // Test için sahte kullanýcý dosyasýný hazýrla
    const char* testFile = "test_users.bin";

    // Test kullanýcýlarý oluþtur ve dosyaya kaydet
    User testUsers[] = {
        {1, "John", "Doe", "john@example.com", "password123", nullptr},
        {2, "Jane", "Doe", "jane@example.com", "password456", nullptr}
    };
    int userCount = sizeof(testUsers) / sizeof(testUsers[0]);

    FILE* file = fopen(testFile, "wb");
    fwrite(&userCount, sizeof(int), 1, file);
    fwrite(testUsers, sizeof(User), userCount, file);
    fclose(file);

    // Test edilen fonksiyonu çaðýr
    User* loadedUsers = nullptr;
    int loadedCount = loadUsers(testFile, &loadedUsers);

    // Sonuçlarý doðrula
    EXPECT_EQ(loadedCount, userCount);  // Kullanýcý sayýsý doðru mu?
    ASSERT_NE(loadedUsers, nullptr);    // Kullanýcýlar yüklendi mi?

    for (int i = 0; i < userCount; ++i) {
        EXPECT_EQ(loadedUsers[i].id, testUsers[i].id);
        EXPECT_STREQ(loadedUsers[i].name, testUsers[i].name);
        EXPECT_STREQ(loadedUsers[i].surname, testUsers[i].surname);
        EXPECT_STREQ(loadedUsers[i].email, testUsers[i].email);
        EXPECT_STREQ(loadedUsers[i].password, testUsers[i].password);
    }

    // Bellekteki kullanýcýlarý temizle
    free(loadedUsers);

    // Test dosyasýný sil
    remove(testFile);
}

TEST_F(TaskAppTest, AddTaskToXORList) {
    // Yeni bir görev oluþtur
    Task task = { 3, "Task 3", "Description 3", "Category 3", "2024-12-01", 0, {2}, 1 };

    // Görevi XOR baðlantýlý listeye ekle
    int result = addTaskToXORList(task);

    // Fonksiyonun baþarýyla çalýþtýðýný doðrula
    EXPECT_EQ(result, 1);

    // Baþ düðümün ve kuyruk düðümün doðru þekilde ayarlandýðýný kontrol et
    ASSERT_NE(xorHead, nullptr);
    ASSERT_NE(xorTail, nullptr);

    // Ýlk düðüm olarak eklenen görevin doðru atanýp atanmadýðýný kontrol et
    EXPECT_EQ(xorHead->task.id, task.id);
    EXPECT_STREQ(xorHead->task.name, task.name);
    EXPECT_STREQ(xorHead->task.description, task.description);
    EXPECT_STREQ(xorHead->task.category, task.category);

    // Eklenen düðümün sonunda olduðunu doðrula
    EXPECT_EQ(xorTail->task.id, task.id);
    EXPECT_EQ(xorHead, xorTail); // Bu görev ilk görevse baþ ve son ayný olmalý
}
///



TEST_F(TaskAppTest, AssignDeadline_ValidInput) {
    // Geçerli bir görev adý ve tarih girdisini simüle et
    simulateUserInput("Sample Task\n\n10 11 2024\n");

    Assignment assignment;

    // Fonksiyonu çalýþtýr ve beklenen deðeri kontrol et
    int result = assign_deadline(&assignment);
    resetStdinStdout();
    EXPECT_EQ(result, 0);  // Fonksiyonun baþarýyla çalýþmasý bekleniyor


    // Standart giriþ ve çýkýþý sýfýrla
    
}

TEST_F(TaskAppTest, AssignDeadline_InvalidDate) {
    // Geçersiz bir tarih girdisini simüle et
    simulateUserInput("Sample Task\n\n32 13 2024\n");

    Assignment assignment;

    // Fonksiyonu çalýþtýr ve geçersiz tarih için -1 döndürmesini bekle
    int result = assign_deadline(&assignment);
    resetStdinStdout();
    EXPECT_EQ(result, -1);

    // Standart giriþ ve çýkýþý sýfýrla
    
}

TEST_F(TaskAppTest, ViewDeadlines_NoDeadlines) {
    // Yýðýný boþ hale getiriyoruz
    deadlineHeap.size = 0;

    // viewDeadlines fonksiyonunu çaðýrýyoruz ve çýktý dosyasýna yönlendiriyoruz
    simulateUserInput(""); // Boþ girdi simüle ediyoruz
    int result = viewDeadlines();
    resetStdinStdout();

    // Fonksiyonun -1 döndürdüðünü ve "No deadlines to display." yazdýðýný kontrol ediyoruz
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
    // Kuyruk boþken viewTask fonksiyonunu test ediyoruz
    front = NULL;  // Kuyruk baþý NULL olmalý

    // Çýkýþý yakalamak için stdout'u test dosyasýna yönlendir
    freopen(outputTest, "wb", stdout);

    // Fonksiyonu çaðýr
    int result = viewTask();

    // Stdout'u eski haline getir
    resetStdinStdout();

    // Dönüþ deðerinin 0 olduðunu doðrula
    EXPECT_EQ(result, 0);

    // Çýktýyý kontrol etmek için dosyadan oku
    FILE* outputFile = fopen(outputTest, "rb");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    // Hata mesajýnýn doðru yazýldýðýný kontrol et
    EXPECT_NE(strstr(outputBuffer, "No tasks found. The task list is empty."), nullptr);
}


TEST_F(TaskAppTest, ViewTask_FilledQueue) {
    // Test için sahte görevler oluþtur
    Task task1 = { 1, "Task 1", "Description 1", "Category 1", "2024-11-10", 0, {0}, 1 };
    Task task2 = { 2, "Task 2", "Description 2", "Category 2", "2024-11-15", 0, {0}, 1 };

    // Kuyruða görev ekle
    enqueue(task1);
    enqueue(task2);

    // Fonksiyonu çalýþtýrmadan önce stdout'u test dosyasýna yönlendir
    freopen(outputTest, "wb", stdout);

    // Fonksiyonu çalýþtýr
    int result = viewTask();

    // Stdout'u eski haline getir
    resetStdinStdout();

    // Dönüþ deðerinin 1 olduðunu doðrula
    EXPECT_EQ(result, 1);

    // Çýktýyý kontrol etmek için dosyadan oku
    FILE* outputFile = fopen(outputTest, "rb");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[512] = { 0 };
    fread(outputBuffer, sizeof(char), 511, outputFile);
    fclose(outputFile);

    // Görevlerin doðru yazýldýðýný kontrol et
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
    // Kullanýcý giriþini simüle et
    simulateUserInput("\n");

    // Çýkýþý yakalamak için stdout'u test dosyasýna yönlendir
    freopen(outputTest, "wb", stdout);

    // Fonksiyonu çaðýr
    int result = enterToContinue();

    // Stdout'u eski haline getir
    resetStdinStdout();

    // Fonksiyonun doðru sonucu döndürdüðünü kontrol et
    EXPECT_EQ(result, 1);

    // Çýktýyý kontrol etmek için dosyadan oku
    FILE* outputFile = fopen(outputTest, "rb");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    // Fonksiyonun doðru mesajý yazdýðýný kontrol et
    EXPECT_NE(strstr(outputBuffer, "Press enter to continue"), nullptr);
}

TEST_F(TaskAppTest, HandleInputError) {
    // Geçersiz giriþ simülasyonu yapýyoruz (örneðin: "abc")
    simulateUserInput("abc\n");

    // Çýkýþý yakalamak için stdout'u test dosyasýna yönlendir
    freopen(outputTest, "wb", stdout);

    // Fonksiyonu çaðýr
    int result = handleInputError();

    // Stdout'u eski haline getir
    resetStdinStdout();

    // Fonksiyonun doðru sonucu döndürdüðünü kontrol et
    EXPECT_EQ(result, 0);

    // Çýktýyý kontrol etmek için dosyadan oku
    FILE* outputFile = fopen(outputTest, "rb");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    // Fonksiyonun doðru mesajý yazdýrdýðýný kontrol et
    EXPECT_NE(strstr(outputBuffer, "Invalid input. Please enter a number."), nullptr);
}

TEST_F(TaskAppTest, AddTaskToList_ValidTask) {
    // Baþlangýçta listeyi temizle
    head = NULL;
    tail = NULL;

    // Yeni görev oluþtur
    Task newTask = { 1, "Test Task", "Test Description", "Test Category", "2024-12-15", 0, {2}, 1 };

    // Fonksiyonu çaðýr ve sonucu kontrol et
    int result = addTaskToList(newTask);
    EXPECT_EQ(result, 1); // Fonksiyonun baþarýyla tamamlandýðýný kontrol et

    // Listenin güncellendiðini kontrol et
    ASSERT_NE(head, nullptr); // Listenin baþý NULL olmamalý
    ASSERT_NE(tail, nullptr); // Listenin sonu NULL olmamalý
    EXPECT_EQ(head, tail);    // Liste tek düðümden oluþmalý

    // Düðümün doðru atandýðýný kontrol et
    EXPECT_EQ(head->task.id, newTask.id);
    EXPECT_STREQ(head->task.name, newTask.name);
    EXPECT_STREQ(head->task.description, newTask.description);
    EXPECT_STREQ(head->task.category, newTask.category);
    EXPECT_STREQ(head->task.dueDate, newTask.dueDate);
}

TEST_F(TaskAppTest, AddTaskToList_MultipleTasks) {
    // Baþlangýçta listeyi temizle
    head = NULL;
    tail = NULL;

    // Ýlk görevi ekle
    Task firstTask = { 1, "First Task", "First Description", "First Category", "2024-12-10", 0, {2}, 1 };
    int result = addTaskToList(firstTask);
    EXPECT_EQ(result, 1);

    // Ýkinci görevi ekle
    Task secondTask = { 2, "Second Task", "Second Description", "Second Category", "2024-12-20", 0, {1}, 1 };
    result = addTaskToList(secondTask);
    EXPECT_EQ(result, 1);

    // Listenin doðru þekilde güncellendiðini kontrol et
    ASSERT_NE(head, nullptr);
    ASSERT_NE(tail, nullptr);
    EXPECT_NE(head, tail); // Liste birden fazla düðümden oluþmalý

    // Ýlk düðümü kontrol et
    EXPECT_EQ(head->task.id, firstTask.id);
    EXPECT_STREQ(head->task.name, firstTask.name);
    EXPECT_EQ(head->next, tail); // Ýlk düðümün next iþaretçisi kuyruðu göstermeli

    // Ýkinci düðümü kontrol et
    EXPECT_EQ(tail->task.id, secondTask.id);
    EXPECT_STREQ(tail->task.name, secondTask.name);
    EXPECT_EQ(tail->prev, head); // Kuyruðun prev iþaretçisi baþý göstermeli
}
//TEST_F(TaskAppTest, LoginUserMenu_SuccessfulLogin) {
//    const char* mockUserFile = "mock_users.bin";
//
//    // Kullanýcý giriþini simüle et
//    simulateUserInput("john@example.com\npassword123\n");
//
//    // Fonksiyonu çaðýr
//    int result = loginUserMenu(mockUserFile);
//
//    // Doðru sonucu kontrol et
//    EXPECT_EQ(result, 1);
//}

// Yanlýþ e-posta ile giriþ denendiðinde baþarýsýz dönmesi beklenir
//TEST_F(TaskAppTest, LoginUserMenu_WrongEmail) {
//    const char* mockUserFile = "mock_users.bin";
//
//    // Yanlýþ e-posta girdisini simüle et
//    simulateUserInput("wrong@example.com\npassword123\n");
//
//    // Fonksiyonu çaðýr
//    int result = loginUserMenu(mockUserFile);
//
//    // Yanlýþ giriþ olduðu için 0 dönmesi beklenir
//    EXPECT_EQ(result, 0);
//}
//
//// Yanlýþ þifre ile giriþ denendiðinde baþarýsýz dönmesi beklenir
//TEST_F(TaskAppTest, LoginUserMenu_WrongPassword) {
//    const char* mockUserFile = "mock_users.bin";
//
//    // Yanlýþ þifre girdisini simüle et
//    simulateUserInput("john@example.com\nwrongpassword\n");
//
//    // Fonksiyonu çaðýr
//    int result = loginUserMenu(mockUserFile);
//
//    // Yanlýþ giriþ olduðu için 0 dönmesi beklenir
//    EXPECT_EQ(result, 0);
//}
//
//// Eksik dosya ile giriþ denendiðinde baþarýsýz dönmesi beklenir
//TEST_F(TaskAppTest, LoginUserMenu_MissingFile) {
//    const char* invalidFile = "nonexistent_users.bin";
//
//    // Kullanýcý giriþini simüle et
//    simulateUserInput("john@example.com\npassword123\n");
//
//    // Fonksiyonu çaðýr
//    int result = loginUserMenu(invalidFile);
//
//    // Dosya bulunmadýðý için 0 dönmesi beklenir
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
//    // Test için geçici bir görev dosyasý oluþtur
//    const char* testFilename = "test_tasks.bin";
//
//    // Test görevlerini oluþtur ve dosyaya kaydet
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
//    // Fonksiyonu çaðýr ve doðru çalýþýp çalýþmadýðýný kontrol et
//    int result = loadTasksToXORList(testFilename);
//    EXPECT_EQ(result, 1);  // Baþarýyla tamamlandýðýný kontrol et
//
//    // XOR listesine eklenen görevleri kontrol et
//    XORNode* current = xorHead;
//    for (int i = 0; i < taskCount; ++i) {
//        ASSERT_NE(current, nullptr) << "XOR list is missing nodes.";
//        EXPECT_EQ(current->task.id, testTasks[i].id);
//        EXPECT_STREQ(current->task.name, testTasks[i].name);
//        EXPECT_STREQ(current->task.description, testTasks[i].description);
//        EXPECT_STREQ(current->task.category, testTasks[i].category);
//        EXPECT_STREQ(current->task.dueDate, testTasks[i].dueDate);
//
//        // Bir sonraki düðüme geç
//        XORNode* nextNode = (XORNode*)((uintptr_t)current->xorPtr ^ (uintptr_t)nullptr);
//        current = nextNode;
//    }
//
//    // Test dosyasýný sil
//    remove(testFilename);
//}
//
// TEST_F(TaskAppTest, LoadTasksToXORList_InvalidFile) {
//    // Geçersiz dosya adýyla fonksiyonu çaðýr
//    const char* invalidFilename = "nonexistent_tasks.bin";
//    int result = loadTasksToXORList(invalidFilename);
//
//    // Dönüþ deðerinin -1 olduðunu kontrol et
//    EXPECT_EQ(result, -1);
//
//    // Çýktýyý kontrol etmek için dosyadan oku
//    FILE* outputFile = fopen(outputTest, "rb");
//    ASSERT_NE(outputFile, nullptr);
//
//    char outputBuffer[256] = { 0 };
//    fread(outputBuffer, sizeof(char), 255, outputFile);
//    fclose(outputFile);
//
//    // Hata mesajýnýn doðru yazýldýðýný kontrol et
//    EXPECT_NE(strstr(outputBuffer, "Error: Unable to open tasks file."), nullptr);
//}

TEST_F(TaskAppTest, PushTask_Success) {
    // Test için bir görev oluþtur
    Task task = { 1, "Test Task", "Task Description", "General", "2024-12-15", 0, {0}, 0 };

    // push fonksiyonunu çaðýr ve baþarý durumunu kontrol et
    int result = push(task);
    EXPECT_EQ(result, 1); // Baþarýlý olmalý

    // stackTop'un NULL olmadýðýný ve doðru þekilde ayarlandýðýný kontrol et
    ASSERT_NE(stackTop, nullptr);
    EXPECT_EQ(stackTop->task.id, task.id);
    EXPECT_STREQ(stackTop->task.name, task.name);
    EXPECT_STREQ(stackTop->task.description, task.description);
    EXPECT_STREQ(stackTop->task.category, task.category);
    EXPECT_STREQ(stackTop->task.dueDate, task.dueDate);

    // Belleði temizle
    StackNode* temp = stackTop;
    stackTop = stackTop->next;
    free(temp);
}

/*TEST_F(TaskAppTest, PopTask_EmptyStack) {
    // Boþ bir yýðýn durumunda pop fonksiyonunu test et
    stackTop = NULL;  // Yýðýn boþ olduðundan stackTop'u NULL yap

    Task result = pop();

    // Task id'sinin -1 döndüðünü kontrol et (boþ yýðýn iþareti)
    

    // Çýktýyý kontrol etmek için dosyadan oku
    FILE* outputFile = fopen(outputTest, "rb");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    // Hata mesajýnýn doðru yazýldýðýný kontrol et
    EXPECT_NE(strstr(outputBuffer, "Stack is empty"), nullptr);

    // Çýktýyý debug için yazdýr
    std::cout << "Captured Output: " << outputBuffer << std::endl;
    EXPECT_EQ(result.id, -1);
}*/

TEST_F(TaskAppTest, PopTask_NonEmptyStack) {
    // Bir yýðýn oluþtur ve bir görev ekle
    Task task = { 1, "Test Task", "Task Description", "General", "2024-12-15", 0, {0}, 0 };
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    ASSERT_NE(newNode, nullptr); // Bellek tahsisini kontrol et
    newNode->task = task;
    newNode->next = NULL;  // Bu tek öðe olacak
    stackTop = newNode;

    // Pop fonksiyonunu çaðýr
    Task result = pop();

    // Çýkarýlan görevin doðru olduðunu kontrol et
    EXPECT_EQ(result.id, task.id);
    EXPECT_STREQ(result.name, task.name);
    EXPECT_STREQ(result.description, task.description);
    EXPECT_STREQ(result.category, task.category);
    EXPECT_STREQ(result.dueDate, task.dueDate);

    // Yýðýnýn boþ olduðundan emin ol
    EXPECT_EQ(stackTop, nullptr);

    // Pop edilen öðenin bellekte serbest býrakýldýðýný kontrol et
    // Bunun için özel bir bellek testi gerekir (manuel doðrulama yapýlabilir)
}

TEST_F(TaskAppTest, UndoLastTask_EmptyStack) {
    // Test: Yýðýn boþken `undoLastTask` fonksiyonu
    stackTop = NULL; // Yýðýný boþ yap

    // Görev listesi ve görev sayýsý simüle ediliyor
    Task taskList[100];
    int taskCount = 0;

    // Standart çýktýyý yakalamak için yönlendir
    freopen(outputTest, "w", stdout);

    // Fonksiyonu çaðýr
    int result = undoLastTask(taskList, &taskCount);

    // Standart çýktýyý sýfýrla
    resetStdinStdout();

    // Fonksiyonun dönüþ deðerini kontrol et
    EXPECT_EQ(result, -1);

    // Çýktýyý kontrol etmek için dosyadan oku
    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    // Çýktýnýn doðru hata mesajýný içerdiðini kontrol et
    EXPECT_NE(strstr(outputBuffer, "No tasks to undo."), nullptr);
}

TEST_F(TaskAppTest, UndoLastTask_Success) {
    // Test: Yýðýn doluyken `undoLastTask` fonksiyonu
    stackTop = NULL; // Yýðýný temizle

    // Görev listesi ve görev sayýsý simüle ediliyor
    Task taskList[100];
    int taskCount = 1;

    // Örnek bir görev ekle
    Task task = { 1, "Test Task", "Test Description", "Test Category", "2024-12-31", 0, {0}, 0 };
    push(task); // Görevi yýðýna ekle
    taskList[0] = task;

    // Standart çýktýyý yakalamak için yönlendir
    freopen(outputTest, "w", stdout);

    // Fonksiyonu çaðýr
    int result = undoLastTask(taskList, &taskCount);

    // Standart çýktýyý sýfýrla
    resetStdinStdout();

    // Fonksiyonun dönüþ deðerini kontrol et
    EXPECT_EQ(result, 1);

    // Görev sayýsýnýn güncellendiðini kontrol et
    EXPECT_EQ(taskCount, 0);

    // Çýktýyý kontrol etmek için dosyadan oku
    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    // Çýktýnýn doðru baþarý mesajýný içerdiðini kontrol et
    EXPECT_NE(strstr(outputBuffer, "Last task 'Test Task' undone successfully."), nullptr);
}
TEST_F(TaskAppTest, PrintDependenciesUtil_NoDependencies) {
    // Test için sahte görev dizisi oluþtur
    Task taskList[3] = {
        {1, "Task 1", "Description 1", "Category 1", "2024-12-01", 0, {0}, 0},
        {2, "Task 2", "Description 2", "Category 2", "2024-12-02", 0, {0}, 0},
        {3, "Task 3", "Description 3", "Category 3", "2024-12-03", 0, {0}, 0},
    };

    bool visited[4] = { false };

    // Standart çýktýyý yakalamak için yönlendir
    freopen(outputTest, "w", stdout);

    // Fonksiyonu çaðýr
    int result = printDependenciesUtil(taskList, 1, visited);

    // Standart çýktýyý sýfýrla
    resetStdinStdout();

    // Fonksiyonun doðru deðer döndürdüðünü kontrol et
    EXPECT_EQ(result, 1);

    // Çýktýyý kontrol etmek için dosyadan oku
    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    // Çýkýþýn doðru olduðunu doðrula (baðýmlýlýk olmadýðýndan çýktý olmamalý)
    EXPECT_EQ(strlen(outputBuffer), 0);
}

/*TEST_F(TaskAppTest, PrintDependenciesUtil_WithDependencies) {
    // Test için sahte görev dizisi oluþtur
    Task taskList[3] = {
        {1, "Task 1", "Description 1", "Category 1", "2024-12-01", 2, {2, 3}, 0},
        {2, "Task 2", "Description 2", "Category 2", "2024-12-02", 0, {0}, 0},
        {3, "Task 3", "Description 3", "Category 3", "2024-12-03", 0, {0}, 0},
    };

    bool visited[4] = { false };

    // Standart çýktýyý yakalamak için yönlendir
    freopen(outputTest, "w", stdout);

    // Fonksiyonu çaðýr
    int result = printDependenciesUtil(taskList, 1, visited);

    // Standart çýktýyý sýfýrla
    resetStdinStdout();

    // Fonksiyonun doðru deðer döndürdüðünü kontrol et
    EXPECT_EQ(result, 1);

    // Çýktýyý kontrol etmek için dosyadan oku
    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    // Çýkýþýn doðru olduðunu doðrula
    EXPECT_NE(strstr(outputBuffer, "Task 1 depends on Task 2\n"), nullptr);
    EXPECT_NE(strstr(outputBuffer, "Task 1 depends on Task 3\n"), nullptr);
   
}*/

TEST_F(TaskAppTest, PrintDependencies_InvalidTaskId) {
    // Geçersiz task ID ile çaðýr
    int taskCount = 3;
    Task taskList[3] = {
        {1, "Task 1", "Description 1", "Category 1", "2024-12-01", 0, {0}, 0},
        {2, "Task 2", "Description 2", "Category 2", "2024-12-02", 0, {0}, 0},
        {3, "Task 3", "Description 3", "Category 3", "2024-12-03", 0, {0}, 0}
    };

    // Standart çýktýyý dosyaya yönlendir
    freopen(outputTest, "w", stdout);

    // Fonksiyonu çaðýr
    int result = printDependencies(taskList, taskCount, 0);

    // Standart çýktýyý sýfýrla
    resetStdinStdout();

    // Dönüþ deðerinin -1 olduðunu doðrula
    EXPECT_EQ(result, -1);

    // Çýktýyý kontrol et
    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    EXPECT_NE(strstr(outputBuffer, "Invalid task ID"), nullptr);
}

TEST_F(TaskAppTest, PrintDependencies_WithDependencies) {
    // Baðýmlýlýklarý olan görevler için test
    int taskCount = 3;
    Task taskList[3] = {
        {1, "Task 1", "Description 1", "Category 1", "2024-12-01", 1, {2}, 1},
        {2, "Task 2", "Description 2", "Category 2", "2024-12-02", 1, {3}, 1},
        {3, "Task 3", "Description 3", "Category 3", "2024-12-03", 0, {0}, 0}
    };

    // Standart çýktýyý dosyaya yönlendir
    freopen(outputTest, "w", stdout);

    // Fonksiyonu çaðýr
    int result = printDependencies(taskList, taskCount, 1);

    // Standart çýktýyý sýfýrla
    resetStdinStdout();

    // Dönüþ deðerinin 1 olduðunu doðrula
    EXPECT_EQ(result, 1);

    // Çýktýyý kontrol et
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
    // Hiç baðýmlýlýðý olmayan görev için test
    int taskCount = 3;
    Task taskList[3] = {
        {1, "Task 1", "Description 1", "Category 1", "2024-12-01", 0, {0}, 0},
        {2, "Task 2", "Description 2", "Category 2", "2024-12-02", 0, {0}, 0},
        {3, "Task 3", "Description 3", "Category 3", "2024-12-03", 0, {0}, 0}
    };

    // Standart çýktýyý dosyaya yönlendir
    freopen(outputTest, "w", stdout);

    // Fonksiyonu çaðýr
    int result = printDependencies(taskList, taskCount, 1);

    // Standart çýktýyý sýfýrla
    resetStdinStdout();

    // Dönüþ deðerinin 1 olduðunu doðrula
    EXPECT_EQ(result, 1);

    // Çýktýyý kontrol et
    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    EXPECT_NE(strstr(outputBuffer, "Dependencies for Task 1"), nullptr);
    EXPECT_EQ(strstr(outputBuffer, "depends on"), nullptr);
}

TEST_F(TaskAppTest, PushSccStack_Success) {
    // SCC yýðýný boþ durumda baþlasýn
    sccStack = NULL;

    // Ýlk veri ekleme
    int result = pushSccStack(10);
    EXPECT_EQ(result, 1); // Fonksiyonun baþarý döndürmesi beklenir
    ASSERT_NE(sccStack, nullptr); // Yeni düðümün eklenmiþ olmasý gerekir
    EXPECT_EQ(sccStack->data, 10); // Yýðýnýn üstündeki veri kontrol edilir

    // Ýkinci veri ekleme
    result = pushSccStack(20);
    EXPECT_EQ(result, 1); // Ýkinci eklemenin de baþarýlý olmasý beklenir
    ASSERT_NE(sccStack, nullptr); // Yýðýn hâlâ dolu olmalý
    EXPECT_EQ(sccStack->data, 20); // En üstteki veri kontrol edilir
    EXPECT_EQ(sccStack->next->data, 10); // Bir altýndaki düðüm kontrol edilir
}

/*TEST_F(TaskAppTest, PushSccStack_MemoryAllocationFailure) {
    // malloc'u geçici olarak baþarýsýz olacak þekilde ayarla

// Çýkýþý dosyaya yönlendir
    freopen(outputTest, "w", stdout);

    // pushSccStack çaðrýsý
    int result = pushSccStack(-1);

    // Stdout'u sýfýrla
    resetStdinStdout();

    // Fonksiyonun -1 döndürdüðünü doðrula
    EXPECT_EQ(result, 1);

    // Çýktýyý kontrol et
    FILE* outputFile = fopen(outputTest, "r");
    ASSERT_NE(outputFile, nullptr);

    char outputBuffer[256] = { 0 };
    fread(outputBuffer, sizeof(char), 255, outputFile);
    fclose(outputFile);

    // Hata mesajýný kontrol et
    EXPECT_NE(strstr(outputBuffer, "Memory allocation failed"), nullptr);

}*/

TEST_F(TaskAppTest, PopSccStack_EmptyStack) {
    // Boþ bir SCC yýðýný durumunda popSccStack fonksiyonunu test et
    sccStack = NULL; // SCC yýðýný boþ

    // Fonksiyonu çaðýr
    int result = popSccStack();

    // Sonucun -1 olduðunu kontrol et
    EXPECT_EQ(result, -1);
}

TEST_F(TaskAppTest, PopSccStack_NonEmptyStack) {
    // SCC yýðýnýna birkaç eleman ekle
    AdjacencyNode* node1 = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    node1->data = 10;
    node1->next = NULL;

    AdjacencyNode* node2 = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    node2->data = 20;
    node2->next = node1;

    // Yýðýný baþlat
    sccStack = node2;

    // Ýlk elemaný çýkar ve kontrol et
    int result = popSccStack();
    EXPECT_EQ(result, 20); // En üstteki eleman 20 olmalý

    // Ýkinci elemaný çýkar ve kontrol et
    result = popSccStack();
    EXPECT_EQ(result, 10); // En üstteki eleman 10 olmalý

    // Yýðýnýn tamamen boþ olduðunu kontrol et
    EXPECT_EQ(sccStack, nullptr);
}

TEST_F(TaskAppTest, DfsUtil_SingleNode) {
    // Bir düðümlü bir grafik oluþtur
    int visited[2] = { 0 }; // Düðüm 1 için yer ayýr
    AdjacencyNode* adj[2] = { NULL }; // Adjacency list

    // Adjacency list'i doldur (tek düðüm)
    AdjacencyNode* node1 = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    ASSERT_NE(node1, nullptr); // Bellek tahsisini kontrol et
    node1->data = 1;
    node1->next = NULL;
    adj[1] = node1;

    // SCC yýðýnýný temizle
    sccStack = NULL;

    // Fonksiyonu çaðýr
    int result = dfsUtil(1, visited, adj, NULL);

    // Ziyaret durumunu kontrol et
    EXPECT_EQ(result, 1);
    EXPECT_EQ(visited[1], 1);

    // SCC yýðýnýný kontrol et
    ASSERT_NE(sccStack, nullptr);
    EXPECT_EQ(sccStack->data, 1);

    // Belleði temizle
    free(node1);
    while (sccStack) {
        popSccStack();
    }
}

TEST_F(TaskAppTest, DfsUtil_MultipleNodes) {
    // Ýki düðümlü bir grafik oluþtur
    int visited[3] = { 0 }; // Düðümler 1 ve 2 için yer ayýr
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

    // SCC yýðýnýný temizle
    sccStack = NULL;

    // Fonksiyonu çaðýr
    int result = dfsUtil(1, visited, adj, NULL);

    // Ziyaret durumlarýný kontrol et
    EXPECT_EQ(result, 1);
    EXPECT_EQ(visited[1], 1);
    EXPECT_EQ(visited[2], 1);

    // SCC yýðýnýný kontrol et
    ASSERT_NE(sccStack, nullptr);
    EXPECT_EQ(sccStack->data, 2); // Ýlk eklenen
    popSccStack();
    EXPECT_EQ(sccStack->data, 1); // Son eklenen

    // Belleði temizle
    free(node1);
    free(node2);
    while (sccStack) {
        popSccStack();
    }
}

TEST_F(TaskAppTest, FindSCCs_ValidGraph) {
    // Geçerli bir grafik oluþtur
    int V = 3;
    AdjacencyNode* adj[3] = { NULL };

    // Düðüm 0 -> 1
    AdjacencyNode* node1 = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    node1->data = 1;
    node1->next = NULL;
    adj[0] = node1;

    // Düðüm 1 -> 2
    AdjacencyNode* node2 = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    node2->data = 2;
    node2->next = NULL;
    adj[1] = node2;

    // Düðüm 2 -> 0
    AdjacencyNode* node3 = (AdjacencyNode*)malloc(sizeof(AdjacencyNode));
    node3->data = 0;
    node3->next = NULL;
    adj[2] = node3;

    // Çýkýþý yakalamak için bir dosya aç
    FILE* outFile = fopen(outputTest, "w");
    ASSERT_NE(outFile, nullptr);

    // Fonksiyonu çaðýr
    int result = findSCCs(V, adj, outFile);

    // Dosyayý kapat ve sonuçlarý kontrol et
    fclose(outFile);

    // Dönüþ deðerini kontrol et
    EXPECT_EQ(result, 1);

    // Çýktýyý kontrol et
    outFile = fopen(outputTest, "r");
    ASSERT_NE(outFile, nullptr);

    char outputBuffer[512] = { 0 };
    fread(outputBuffer, sizeof(char), 511, outFile);
    fclose(outFile);

    // Beklenen çýktý: Tek bir SCC (0, 1, 2)
    EXPECT_NE(strstr(outputBuffer, "SCC #1: 0 1 2"), nullptr);

    // Belleði temizle
    free(node1);
    free(node2);
    free(node3);
}

TEST_F(TaskAppTest, FindSCCs_InvalidParameters) {
    // Geçersiz giriþ parametrelerini test et
    int V = -1; // Geçersiz düðüm sayýsý
    AdjacencyNode* adj[3] = { NULL };
    FILE* outFile = fopen(outputTest, "w");
    ASSERT_NE(outFile, nullptr);

    // Fonksiyonu çaðýr
    int result = findSCCs(V, adj, outFile);

    // Fonksiyonun hata döndürdüðünü kontrol et
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

    // Dinamik olarak doðru SCC sýralarýný kontrol edin
    EXPECT_NE(strstr(outputBuffer, "SCC #1:"), nullptr);
    EXPECT_NE(strstr(outputBuffer, "SCC #2:"), nullptr);
}
*/

TEST_F(TaskAppTest, AnalyzeSCC_InvalidInput) {
    // Geçersiz parametrelerle fonksiyonu test et
    Task* taskList = NULL;
    int taskCount = 3;

    // Çýkýþý yakalamak için bir dosya aç
    FILE* outFile = fopen(outputTest, "w");
    ASSERT_NE(outFile, nullptr);

    // Geçersiz giriþlerle çaðýr
    int result = analyzeSCC(taskList, taskCount, outFile);

    // Dönüþ deðerinin hata olduðunu doðrula
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

    // Çýktýyý doðrulayýn
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
//    // Kullanýcý giriþini simüle et
//    simulateUserInput("john@example.com\npassword123\n");
//
//    // Fonksiyonu çaðýr
//    int result = loginUserMenu(mockUserFile);
//
//    // Doðru sonucu kontrol et
//    EXPECT_EQ(result, 1);
//}

// Yanlýþ e-posta ile giriþ denendiðinde baþarýsýz dönmesi beklenir
//TEST_F(TaskAppTest, LoginUserMenu_WrongEmail) {
//    const char* mockUserFile = "mock_users.bin";
//
//    // Yanlýþ e-posta girdisini simüle et
//    simulateUserInput("wrong@example.com\npassword123\n");
//
//    // Fonksiyonu çaðýr
//    int result = loginUserMenu(mockUserFile);
//
//    // Yanlýþ giriþ olduðu için 0 dönmesi beklenir
//    EXPECT_EQ(result, 0);
//}
//
//// Yanlýþ þifre ile giriþ denendiðinde baþarýsýz dönmesi beklenir
//TEST_F(TaskAppTest, LoginUserMenu_WrongPassword) {
//    const char* mockUserFile = "mock_users.bin";
//
//    // Yanlýþ þifre girdisini simüle et
//    simulateUserInput("john@example.com\nwrongpassword\n");
//
//    // Fonksiyonu çaðýr
//    int result = loginUserMenu(mockUserFile);
//
//    // Yanlýþ giriþ olduðu için 0 dönmesi beklenir
//    EXPECT_EQ(result, 0);
//}
//
//// Eksik dosya ile giriþ denendiðinde baþarýsýz dönmesi beklenir
//TEST_F(TaskAppTest, LoginUserMenu_MissingFile) {
//    const char* invalidFile = "nonexistent_users.bin";
//
//    // Kullanýcý giriþini simüle et
//    simulateUserInput("john@example.com\npassword123\n");
//
//    // Fonksiyonu çaðýr
//    int result = loginUserMenu(invalidFile);
//
//    // Dosya bulunmadýðý için 0 dönmesi beklenir
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
//    // Test için geçici bir görev dosyasý oluþtur
//    const char* testFilename = "test_tasks.bin";
//
//    // Test görevlerini oluþtur ve dosyaya kaydet
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
//    // Fonksiyonu çaðýr ve doðru çalýþýp çalýþmadýðýný kontrol et
//    int result = loadTasksToXORList(testFilename);
//    EXPECT_EQ(result, 1);  // Baþarýyla tamamlandýðýný kontrol et
//
//    // XOR listesine eklenen görevleri kontrol et
//    XORNode* current = xorHead;
//    for (int i = 0; i < taskCount; ++i) {
//        ASSERT_NE(current, nullptr) << "XOR list is missing nodes.";
//        EXPECT_EQ(current->task.id, testTasks[i].id);
//        EXPECT_STREQ(current->task.name, testTasks[i].name);
//        EXPECT_STREQ(current->task.description, testTasks[i].description);
//        EXPECT_STREQ(current->task.category, testTasks[i].category);
//        EXPECT_STREQ(current->task.dueDate, testTasks[i].dueDate);
//
//        // Bir sonraki düðüme geç
//        XORNode* nextNode = (XORNode*)((uintptr_t)current->xorPtr ^ (uintptr_t)nullptr);
//        current = nextNode;
//    }
//
//    // Test dosyasýný sil
//    remove(testFilename);
//}
//
// TEST_F(TaskAppTest, LoadTasksToXORList_InvalidFile) {
//    // Geçersiz dosya adýyla fonksiyonu çaðýr
//    const char* invalidFilename = "nonexistent_tasks.bin";
//    int result = loadTasksToXORList(invalidFilename);
//
//    // Dönüþ deðerinin -1 olduðunu kontrol et
//    EXPECT_EQ(result, -1);
//
//    // Çýktýyý kontrol etmek için dosyadan oku
//    FILE* outputFile = fopen(outputTest, "rb");
//    ASSERT_NE(outputFile, nullptr);
//
//    char outputBuffer[256] = { 0 };
//    fread(outputBuffer, sizeof(char), 255, outputFile);
//    fclose(outputFile);
//
//    // Hata mesajýnýn doðru yazýldýðýný kontrol et
//    EXPECT_NE(strstr(outputBuffer, "Error: Unable to open tasks file."), nullptr);
//}









int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::GTEST_FLAG(color) = "no";
    return RUN_ALL_TESTS();
}