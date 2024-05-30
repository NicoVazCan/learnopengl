#include "shader.h"

#include "glad/glad.h"
#include <SDL2/SDL.h>


static inline int
LoadShader(unsigned *shdr, const char *file, GLenum shaderType)
{
  const char *src;

  SDL_assert(shdr);

  if (!(src = SDL_LoadFile(file, NULL)))
    return SDL_SetError("Failed to read \"%s\" file: %s", file, SDL_GetError());
  *shdr = glCreateShader(shaderType);
  glShaderSource(*shdr, 1, &src, NULL);

  return 0;
}

static inline int
CompileShader(unsigned shdr)
{
  int ok;

  glCompileShader(shdr);
  glGetShaderiv(shdr, GL_COMPILE_STATUS, &ok);
  if (!ok)
  {
    char log[512];

    glGetShaderInfoLog(shdr, 512, NULL, log);
    return SDL_SetError("Failed to compile shader: %s", log);
  }

  return 0;
}

int
CreateProgram(unsigned *prg, const char *vtxSrc, const char *fragSrc)
{
  unsigned vtxShdr, fragShdr;
  int ok;

  SDL_assert(prg);

  if (LoadShader(&vtxShdr, vtxSrc, GL_VERTEX_SHADER))
    return SDL_SetError("Failed to load vertex shader: %s", SDL_GetError());

  if (LoadShader(&fragShdr, fragSrc, GL_FRAGMENT_SHADER))
    return SDL_SetError("Failed to load fragment shader: %s", SDL_GetError());

  if (CompileShader(vtxShdr))
    return SDL_SetError("Failed to compile vertex shader: %s", SDL_GetError());

  if (CompileShader(fragShdr))
    return SDL_SetError(
      "Failed to compile fragment shader: %s",
      SDL_GetError()
    );

  *prg = glCreateProgram();
  glAttachShader(*prg, vtxShdr);
  glAttachShader(*prg, fragShdr);
  glLinkProgram(*prg);
  glGetProgramiv(*prg, GL_LINK_STATUS, &ok);
  if (!ok)
  {
    char log[512];

    glGetShaderInfoLog(*prg, 512, NULL, log);
    return SDL_SetError("Failed to link program: %s", log);
  }

  glDeleteShader(vtxShdr);
  glDeleteShader(fragShdr);

  return 0;
}