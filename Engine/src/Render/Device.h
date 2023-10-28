#pragma once

#include "../Core.h"

#include <vulkan/vulkan.h>

#include <vector>
#include <map>
#include <optional>

class Window;

class SwapChain;


struct SwapChainSupportDetails 
{
	VkSurfaceCapabilitiesKHR Capabilities;
	std::vector<VkSurfaceFormatKHR> Formats;
	std::vector<VkPresentModeKHR> PresentModes;
};

class Window;

class ENGINE_API Device
{
public:
	Device(Window* Window);
	
	~Device();


private:

#ifdef NDEBUG
	const bool EnableValidationLayers = false;
#else
	const bool EnableValidationLayers = true;
#endif

private:
	void createInstance();

	void setupDebugMessenger();

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& CreateInfo);

	bool checkValidationLayerSupport();

	void pickPhysicalDevice();

	int rateDeviceSuitability(VkPhysicalDevice Device);

	void createLogicalDevice();
	
	void setupSurface();

private:

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> GraphicsFamily;
		std::optional<uint32_t> PresentFamily;

		inline bool isComplete() { return GraphicsFamily.has_value(); }
	};

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice Device);


private:

	VkPhysicalDevice GPU = VK_NULL_HANDLE;

	VkDevice LogicalDevice;
	
	VkInstance Instance;

	VkDebugUtilsMessengerEXT DebugMessenger;

	VkQueue GraphicQueue;
	
	VkQueue PresentQueue;

	VkSurfaceKHR WindowSurface;

	Window* WindowPtr;


private:
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

	friend class SwapChain;
};

