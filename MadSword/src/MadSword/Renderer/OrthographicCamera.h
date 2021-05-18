#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

namespace MadSword
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left,float right,float bottom,float top);

		void SetPosition(glm::vec3& position);
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetRotation(glm::vec3& rotation);
		const glm::vec3& GetRotation() const { return m_Rotation; }
		void SetScale(glm::vec3& scale);
		const glm::vec3& GetScale() const { return m_Scale; }

		const glm::mat4& GetProjectionMatrix() const { return m_Projection; }
		const glm::mat4& GetViewMatrix() const { return m_View; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjection; }
	private:
		void Update();
	private:
		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::mat4 m_ViewProjection;
		
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
		
	};
}
