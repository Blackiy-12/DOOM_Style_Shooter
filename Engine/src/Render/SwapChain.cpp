#include "SwapChain.h"

#include "Device.h"
#include "Window.h"

#include <Logger.h>
#include <SDL2/SDL_vulkan.h>

#include <algorithm>

SwapChain::SwapChain(Device* Device, Window* Window)
    :   DevicePtr(Device),
        WindowPtr(Window)
{
    init();
}

SwapChain::~SwapChain()
{
    for (auto ImageView : SwapChainImageViews) 
        vkDestroyImageView(DevicePtr->LogicalDevice, ImageView, nullptr);


    vkDestroySwapchainKHR(DevicePtr->LogicalDevice, SwapChainVK, nullptr);
}

void SwapChain::init()
{
	createSwapChain();
    createImageViews();
}

void SwapChain::createSwapChain()
{
    ENGINE_INFO("Create Swap Chain: start");
    SwapChainSupportDetails SwapChainSupport = querySwapChainSupport(&(DevicePtr->GPU), &(DevicePtr->WindowSurface));

    VkSurfaceFormatKHR SurfaceFormat = chooseSwapSurfaceFormat(SwapChainSupport.Formats);
    VkPresentModeKHR PresentMode = chooseSwapPresentMode(SwapChainSupport.PresentModes);
    VkExtent2D Extent = chooseSwapExtent(SwapChainSupport.Capabilities);

    uint32_t imageCount = SwapChainSupport.Capabilities.minImageCount + 1;

    if (SwapChainSupport.Capabilities.maxImageCount > 0 && imageCount > SwapChainSupport.Capabilities.maxImageCount) 
        imageCount = SwapChainSupport.Capabilities.maxImageCount;

    VkSwapchainCreateInfoKHR CreateInfo{};
    CreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    CreateInfo.surface = DevicePtr->WindowSurface;

    CreateInfo.minImageCount = imageCount;
    CreateInfo.imageFormat = SurfaceFormat.format;
    CreateInfo.imageColorSpace = SurfaceFormat.colorSpace;
    CreateInfo.imageExtent = Extent;
    CreateInfo.imageArrayLayers = 1;
    CreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    Device::QueueFamilyIndices Indices = DevicePtr->findQueueFamilies(DevicePtr->GPU);
    uint32_t QueueFamilyIndices[] = { Indices.GraphicsFamily.value(), Indices.PresentFamily.value() };

    if (Indices.GraphicsFamily != Indices.PresentFamily) 
    {
        CreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        CreateInfo.queueFamilyIndexCount = 2;
        CreateInfo.pQueueFamilyIndices = QueueFamilyIndices;
    }
    else 
        CreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

    CreateInfo.preTransform = SwapChainSupport.Capabilities.currentTransform;
    CreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    CreateInfo.presentMode = PresentMode;
    CreateInfo.clipped = VK_TRUE;

    CreateInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(DevicePtr->LogicalDevice, &CreateInfo, nullptr, &SwapChainVK) != VK_SUCCESS) 
    {
        ENGINE_ERROR("Failed to create swap chain");
        exit(-1);
    }

    vkGetSwapchainImagesKHR(DevicePtr->LogicalDevice, SwapChainVK, &imageCount, nullptr);
    SwapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(DevicePtr->LogicalDevice, SwapChainVK, &imageCount, SwapChainImages.data());

    SwapChainImageFormat = SurfaceFormat.format;
    SwapChainExtent = Extent;

    ENGINE_INFO("Create Swap Chain: finished");
}

void SwapChain::createImageViews()
{
    ENGINE_INFO("Create Image Views: start");

    SwapChainImageViews.resize(SwapChainImages.size());

    for (size_t i = 0; i < SwapChainImages.size(); i++) 
    {
        VkImageViewCreateInfo CreateInfo{};
        CreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        CreateInfo.image = SwapChainImages[i];
        CreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        CreateInfo.format = SwapChainImageFormat;
        CreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        CreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        CreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        CreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        CreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        CreateInfo.subresourceRange.baseMipLevel = 0;
        CreateInfo.subresourceRange.levelCount = 1;
        CreateInfo.subresourceRange.baseArrayLayer = 0;
        CreateInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(DevicePtr->LogicalDevice, &CreateInfo, nullptr, &SwapChainImageViews[i]) != VK_SUCCESS) 
        {
            ENGINE_ERROR("Failed to create image views");
            exit(-1);
        }
    }

    ENGINE_INFO("Create Image Views: finished");
}

VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableFormats)
{
    for (const auto& AvailableFormat : AvailableFormats) 
    {
        if (AvailableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && AvailableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
        {
            return AvailableFormat;
        }
    }

    return AvailableFormats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& AvailablePresentModes)
{
    for (const auto& AvailablePresentMode : AvailablePresentModes) 
    {
        if (AvailablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) return AvailablePresentMode;
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities)
{
    if (Capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) 
    {
        return Capabilities.currentExtent;
    }


    int width, height;
    SDL_Vulkan_GetDrawableSize(WindowPtr->WindowPtr, &width, &height);

    VkExtent2D ActualExtent = 
    {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };

    ActualExtent.width = std::clamp(ActualExtent.width, Capabilities.minImageExtent.width, Capabilities.maxImageExtent.width);
    ActualExtent.height = std::clamp(ActualExtent.height, Capabilities.minImageExtent.height, Capabilities.maxImageExtent.height);

    return ActualExtent;

}

SwapChainSupportDetails SwapChain::querySwapChainSupport(VkPhysicalDevice* Device, VkSurfaceKHR* Surface)
{
    SwapChainSupportDetails Details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*Device, *Surface, &Details.Capabilities);

    uint32_t FormatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(*Device, *Surface, &FormatCount, nullptr);

    if (FormatCount != 0)
    {
        Details.Formats.resize(FormatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(*Device, *Surface, &FormatCount, Details.Formats.data());
    }

    uint32_t PresentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(*Device, *Surface, &PresentModeCount, nullptr);

    if (PresentModeCount != 0) 
    {
        Details.PresentModes.resize(PresentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(*Device, *Surface, &PresentModeCount, Details.PresentModes.data());
    }

    return Details;
}
