
#include "RotationMaterial.h"
#include "Node.h"
#include <glm/gtc/type_ptr.hpp>

RotationMaterial::RotationMaterial(std::string name) : MaterialGL(name) {}

RotationMaterial::~RotationMaterial() {}

void RotationMaterial::render(Node *o) {
}

void RotationMaterial::animate(Node *o, const float elapsedTime) {
    // Rotation
    const float dt = elapsedTime;

    const float deltaAngle = m_speed * dt;
    o->frame()->rotate(glm::vec3(0, 1, 0), deltaAngle);
}
