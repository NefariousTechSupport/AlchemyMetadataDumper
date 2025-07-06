#ifndef ALCHEMY_SUPPORT_H
#define ALCHEMY_SUPPORT_H




typedef char* va_list;

#define va_start(v,l)   ((void)l,__builtin_va_info(&v))
#define va_end(v)       ((void)v)
// these are currently unused
//#define va_arg(v,l)   
//#define va_copy(d,s)  





#endif // ALCHEMY_SUPPORT_H