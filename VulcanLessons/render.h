#pragma once
#define GLFW_INCLUDE_VULKAN
#include <fstream>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>

#include "Mesh.h"

const int MAX_FRAME_DRAWS = 3;

namespace VKRENDER {

	
	// List of validation layers to use
	// VK_LAYER_LUNARG_standard_validation = All standard validation layers
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	
	struct QueueFamilyIndices {
		int graphicsFamily = -1;// Location of Graphics Queue Family
		int presentationFamily = -1;		// Location of Presentation Queue Family

		// Check if queue families are valid
		bool isValid() {
			return graphicsFamily >= 0 && presentationFamily >= 0;
		}
	};

	struct SwapChainDetails {
		VkSurfaceCapabilitiesKHR surfaceCapabilities;		// Surface properties, e.g. image size/extent
		std::vector<VkSurfaceFormatKHR> formats;			// Surface image formats, e.g. RGBA and size of each colour
		std::vector<VkPresentModeKHR> presentationModes;	// How images should be presented to screen
	};

	struct SwapchainImage {
		VkImage image;
		VkImageView imageView;
	};

	static std::vector<char> readFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::binary | std::ios::ate);
		if (!file.is_open()) {
			throw std::runtime_error("Failed to open a file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> fileBuffer(fileSize);

		file.seekg(0);
		file.read(fileBuffer.data(), fileSize);
		file.close();
		return fileBuffer;
	}
	
	class Render {
	public:
		Render(GLFWwindow* win);
		~Render();
		void draw();
	private:
		struct Device {
			VkPhysicalDevice physicalDevice;
			VkDevice logicalDevice;
		};


		// Scene Objects
		std::vector<Mesh> meshList;

		Device mainDevice;
		
		GLFWwindow* win;
		VkInstance instance;

		VkQueue graphicsQueue;
		VkQueue presentationQueue;


		VkSurfaceKHR surface;
		VkSwapchainKHR swapchain;
		std::vector<SwapchainImage> swapChainImages;
		std::vector<VkFramebuffer> swapChainFramebuffers;
		std::vector<VkCommandBuffer> commandBuffers;
		
		// - Pipeline
		VkPipeline graphicsPipeline;
		VkPipelineLayout pipelineLayout;
		VkRenderPass renderPass;

		// - Pools
		VkCommandPool graphicsCommandPool;

		int currentFrame = 0;
		
		// - Utility
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;

		// - Synchronisation
		std::vector<VkSemaphore> imageAvailable;
		std::vector<VkSemaphore> renderFinished;
		std::vector<VkFence> drawFences;

		void init();
		void createInstance();
		bool checkInstanceExtensionSupport(std::vector<const char*>& checkExtensions);
		void clean();

		void getPhysicalDevice();
		bool checkDeviceSuitable(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);

		void createLogicalDevice();
		void createSurface();
		void createSwapChain();
		void createRenderPass();
		void createGraphicsPipeline();
		void createFramebuffers();
		void createCommandPool();
		void createCommandBuffers();
		void createSynchronisation();
		void recordCommands();

		// -- Getter Functions
		QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);
		SwapChainDetails getSwapChainDetails(VkPhysicalDevice device);

		// -- Choose Functions
		VkSurfaceFormatKHR chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
		VkPresentModeKHR chooseBestPresentationMode(const std::vector<VkPresentModeKHR> presentationModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);

		// -- Create Functions
		VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
		VkShaderModule createShaderModule(const std::vector<char>& code);
	};

}
