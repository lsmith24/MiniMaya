#pragma once
#include "vtxdisplay.h"
#include "drawable.h"

VtxDisplay::VtxDisplay(OpenGLContext *context)
    : Drawable(context), repVtx(nullptr)
{}

GLenum VtxDisplay::drawMode() {
    return GL_POINTS;
}

void VtxDisplay::create() {
    std::vector<glm::vec4> pos {glm::vec4(repVtx->pos, 1)};
    std::vector<glm::vec4> col {glm::vec4(1, 1, 1, 0)};
    std::vector<GLuint> indices {0};
    count = 1;

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

void VtxDisplay::updateVertex(Vertex *v) {
    if (this->repVtx != nullptr) {
        destroy();
    }
    this->repVtx = v;
}
