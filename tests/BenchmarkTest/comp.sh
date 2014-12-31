#!/bin/sh

###################################################################################
#
# JHPCN-DF : Data compression library based on
#            Jointed Hierarchical Precision Compression Number Data Format
#
# Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
#
###################################################################################

GCC=g++
GCC_OPT="-g --std=c++11 -O3 -march=native -fopenmp"
ICPC=icpc
ICPC_OPT="-g --std=c++11 -O3 -xHost -ip -fno-alias -qopt-report=5 -openmp"
#PGCC=pgc++
#PGCC_OPT="-g --c++0x -fastsse -O3 -Mipa=fast,inline -Msmartalloc -mp -Msafeptr"


## FOCUSでPGIコンパイラ+C++11のコードがまともにコンパイルできないのでPGIの評価は中断
## ヘッダファイルを明示的に4.8のものを使うように指定すれば対応できそうだが・・・

rm a.out_*

for REAL_TYPE in float double
do
    for BM in XOR ZeroPadding
    do
        ${GCC} ${GCC_OPT}   -DREAL_TYPE=${REAL_TYPE} -I ../../src -I./ ${BM}Benchmark.cpp -o a.out_GNU_${REAL_TYPE}_${BM}
        #${ICPC} ${ICPC_OPT} -DREAL_TYPE=${REAL_TYPE} -I ../../src -I./ ${BM}Benchmark.cpp -o a.out_INTEL_${REAL_TYPE}_${BM}
        #${PGCC} ${PGCC_OPT} -DREAL_TYPE=${REAL_TYPE} -I ../../src -I./ ${BM}Benchmark.cpp -o a.out_PGI_${REAL_TYPE}_${BM}
    done
done

