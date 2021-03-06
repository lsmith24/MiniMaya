#pragma once
#include "drawable.h"
#include "smartpointerhelp.h"
#include "halfedge.h"
#include "face.h"
#include "vertex.h"
#include <glm/glm.hpp>

class Mesh: public Drawable {
public:
    std::vector<uPtr<HalfEdge>> edges;
    std::vector<uPtr<Face>> faces;
    std::vector<uPtr<Vertex>> vertices;

    virtual void create() override;
    GLenum drawMode() override;
    void makeCube();
    Mesh(OpenGLContext* context);
};
