#pragma once

namespace MadSword {
	enum class RendererAPI
	{
		None=0,OpenGL=1,Direct12=2,Metal=3,Vulkan=4
	};

	class Renderer {
	public:
		inline static RendererAPI GetAPI() { return s_RendererAPI; }
	private:
		static RendererAPI s_RendererAPI;
	};
}