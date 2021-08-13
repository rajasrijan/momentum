/*
 * Copyright 2009-2021 Srijan Kumar Sharma
 *
 * This file is part of Momentum.
 *
 * Momentum is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Momentum is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Momentum.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef __cplusplus
extern "C"
{
#endif
    extern int get_spin_lock(void *);
    extern int release_spin_lock(void *);
    void printf(const char *format, ...);
    int __cxa_guard_acquire(unsigned long int *guard_object);
    void __cxa_guard_release(unsigned long int *guard_object);
#define assert(x)                                                                                                                                              \
    if (x != 0)                                                                                                                                                \
    {                                                                                                                                                          \
        printf("Assert at [%s]", #x);                                                                                                                          \
        __asm__("cli;hlt;");                                                                                                                                   \
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
        return 0;
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