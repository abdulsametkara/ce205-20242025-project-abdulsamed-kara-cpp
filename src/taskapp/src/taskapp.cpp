
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



int main() {
	const char* pathFileUsers = "users.bin";


	mainMenu(pathFileUsers);

	return 0;
}