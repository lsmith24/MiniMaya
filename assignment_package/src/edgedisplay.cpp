#pragma once
#include "edgedisplay.h"
#include "vertex.h"

EdgeDisplay::EdgeDisplay(OpenGLContext *context)
    : Drawable(context), repEdge(nullptr)
{}

GLenum EdgeDisplay::drawMode() {
    return GL_LINES;
}

void EdgeDisplay::create() {
    if (repEdge == nullptr) {
        return;
    }
    std::vector<glm::vec4> pos {glm::vec4(repEdge->vtx->pos, 1),
                               glm::vec4(repEdge->sym->vtx->pos, 1)};
    std::vector<glm::vec4> col {glm::vec4(1, 1, 0, 0), glm::vec4(1, 0, 0, 0)};
    std::vector<GLuint> indices {0, 1};
    count = 2;

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

void EdgeDisplay::updateEdge(HalfEdge *he) {
    if (this->repEdge != nullptr) {
        destroy();
    }
    this->repEdge = he;
}
