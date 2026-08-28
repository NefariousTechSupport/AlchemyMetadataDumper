#ifndef ALCHEMY_SUPPORT_H
#define ALCHEMY_SUPPORT_H



typedef __builtin_va_list va_list;

#define va_start(v,l) __builtin_va_start(v,l)
#define va_end(v) __builtin_va_end(v)



#endif // ALCHEMY_SUPPORT_H
