#pragma once

#include "RendererAPI.h"

namespace MadSword
{
	class RenderCommand
	{
	public:
		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);
	private:
		static RendererAPI* s_RendererAPI;
	};
}
