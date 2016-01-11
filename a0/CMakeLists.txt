cmake_minimum_required(VERSION 2.8)

set(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_C_COMPILER /u/wbcowan/gnuarm-4.0.2/arm-elf/bin/gcc)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

project("CS452 Assignment 0" NONE)

set(A0_OUTPUT "a0_v6lai.elf")
set(LIB_COLLECTIONS "libcollections")

set(XCC "gcc")
set(AS "as")
set(AR "ar")
set(LD "ld")

set(CFLAGS -g -c -fPIC -Wall -I. -I../include -I../src -I../src/collections -I../src/clock -I../src/io -I../src/trains -mcpu=arm920t -msoft-float)
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
    OUTPUT circular_buffer.o
    COMMAND ${XCC} ${CFLAGS} -o circular_buffer.o ../src/collections/circular_buffer.c
    DEPENDS src/collections/circular_buffer.c src/collections/circular_buffer.h
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
    OUTPUT clock.o
    COMMAND ${XCC} ${CFLAGS} -o clock.o ../src/clock/clock.c
    DEPENDS src/clock/clock.c src/clock/clock.h
    )

add_custom_command(
    OUTPUT cio.o
    COMMAND ${XCC} ${CFLAGS} -o cio.o ../src/io/cio.c
    DEPENDS src/io/cio.c src/io/cio.h
    )

add_custom_command(
    OUTPUT io_controller.o
    COMMAND ${XCC} ${CFLAGS} -o io_controller.o ../src/io/io_controller.c
    DEPENDS src/io/io_controller.c src/io/io_controller.h
    )

add_custom_command(
    OUTPUT pretty_io.o
    COMMAND ${XCC} ${CFLAGS} -o pretty_io.o ../src/io/pretty_io.c
    DEPENDS src/io/pretty_io.c src/io/pretty_io.h
    )

add_custom_command(
    OUTPUT pio.o
    COMMAND ${XCC} ${CFLAGS} -o pio.o ../src/io/pio.c
    DEPENDS src/io/pio.c src/io/pio.h
    )

add_custom_command(
    OUTPUT switches.o
    COMMAND ${XCC} ${CFLAGS} -o switches.o ../src/trains/switches.c
    DEPENDS src/trains/switches.c src/trains/switches.h
    )

add_custom_command(
    OUTPUT timer.o
    COMMAND ${XCC} ${CFLAGS} -o timer.o ../src/clock/timer.c
    DEPENDS src/clock/timer.c src/clock/timer.h
    )

add_custom_command(
    OUTPUT trains.o
    COMMAND ${XCC} ${CFLAGS} -o trains.o ../src/trains/trains.c
    DEPENDS src/trains/trains.c src/trains/trains.h
    )

add_custom_command(
    OUTPUT trains_controller.o
    COMMAND ${XCC} ${CFLAGS} -o trains_controller.o ../src/trains/trains_controller.c
    DEPENDS src/trains/trains_controller.c src/trains/trains_controller.h
    )

add_custom_command(
    OUTPUT a0.o
    COMMAND ${XCC} ${CFLAGS} -o a0.o ../src/main.c
    DEPENDS src/main.c src/io/pio.h
    )

set(OBJECT_FILES
    a0.o
    clock.o
    cio.o
    io_controller.o
    pretty_io.o
    pio.o
    switches.o
    timer.o
    trains.o
    trains_controller.o
    )

add_custom_command(
    OUTPUT a0.elf
    COMMAND ${LD} ${LDFLAGS} -o a0.elf ${OBJECT_FILES} -lcollections -lgcc
    DEPENDS ${OBJECT_FILES}
    )

add_custom_target(
    ${A0_OUTPUT}
    ALL
    DEPENDS a0.elf
    )


add_custom_command(
    TARGET ${A0_OUTPUT}
    POST_BUILD
    COMMAND ../copy_elf.sh
    DEPENDS a0.elf
    )

add_dependencies(
    ${A0_OUTPUT}
    ${LIB_COLLECTIONS}
    )

