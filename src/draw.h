#ifndef DRAW_H_
# define DRAW_H_

#include "glad/glad.h"


typedef struct Draw
{
  unsigned VBO, EBO, VAO, prg, texs[2];
} Draw;


# ifdef __cplusplus
extern "C" {
# endif

int InitDraw(Draw *draw);
void DoDraw(const Draw *draw);
void QuitDraw(const Draw *draw);

# ifdef __cplusplus
}
# endif

#endif