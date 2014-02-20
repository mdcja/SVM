#
# Author - Julian Martinez del Campo
# Desc   - Makefile for Assignment 3
#

CC = gcc
CFLAGS = -Wall -g -ansi -std=c99
LFLAGS = -lm
TESTDIR = tests

all: debug svm

#debug: CFLAGS += -DDEBUG_INST
#debug: CFLAGS += -DDEBUG_MEM
#debug: CFLAGS += -DDEBUG_REG
#debug: CFLAGS += -DDEBUG_FLAGS
#debug: CFLAGS += -fprofile-arcs -ftest-coverage
debug: debug.c memory.o cpu.o 
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

svm: svm.o memory.o cpu.o
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

svm.o: svm.c cpu.h
	$(CC) $(CFLAGS) -c svm.c

memory.o: memory.h memory.c
	$(CC) $(CFLAGS) -c memory.c

cpu.o: cpu.h cpu.c
	$(CC) $(CFLAGS) -c cpu.c

test: svm 
	# run Test1.o
	cat $(TESTDIR)/Test1.o | ./svm > $(TESTDIR)/Test1.RUN
	-diff $(TESTDIR)/Test1.RUN $(TESTDIR)/Test1.CORRECT
	# run factors.o
	cat $(TESTDIR)/factors.o | ./svm > $(TESTDIR)/factors.RUN
	-diff $(TESTDIR)/factors.RUN $(TESTDIR)/factors.CORRECT
	# run pfactors.o
	cat $(TESTDIR)/pfactors.o | ./svm > $(TESTDIR)/pfactors.RUN
	-diff $(TESTDIR)/pfactors.RUN $(TESTDIR)/pfactors.CORRECT
	# run willjump.o
	cat $(TESTDIR)/willjump.o | ./svm > $(TESTDIR)/willjump.RUN
	-diff $(TESTDIR)/willjump.RUN $(TESTDIR)/willjump.CORRECT
	# run willshake.o
	cat $(TESTDIR)/willshake.o | ./svm > $(TESTDIR)/willshake.RUN
	-diff $(TESTDIR)/willshake.RUN $(TESTDIR)/willshake.CORRECT
	# run halt.o
	#./sasm.lion < $(TESTDIR)/halt.svm > $(TESTDIR)/halt.o
	cat $(TESTDIR)/halt.o | ./svm > $(TESTDIR)/halt.RUN
	-diff $(TESTDIR)/halt.RUN $(TESTDIR)/halt.CORRECT
	# run load.o
	#./sasm.lion < $(TESTDIR)/load.svm > $(TESTDIR)/load.o
	cat $(TESTDIR)/load.o | ./svm > $(TESTDIR)/load.RUN
	-diff $(TESTDIR)/load.RUN $(TESTDIR)/load.CORRECT
	# run out.o
	#./sasm.lion < $(TESTDIR)/out.svm > $(TESTDIR)/out.o
	cat $(TESTDIR)/out.o | ./svm > $(TESTDIR)/out.RUN
	-diff $(TESTDIR)/out.RUN $(TESTDIR)/out.CORRECT
	# run add.o
	#./sasm.lion < $(TESTDIR)/add.svm > $(TESTDIR)/add.o
	cat $(TESTDIR)/add.o | ./svm > $(TESTDIR)/add.RUN
	-diff $(TESTDIR)/add.RUN $(TESTDIR)/add.CORRECT
	# run sub.o
	#./sasm.lion < $(TESTDIR)/sub.svm > $(TESTDIR)/sub.o
	cat $(TESTDIR)/sub.o | ./svm > $(TESTDIR)/sub.RUN
	-diff $(TESTDIR)/sub.RUN $(TESTDIR)/sub.CORRECT
	# run jmp.o
	#./sasm.lion < $(TESTDIR)/jmp.svm > $(TESTDIR)/jmp.o
	cat $(TESTDIR)/jmp.o | ./svm > $(TESTDIR)/jmp.RUN
	-diff $(TESTDIR)/jmp.RUN $(TESTDIR)/jmp.CORRECT
	# run jmp2.o
	#./sasm.lion < $(TESTDIR)/jmp2.svm > $(TESTDIR)/jmp2.o
	cat $(TESTDIR)/jmp2.o | ./svm > $(TESTDIR)/jmp2.RUN
	-diff $(TESTDIR)/jmp2.RUN $(TESTDIR)/jmp.CORRECT
	# run jmp3.o
	#./sasm.lion < $(TESTDIR)/jmp3.svm > $(TESTDIR)/jmp3.o
	cat $(TESTDIR)/jmp3.o | ./svm > $(TESTDIR)/jmp3.RUN
	-diff $(TESTDIR)/jmp3.RUN $(TESTDIR)/jmp.CORRECT
	# run jmp4.o
	#./sasm.lion < $(TESTDIR)/jmp4.svm > $(TESTDIR)/jmp4.o
	cat $(TESTDIR)/jmp4.o | ./svm > $(TESTDIR)/jmp4.RUN
	-diff $(TESTDIR)/jmp4.RUN $(TESTDIR)/jmp.CORRECT
	# run jmp5.o
	#./sasm.lion < $(TESTDIR)/jmp5.svm > $(TESTDIR)/jmp5.o
	cat $(TESTDIR)/jmp5.o | ./svm > $(TESTDIR)/jmp5.RUN
	-diff $(TESTDIR)/jmp5.RUN $(TESTDIR)/jmp5.CORRECT
	# run err1.o
	#./sasm.lion < $(TESTDIR)/err1.svm > $(TESTDIR)/err1.o
	cat $(TESTDIR)/err1.o | ./svm > $(TESTDIR)/err1.RUN
	-diff $(TESTDIR)/err1.RUN $(TESTDIR)/err1.CORRECT
	# run err2.o
	#./sasm.lion < $(TESTDIR)/err2.svm > $(TESTDIR)/err2.o
	cat $(TESTDIR)/err2.o | ./svm > $(TESTDIR)/err2.RUN
	-diff $(TESTDIR)/err2.RUN $(TESTDIR)/err2.CORRECT
	# run err3.o
	#./sasm.lion < $(TESTDIR)/err3.svm > $(TESTDIR)/err3.o
	cat $(TESTDIR)/err3.o | ./svm > $(TESTDIR)/err3.RUN
	-diff $(TESTDIR)/err3.RUN $(TESTDIR)/err3.CORRECT

