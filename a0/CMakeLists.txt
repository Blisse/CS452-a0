cmake_minimum_required(VERSION 2.8)

set(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_C_COMPILER /u/wbcowan/gnuarm-4.0.2/arm-elf/bin/gcc)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

project("CS452 Assignment 0" NONE)

set(A0_OUTPUT "a0_v6lai.elf")
set(LIB_COLLECTIONS "libcollections")
set(LIB_IO "libpio")

set(XCC "gcc")
set(AS "as")
set(AR "ar")
set(LD "ld")

set(CFLAGS -g -c -fPIC -Wall -I. -I../include -I../src -I../src/collections -I../src/clock -I../src/io -mcpu=arm920t -msoft-float)
# -g: include hooks for gdb
# -c: only compile
# -fpic: emit position-independent code
# -Wall: report all warnings
# -mcpu=arm920t: generate code for the 920t architecture
# -msoft-float: use software for floating point

set(ASFLAGS -mcpu=arm920t -mapcs-32)
# -mapcs-32: always create a complete stack frame

set(ARFLAGS rcs)

set(LDFLAGS -init main -Map a0.map -N -T ../src/orex.ld -L/u/wbcowan/gnuarm-4.0.2/lib/gcc/arm-elf/4.0.2 -L../lib -L.)


add_custom_command(
    OUTPUT circular_buffer.s
    COMMAND ${XCC} -S ${CFLAGS} -o circular_buffer.s ../src/collections/circular_buffer.c
    DEPENDS src/collections/circular_buffer.c src/collections/circular_buffer.h
    )

add_custom_command(
    OUTPUT circular_buffer.o
    COMMAND ${AS} ${ASFLAGS} -o circular_buffer.o circular_buffer.s
    DEPENDS circular_buffer.s
    )

add_custom_command(
    OUTPUT libcollections.a
    COMMAND ${AR} ${ARFLAGS} libcollections.a circular_buffer.o
    DEPENDS circular_buffer.o
    )

add_custom_target(
    ${LIB_COLLECTIONS}
    DEPENDS libcollections.a
    )





add_custom_command(
    OUTPUT pio.s
    COMMAND ${XCC} -S ${CFLAGS} -o pio.s ../src/io/pio.c
    DEPENDS src/io/pio.c include/pio.h
    )

add_custom_command(
    OUTPUT pio.o
    COMMAND ${AS} ${ASFLAGS} -o pio.o pio.s
    DEPENDS pio.s
    )

add_custom_command(
    OUTPUT libpio.a
    COMMAND ${AR} ${ARFLAGS} libpio.a pio.o
    DEPENDS pio.o
    )

add_custom_target(
    ${LIB_IO}
    DEPENDS libpio.a
    )




add_custom_command(
    OUTPUT clock.s
    COMMAND ${XCC} -S ${CFLAGS} -o clock.s ../src/clock/clock.c
    DEPENDS src/clock/clock.c src/clock/clock.h
    )

add_custom_command(
    OUTPUT clock.o
    COMMAND ${AS} ${ASFLAGS} -o clock.o clock.s
    DEPENDS clock.s
    )

add_custom_command(
    OUTPUT a0.s
    COMMAND ${XCC} -S ${CFLAGS} -o a0.s ../src/main.c
    DEPENDS src/main.c
    )

add_custom_command(
    OUTPUT a0.o
    COMMAND ${AS} ${ASFLAGS} -o a0.o a0.s
    DEPENDS a0.s
    )

add_custom_command(
    OUTPUT a0.elf
    COMMAND ${LD} ${LDFLAGS} -o a0.elf a0.o clock.o -lpio -lcollections -lgcc
    DEPENDS a0.o clock.o
    )

add_custom_target(
    ${A0_OUTPUT}
    ALL
    DEPENDS a0.elf libpio.a
    )




add_custom_command(
    TARGET ${A0_OUTPUT}
    POST_BUILD
    COMMAND ../copy_elf.sh
    DEPENDS a0.elf
    )

add_dependencies(
    ${A0_OUTPUT}
    ${LIB_IO}
    ${LIB_COLLECTIONS}
    )

add_dependencies(
    ${LIB_IO}
    ${LIB_COLLECTIONS}
    )
