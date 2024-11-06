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





int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::GTEST_FLAG(color) = "no";
    return RUN_ALL_TESTS();
}