Benchmark algorithms comparison based on SIMD engine ARM NEON
prerequisites to make:
$sudo apt install make qemu qemu-user 

Each benchmark will compile 3 different binaries. Each binary is named as such:
(SIMPLE BINARY NAME): The default ummodified program. By default uses compilation flags for GCOV tests. If you want a fair comparison with the other binaries simply change -O0 to -O3 and remove the GCOVFLAGS
*_av: autovectorized binary
*_opt: vectorized binary using NEON INSTRINSICS. The modified code is wrapped in 
#ifdef opt tags. 
To compile each benchmark just go to their directory and use $make

Each benchmark might contain a runme.sh as a basic script as how to run them.

TODO: add apropriate makefile to GSM
