#!/bin/bash

source ZprimeDM/scripts/librun.sh

# sample list
MC="LLPWorkshop2017/filelists/Pythia8_jetjet.IDTRKVALID.list LLPWorkshop2017/filelists/Rhadron.IDTRKVALID.list LLPWorkshop2017/filelists/minbias.IDTRKVALID.list"
DATA="LLPWorkshop2017/filelists/PixelBeam.IDTRKVALID.txt LLPWorkshop2017/filelists/EnhancedBias.IDTRKVALID.txt"

# Process
RUNLIST=""

export PACKAGE=LLPWorkshop2017

runOne llp llp_mc "-m -l" ${MC}
runOne llp llp_data "-l" ${DATA}

for RUN in ${RUNLIST}
do
    echo "Waiting for PID ${RUN}"
    wait ${RUN} || exit 1
    echo "COMPLETED WITH ${?}"
done

#
# Merge
#
