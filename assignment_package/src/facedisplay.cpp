#pragma once
#include "facedisplay.h"
#include "vertex.h"
#include "halfedge.h"

FaceDisplay::FaceDisplay(OpenGLContext *context)
    : Drawable(context), repFace(nullptr)
{}

GLenum FaceDisplay::drawMode() {
    return GL_LINES;
}

void FaceDisplay::create() {
    HalfEdge *edge = repFace->edge;
    std::vector<glm::vec4> pos {
        glm::vec4(edge->vtx->pos, 1.f),
        glm::vec4(edge->next->vtx->pos, 1.f),
        glm::vec4(edge->next->next->vtx->pos, 1.f),
        glm::vec4(edge->next->next->next->vtx->pos, 1.f),
    };
    std::vector<glm::vec4> col {glm::vec4(glm::vec3(1, 1, 1) - (repFace->rgb), 1)};
    std::vector<GLuint> indices {0, 1, 1, 2, 2, 3, 3, 0};
    count = 8;

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

void FaceDisplay::updateFace(Face *f) {
    if(this->repFace != nullptr) {
        destroy();
    }
    this->repFace = f;
}
