#ifdef __cplusplus
typedef unsigned long	pthread_t;
extern "C" {
#endif

#if !defined(__x86_64__) || __SIZEOF_POINTER__ != 8
# error "x86_64 Architecture required"
#endif

#if !defined(__clang__)
# error "Compile with clang >:("
#endif

#if !defined(__GNUC__)
# error "GNUC Required"
#endif

#ifndef _SSIZE_T
# define _SSIZE_T
typedef __typeof__(long) ssize_t;
#endif

#ifndef _SIZE_T
# define _SIZE_T
typedef __typeof__(sizeof(int)) size_t;
#endif

#ifndef _UINTPTR_T
# define _UINTPTR_T
typedef __typeof__(sizeof(int)) uintptr_t;
#endif

#if !defined(__always_inline)
# define __always_inline __attribute__((always_inline))
#endif

static __always_inline uintptr_t	__get_tp(void)
{
	uintptr_t tp;
	__asm__ ("mov %%fs:0,%0" : "=r" (tp) );
	return tp;
}

struct __ptcb
{
	void (*__f)(void *);
	void *__x;
	struct __ptcb *__next;
};

struct __locale_map;

struct __locale_struct
{
	const struct __locale_map *_c[6];
};

typedef struct __locale_struct * locale_t;

struct __pthread
{
	struct __pthread *self;
#ifndef TLS_ABOVE_TP
	uintptr_t *dtv;
#endif
	struct __pthread *prev, *next;
	uintptr_t sysinfo;
#ifndef TLS_ABOVE_TP
#ifdef CANARY_PAD
	uintptr_t canary_pad;
#endif
	uintptr_t canary;
#endif
	int tid;
	int errno_val;
	volatile int detach_state;
	volatile int cancel;
	volatile unsigned char canceldisable, cancelasync;
	unsigned char tsd_used:1;
	unsigned char dlerror_flag:1;
	unsigned char *map_base;
	size_t map_size;
	void *stack;
	size_t stack_size;
	size_t guard_size;
	void *result;
	struct __ptcb *cancelbuf;
	void **tsd;
	struct
	{
		volatile void *volatile head;
		long off;
		volatile void *volatile pending;
	} robust_list;
	int h_errno_val;
	volatile int timer_id;
	locale_t locale;
	volatile int killlock[1];
	char *dlerror_buf;
	void *stdio_locks;
#ifdef TLS_ABOVE_TP
	uintptr_t canary;
	uintptr_t *dtv;
#endif
};

#if !defined(__EXIT_SUCCESS)
# define	__EXIT_SUCCESS	0
#endif

#if !defined(__EXIT_FAILURE)
# define	__EXIT_FAILURE	1
#endif

#ifndef __cplusplus
typedef struct __pthread * pthread_t;
#endif

#ifdef TLS_ABOVE_TP
#define __pthread_self() ((pthread_t)(__get_tp() - sizeof(struct __pthread) - TP_OFFSET))
#else
#define __pthread_self() ((pthread_t)__get_tp())
#endif

#if !defined(errno)
__attribute__((const)) static __always_inline int	*__errno_location(void)
{
	return &__pthread_self()->errno_val;
}

#define errno (*__errno_location())
#endif

static __always_inline long	__syscall_r(unsigned long __r)
{
	if (__r > -4096UL)
	{
		errno = -__r;
		return (-1);
	}
	return (__r);
}

static __always_inline long	__syscall3(long __n, long __a1, long __a2, long __a3)
{
	register unsigned long	r;

	__asm__ __volatile__ (
		"syscall" : "=a"(r) : "a"(__n), "D"(__a1),
		"S"(__a2), "d"(__a3) : "rcx", "r11", "memory");
	return (__syscall_r(r));
}

#if !defined(__NR_write)
# define __NR_write	1
#endif

static ssize_t	write(int __fd, const void *__buf, size_t __size)
{
	return ((ssize_t)__syscall3(__NR_write, __fd, (long)__buf, (long)__size));
}

#if !defined(__BUFSIZE)
# define	__BUFSIZE	14
#endif

#if !defined(__STDOUT_FILENO)
# define	__STDOUT_FILENO	1
#endif

int	main(int argc, char **argv, char *envp[])
{
	(void)argc; (void)argv; (void)envp;

	static const char _buf[__BUFSIZE] = "Hello, world!\n";

	const ssize_t _bytes = write(
		__STDOUT_FILENO, _buf, __BUFSIZE);

	if (_bytes != __BUFSIZE)
		return (__EXIT_FAILURE);
	
	return (__EXIT_SUCCESS);
}

#ifdef __cplusplus
}
#endif
