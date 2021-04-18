#pragma once

namespace MadSword {
	class Shader {
	public:
		Shader(const std::string& vertSrc, const std::string& fragSrc);
		~Shader();
		void Bind();
		void UnBind();
	private:
		unsigned int m_RendererID;
	};
}