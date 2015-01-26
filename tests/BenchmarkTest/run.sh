#!/bin/sh
PAD=10
export OMP_NUM_THREADS=4

for SIZE in 256 512
do
    for REAL_TYPE in float double
    do
        RESULTS=result_${REAL_TYPE}_${SIZE}
        echo "************************************" |tee ${RESULTS}
        echo "Benchmark Test for bitwise operation" |tee -a ${RESULTS}
        echo "************************************" |tee -a ${RESULTS}

        echo " XOR test for ${REAL_TYPE} with g++ start at `date`" |tee -a ${RESULTS}
        ./a.out_GNU_${REAL_TYPE}_XOR ${SIZE} 2>&1 |tee -a ${RESULTS}
        echo |tee -a ${RESULTS}
        ./a.out_GNU_${REAL_TYPE}_AND ${SIZE} 2>&1 |tee -a ${RESULTS}
        echo |tee -a ${RESULTS}
        ./a.out_GNU_${REAL_TYPE}_ZeroPadding  ${SIZE} ${PAD} 2>&1 |tee -a ${RESULTS}
        echo |tee -a ${RESULTS}
        ./a.out_GNU_${REAL_TYPE}_ZeroPadding2 ${SIZE} 2>&1 |tee -a ${RESULTS}
        echo |tee -a ${RESULTS}
        echo |tee -a ${RESULTS}

        echo "${BM}test for ${REAL_TYPE} with icpc start at `date`" |tee -a ${RESULTS}
        ./a.out_INTEL_${REAL_TYPE}_XOR ${SIZE} 2>&1 |tee -a ${RESULTS}
        echo |tee -a ${RESULTS}
        ./a.out_INTEL_${REAL_TYPE}_AND ${SIZE} 2>&1 |tee -a ${RESULTS}
        echo |tee -a ${RESULTS}
        ./a.out_INTEL_${REAL_TYPE}_ZeroPadding  ${SIZE} ${PAD} 2>&1 |tee -a ${RESULTS}
        echo |tee -a ${RESULTS}
        ./a.out_INTEL_${REAL_TYPE}_ZeroPadding2 ${SIZE} 2>&1 |tee -a ${RESULTS}
        echo |tee -a ${RESULTS}
        echo |tee -a ${RESULTS}
    done
done