testdebug: debug
	# run Test1.o
	cat $(TESTDIR)/Test1.o | ./debug > $(TESTDIR)/Test1.DEBUG
	# run factors.o
	cat $(TESTDIR)/factors.o | ./debug > $(TESTDIR)/Test1.DEBUG
	# run pfactors.o
	cat $(TESTDIR)/pfactors.o | ./debug > $(TESTDIR)/Test1.DEBUG
	# run halt.o
	cat $(TESTDIR)/halt.o | ./debug > $(TESTDIR)/Test1.DEBUG
	# run load.o
	cat $(TESTDIR)/load.o | ./debug > $(TESTDIR)/load.DEBUG
	# run out.o
	cat $(TESTDIR)/out.o | ./debug > $(TESTDIR)/out.DEBUG
	# run add.o
	cat $(TESTDIR)/add.o | ./debug > $(TESTDIR)/add.DEBUG
	# run sub.o
	cat $(TESTDIR)/sub.o | ./debug > $(TESTDIR)/sub.DEBUG
	# run jmp.o
	cat $(TESTDIR)/jmp.o | ./debug > $(TESTDIR)/jmp.DEBUG
	# run jmp2.o
	cat $(TESTDIR)/jmp2.o | ./debug > $(TESTDIR)/jmp2.DEBUG
	# run jmp3.o
	cat $(TESTDIR)/jmp3.o | ./debug > $(TESTDIR)/jmp3.DEBUG
	# run jmp4.o
	cat $(TESTDIR)/jmp4.o | ./debug > $(TESTDIR)/jmp4.DEBUG
	# run jmp5.o
	cat $(TESTDIR)/jmp5.o | ./debug > $(TESTDIR)/jmp5.DEBUG
	# run err1.o
	cat $(TESTDIR)/err1.o | ./debug > $(TESTDIR)/err1.DEBUG
	# run err2.o
	cat $(TESTDIR)/err2.o | ./debug > $(TESTDIR)/err2.DEBUG
	# run err3.o
	cat $(TESTDIR)/err3.o | ./debug > $(TESTDIR)/err3.DEBUG

clean:
	# remove executables
	-rm svm debug
	# remove object files
	-rm svm.o cpu.o memory.o
	# remove gcov files
	-rm *.gcov *.gcno *.gcda
	# remove test runs
	-rm $(TESTDIR)/*.RUN 

tar: 
	tar -cf MartinezDelCampoJ.tar *.h *.c Makefile README tests .git

gz: MartinezDelCampoJ.tar
	gzip MartinezDelCampoJ.tar
