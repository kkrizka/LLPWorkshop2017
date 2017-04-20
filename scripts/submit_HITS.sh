#!/bin/bash

OUTDIR=/global/projecta/projectdirs/atlas/kkrizka/data/LLPWorkshop17/scratch/r20p7PixClusters

for i in $(find /global/projecta/projectdirs/atlas/kkrizka/data/LLPWorkshop17/HITS -type f)
do
    infile=$(basename ${i})
    inds=$(basename $(dirname ${i}))
    if [ ! -d ${OUTDIR}/${inds}/${infile} ]; then
	qsub -t 1-10:1 pdsfBatchShell_r7772.sh ${i}
    fi
done