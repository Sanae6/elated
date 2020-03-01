#include "Window.hpp"
namespace win {
	GLFWwindow* _window;
	bool _init = false;
	bool closed = false;
	bool shouldClose() {
		return closed;
	}
	void setShouldClose(bool close) {
		closed = close;
	}

	namespace scripts {
		duk_ret_t setTitle(duk_context* ctx) {
			const char* c = duk_require_string(ctx, 0);
			printf("%s\n",c);
			glfwSetWindowTitle(_window, c);
			return 0;
		}
		duk_ret_t shouldClose(duk_context* ctx) {
			duk_push_boolean(ctx, win::shouldClose());
			return 0;
		}
		duk_ret_t setShouldClose(duk_context* ctx) {
			win::setShouldClose(duk_get_boolean(ctx, 0));
			return 0;
		}
		duk_ret_t keyCallback(duk_context* ctx) {
			duk_dup(ctx, 0);
			int t = duk_get_type(ctx, 0);
			duk_put_global_string(ctx, "_keyCallback");
			printf("ok boomer %d\n",t);
			return 0;
		}
	}
	void _framebufferCB(GLFWwindow* win, int w, int h) {
		glViewport(0, 0, w, h);
	}
	char* act2string(int action) {
		if (action == GLFW_PRESS) return "press";
		if (action == GLFW_RELEASE) return "release";
		if (action == GLFW_REPEAT) return "hold";
		return "help me";
	}
	void _keyCB(GLFWwindow* win, int key, int scancode, int action, int mods) {
		duk_bool_t result = duk_get_global_string(scr::_context, "_keyCallback");
		if (result) {
			duk_push_int(scr::_context, key);
			duk_push_string(scr::_context, act2string(action));
			if (duk_pcall(scr::_context, 2) != 0) {
				printf("Key Callback failed to run: %s\n", duk_safe_to_string(scr::_context, -1));
			}
			duk_pop(scr::_context);
		}
	}
	void* glProxy(const char* proc) {
		if (strncmp(proc,"GL_",strlen("GL_"))) {
			
		}
		return NULL;
	}
	int initialize() {
		if (_init) {
			printf("Window already initialized! You shouldn't be seeing this\n");
			return 1;
		}
		if (!glfwInit()) {
			const char* error;
			glfwGetError(&error);
			printf("GLFW error! Error: %s\n", error);
			return 1;
		};
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		_window = glfwCreateWindow(600, 600, "Romantic", 0, 0);
		if (!_window) {
			const char* error;
			glfwGetError(&error);
			printf("Could not create window! Error: %s", error);
			return 1;
		}
		glfwSetFramebufferSizeCallback(_window, _framebufferCB);
		glfwSetKeyCallback(_window, _keyCB);
		glfwMakeContextCurrent(_window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			printf("Failed to initialize GLAD\n");
			return 1;
		}
		duk_push_global_object(scr::_context);
		duk_push_object(scr::_context);
		const duk_function_list_entry funcs[] = {
			{"setTitle", scripts::setTitle, 1},
			{"shouldClose", scripts::shouldClose, 0},
			{"setShouldClose", scripts::setShouldClose, 1},
			{"onKeyAction",scripts::keyCallback, 1},
			{ NULL, NULL, 0 }
		};
		duk_put_function_list(scr::_context, -1, funcs);
		duk_put_global_string(scr::_context, "win");

		duk_gl_push_opengl_bindings(scr::_context);
		return 0;
	};
	void loop() {
		glfwPollEvents();
		if (closed) {
			glfwSetWindowShouldClose(_window, true);
		}
	}
	void quit() {
		if (_init) return;
		glfwTerminate();
	}
}