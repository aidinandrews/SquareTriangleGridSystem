#include <iostream>
#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#define TINY_OBJ_LOADED
#include "tiny_obj_loader.h"

#include "engine.h"
#include "Test.h"

int main() {
	//testing();
	//return 0;

	Engine app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

//#include "tutorialCode.h"