#include "mspch.h"
#include "OrthographicCamera.h"

namespace MadSword
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
	{
		m_Projection = ortho(left, right, bottom, top,-1.f,1.f);
		m_View = mat4(1);
		m_ViewProjection = m_Projection * m_View;
	}
	void OrthographicCamera::SetPosition(glm::vec3& position)
	{
		m_Position = position;
		Update();
	}
	void OrthographicCamera::SetRotation(glm::vec3& rotation)
	{
		m_Rotation = rotation;
		Update();
	}
	void OrthographicCamera::Update()
	{
		mat4 transform = translate(mat4(1.0), m_Position) *
			rotate(mat4(1.0), m_Rotation.x, vec3(1, 0, 0)) *
			rotate(mat4(1.0), m_Rotation.y, vec3(0, 1, 0)) *
			rotate(mat4(1.0), m_Rotation.z, vec3(0, 0, 1));
		
		m_View = inverse(transform);
		//m_View = lookAt(m_Position, vec3(0.0f), glm::vec3(0.f, 1.0f, 0.0f));
		m_ViewProjection = m_Projection * m_View;
	}
}
