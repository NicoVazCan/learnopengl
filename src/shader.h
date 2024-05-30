#ifndef SHADER_H_
# define SHADER_H_

# ifdef __cplusplus
extern "C" {
# endif

int CreateProgram(unsigned *prg, const char *vtxSrc, const char *fragSrc);

# ifdef __cplusplus
}
# endif

#endif