#pragma once
#include "halfedge.h"
#include "drawable.h"

class EdgeDisplay : public Drawable {
public:
    void create() override;
    GLenum drawMode() override;
    void updateEdge(HalfEdge* he);

    HalfEdge *repEdge;

    EdgeDisplay(OpenGLContext *context);
};
