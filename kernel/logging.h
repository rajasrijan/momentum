#ifndef LOGGING_H
#define LOGGING_H

#define log_error(...) log(ERROR, __VA_ARGS__)
#define log_warning(...) log(WARNING, __VA_ARGS__)
#define log_info(...) log(INFO, __VA_ARGS__)
#define log_debug(...) log(DEBUG, __VA_ARGS__)
#define log_all(...) log(ALL, __VA_ARGS__)

#ifdef __cplusplus
extern "C"
{
#endif //    __cplusplus
    enum log_lvl
    {
        ERROR = 0,
        WARNING,
        INFO,
        DEBUG,
        ALL
    };
    extern int current_log_level;
    void set_log_lvl(int log_lvl);
    int log(const int log_lvl, const char *format, ...);
#ifdef __cplusplus
}
#endif //    __cplusplus
#endif //    LOGGING_H