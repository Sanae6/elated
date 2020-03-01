#include "Graphics.hpp"

namespace gfx {
	struct GraphicsData {

	};
	namespace scripts {

	}
	namespace shaders {
		Shader::Shader(const char* vertName, const char* fragName){
			bool hv = vertName != NULL;
			bool hf = fragName != NULL;
			if (hv) {
				const char* v = fs::readFileText(vertName);
				vertex = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(vertex,1,&v,(const GLint*)(&(sizeof (v))));
			}
			if (fragName != NULL)const char* f = fs::readFileText(fragName);
		}
		Shader* createShader(const char* name) {
			Shader* s = new Shader(name,name);
			return s;
		}
		void addShader(Shader* shader) {
			shaderlist.push_back(shader);
		}
	}
	std::vector<shaders::Shader*> shaderlist;
	bool initialize() {
		return 0;
	}
	void loopStart() {

	}
	void loopEnd() {

	}
	void quit() {

	}
}