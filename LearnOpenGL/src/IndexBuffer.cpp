#include "IndexBuffer.h"
#include "Macros.h"


IndexBuffer::IndexBuffer(const unsigned int* indices, unsigned int count)
	: mNumIndices{ count }
{
	int bytes = sizeof(unsigned int) * count;
	createBuffer(bytes, indices);
}

IndexBuffer::IndexBuffer(const std::vector<unsigned int>& indices)
	: mNumIndices{ static_cast<unsigned int>(indices.size()) }
{
	int bytes = static_cast<int>(sizeof(unsigned int) * indices.size());
	createBuffer(bytes, indices.data());
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &ObjectID));
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(kIndexBufferType, ObjectID));
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(kIndexBufferType, 0));
}

unsigned int IndexBuffer::GetNumIndices() const
{
	return mNumIndices;
}

void IndexBuffer::createBuffer(int bytes, const unsigned int* data)
{
	GLCall(glGenBuffers(1, &ObjectID));
	Bind();
	GLCall(glBufferData(kIndexBufferType, bytes, data, GL_STATIC_DRAW));
}
