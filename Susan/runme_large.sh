#!/bin/sh
./susan input_large.pgm output_large.smoothing.pgm -s
./susan input_large.pgm output_large.edges.pgm -e
./susan input_large.pgm output_large.corners.pgm -c

./susan_av input_large.pgm output_large.smoothing.pgm -s
./susan_av input_large.pgm output_large.edges.pgm -e
./susan_av input_large.pgm output_large.corners.pgm -c

./susan_opt input_large.pgm output_large.smoothing.pgm -s
./susan_opt input_large.pgm output_large.edges.pgm -e
./susan_opt input_large.pgm output_large.corners.pgm -c

