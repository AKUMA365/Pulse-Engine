#ifndef RENDER_H
#define RENDER_H

#include <QVulkanWindow>
#include <QVulkanDeviceFunctions>

class VulkanRenderer : public QVulkanWindowRenderer
{
public:
    VulkanRenderer(QVulkanWindow *w);

    void initResources() override;
    void initSwapChainResources() override;
    void releaseSwapChainResources() override;
    void releaseResources() override;
    void startNextFrame() override;

private:
    VkShaderModule createShaderModule(const QString &name);

    QVulkanWindow *m_window;
    QVulkanDeviceFunctions *m_devFuncs;
    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
    VkPipeline m_pipeline = VK_NULL_HANDLE;
    float m_green = 0.0f;
};

class VulkanWindow : public QVulkanWindow
{
public:
    QVulkanWindowRenderer *createRenderer() override;
};

#endif // RENDER_H