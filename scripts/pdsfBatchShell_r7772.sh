#!/bin/sh
#$ -l cvmfs=1
#$ -l h_vmem=6G

## Example qsub command:
# qsub  -e  /dev/null -o /dev/null -t 1-20 /global/projecta/projectdirs/atlas/spgriso/data/LLPWorkshop17/scratch/commands/pdsfBatchShell_r7772.sh [BASEDIR] [SUBDIR]

##################################################
# Settings
##################################################
ASetupStr="--dbrelease=current AtlasProd1,20.7.5.1.1,here"

#List of dynamically needed expansions (in the code with __XXX__)
# BASE_DIR -> Test area
# OUTDIR -> Base output directory
# SUBDIR -> optional sub-directory

BASEDIR="/global/projecta/projectdirs/atlas/kkrizka/data/LLPWorkshop17/scratch/r20p7PixClusters/"
OUTDIR="/global/projecta/projectdirs/atlas/kkrizka/data/LLPWorkshop17/scratch/r20p7PixClusters/"

if [ ${#} != 1 ]; then
    echo "usage: ${0} inputfile"
    exit -1
fi
INFILE=$(basename ${1})
INDS=$(basename $(dirname ${1}))

#SAMPLE="/global/projecta/projectdirs/atlas/spgriso/data/LLPWorkshop17/HITS/mc15_13TeV.402735.PythiaRhad_AUET2BCTEQ6L1_gen_gluino_p1_2000_qq_100_1ns.merge.HITS.e4732_s2821_s2183/HITS.07693686._000001.pool.root.1"
SAMPLE=${1}
myMaxEvents=100
mySkipEvents=0
if ! [ -z "${SGE_TASK_ID}" ] && ! [ ${SGE_TASK_ID} == "undefined" ]; then

    #Split input file into 100 events chunks
    mySkipEvents=$(((SGE_TASK_ID-1)*100))

fi
echo "=============================="
echo "sample = $SAMPLE"
echo "myMaxEvents = ${myMaxEvents}"
echo "mySkipEvents = ${mySkipEvents}"
echo "=============================="

echo "Starting batch job. Processing sample: $SAMPLE"

##################################################
# END of Settings
##################################################

#Setup ATLAS software on PDSF
shopt -s expand_aliases
source /common/atlas/scripts/setupATLAS.sh
setupATLAS
lsetup asetup

echo "Setting up ATLAS software $ASetupStr in $BASEDIR/"
cd $BASEDIR/
asetup ${ASetupStr}


echo "------ ENVIRONMENT ---------"
printenv
echo "---- END OF ENVIRONMENT ----"
echo ""

CPYDIR=$OUTDIR/$INDS/$INFILE
echo "Output: ${CPYDIR}"
mkdir -pv ${CPYDIR}

cd $TMPDIR
WRKDIR=$PWD
echo "Work dir: ${PWD}"

# Setup job settings and run it (from r7772.txt)
Reco_tf.py --digiSteeringConf 'StandardSignalOnlyTruth' --conditionsTag 'default:OFLCOND-MC15c-SDR-09' --ignoreErrors True --ignorePatterns 'Py:TrigConf2COOLLib.py.+ERROR.===================================+' --autoConfiguration 'everything' --postInclude 'default:RecJobTransforms/UseFrontier.py' --steering 'doRDO_TRIG' --preInclude 'RDOtoRDOTrigger:RecExPers/RecoOutputMetadataList_jobOptions.py' --postExec 'all:CfgMgr.MessageSvc().setError+=["HepMcParticleLink"]' 'HITtoRDO:from AthenaCommon.AlgSequence import AlgSequence;job = AlgSequence();StreamRDO = job.StreamRDO;StreamRDO.ItemList += ["SiHitCollection#PixelHits"];StreamRDO.ForceRead  = TRUE' 'RAWtoESD:from AthenaCommon.AlgSequence import AlgSequence;job = AlgSequence();StreamESD = job.StreamESD;StreamESD.ItemList += ["SiHitCollection#PixelHits"];StreamESD.ForceRead  = TRUE' 'ESDtoAOD:fixedAttrib=[s if "CONTAINER_SPLITLEVEL = \"99\"" not in s else "" for s in svcMgr.AthenaPoolCnvSvc.PoolAttributes];svcMgr.AthenaPoolCnvSvc.PoolAttributes=fixedAttrib' --preExec 'all:rec.Commissioning.set_Value_and_Lock(True);from AthenaCommon.BeamFlags import jobproperties;jobproperties.Beam.numberOfCollisions.set_Value_and_Lock(20.0);from LArROD.LArRODFlags import larRODFlags;larRODFlags.NumberOfCollisions.set_Value_and_Lock(20);larRODFlags.nSamples.set_Value_and_Lock(4);larRODFlags.doOFCPileupOptimization.set_Value_and_Lock(True);larRODFlags.firstSample.set_Value_and_Lock(0);larRODFlags.useHighestGainAutoCorr.set_Value_and_Lock(True);from InDetRecExample.InDetJobProperties import InDetFlags;InDetFlags.doSlimming.set_Value_and_Lock(False)' 'RAWtoESD:from CaloRec.CaloCellFlags import jobproperties;jobproperties.CaloCellFlags.doLArCellEmMisCalib=False' 'ESDtoAOD:TriggerFlags.AODEDMSet="AODSLIM"' 'ESDtoDPD:from InDetPrepRawDataToxAOD.InDetDxAODJobProperties import InDetDxAODFlags;InDetDxAODFlags.DumpSctInfo.set_Value_and_Lock(False);InDetDxAODFlags.DumpPixelRdoInfo.set_Value_and_Lock(True);InDetDxAODFlags.DumpPixelNNInfo.set_Value_and_Lock(True);InDetDxAODFlags.ThinHitsOnTrack.set_Value_and_Lock(True)' --triggerConfig 'RDOtoRDOTrigger=MCRECO:DBF:TRIGGERDBMC:2046,20,56' --geometryVersion 'default:ATLAS-R2-2015-03-01-00' --numberOfCavernBkg '0' \
--inputHITSFile $SAMPLE --outputRDOFile RDO.${SGE_TASK_ID}.pool.root --outputESDFile ESD.${SGE_TASK_ID}.pool.root --outputAODFile AOD.${SGE_TASK_ID}.pool.root --outputDAOD_IDTRKVALIDFile DAOD_IDTRKVALID.${SGE_TASK_ID}.pool.root \
--maxEvents=${myMaxEvents} --skipEvents=${mySkipEvents} >& log.RecoTf


#compress log file
gzip $WRKDIR/log.*

#List output
echo ""
echo "List of local output files:"
echo "ls -lh ${WRKDIR}/"
ls -lh $WRKDIR/

#now copy the output files and log file to output location
echo ""
echo "Now copying to ${CPYDIR}"
mv $WRKDIR/*root* $CPYDIR/
for pippo in $WRKDIR/log.*; do
  newName=`echo $pippo | sed 's/gz$/'${SGE_TASK_ID}'.gz/g'`
  mv $pippo $newName
done
mv $WRKDIR/log.* $CPYDIR/
echo ""
echo "Content of destination: $CPYDIR"
ls -lh $CPYDIR/
