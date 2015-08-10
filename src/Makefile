# Makefile created by make.php
# ---
cc=gcc
opt=-Wall -O2 -g
opt_lm=-lm -Wall -O2 -g
objs=k_fomatter.o k_function.o k_main.o k_parser.o k_parser_test.o k_run.o k_run_test.o k_smf.o k_smf_test.o k_smf_writer.o k_sutoton.o k_sutoton_test.o k_test.o k_token.o k_token_def.o k_track.o s_hash.o s_hook_memory.o s_hook_memory_test.o s_lib.o s_list.o s_list_test.o s_str.o s_str_test.o s_stream.o s_stream_test.o s_test.o s_value.o s_value_test.o sakura.o
# ---
# make sakura
sakurac: $(objs)
	$(cc) $(opt_lm) -o sakurac $(objs)
	ctags *.c *.h
# ---
# every object
k_fomatter.o:k_fomatter.c 
	$(cc) $(opt) -c k_fomatter.c
k_function.o:k_function.c k_function.h
	$(cc) $(opt) -c k_function.c
k_main.o:k_main.c k_main.h
	$(cc) $(opt) -c k_main.c
k_parser.o:k_parser.c k_parser.h
	$(cc) $(opt) -c k_parser.c
k_parser_test.o:k_parser_test.c 
	$(cc) $(opt) -c k_parser_test.c
k_run.o:k_run.c k_run.h
	$(cc) $(opt) -c k_run.c
k_run_test.o:k_run_test.c 
	$(cc) $(opt) -c k_run_test.c
k_smf.o:k_smf.c k_smf.h
	$(cc) $(opt) -c k_smf.c
k_smf_test.o:k_smf_test.c 
	$(cc) $(opt) -c k_smf_test.c
k_smf_writer.o:k_smf_writer.c k_smf_writer.h
	$(cc) $(opt) -c k_smf_writer.c
k_sutoton.o:k_sutoton.c k_sutoton.h
	$(cc) $(opt) -c k_sutoton.c
k_sutoton_test.o:k_sutoton_test.c 
	$(cc) $(opt) -c k_sutoton_test.c
k_test.o:k_test.c k_test.h
	$(cc) $(opt) -c k_test.c
k_token.o:k_token.c k_token.h
	$(cc) $(opt) -c k_token.c
k_token_def.o:k_token_def.c k_token_def.h
	$(cc) $(opt) -c k_token_def.c
k_track.o:k_track.c k_track.h
	$(cc) $(opt) -c k_track.c
s_hash.o:s_hash.c s_hash.h
	$(cc) $(opt) -c s_hash.c
s_hook_memory.o:s_hook_memory.c s_hook_memory.h
	$(cc) $(opt) -c s_hook_memory.c
s_hook_memory_test.o:s_hook_memory_test.c 
	$(cc) $(opt) -c s_hook_memory_test.c
s_lib.o:s_lib.c s_lib.h
	$(cc) $(opt) -c s_lib.c
s_list.o:s_list.c s_list.h
	$(cc) $(opt) -c s_list.c
s_list_test.o:s_list_test.c 
	$(cc) $(opt) -c s_list_test.c
s_str.o:s_str.c s_str.h
	$(cc) $(opt) -c s_str.c
s_str_test.o:s_str_test.c 
	$(cc) $(opt) -c s_str_test.c
s_stream.o:s_stream.c s_stream.h
	$(cc) $(opt) -c s_stream.c
s_stream_test.o:s_stream_test.c 
	$(cc) $(opt) -c s_stream_test.c
s_test.o:s_test.c s_test.h
	$(cc) $(opt) -c s_test.c
s_value.o:s_value.c s_value.h
	$(cc) $(opt) -c s_value.c
s_value_test.o:s_value_test.c 
	$(cc) $(opt) -c s_value_test.c
sakura.o:sakura.c sakura.h
	$(cc) $(opt) -c sakura.c
# ---
.PHONY:clean
clean:
	rm -f sakurac $(objs)
	rm -f tags
# ---
.PHONY: all
all: sakurac
