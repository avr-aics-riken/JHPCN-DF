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
GCC_OPT="-g --std=c++11 -O3 -fopenmp -masm=intel"
ICPC=icpc
ICPC_OPT="-g --std=c++11 -O3 -ip -fno-alias -unroll=0 -qopt-report=5 -openmp -masm=intel"
# for Ivy bridge (FOCUS D/E system)
#GCC_OPT="${GCC_OPT}   -march=core-avx-i"
#ICPC_OPT="${ICPC_OPT} -xCORE-AVX-I"
# for Sandy bridge 
#GCC_OPT="${GCC_OPT}   -march=corei7-avx"
#ICPC_OPT="${ICPC_OPT} -xAVX"
# for Westmere (FOCUS A/C system)
GCC_OPT="${GCC_OPT}   -march=corei7"
ICPC_OPT="${ICPC_OPT} -xSSE4.2"


#PGCC=pgc++
#PGCC_OPT="-g --c++0x -fastsse -O3 -Mipa=fast,inline -Msmartalloc -mp -Msafeptr"
## FOCUSでPGIコンパイラ+C++11のコードがまともにコンパイルできないのでPGIの評価は中断
## ヘッダファイルを明示的にgcc 4.8のものを使うように指定すれば対応できそうだが・・・

echo "Benchmark program compile start at `date`" |tee compile.log
echo "${GCC} --version" |tee -a compile.log
${GCC} --version 2>&1 |tee -a compile.log
echo "Compiler option for g++:   ${GCC_OPT}" |tee -a compile.log
echo "${ICPC} --version"  |tee -a compile.log
${ICPC} --version 2>&1 |tee -a compile.log
echo "Compiler option for Intel: ${ICPC_OPT}" |tee -a compile.log

for REAL_TYPE in float double
do
    for BM in XOR AND ZeroPadding ZeroPadding2
    do
        ${GCC} ${GCC_OPT}   -DREAL_TYPE=${REAL_TYPE} -I ../../src -I./ ${BM}Benchmark.cpp -o a.out_GNU_${REAL_TYPE}_${BM}   2>&1 |tee -a compile.log
        ${ICPC} ${ICPC_OPT} -DREAL_TYPE=${REAL_TYPE} -I ../../src -I./ ${BM}Benchmark.cpp -o a.out_INTEL_${REAL_TYPE}_${BM} 2>&1 |tee -a compile.log
        #${PGCC} ${PGCC_OPT} -DREAL_TYPE=${REAL_TYPE} -I ../../src -I./ ${BM}Benchmark.cpp -o a.out_PGI_${REAL_TYPE}_${BM}
    done
done

