#include "draw.h"

#include "shader.h"
#include <SDL2/SDL.h>

#define VTX_SHDR_SRC  "../media/shaders/vertex.shader"
#define FRAG_SHDR_SRC "../media/shaders/fragment.shader"

#define TEX0_BMP "../media/box.bmp"
#define TEX1_BMP "../media/awesomeface.bmp"


static const float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

static const unsigned indices[] = {  
  0, 1, 3, // first triangle
  1, 2, 3  // second triangle
};

static int LoadBMP(const char *bmp, int hasAlpha)
{
  SDL_Surface *surf;
  Uint32 sdlFrmt;
  GLenum glFrmt;

  if (hasAlpha)
  {
    sdlFrmt = SDL_PIXELFORMAT_RGBA32;
    glFrmt = GL_RGBA;
  }
  else
  {
    sdlFrmt = SDL_PIXELFORMAT_RGB24;
    glFrmt = GL_RGB;
  }

  surf = SDL_LoadBMP(bmp);
  if (!surf)
    return SDL_SetError("Failed to load texture: %s", SDL_GetError());

  if ((surf->w & (surf->w - 1)) != 0 )
    SDL_LogWarn(SDL_LOG_CATEGORY_CUSTOM, "%s width is not a power of 2", bmp);

  if ((surf->h & (surf->h - 1)) != 0 )
    SDL_LogWarn(SDL_LOG_CATEGORY_CUSTOM, "%s height is not a power of 2", bmp);

  {
    SDL_Surface *frmtSurf;

    frmtSurf = SDL_ConvertSurfaceFormat(surf, sdlFrmt, 0);
    if (!frmtSurf)
      return SDL_SetError("Failed to convert texture: %s", SDL_GetError());

    SDL_FreeSurface(surf);
    surf = frmtSurf;
  }

  if (SDL_MUSTLOCK(surf))
  {
    if (SDL_LockSurface(surf))
      return SDL_SetError("Failed to lock surface: %s", SDL_GetError());
    glTexImage2D(GL_TEXTURE_2D, 0, glFrmt, surf->w, surf->h, 0, glFrmt, GL_UNSIGNED_BYTE, surf->pixels);
    SDL_UnlockSurface(surf);
  }
  else
    glTexImage2D(GL_TEXTURE_2D, 0, glFrmt, surf->w, surf->h, 0, glFrmt, GL_UNSIGNED_BYTE, surf->pixels);
  
  SDL_FreeSurface(surf);

  return 0;
}

int
InitDraw(Draw *draw)
{
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    return SDL_SetError(
      "Failed to initialize GLAD"
    );

  if (CreateProgram(&draw->prg, VTX_SHDR_SRC, FRAG_SHDR_SRC))
    return SDL_SetError(
      "Failed to create shader program: %s",
      SDL_GetError()
    );

  glGenVertexArrays(1, &draw->VAO);
  glGenBuffers(1, &draw->VBO);
  glGenBuffers(1, &draw->EBO);

  glBindVertexArray(draw->VAO);

  glBindBuffer(GL_ARRAY_BUFFER, draw->VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, draw->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (void*)(3 * sizeof (float)));
  glEnableVertexAttribArray(1);
  // texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (void*)(6 * sizeof (float)));
  glEnableVertexAttribArray(2); 


  glGenTextures(2, draw->texs);


  glBindTexture(GL_TEXTURE_2D, draw->texs[0]);

  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  if (LoadBMP(TEX0_BMP, 0))
    return SDL_SetError("Failed to load texture: %s", SDL_GetError());
  glGenerateMipmap(GL_TEXTURE_2D);


  glBindTexture(GL_TEXTURE_2D, draw->texs[1]);

  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  if (LoadBMP(TEX1_BMP, 1))
    return SDL_SetError("Failed to load texture: %s", SDL_GetError());
  glGenerateMipmap(GL_TEXTURE_2D);


  glUseProgram(draw->prg);
  glUniform1i(glGetUniformLocation(draw->prg, "texture1"), 0);
  glUniform1i(glGetUniformLocation(draw->prg, "texture2"), 1);

  return 0;
}

void
DoDraw(const Draw *draw)
{
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);

  glActiveTexture(GL_TEXTURE0); 
  glBindTexture(GL_TEXTURE_2D, draw->texs[0]);
  glActiveTexture(GL_TEXTURE1); 
  glBindTexture(GL_TEXTURE_2D, draw->texs[1]);

  glUseProgram(draw->prg);
  glBindVertexArray(draw->VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void
QuitDraw(const Draw *draw)
{
  glDeleteVertexArrays(1, &draw->VAO);
  glDeleteBuffers(1, &draw->VBO);
  glDeleteBuffers(1, &draw->EBO);
  glDeleteTextures(2, draw->texs);
  glDeleteProgram(draw->prg);
}
