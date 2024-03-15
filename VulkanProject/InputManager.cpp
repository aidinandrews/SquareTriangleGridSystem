#include "InputManager.h"

float globalScale;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	
	globalScale += float(yoffset / 10);
}