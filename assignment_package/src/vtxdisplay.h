#include "drawable.h"
#include "vertex.h"

class VtxDisplay : public Drawable {

public:
    void create() override;
    GLenum drawMode() override;
    void updateVertex(Vertex *v);

    VtxDisplay(OpenGLContext *context);
    Vertex *repVtx;
};
