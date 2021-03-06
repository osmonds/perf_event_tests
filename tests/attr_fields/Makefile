CC = gcc
CFLAGS = -Wall -O2 -g -I../../include
LFLAGS =
LIB = ../../lib

all:	\
	disabled \
	enable_on_exec \
	exclusive \
	format_id_support \
	inherit \
	inherit_stat \
	read_format_range_check \
	sample_type_range_check

###

$(LIB)/test_utils.o:	$(LIB)/test_utils.c
	cd $(LIB) && make

$(LIB)/branches_testcode.o:	$(LIB)/branches_testcode.c
	cd $(LIB) && make

$(LIB)/matrix_multiply.o:	$(LIB)/matrix_multiply.c
	cd $(LIB) && make

$(LIB)/perf_helpers.o:	$(LIB)/perf_helpers.c
	cd $(LIB) && make

$(LIB)/detect_cache.o:	$(LIB)/detect_cache.c
	cd $(LIB) && make

###

inherit:	inherit.o $(LIB)/test_utils.o \
					$(LIB)/perf_helpers.o 
	$(CC) $(LFLAGS) -o inherit inherit.o \
				$(LIB)/test_utils.o \
				$(LIB)/perf_helpers.o -lpthread

inherit.o:	inherit.c
	$(CC) $(CFLAGS) -c inherit.c

###

inherit_stat:	inherit_stat.o $(LIB)/test_utils.o \
					$(LIB)/perf_helpers.o \
					$(LIB)/instructions_testcode.o
	$(CC) $(LFLAGS) -o inherit_stat inherit_stat.o \
				$(LIB)/test_utils.o \
				$(LIB)/instructions_testcode.o \
				$(LIB)/perf_helpers.o -lpthread

inherit_stat.o:	inherit_stat.c
	$(CC) $(CFLAGS) -c inherit_stat.c


###

disabled:	disabled.o $(LIB)/test_utils.o \
					$(LIB)/perf_helpers.o \
					$(LIB)/instructions_testcode.o
	$(CC) $(LFLAGS) -o disabled disabled.o \
				$(LIB)/test_utils.o \
				$(LIB)/perf_helpers.o \
				$(LIB)/instructions_testcode.o

disabled.o:	disabled.c
	$(CC) $(CFLAGS) -c disabled.c

###

enable_on_exec:	enable_on_exec.o $(LIB)/test_utils.o \
					$(LIB)/perf_helpers.o \
					$(LIB)/instructions_testcode.o
	$(CC) $(LFLAGS) -o enable_on_exec enable_on_exec.o \
				$(LIB)/test_utils.o \
				$(LIB)/perf_helpers.o \
				$(LIB)/instructions_testcode.o

enable_on_exec.o:	enable_on_exec.c
	$(CC) $(CFLAGS) -c enable_on_exec.c

###

exclusive:	exclusive.o $(LIB)/test_utils.o \
					$(LIB)/perf_helpers.o \
					$(LIB)/instructions_testcode.o
	$(CC) $(LFLAGS) -o exclusive exclusive.o \
				$(LIB)/test_utils.o \
				$(LIB)/perf_helpers.o \
				$(LIB)/instructions_testcode.o

exclusive.o:	exclusive.c
	$(CC) $(CFLAGS) -c exclusive.c


###                                                                            

format_id_support:	format_id_support.o \
	$(LIB)/perf_helpers.o \
	$(LIB)/test_utils.o \
	$(LIB)/matrix_multiply.o
	$(CC) $(LFLAGS) -o format_id_support format_id_support.o \
				$(LIB)/perf_helpers.o \
				$(LIB)/test_utils.o \
				$(LIB)/matrix_multiply.o

format_id_support.o:	format_id_support.c
	$(CC) $(CFLAGS) -c format_id_support.c

###


read_format_range_check:	read_format_range_check.o $(LIB)/test_utils.o \
					$(LIB)/perf_helpers.o \
					$(LIB)/instructions_testcode.o
	$(CC) $(LFLAGS) -o read_format_range_check read_format_range_check.o \
				$(LIB)/test_utils.o \
				$(LIB)/perf_helpers.o \
				$(LIB)/instructions_testcode.o

read_format_range_check.o:	read_format_range_check.c
	$(CC) $(CFLAGS) -c read_format_range_check.c

###

sample_type_range_check:	sample_type_range_check.o $(LIB)/test_utils.o \
					$(LIB)/perf_helpers.o \
					$(LIB)/instructions_testcode.o
	$(CC) $(LFLAGS) -o sample_type_range_check sample_type_range_check.o \
				$(LIB)/test_utils.o \
				$(LIB)/perf_helpers.o \
				$(LIB)/instructions_testcode.o

sample_type_range_check.o:	sample_type_range_check.c
	$(CC) $(CFLAGS) -c sample_type_range_check.c

###

clean:	
	rm -f *~ *.o \
	disabled \
	enable_on_exec \
	exclusive \
	format_id_support \
	inherit \
	inherit_stat \
	read_format_range_check \
	sample_type_range_check

