#include <SDL2/SDL.h>
#include "glad/glad.h"

#include "window.h"
#include "draw.h"


static inline int Init(void)
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    return SDL_SetError(
      "SDL could not initialize! SDL Error: %s",
      SDL_GetError()
    );

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  return 0;
}

static inline void Quit(void)
{
  SDL_Quit();
}


#define MSPF 33

int main(SDL_UNUSED int argc, SDL_UNUSED const char *argv[])
{
  Window window;
  Draw draw;
  int run = 1;

  if (Init())
  {
    SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Failed to init: %s", SDL_GetError());
    return -1;
  }

  if (CreateWindow(&window))
  {
    SDL_LogError(
      SDL_LOG_CATEGORY_CUSTOM,
      "Failed to create window: %s",
      SDL_GetError()
    );
    return -1;
  }

  if (InitDraw(&draw))
  {
    SDL_LogError(
      SDL_LOG_CATEGORY_CUSTOM,
      "Failed to init draw: %s",
      SDL_GetError()
    );
    return -1;
  }
  

  while (run)
  {
    SDL_Event event;
    Uint32 ticks, delta;

    ticks = SDL_GetTicks();

    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT:
        run = 0;
        break;
      default:
        break;
      }
    }

    DoDraw(&draw);
    SwapWindow(&window);

    delta = SDL_GetTicks() - ticks;
    if (delta < MSPF)
      SDL_Delay(MSPF - delta);
  }


  QuitDraw(&draw);

  DestroyWindow(&window);

  Quit();

  return 0;
}