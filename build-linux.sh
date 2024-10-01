#!/bin/sh
# https://aleksander.es/data/valgrind-memcheck.pdf
#make clean; make debug; valgrind --verbose --leak-check=full --show-reachable=yes --leak-resolution=high --run-libc-freeres=no --log-file=supdata.log ./saVermadaRetro
make -f Makefile.linux clean; make -f Makefile.linux; gdb -ex run -ex quit --batch ./saVermadaRetro
#make debug; gdb -ex run -ex quit --batch ./saVermadaRetro
