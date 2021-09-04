#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

#include "render.h"

GLFWwindow* win = nullptr;
std::unique_ptr<VKRENDER::Render> render;

void init(std::string title="TEST", int w=800, int h=600) {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	win = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);

}

int main() {
	init();
	//unsigned extCount = 0;
	//vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
	//std::cout << extCount << std::endl;
	auto render = std::make_unique<VKRENDER::Render>(win);
	while(!glfwWindowShouldClose(win)) {
		glfwPollEvents();
		render->draw();
	}

	glfwDestroyWindow(win);
	glfwTerminate();
}
