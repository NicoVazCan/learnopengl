#ifndef WINDOW_H_
# define WINDOW_H_

#include <SDL2/SDL.h>

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480


typedef struct Window
{
  SDL_Window *sdlWindow;
  SDL_GLContext *sdlGLCtx;
} Window;


# ifdef __cplusplus
extern "C" {
# endif

int CreateWindow(Window *window);
void DestroyWindow(const Window *window);
void SwapWindow(const Window *window);

# ifdef __cplusplus
}
# endif

#endif