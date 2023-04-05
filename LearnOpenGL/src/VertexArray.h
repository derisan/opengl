#pragma once

#include <unordered_map>
#include <memory>
#include <string_view>
#include "GLObject.h"


class VertexBuffer;
class IndexBuffer;
class VertexLayout;


class VertexArray
	: public GLObject
{
	using VertexArrayMap = std::unordered_map<std::string, std::shared_ptr<VertexArray>>;

public:
	static std::shared_ptr<VertexArray> GetVertexArray(std::string_view vertexArrayName);
	static void Clear();

private:
	inline static VertexArrayMap sVertexArrays;

public:
	VertexArray();
	~VertexArray();

	void SetVertexBuffer(const VertexBuffer& buffer, const VertexLayout& layout);
	void SetIndexBuffer(const IndexBuffer& buffer);

	virtual void Bind() const override;
	virtual void Unbind() const override;

	unsigned int GetNumIndices() const;

private:
	std::unique_ptr<VertexBuffer> mVertexBuffer;
	std::unique_ptr<IndexBuffer> mIndexBuffer;
};

