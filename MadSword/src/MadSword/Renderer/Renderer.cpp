#include "mspch.h"

#include "Renderer.h"
#include "RenderCommand.h"

namespace MadSword {
	Renderer::SceneData* Renderer::m_ScaneData = new SceneData();
	
	void Renderer::BeginScene(std::shared_ptr<OrthographicCamera>& camera)
	{
		m_ScaneData->camera = camera.get();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader,
		const std::shared_ptr<VertexArray>& vertexArray)
	{
		OrthographicCamera* camera = m_ScaneData->camera;
		shader->Bind();
		shader->SetMat4("VP", camera->GetViewProjectionMatrix());
		RenderCommand::DrawIndexed(vertexArray);
	}

}
