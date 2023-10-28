#include "Device.h"

#include "Window.h"
#include <Logger.h>

#include <SDL2/SDL_vulkan.h>

#include <map>

const std::vector<const char*> ValidationLayers = { "VK_LAYER_KHRONOS_validation" };

struct QueueFamilyIndices 
{
    std::optional<uint32_t> GraphicsFamily;

    inline bool isComplete() { return GraphicsFamily.has_value(); }
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice Device);


VkResult createDebugUtilsMessengerEXT(  VkInstance Instance, 
                                        const VkDebugUtilsMessengerCreateInfoEXT* CreateInfoPtr,
                                        const VkAllocationCallbacks* AllocatorPtr, 
                                        VkDebugUtilsMessengerEXT* DebugMessengerPtr) 

{
    auto Func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Instance, "vkCreateDebugUtilsMessengerEXT");

    if (Func != nullptr) 
        return Func(Instance, CreateInfoPtr, AllocatorPtr, DebugMessengerPtr);
   
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void destroyDebugUtilsMessengerEXT( VkInstance Instance,
                                    VkDebugUtilsMessengerEXT DebugMessenger,
                                    const VkAllocationCallbacks* AllocatorPtr)
{
    auto Func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Instance, "vkDestroyDebugUtilsMessengerEXT");

    if (Func != nullptr) 
        Func(Instance, DebugMessenger, AllocatorPtr);
}

Device::Device(Window* Window)
    : WindowPtr(Window)
{
    createInstance();
    setupDebugMessenger();
    pickPhysicalDevice();
    createLogicalDevice();
    setupSurface();
}

Device::~Device()
{
    vkDestroyDevice(LogicalDevice, nullptr);

    if (EnableValidationLayers) 
        destroyDebugUtilsMessengerEXT(Instance, DebugMessenger, nullptr);

    vkDestroyInstance(Instance, nullptr);

}

void Device::createInstance()
{
    ENGINE_INFO("Create Instance: start");

    if (EnableValidationLayers && checkValidationLayerSupport() == false) 
    {
        ENGINE_ERROR("Create Instance: validation layers requested, but not available");
        exit(-1);
    }

    VkApplicationInfo AppInfo{};
    AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    AppInfo.pApplicationName = this->WindowPtr->WindowName.c_str();
    AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    AppInfo.pEngineName = "No Engine";
    AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    AppInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo CreatedInfo{};
    CreatedInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    CreatedInfo.pApplicationInfo = &AppInfo;

    unsigned int ExtensionsSize = 0;
    std::vector<const char *> Extensions;

    bool Result = SDL_Vulkan_GetInstanceExtensions(this->WindowPtr->WindowPtr, &ExtensionsSize, nullptr);

    if (Result == false)
    {
        ENGINE_ERROR("Create Instance: can't get instance size from SDL");
        exit(-1);
    }

    Extensions.resize(ExtensionsSize);

    Result = SDL_Vulkan_GetInstanceExtensions(this->WindowPtr->WindowPtr, &ExtensionsSize, Extensions.data());
    if (Result == false)
    {
        ENGINE_ERROR("Create Instance: can't get instance from SDL");
        exit(-1);
    }


    if (EnableValidationLayers)
        Extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    CreatedInfo.ppEnabledExtensionNames = Extensions.data();
    CreatedInfo.enabledExtensionCount = Extensions.size();

    VkDebugUtilsMessengerCreateInfoEXT DebugCreatedInfo{};
    if (EnableValidationLayers) {
        CreatedInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
        CreatedInfo.ppEnabledLayerNames = ValidationLayers.data();

        populateDebugMessengerCreateInfo(DebugCreatedInfo);
        CreatedInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&DebugCreatedInfo;
    }
    else {
        CreatedInfo.enabledLayerCount = 0;

        CreatedInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&CreatedInfo, nullptr, &Instance) != VK_SUCCESS)
    {
        ENGINE_ERROR("Create Instance: Vulkan can't create instance");
        exit(-1);
    }


    ENGINE_INFO("Create Instance: finished");
}

void Device::setupDebugMessenger()
{
    ENGINE_INFO("Setup Debug Messenger : start");
    if (!EnableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT CreateInfo;
    populateDebugMessengerCreateInfo(CreateInfo);

    if (createDebugUtilsMessengerEXT(Instance, &CreateInfo, nullptr, &DebugMessenger) != VK_SUCCESS)
    {
        ENGINE_INFO("Setup Debug Messenger: failed to set up debug messenger");
        exit(-1);
    }

    ENGINE_INFO("Setup Debug Messenger: finished");
}

void Device::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& CreateInfo)
{
    CreateInfo = {};
    CreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    CreateInfo.messageSeverity =  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    CreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    CreateInfo.pfnUserCallback = debugCallback;
}

