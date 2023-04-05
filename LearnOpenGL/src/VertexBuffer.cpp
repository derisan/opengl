#include "VertexBuffer.h"
#include "Macros.h"

VertexBuffer::VertexBuffer(const float *vertices, int bytes)
{
	createBuffer(vertices, bytes);
}

VertexBuffer::VertexBuffer(const std::vector<float>& vertices)
{
	int bytes = static_cast<int>(sizeof(float) * vertices.size());
	createBuffer(vertices.data(), bytes);
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &ObjectID));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(kVertexBufferType, ObjectID));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(kVertexBufferType, 0));
}

void VertexBuffer::createBuffer(const float* data, int bytes)
{
	GLCall(glGenBuffers(1, &ObjectID));
	Bind();
	GLCall(glBufferData(kVertexBufferType, bytes, data, GL_STATIC_DRAW));
}
