//
// Created by botanick on 15.11.15.
//
#include "Controls.h"

#include <GLFW/glfw3.h>

#include "TreeGenerator.h"
#include "heightGenerator.h"

void Controls::init(Renderer &renderer) {
    callbacks[GLFW_KEY_T] = [](Renderer &rend, RenderContext &context) {
        glm::vec3 &cameraPos = context.cameraPos;
        float height = getHeight(cameraPos.x, cameraPos.z);
        rend.add(TreeGenerator::generateTree(glm::vec3(cameraPos.x, height, cameraPos.z)));
    };
}

void Controls::onBeforeRender(GLFWwindow *const window, Renderer &renderer, RenderContext &renderContext) {
    for (auto &cb : callbacks) {
        if (glfwGetKey(window, cb.first) == GLFW_PRESS && pressed.find(cb.first) == pressed.end()) {
            pressed.insert(cb.first);
            cb.second(renderer, renderContext);
        } else if (glfwGetKey(window, cb.first) == GLFW_RELEASE) {
            pressed.erase(cb.first);
        }
    }
}

