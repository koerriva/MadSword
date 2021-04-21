#pragma once
#include "MadSword/Renderer/RendererAPI.h"

namespace MadSword
{
	class OpenGLRendererAPI :public RendererAPI
	{
	public:
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;
		void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
		
	};
}
