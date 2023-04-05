#include "VertexArray.h"
#include "Macros.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexLayout.h"

std::shared_ptr<VertexArray> VertexArray::GetVertexArray(std::string_view vertexArrayName)
{
	auto iter = sVertexArrays.find(vertexArrayName.data());

	if (iter != sVertexArrays.end())
	{
		return iter->second;
	}

	// (TODO): create VBO, layout, EBO and VAO

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	if (vertexArrayName == "Rectangle")
	{
		vertices.reserve(20);
		indices.reserve(6);

		vertices.assign({
			// Pos		         // Normal		   // Tex coord
			-0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			 -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			});

		indices.assign({
			0, 1, 2,
			0, 2, 3
			});
	}
	else if (vertexArrayName == "Cube")
	{
		vertices.reserve(192);
		indices.reserve(36);

		vertices.assign({
			// Position				// Normal			// Tex coord
			-0.5f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,		0.022491f, 0.977051f,
			0.5f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,		0.977051f, 0.977051f,
			-0.5f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,		0.022491f, 0.022491f,
			0.5f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,		0.977051f, 0.022491f,
			-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		0.977051f, 0.022491f,
			0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		0.022491f, 0.022491f,
			0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,		0.022491f, 0.977051f,
			-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,		0.977051f, 0.977051f,
			-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		0.977051f, 0.022491f,
			0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		0.022491f, 0.022491f,
			0.5f, 0.0f, 0.5f,		0.0f, 0.0f, 1.0f,		0.022491f, 0.977051f,
			-0.5f, 0.0f, 0.5f,		0.0f, 0.0f, 1.0f,		0.977051f, 0.977051f,
			-0.5f, 0.5f, -0.5f,		-1.0f, 0.0f, 0.0f,		0.977051f, 0.022491f,
			-0.5f, 0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,		0.022491f, 0.022491f,
			-0.5f, 0.0f, 0.5f,		-1.0f, 0.0f, 0.0f,		0.022491f, 0.977051f,
			-0.5f, 0.0f, -0.5f,		-1.0f, 0.0f, 0.0f,		0.977051f, 0.977051f,
			0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,		0.977051f, 0.022491f,
			-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,		0.022491f, 0.022491f,
			-0.5f, 0.0f, -0.5f,		0.0f, 0.0f, -1.0f,		0.022491f, 0.977051f,
			0.5f, 0.0f, -0.5f,		0.0f, 0.0f, -1.0f,		0.977051f, 0.977051f,
			0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,		0.977051f, 0.022491f,
			0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.022491f, 0.022491f,
			0.5f, 0.0f, -0.5f,		1.0f, 0.0f, 0.0f,		0.022491f, 0.977051f,
			0.5f, 0.0f, 0.5f,		1.0f, 0.0f, 0.0f,		0.977051f, 0.977051f
			});


		indices.assign({
			0,1,2,
			1,3,2,
			4,5,6,
			4,6,7,
			8,9,10,
			8,10,11,
			12,13,14,
			12,14,15,
			16,17,18,
			16,18,19,
			20,21,22,
			20,22,23
			});
	}

	VertexBuffer vb{ vertices };
	VertexLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	IndexBuffer ib{ indices };

	auto vertexArray = std::make_shared<VertexArray>();
	vertexArray->SetVertexBuffer(vb, layout);
	vertexArray->SetIndexBuffer(ib);

	sVertexArrays.emplace(vertexArrayName, vertexArray);
	return vertexArray;
}

void VertexArray::Clear()
{
	sVertexArrays.clear();
}

VertexArray::VertexArray()
{
	GLCall(glCreateVertexArrays(1, &ObjectID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &ObjectID));
}

void VertexArray::SetVertexBuffer(const VertexBuffer& buffer, const VertexLayout& layout)
{
	mVertexBuffer = std::make_unique<VertexBuffer>(buffer);

	this->Bind();
	mVertexBuffer->Bind();

	const auto& attribs = layout.GetAttributes();
	const auto stride = layout.GetStride();
	unsigned long long offset = 0;
	for (auto i = 0; i < attribs.size(); i++)
	{
		const auto& attrib = attribs[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, attrib.Count, attrib.DataType, attrib.Normalized, stride,
			reinterpret_cast<const void*>(offset)));
		offset += attrib.Count * VertexAttribute::GetSizeOfDataType(attrib.DataType);
	}

	this->Unbind();
	mVertexBuffer->Unbind();
}

void VertexArray::SetIndexBuffer(const IndexBuffer& buffer)
{
	mIndexBuffer = std::make_unique<IndexBuffer>(buffer);

	this->Bind();
	mIndexBuffer->Bind();

	this->Unbind();
	mIndexBuffer->Unbind();
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(ObjectID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

unsigned int VertexArray::GetNumIndices() const
{
	return mIndexBuffer->GetNumIndices();
}
