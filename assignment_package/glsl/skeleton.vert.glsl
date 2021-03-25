#version 150

uniform mat4 u_Model; //transformation
uniform mat4 u_ModelInvTr; //inverse transpose of model
uniform mat4 u_ViewProj; //camera's transformation
uniform mat4 u_bindMatrices[100]; //array of mats, bind matrix of joint
uniform mat4 u_jointTransforms[100]; //array of mats, overall transformation of joint

in vec4 vs_Pos;
in vec4 vs_Nor;
in vec4 vs_Col;
in ivec2 vs_IDs;
in vec2 vs_Weights;

out vec4 fs_Nor;
out vec4 fs_Col;

void main(void) {
    fs_Col = vs_Col;
    vec4 sum = vec4(0.f);
    for (int i = 0; i < 2; i++) {
        mat4 bind = u_bindMatrices[vs_IDs[i]]; //fill w/ bind mats
        mat4 overall = u_jointTransforms[vs_IDs[i]]; //fill w/ overall transforms
        sum = sum + overall * bind * vs_Weights[i] * vs_Pos;
    }
    gl_Position = u_ViewProj * (u_Model * sum);
}
