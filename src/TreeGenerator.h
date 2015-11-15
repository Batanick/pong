//
// Created by botanick on 15.11.15.
//

#ifndef PONG_TREEGENERATOR_H
#define PONG_TREEGENERATOR_H


#include "RenderCommon.h"

class TreeGenerator {
public:
    static PRenderable generateTupelo(glm::vec3 position);

    static PRenderable generateOak(glm::vec3 position);
};


#endif //PONG_TREEGENERATOR_H
