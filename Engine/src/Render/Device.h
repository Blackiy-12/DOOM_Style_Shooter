#pragma once

#include "../Core.h"

#include <vulkan/vulkan.h>

#include <vector>

class Window;

struct SwapChainSupportDetails 
{
	VkSurfaceCapabilitiesKHR Capabilities;
	std::vector<VkSurfaceFormatKHR> Formats;
	std::vector<VkPresentModeKHR> PresentModes;
};

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

	VkPhysicalDevice GPU = VK_NULL_HANDLE;

	VkDevice LogicalDevice;
	
	VkInstance Instance;

	VkDebugUtilsMessengerEXT DebugMessenger;

	VkQueue GraphicQueue;

	VkSurfaceKHR WindowSurface;

	Window* WindowPtr;

private:
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
};

