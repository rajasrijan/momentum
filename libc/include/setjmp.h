#ifndef SETJMP_H
#define SETJMP_H
typedef struct __jmp_buf
{

} jmp_buf;

typedef struct __sigjmp_buf
{

} sigjmp_buf;

int setjmp(jmp_buf env);
int sigsetjmp(sigjmp_buf env, int savesigs);

void longjmp(jmp_buf env, int val);
void siglongjmp(sigjmp_buf env, int val);
#endif //SETJMP_H