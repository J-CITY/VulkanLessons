#pragma once
#define GLFW_INCLUDE_VULKAN
#include <fstream>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>

#include "Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MeshModel.h"


const int MAX_FRAME_DRAWS = 30;

const int MAX_OBJECTS = 20;

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
		int createMeshModel(std::string modelFile);
		void updateModel(int modelId, glm::mat4 newModel);
	private:
		struct Device {
			VkPhysicalDevice physicalDevice;
			VkDevice logicalDevice;
		};

		struct UboViewProjection {
			glm::mat4 projection;
			glm::mat4 view;
		} uboViewProjection;

		// Scene Objects
		std::vector<MeshModel> meshList;

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

		VkSampler textureSampler;

		// - Descriptors
		VkDescriptorSetLayout descriptorSetLayout;
		VkDescriptorSetLayout samplerSetLayout;
		VkPushConstantRange pushConstantRange;
		VkDescriptorSetLayout inputSetLayout;

		VkDescriptorPool descriptorPool;
		VkDescriptorPool samplerDescriptorPool;
		VkDescriptorPool inputDescriptorPool;
		
		std::vector<VkDescriptorSet> descriptorSets;
		std::vector<VkDescriptorSet> samplerDescriptorSets;

		std::vector<VkDescriptorSet> inputDescriptorSets;

		std::vector<VkBuffer> vpUniformBuffer;
		std::vector<VkDeviceMemory> vpUniformBufferMemory;

		std::vector<VkBuffer> modelDUniformBuffer;
		std::vector<VkDeviceMemory> modelDUniformBufferMemory;

		// - Assets
		std::vector<VkImage> textureImages;
		std::vector<VkDeviceMemory> textureImageMemory;
		std::vector<VkImageView> textureImageViews;
		
		// - Pipeline
		VkPipeline graphicsPipeline;
		VkPipelineLayout pipelineLayout;
		// Second shader
		VkPipeline secondPipeline;
		VkPipelineLayout secondPipelineLayout;
		
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

		std::vector<VkImage> colourBufferImage;
		std::vector<VkDeviceMemory> colourBufferImageMemory;
		std::vector<VkImageView> colourBufferImageView;

		std::vector<VkImage> depthBufferImage;
		std::vector<VkDeviceMemory> depthBufferImageMemory;
		std::vector<VkImageView> depthBufferImageView;

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
		void createDescriptorSetLayout();
		void createPushConstantRange();
		void createGraphicsPipeline();
		void createColourBufferImage();
		void createFramebuffers();
		void createCommandPool();
		void createCommandBuffers();
		void createSynchronisation();
		void createTextureSampler();

		void createUniformBuffers();
		void createDescriptorPool();
		void createDescriptorSets();

		void createInputDescriptorSets();
		
		void updateUniformBuffers(uint32_t imageIndex);

		void createDepthBufferImage();
		
		
		void recordCommands(uint32_t currentImage);

		// -- Getter Functions
		QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);
		SwapChainDetails getSwapChainDetails(VkPhysicalDevice device);

		// -- Choose Functions
		VkSurfaceFormatKHR chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
		VkPresentModeKHR chooseBestPresentationMode(const std::vector<VkPresentModeKHR> presentationModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
		VkFormat chooseSupportedFormat(const std::vector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags featureFlags);

		// -- Create Functions
		VkImage createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags useFlags,
			VkMemoryPropertyFlags propFlags, VkDeviceMemory* imageMemory);
		VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
		VkShaderModule createShaderModule(const std::vector<char>& code);

		int createTextureImage(std::string fileName);
		int createTexture(std::string fileName);
		int createTextureDescriptor(VkImageView textureImage);
		
	};

}
