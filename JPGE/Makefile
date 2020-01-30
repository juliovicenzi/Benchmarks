CC = arm-linux-gnueabi-g++
CFLAGS = -static -march=armv7-a -mtune=cortex-a9 -mfpu=neon -mfloat-abi=softfp 
GCOVFLAGS=  -fprofile-arcs -ftest-coverage 
LIBS= 
OBJS= jpge.cpp jpgd.cpp encoder.cpp 
NOVEC = -fno-tree-vectorize
VEC = -fopt-info-vec-optimized
TARGET=encoder
	
make: $(OBJS)  
	#arm (CHANGE TO O3 FOR BENCHMARKING, O0 FOR PROFILING DATA)
	$(CC) $(CFLAGS)  -O0 $(NOVEC) $^ -o $(TARGET) $(LIBS) $(GCOVFLAGS)
	#auto vectorization 
	$(CC) $(CFLAGS) -O3 $(VEC)=$(TARGET)_av.txt $^ -o $(TARGET)_av $(LIBS)
	#neon intrinsics
	$(CC) $(CFLAGS) -O3 $(VEC)=$(TARGET)_opt.txt $^ -o $(TARGET)_opt $(LIBS) -Dopt

.PHONY: clean
clean:
	$(RM) *_av $(TARGET) *_opt 
