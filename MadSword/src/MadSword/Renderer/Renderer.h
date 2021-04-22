#pragma once
#include "RendererAPI.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace MadSword {
	class Renderer {
	public:
		static void BeginScene(std::shared_ptr<OrthographicCamera>& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader,const std::shared_ptr<VertexArray>& vertexArray);
		
		static RendererAPI::API GetAPI() { return RendererAPI::Type(); }
	private:
		struct SceneData
		{
			OrthographicCamera* camera;
		};
	private:
		static SceneData* m_ScaneData;
	};
}
