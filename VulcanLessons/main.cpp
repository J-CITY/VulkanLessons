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

	float angle = 0.0f;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;
	
	while(!glfwWindowShouldClose(win)) {
		glfwPollEvents();

		float now = (float)glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		angle += 10.0f * deltaTime;
		if (angle > 360.0f) { angle -= 360.0f; }

		glm::mat4 firstModel(1.0f);
		glm::mat4 secondModel(1.0f);

		firstModel = glm::translate(firstModel, glm::vec3(0.0f, 0.0f, -2.5f));
		firstModel = glm::rotate(firstModel, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

		secondModel = glm::translate(secondModel, glm::vec3(0.0f, 0.0f, -3.0f));
		secondModel = glm::rotate(secondModel, glm::radians(-angle * 10), glm::vec3(0.0f, 0.0f, 1.0f));

		render->updateModel(0, firstModel);
		render->updateModel(1, secondModel);
		
		render->draw();
	}

	glfwDestroyWindow(win);
	glfwTerminate();
}
