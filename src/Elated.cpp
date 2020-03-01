#include "Elated.hpp"

using namespace std;

double clockToMilliseconds(clock_t ticks) { ///https://stackoverflow.com/questions/28530798/how-to-make-a-basic-fps-counter
	// units/(units/time) => time (seconds) * 1000 = milliseconds
	return (ticks / (double)CLOCKS_PER_SEC) * 1000.0;
}

int supercode() {
	int res = scr::initialize();
	if (res != 0)return res;
	res = win::initialize();
	if (res != 0)return res;
	res = game::initialize();
	if (res != 0)return res;
	clock_t dt = 0;
	unsigned int frames = 0;
	double  frameRate = 60;
	double  averageFrameTimeMilliseconds = 33.333;
	while (!win::shouldClose()) {
		win::loop();
		clock_t begin = clock();
		game::render();
		clock_t end = clock();
		dt += end - begin;
		frames++;
		game::update(dt);
		if (clockToMilliseconds(dt) > 1000.0) { //every second
			frameRate = (double)frames * 0.5 + frameRate * 0.5; //more stable
			frames = 0;
			dt -= CLOCKS_PER_SEC;
			averageFrameTimeMilliseconds = 1000.0 / (frameRate == 0 ? 0.001 : frameRate);

			std::cout << "CPU time was:" << averageFrameTimeMilliseconds << std::endl;
		}
	}
	return 0;
}

int main()
{
	int result = supercode();
	if (result != 0) {
		printf("Game did not initialize properly!\n");
	}
	gfx::quit();
	scr::quit();
	win::quit();
	return result;
}
