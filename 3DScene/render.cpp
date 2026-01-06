//
// Created by Dima Semchenko on 06.01.2026.
//

#include "render.h"

#pragma once

QVulkanWindowRenderer* VulkanWindow::createRenderer()
{
    return new VulkanRenderer(this);
}

VulkanRenderer::VulkanRenderer(QVulkanWindow* w)
{

}

