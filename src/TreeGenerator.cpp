//
// Created by botanick on 15.11.15.
//

#include "TreeGenerator.h"

#include "Tree.h"

PRenderable TreeGenerator::generateTupelo(glm::vec3 position) {
    return Tree::tupelo(position);
}


PRenderable TreeGenerator::generateOak(glm::vec3 position) {
    return Tree::oak(position);
}
