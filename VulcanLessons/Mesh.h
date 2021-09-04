#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>


struct Vertex {
	glm::vec3 pos; // Vertex Position (x, y, z)
	glm::vec3 col; // Vertex Colour (r, g, b)
};

class Mesh {
public:
	Mesh();
	Mesh(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, 
		VkQueue transferQueue, VkCommandPool transferCommandPool, 
		std::vector<Vertex> * vertices, std::vector<uint32_t> * indices);

	void setModel(glm::mat4 newModel);
	glm::mat4 getModel();
	
	int getVertexCount();
	VkBuffer getVertexBuffer();

	int getIndexCount();
	VkBuffer getIndexBuffer();

	void destroyBuffers();

	~Mesh();

private:
	glm::mat4 model;
	
	int vertexCount;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	int indexCount;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	VkPhysicalDevice physicalDevice;
	VkDevice device;

	void createVertexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<Vertex> * vertices);
	void createIndexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<uint32_t> * indices);
};

