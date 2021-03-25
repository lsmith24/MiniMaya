#include "joint.h"
#include "jointdisplay.h"
#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"

JointDisplay::JointDisplay(OpenGLContext *context)
    : Drawable(context), repJoint(nullptr)
{}

GLenum JointDisplay::drawMode() {
    return GL_LINES;
}

void JointDisplay::create() {
    if (repJoint == nullptr) {
        return;
    }
    //same as joint create but draw in white
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> col;
    std::vector<GLuint> indices;

    glm::vec4 xStart = glm::vec4(0, 0.5, 0, 1);
    glm::vec4 yStart = glm::vec4(0, 0, 0.5, 1);
    glm::vec4 zStart = glm::vec4(0.5, 0, 0, 1);

    //x axis circle
    for (int i = 0; i < 12; i++) {
        if (i == 11) {
            indices.push_back(i);
            indices.push_back(0);
        }
        else {
            indices.push_back(i);
            indices.push_back(i + 1);
        }
        glm::vec4 turn = glm::rotateX(xStart, glm::radians(i * 30.f));
        turn = repJoint->getOverallTransformation() * turn;
        pos.push_back(turn);
        col.push_back(glm::vec4(1, 1, 1, 1));
    }
    //y axis circle
    for (int i = 12; i < 24; i++) {
        if (i == 23) {
            indices.push_back(i);
            indices.push_back(12);
        }
        else {
            indices.push_back(i);
            indices.push_back(i + 1);
        }
        glm::vec4 turn = glm::rotateY(yStart, glm::radians(i * 30.f));
        turn = repJoint->getOverallTransformation() * turn;
        pos.push_back(turn);
        col.push_back(glm::vec4(1, 1, 1, 1));
    }
    //z axis circle
    for (int i = 24; i < 36; i++) {
        if (i == 35) {
            indices.push_back(i);
            indices.push_back(24);
        }
        else {
            indices.push_back(i);
            indices.push_back(i + 1);
        }
        glm::vec4 turn = glm::rotateZ(zStart, glm::radians(i * 30.f));
        turn = repJoint->getOverallTransformation() * turn;
        pos.push_back(turn);
        col.push_back(glm::vec4(1, 1, 1, 1));
    }
    count = indices.size();

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);

}

void JointDisplay::updateJoint(Joint *j) {
    if (this->repJoint != nullptr) {
        destroy();
    }
    this->repJoint = j;
}
