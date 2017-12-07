#ifdef __cplusplus
extern "C" {
#endif
extern int get_spin_lock(void *);
extern int release_spin_lock(void *);
void printf(const char *format, ...);
int __cxa_guard_acquire(unsigned long int *guard_object);
void __cxa_guard_release(unsigned long int *guard_object);
#define assert(x)                     \
	if (x != 0)                       \
	{                                 \
		printf("Assert at [%s]", #x); \
		__asm__("cli;hlt;");          \
	}
unsigned long int guard = 0;
int __cxa_guard_acquire(unsigned long int *guard_object)
{
	if (*guard_object)
		return 0;
	get_spin_lock(&guard);
	if (*guard_object)
	{
		release_spin_lock(&guard);
		return 0;
	}
	return 1;
}
void __cxa_guard_release(unsigned long int *guard_object)
{
	*guard_object = 1;
	release_spin_lock(&guard);
}
void *__dso_handle;
void *__gxx_personality_v0;
void __cxa_pure_virtual(void);

struct
{
	void (*destructor)(void *);
	void *arg;
	void *dso;
} __atexit_funcs[100];
unsigned int fn_count = 0;

int __cxa_atexit(void (*destructor)(void *), void *arg, void *dso);

int __cxa_atexit(void (*destructor)(void *), void *arg, void *dso)
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