bool Device::checkValidationLayerSupport()
{
    uint32_t LayerCount;
    vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);

    std::vector<VkLayerProperties> AvailableLayers(LayerCount);
    vkEnumerateInstanceLayerProperties(&LayerCount, AvailableLayers.data());

    for (const char* LayerName : ValidationLayers) 
    {
        bool LayerFound = false;

        for (const auto& LayerProperties : AvailableLayers) 
        {
            if (strcmp(LayerName, LayerProperties.layerName) == 0) 
            {
                LayerFound = true;
                break;
            }
        }

        if (LayerFound == false) 
            return false;
    }

    return true;
}

void Device::pickPhysicalDevice()
{
    ENGINE_INFO("Pick Physical Device: start");

    uint32_t DeviceCount = 0;
    vkEnumeratePhysicalDevices(Instance, &DeviceCount, nullptr);

    if (DeviceCount == 0)
    {
        ENGINE_ERROR("Pick Physical Device: can't find GPUs with Vulkan support");
        exit(-1);
    }

    std::vector<VkPhysicalDevice> Devices(DeviceCount);
    vkEnumeratePhysicalDevices(Instance, &DeviceCount, Devices.data());

    std::multimap<int, VkPhysicalDevice> Candidates;

    for (const auto& Device : Devices) 
    {
        int Score = rateDeviceSuitability(Device);
        Candidates.insert(std::make_pair(Score, Device));
    }

    if (Candidates.rbegin()->first > 0)
        GPU = Candidates.rbegin()->second;

    if (GPU == VK_NULL_HANDLE) 
    {
        ENGINE_ERROR("Pick Physical Device: can't find suitable GPU");
        exit(-1);
    }

    ENGINE_INFO("Pick Physical Device: Device picked");
}

int Device::rateDeviceSuitability(VkPhysicalDevice Device)
{
    VkPhysicalDeviceProperties DeviceProperties;
    VkPhysicalDeviceFeatures DeviceFeatures;
    vkGetPhysicalDeviceProperties(Device, &DeviceProperties);
    vkGetPhysicalDeviceFeatures(Device, &DeviceFeatures);

    int Score = 0;

    if (DeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) Score += 1000;

    Score += DeviceProperties.limits.maxImageDimension2D;

    if (!DeviceFeatures.geometryShader) return 0;

    if (findQueueFamilies(Device).isComplete() == false) return 0;

    return Score;
}

void Device::createLogicalDevice()
{
    ENGINE_INFO("Create Logical Device: start");
    QueueFamilyIndices Indices = findQueueFamilies(GPU);

    VkDeviceQueueCreateInfo QueueCreateInfo{};
    QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    QueueCreateInfo.queueFamilyIndex = Indices.GraphicsFamily.value();
    QueueCreateInfo.queueCount = 1;

    float QueuePriority = 1.0f;
    QueueCreateInfo.pQueuePriorities = &QueuePriority;

    VkPhysicalDeviceFeatures DeviceFeatures{};

    VkDeviceCreateInfo CreateInfo{};
    CreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    CreateInfo.pQueueCreateInfos = &QueueCreateInfo;
    CreateInfo.queueCreateInfoCount = 1;
    CreateInfo.pEnabledFeatures = &DeviceFeatures;

    CreateInfo.enabledExtensionCount = 0;

    if (EnableValidationLayers) 
    {
        CreateInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
        CreateInfo.ppEnabledLayerNames = ValidationLayers.data();
    }

    else 
        CreateInfo.enabledLayerCount = 0;


    if (vkCreateDevice(GPU, &CreateInfo, nullptr, &LogicalDevice) != VK_SUCCESS)
    {
        ENGINE_ERROR("Failed to create logical device");
        exit(-1);
    }

    vkGetDeviceQueue(LogicalDevice, Indices.GraphicsFamily.value(), 0, &GraphicQueue);

    ENGINE_INFO("Create Logical Device: finished");
}

void Device::setupSurface()
{
    this->WindowPtr->createSurface(&WindowSurface, Instance);

    ENGINE_INFO("Window surface created");
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice Device) 
{
    QueueFamilyIndices Indices;

    uint32_t QueueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, QueueFamilies.data());

    int i = 0;
    for (const auto& QueueFamily : QueueFamilies) 
    {
        if (QueueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            Indices.GraphicsFamily = i;

        if (Indices.isComplete()) break;

        i++;
    }

    return Indices;
}

VKAPI_ATTR VkBool32 VKAPI_CALL Device::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{ 
    ENGINE_ERROR("Vulakn validation layer: {}\n", pCallbackData->pMessage);
    return VK_FALSE;  
}
