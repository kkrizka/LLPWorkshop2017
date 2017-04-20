#!/bin/bash

INDIR=/global/projecta/projectdirs/atlas/kkrizka/data/LLPWorkshop17/scratch/r20p7PixClusters
OUTDIR=/global/projecta/projectdirs/atlas/kkrizka/data/LLPWorkshop17/DAOD_IDTRKVALID

for indspath in ${INDIR}/*
do
    ds=$(basename ${indspath})
    echo ${ds}

    outdspath=${OUTDIR}/${ds}
    if [ ! -d ${outdspath} ]; then
	mkdir ${outdspath}
    fi

    for infile in $(find ${indspath} -name 'DAOD_IDTRKVALID.*.pool.root')
    do
	cp ${infile} ${outdspath}/DAOD_IDTRKVALID.$(find ${outdspath} -maxdepth 1 -name 'DAOD_IDTRKVALID*root' | wc -l).root
    done
done