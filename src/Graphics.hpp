#include "Scripting.hpp"
#include "Window.hpp"

namespace gfx {
	namespace shaders {
		struct Shader {
			unsigned int vertex;
			unsigned int fragment;
			GLuint program;
			Shader(const char*, const char*);
		};
		Shader* createShader();
	}
	extern std::vector<shaders::Shader*> shaderlist;
	bool initialize();
	void loopStart();
	void loopEnd();
	void quit();
}