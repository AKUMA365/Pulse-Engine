#include "render.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QVulkanInstance>

QVulkanWindowRenderer* VulkanWindow::createRenderer()
{
    return new VulkanRenderer(this);
}

VulkanRenderer::VulkanRenderer(QVulkanWindow* w)
    : m_window(w)
{
}

VkShaderModule VulkanRenderer::createShaderModule(const QString &name)
{
    // Попытка 1: Прямой путь
    QString finalPath = name;
    QFile file(finalPath);

    // Попытка 2: Если не нашли, ищем относительно папки сборки (вверх в 3DScene)
    if (!file.exists()) {
        finalPath = "../3DScene/" + name;
        file.setFileName(finalPath);
    }

    // Попытка 3: Абсолютный путь (для CLion на macOS часто нужно)
    if (!file.exists()) {
        // ВНИМАНИЕ: Если всё равно красный экран, проверьте этот путь!
        finalPath = "/Users/semchenkodmitriyxgmail.com/CLionProjects/Pulse-Engine/3DScene/" + name;
        file.setFileName(finalPath);
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "ERROR: Could not find shader file anywhere!";
        qWarning() << "Searched in:" << finalPath;
        qWarning() << "Current Dir:" << QDir::currentPath();
        return VK_NULL_HANDLE;
    }

    QByteArray blob = file.readAll();
    file.close();

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = blob.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(blob.constData());

    VkShaderModule shaderModule;
    if (m_devFuncs->vkCreateShaderModule(m_window->device(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        return VK_NULL_HANDLE;
    }
    return shaderModule;
}

void VulkanRenderer::initResources()
{
    qDebug() << "--- INIT RESOURCES ---";
    m_devFuncs = m_window->vulkanInstance()->deviceFunctions(m_window->device());
    VkDevice dev = m_window->device();

    VkShaderModule vertShader = createShaderModule("basic.vert.spv");
    VkShaderModule fragShader = createShaderModule("basic.frag.spv");

    // МЫ НЕ ДЕЛАЕМ RETURN, ЕСЛИ ШЕЙДЕРОВ НЕТ. МЫ ПРОСТО НЕ СОЗДАЕМ ПАЙПЛАЙН.
    if (vertShader && fragShader) {
        VkPipelineShaderStageCreateInfo vertStageInfo = {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
        vertStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertStageInfo.module = vertShader;
        vertStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragStageInfo = {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
        fragStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragStageInfo.module = fragShader;
        fragStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = {vertStageInfo, fragStageInfo};

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};

        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        VkPipelineViewportStateCreateInfo viewportState = {VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
        VkPipelineDynamicStateCreateInfo dynamicState = {VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
        dynamicState.dynamicStateCount = 2;
        dynamicState.pDynamicStates = dynamicStates;

        VkPipelineRasterizationStateCreateInfo rasterizer = {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_NONE; // Отключаем отсечение для гарантии
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;

        VkPipelineMultisampleStateCreateInfo multisampling = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.colorWriteMask = 0xF; // RGBA
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending = {VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
        m_devFuncs->vkCreatePipelineLayout(dev, &pipelineLayoutInfo, nullptr, &m_pipelineLayout);

        VkGraphicsPipelineCreateInfo pipelineInfo = {VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = m_pipelineLayout;
        pipelineInfo.renderPass = m_window->defaultRenderPass();

        if (m_devFuncs->vkCreateGraphicsPipelines(dev, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline) == VK_SUCCESS) {
            qDebug() << "SUCCESS: Pipeline created!";
        }
    } else {
        qWarning() << "WARNING: Shaders missing. Rendering RED background only.";
    }

    if (vertShader) m_devFuncs->vkDestroyShaderModule(dev, vertShader, nullptr);
    if (fragShader) m_devFuncs->vkDestroyShaderModule(dev, fragShader, nullptr);

    // ВАЖНО: Запускаем цикл принудительно!
    m_window->requestUpdate();
}

void VulkanRenderer::initSwapChainResources() { }
void VulkanRenderer::releaseSwapChainResources() { }

void VulkanRenderer::releaseResources()
{
    if (m_pipeline) {
        m_devFuncs->vkDestroyPipeline(m_window->device(), m_pipeline, nullptr);
        m_pipeline = VK_NULL_HANDLE;
    }
    if (m_pipelineLayout) {
        m_devFuncs->vkDestroyPipelineLayout(m_window->device(), m_pipelineLayout, nullptr);
        m_pipelineLayout = VK_NULL_HANDLE;
    }
}

void VulkanRenderer::startNextFrame()
{
    VkClearValue clearValues[2];
    memset(clearValues, 0, sizeof(clearValues));

    // ЛОГИКА ЦВЕТА:
    // Если пайплайн есть (всё ок) -> ЗЕЛЕНЫЙ
    // Если пайплайна нет (ошибка шейдера) -> КРАСНЫЙ
    if (m_pipeline) {
        m_green += 0.01f;
        if (m_green > 1.0f) m_green = 0.0f;
        clearValues[0].color = {{0.0f, m_green, 0.0f, 1.0f}};
    } else {
        // Ярко-красный цвет ошибки
        clearValues[0].color = {{1.0f, 0.0f, 0.0f, 1.0f}};
    }

    clearValues[1].depthStencil = {1.0f, 0};

    VkRenderPassBeginInfo rpBeginInfo = {VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
    rpBeginInfo.renderPass = m_window->defaultRenderPass();
    rpBeginInfo.framebuffer = m_window->currentFramebuffer();
    rpBeginInfo.renderArea.extent.width = m_window->swapChainImageSize().width();
    rpBeginInfo.renderArea.extent.height = m_window->swapChainImageSize().height();
    rpBeginInfo.clearValueCount = 2;
    rpBeginInfo.pClearValues = clearValues;

    VkCommandBuffer cmdBuf = m_window->currentCommandBuffer();
    m_devFuncs->vkCmdBeginRenderPass(cmdBuf, &rpBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    if (m_pipeline) {
        m_devFuncs->vkCmdBindPipeline(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);

        VkViewport viewport = {0.0f, 0.0f, (float)rpBeginInfo.renderArea.extent.width, (float)rpBeginInfo.renderArea.extent.height, 0.0f, 1.0f};
        m_devFuncs->vkCmdSetViewport(cmdBuf, 0, 1, &viewport);

        VkRect2D scissor = {{0, 0}, rpBeginInfo.renderArea.extent};
        m_devFuncs->vkCmdSetScissor(cmdBuf, 0, 1, &scissor);

        m_devFuncs->vkCmdDraw(cmdBuf, 3, 1, 0, 0);
    }

    m_devFuncs->vkCmdEndRenderPass(cmdBuf);
    m_window->frameReady();
    m_window->requestUpdate();
}