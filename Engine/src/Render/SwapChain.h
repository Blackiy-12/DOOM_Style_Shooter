#pragma once

#include "../Core.h"

#include <vulkan/vulkan.h>

#include <vector>

struct SwapChainSupportDetails;

class Device;

class Window;

class ENGINE_API SwapChain
{
public:
	SwapChain(Device* Device, Window* Window);

	~SwapChain();


private:
	void init();

	void createSwapChain();

	void createImageViews();

private:
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableFormats);
	
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& AvailablePresentModes);
	
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities);

private:
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice* Device, VkSurfaceKHR* Surface);

private:
	Device* DevicePtr;
	
	Window* WindowPtr;

	VkSwapchainKHR SwapChainVK;

	std::vector<VkImage> SwapChainImages;

	VkFormat SwapChainImageFormat;

	VkExtent2D SwapChainExtent;

	std::vector<VkImageView> SwapChainImageViews;
};

