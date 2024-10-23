//#define ENABLE_TASK_TEST  // Uncomment this line to enable the Task tests

#include "gtest/gtest.h"
#include "../../task/header/task.h"  // Adjust this include path based on your project structure


int main(int argc, char** argv) {
#ifdef ENABLE_TASK_TEST
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#else
	return 0;
#endif
}