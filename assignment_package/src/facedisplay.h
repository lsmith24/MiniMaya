#pragma once
#include "face.h"
#include "drawable.h"

class FaceDisplay : public Drawable {

public:
    void create() override;
    GLenum drawMode() override;
    void updateFace(Face *f);

    FaceDisplay(OpenGLContext *context);
    Face *repFace;
};
