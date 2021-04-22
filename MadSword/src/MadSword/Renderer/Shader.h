#pragma once

#include <glm/glm.hpp>
using namespace glm;
namespace MadSword {
	class Shader {
	public:
		virtual ~Shader() {};
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void SetMat4(const std::string& name,const mat4& value) = 0;

		static Shader* Create(const std::string& vertSrc, const std::string& fragSrc);
	};
}