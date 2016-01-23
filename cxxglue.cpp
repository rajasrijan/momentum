#ifdef __cplusplus
extern "C"
{
#endif

void printf(const char *format, ...);
#define assert(x) if(x!=0){printf("Assert at [%s]",#x);__asm__("cli;hlt;");}

void *__dso_handle;
void *__gxx_personality_v0;
void __cxa_pure_virtual(void);

struct
{
    void (*destructor) (void *);
    void *arg;
    void *dso;
} __atexit_funcs[100];
unsigned int fn_count = 0;

int __cxa_atexit(void (*destructor) (void *), void *arg, void *dso);

int __cxa_atexit(void (*destructor) (void *), void *arg, void *dso)
{
    __atexit_funcs[fn_count].destructor = destructor;
    __atexit_funcs[fn_count].arg = arg;
    __atexit_funcs[fn_count].dso = dso;
    fn_count++;
}

void __cxa_pure_virtual()
{
    // Do nothing or print an error message.
    assert(1);
}
namespace __cxxabiv1
{
void *vtable[100];
}
#ifdef __cplusplus
}
#endif