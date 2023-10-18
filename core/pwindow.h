#ifndef PWINDOW_H
#define PWINDOW_H
#include <SDL2/SDL.h>

#include <memory>
#include <string>

enum class WINDOW_TYPE{
	SDL,
	VULKAN
};

class PEventLoop;

struct SDLWindowDeleter {
	void operator()(SDL_Window* window) {
		if (window != nullptr) {
			SDL_DestroyWindow(window);
		}
	}
};

struct SDLRendererDeleter {
	void operator()(SDL_Renderer* renderer) {
		if (renderer != nullptr) {
			SDL_DestroyRenderer(renderer);
		}
	}
};

class PWindow {
   public:
	PWindow(const std::string& title, int x = SDL_WINDOWPOS_CENTERED, int y = SDL_WINDOWPOS_CENTERED, int w = 800,
			int h = 600, unsigned int flags = SDL_WINDOW_SHOWN);

	~PWindow() {
		_window.reset();
		_renderer.reset();
		SDL_Quit();
	}
	[[nodiscard]] auto type() const -> WINDOW_TYPE { return _type; }
	PEventLoop create_event_loop();

   private:
	std::string _title;
	int _x, _y, _w, _h;
	unsigned int _flags;
	std::shared_ptr<SDL_Window> _window;
	std::shared_ptr<SDL_Renderer> _renderer;
	WINDOW_TYPE _type;

};

#endif
