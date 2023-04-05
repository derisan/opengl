#version 460 core

// in
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

// out
out vec2 texCoord;
out vec3 normal;
out vec3 fragPosition;

// uniform
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uITModel; // Inversed,transposed model matrix

void main()
{
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0f);
	texCoord = aTexCoord;
	normal = vec3(uITModel * vec4(aNormal, 1.0f));
	fragPosition = vec3(uModel * vec4(aPos, 1.0f));
}