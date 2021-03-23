#include "joint.h"
#include "glm/glm.hpp"
#include "mygl.h"
#include <glm/gtx/rotate_vector.hpp>

Joint::Joint(OpenGLContext *context, QString s, Joint* p, glm::vec3 pos, glm::quat q, glm::mat4 b)
    : Drawable(context), name(s), parent(p), pos(pos), rotation(q), bindMatrix(b)
{
    jointLastID++;
    setText(0, name);
}

glm::mat4 Joint::getLocalTransformation() {
    glm::mat4 posMat = glm::mat4(glm::vec4(1, 0, 0, 0),
                              glm::vec4(0, 1, 0, 0),
                              glm::vec4(0, 0, 1, 0),
                              glm::vec4(this->pos, 1));
    glm::mat4 rotMat = glm::mat4_cast(this->rotation);
    return posMat * rotMat;
}

glm::mat4 Joint::getOverallTransformation() {
    if (parent == nullptr) {
        return this->getLocalTransformation();
    }

    return (parent->getLocalTransformation() * this->getLocalTransformation());
}

GLenum Joint::drawMode() {
    return GL_LINES;
}

void Joint::create() {
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> col;
    std::vector<GLuint> indices;

    //need 3 circles of 12 lines
    glm::vec4 xStart = glm::vec4(0, 0.5, 0, 1);
    glm::vec4 yStart = glm::vec4(0, 0, 0.5, 1);
    glm::vec4 zStart = glm::vec4(0.5, 0, 0, 1);

    //x axis circle
    for (int i = 0; i < 12; i++) {
        if (i == 11) {
            indices.push_back(i);
            indices.push_back(0);
        }
        indices.push_back(i);
        indices.push_back(i + 1);
        glm::vec4 turn = glm::rotateX(xStart, glm::radians(i * 30.f));
        turn = getOverallTransformation() * turn;
        pos.push_back(turn);
        col.push_back(glm::vec4(1, 0, 0, 1));
    }
    //y axis circle
    for (int i = 12; i < 24; i++) {
        if (i == 23) {
            indices.push_back(i);
            indices.push_back(12);
        }
        indices.push_back(i);
        indices.push_back(i + 1);
        glm::vec4 turn = glm::rotateY(yStart, glm::radians(i * 30.f));
        turn = getOverallTransformation() * turn;
        pos.push_back(turn);
        col.push_back(glm::vec4(0, 1, 0, 1));
    }
    //z axis circle
    for (int i = 24; i < 36; i++) {
        if (i == 35) {
            indices.push_back(i);
            indices.push_back(24);
        }
        indices.push_back(i);
        indices.push_back(i + 1);
        glm::vec4 turn = glm::rotateZ(zStart, glm::radians(i * 30.f));
        turn = getOverallTransformation() * turn;
        pos.push_back(turn);
        col.push_back(glm::vec4(0, 0, 1, 1));
    }

    //draw line to parent
    if (parent != nullptr) {
        indices.push_back(36);
        indices.push_back(37);
        pos.push_back(this->getOverallTransformation() * glm::vec4(0, 0, 0, 1)); //world positions
        pos.push_back(this->parent->getOverallTransformation() * glm::vec4(0, 0, 0, 1));
        col.push_back(glm::vec4(0.f, 0.968, 1.f, 1.f)); //torquoise
        col.push_back(glm::vec4(1.f, 0.f, 0.764, 1.f)); //pink
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

int Joint::jointLastID = 0;
