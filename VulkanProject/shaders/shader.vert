#version 460

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(binding = 2) readonly buffer storageBuffer {
	mat4 model[];
} ObjectData;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in uint modelMatrixIndex;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    vec4 modelSpacePos = ObjectData.model[modelMatrixIndex] * vec4(inPosition, 1.0);
    gl_Position = ubo.proj * modelSpacePos;
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}