//
// Created by botanick on 15.11.15.
//

#ifndef PONG_CONTROLS_H
#define PONG_CONTROLS_H

#include "Renderer.h"
#include <map>
#include <set>

class Controls {
public:
    Controls() {
        //nothing }
    }

    void onBeforeRender(GLFWwindow *const window, Renderer &renderer, RenderContext &context);

    void init(Renderer &renderer);

    bool shiftPressed();

    typedef void(*PressCallback)(Renderer &renderer, RenderContext &context, Controls &controls);

private:
    std::map<unsigned int, PressCallback> callbacks;
    std::set<unsigned int> pressed;
};


#endif //PONG_CONTROLS_H
