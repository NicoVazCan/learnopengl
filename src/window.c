#include "window.h"


int
CreateWindow(Window *window)
{
  window->sdlWindow = SDL_CreateWindow(
    "SDL Tutorial",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
  );
  if (!window->sdlWindow)
    return SDL_SetError(
      "Window could not be created! SDL Error: %s",
      SDL_GetError()
    );

  window->sdlGLCtx = SDL_GL_CreateContext(window->sdlWindow);
  if (!window->sdlGLCtx)
    return SDL_SetError(
      "OpenGL context could not be created! SDL Error: %s",
      SDL_GetError()
    );

  if (SDL_GL_SetSwapInterval( 1 ) < 0)
    return SDL_SetError(
      "Unable to set VSync! SDL Error: %s",
      SDL_GetError()
    );

  return 0;
}

void
DestroyWindow(const Window *window)
{
  SDL_GL_DeleteContext(window->sdlGLCtx);
  SDL_DestroyWindow(window->sdlWindow);
}


void
SwapWindow(const Window *window)
{
  SDL_GL_SwapWindow(window->sdlWindow);
}