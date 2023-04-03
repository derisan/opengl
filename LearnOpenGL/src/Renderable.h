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
	void SetMVPMatrix( const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection );
	void SetModelMatrix( const glm::mat4& mat );
	void SetViewMatrix( const glm::mat4& mat );
	void SetProjectionMatrix( const glm::mat4& mat);

	void Draw( const Shader& shader ) const;
	
private:
	std::shared_ptr<VertexArray> mVertexArray;
	std::vector<std::shared_ptr<Texture>> mTextures;
	glm::mat4 mModel = glm::mat4{ 1.0f };
	glm::mat4 mView = glm::mat4{ 1.0f };
	glm::mat4 mProjection = glm::mat4{ 1.0f };
};

