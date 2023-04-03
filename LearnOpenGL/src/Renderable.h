#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

class VertexArray;
class Texture;
class Shader;

class Renderable
{
public:
	void SetVertexArray( const std::shared_ptr<VertexArray> vertexArray );
	void SetTexture( const std::shared_ptr<Texture>& texture );
	void SetMVPMatrix( const glm::mat4& mat );

	void Draw( const Shader& shader ) const;
	
private:
	std::shared_ptr<VertexArray> mVertexArray;
	std::vector<std::shared_ptr<Texture>> mTextures;
	glm::mat4 mMVP = glm::mat4{ 1.0f };
};

