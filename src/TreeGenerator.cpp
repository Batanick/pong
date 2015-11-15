//
// Created by botanick on 15.11.15.
//

#include "TreeGenerator.h"

#include "Tree.h"

PRenderable TreeGenerator::generateTree(glm::vec3 position) {
    const std::shared_ptr<Tree> &result = std::shared_ptr<Tree>(new Tree(position));
    return result;
}
