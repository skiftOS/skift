CC:=i686-pc-skift-gcc
CXX:=i686-pc-skift-g++
LD:=i686-pc-skift-ld
LDFLAGS:= \
	--sysroot=$(SYSROOT)
AR:=i686-pc-skift-ar
ARFLAGS:=rcs
AS=nasm
ASFLAGS=-f elf32
STRIP:=i686-pc-skift-strip
