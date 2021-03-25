#pragma once
#include "joint.h"
#include "drawable.h"

class JointDisplay : public Drawable {
public:
    void create() override;
    GLenum drawMode() override;
    void updateJoint(Joint* j);

    Joint *repJoint;
    JointDisplay(OpenGLContext *context);
};
