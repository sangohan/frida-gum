noinst_LTLIBRARIES = libgum-tests-core.la

arch_sources = $(NULL)
arch_includes = $(NULL)
os_sources = $(NULL)
script_sources = $(NULL)

if ARCH_I386
arch_sources += \
	arch-x86/codewriter.c \
	arch-x86/functionparser.c \
	arch-x86/relocator.c \
	arch-x86/stalker-x86.c
arch_includes += \
	-I $(top_srcdir)/gum/arch-x86 \
	-I $(top_srcdir)/ext/udis86
endif

if ARCH_ARM
arch_sources += \
	arch-arm/armwriter.c \
	arch-arm/armrelocator.c \
	arch-arm/thumbwriter.c \
	arch-arm/thumbrelocator.c \
	arch-arm/stalker-arm.c
arch_includes += \
	-I $(top_srcdir)/gum/arch-arm
endif

if HAVE_V8
script_sources += script.c
endif

libgum_tests_core_la_SOURCES = \
	$(arch_sources) \
	$(os_sources) \
	$(script_sources) \
	closure.c \
	memory.c \
	process.c \
	symbolutil.c \
	backtracer.c \
	interceptor.c

EXTRA_DIST = \
             targetfunctions.c \
             specialfunctions.c

AM_CPPFLAGS = \
	-include config.h \
	-I $(top_srcdir) \
	-I $(top_srcdir)/gum \
	-I $(top_srcdir)/libs \
	-I $(top_srcdir)/tests \
	-I $(top_srcdir)/tests/stubs \
	$(arch_includes) \
	$(GUM_CFLAGS)
