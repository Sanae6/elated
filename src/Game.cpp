#include "Game.hpp"
namespace game {
	int initialize() {
		glViewport(0, 0, 600, 600);
		scr::evalFile("game.js");
		glClearColor(1, 0, 0, 1);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glEnable(GL_DEPTH_TEST);
		return 0;
	}
	void render() {
	
	}
	void update(clock_t delta) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		duk_get_global_string(scr::_context, "gameloop");
		if (!duk_is_null_or_undefined(scr::_context, 0)) {
			duk_push_number(scr::_context, 1.0);
			if (duk_pcall(scr::_context, 1) != 0) {
				printf("loop failed: %s\n", duk_safe_to_string(scr::_context,-1));
			}
			duk_pop(scr::_context);
		}
		glfwSwapBuffers(win::_window);
	}
}