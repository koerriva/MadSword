#pragma once

#include <glm/glm.hpp>
using namespace glm;
namespace MadSword {
	class Shader {
	public:
		virtual ~Shader() = default;
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		static Shader* Create(const std::string& vertSrc, const std::string& fragSrc);
	};
}