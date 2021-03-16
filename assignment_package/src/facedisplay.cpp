#pragma once
#include "facedisplay.h"
#include "vertex.h"
#include "halfedge.h"
#include <iostream>

FaceDisplay::FaceDisplay(OpenGLContext *context)
    : Drawable(context), repFace(nullptr)
{}

GLenum FaceDisplay::drawMode() {
    return GL_LINES;
}

void FaceDisplay::create() {
    if (repFace == nullptr) {
        return;
    }
    HalfEdge *edge = repFace->edge;
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> col;
    std::vector<GLuint> indices;
    HalfEdge* cur = edge;
    int n = 0;

    indices.push_back(0);
    for (cur = edge; cur->next != edge; cur = cur->next) {
        pos.push_back(glm::vec4(cur->vtx->pos, 1));
        col.push_back(glm::vec4(glm::vec3(1, 1, 1) - (repFace->rgb), 1));
        n++;
        indices.push_back(n);
        indices.push_back(n);
    }
    pos.push_back(glm::vec4(cur->vtx->pos, 1));
    col.push_back(glm::vec4(glm::vec3(1, 1, 1) - (repFace->rgb), 1));
    indices.push_back(0);

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

void FaceDisplay::updateFace(Face *f) {
    if(this->repFace != nullptr) {
        destroy();
    }
    this->repFace = f;
}
