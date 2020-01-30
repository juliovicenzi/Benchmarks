# --------------------------------------------------------------------- 
# ---		 EPIC (Efficient Pyramid Image Coder)               ---
# ---	 Designed by Eero P. Simoncelli and Edward H. Adelson       ---
# ---		    Written by Eero P. Simoncelli                   ---
# ---  Developed at the Vision Science Group, The Media Laboratory  ---
# ---	Copyright 1989, Massachusetts Institute of Technology       ---
# ---			 All rights reserved.                       ---
# ---------------------------------------------------------------------
#
# Permission to use, copy, or modify this software and its documentation
# for educational and research purposes only and without fee is hereby
# granted, provided that this copyright notice appear on all copies and
# supporting documentation.  For any other uses of this software, in
# original or modified form, including but not limited to distribution
# in whole or in part, specific prior permission must be obtained from
# M.I.T. and the authors.  These programs shall not be used, rewritten,
# or adapted as the basis of a commercial software or hardware product
# without first obtaining appropriate licenses from M.I.T.  M.I.T. makes
# no representations about the suitability of this software for any
# purpose.  It is provided "as is" without express or implied warranty.
#
# ---------------------------------------------------------------------

CC = arm-linux-gnueabi-gcc
CFLAGS = -static -march=armv7-a -mtune=cortex-a9 -mfpu=neon -mfloat-abi=softfp 
GCOVFLAGS=  -fprofile-arcs -ftest-coverage 
LIBS= -lm -lc
OBJS= utilities.c parse_args.c fileio.c quantize.c run_length.c huffman.c epic.c build_pyr.c convolve.c edges.c 
NOVEC = -fno-tree-vectorize
VEC = -fopt-info-vec-optimized
TARGET=../bin/epic

make: $(OBJS)  
	#arm (CHANGE TO O3 FOR BENCHMARKING, O0 FOR PROFILING DATA)
	$(CC) $(CFLAGS)  -O0 $(NOVEC) $^ -o $(TARGET) $(LIBS) $(GCOVFLAGS)
	#auto vectorization 
	$(CC) $(CFLAGS) -O3 $(VEC)=$(TARGET)_av.txt $^ -o $(TARGET)_av $(LIBS)
	#neon intrinsics
	$(CC) $(CFLAGS) -O3 $(VEC)=$(TARGET)_opt.txt $^ -o $(TARGET)_opt $(LIBS) -Dopt

.PHONY: clean
clean:
	$(RM) -rf $(TARGET) $(TARGET)_av $(TARGET)_opt *.txt ../bin/*.txt
