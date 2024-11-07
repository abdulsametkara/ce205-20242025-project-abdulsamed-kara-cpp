#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <stdexcept>
#include "../../task/header/task.h"  // Adjust this include path based on your project structure
#include "../../taskapp/header/taskapp.h"
#include "task.h"
#include "../../task/src/task.cpp"

using namespace std;


/**
 * @brief Entry point of the application.
 *
 * This function initializes the application by specifying the path to the user database file
 * and calls the main menu function to start the program.
 *
 * @return int Returns 0 to indicate successful execution.
 */
int main() {
    const char* pathFileUsers = "users.bin";

    mainMenu(pathFileUsers);

    return 0;
}
