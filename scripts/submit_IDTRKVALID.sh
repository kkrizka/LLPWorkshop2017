#!/bin/bash

if [ ${#} != 1 ]; then
    echo "usage: ${0} dataset"
    exit -1
fi

INDS=${1}
OUTDS=$(echo ${INDS} | cut -d . -f 1-3).20170421-02

#
# Setup ATLAS
ASetupStr="--dbrelease=current AtlasProd1,20.7.5.1.1,here"
shopt -s expand_aliases
source /common/atlas/scripts/setupATLAS.sh
setupATLAS
lsetup panda "asetup ${ASetupStr}"

#
# Submit
pathena --trf "Reco_tf.py --digiSteeringConf 'StandardSignalOnlyTruth' --conditionsTag 'default:OFLCOND-MC15c-SDR-09' --ignoreErrors True --ignorePatterns 'Py:TrigConf2COOLLib.py.+ERROR.===================================+' --autoConfiguration 'everything' --postInclude 'default:RecJobTransforms/UseFrontier.py' --steering 'doRDO_TRIG' --preInclude 'RDOtoRDOTrigger:RecExPers/RecoOutputMetadataList_jobOptions.py' --postExec 'all:CfgMgr.MessageSvc().setError+=[\"HepMcParticleLink\"]' 'HITtoRDO:from AthenaCommon.AlgSequence import AlgSequence;job = AlgSequence();StreamRDO = job.StreamRDO;StreamRDO.ItemList += [\"SiHitCollection#PixelHits\"];StreamRDO.ForceRead  = TRUE' 'RAWtoESD:from AthenaCommon.AlgSequence import AlgSequence;job = AlgSequence();StreamESD = job.StreamESD;StreamESD.ItemList += [\"SiHitCollection#PixelHits\"];StreamESD.ForceRead  = TRUE' 'ESDtoAOD:fixedAttrib=[s if \"CONTAINER_SPLITLEVEL = \\\"99\\\"\" not in s else \"\" for s in svcMgr.AthenaPoolCnvSvc.PoolAttributes];svcMgr.AthenaPoolCnvSvc.PoolAttributes=fixedAttrib' --preExec 'all:rec.Commissioning.set_Value_and_Lock(True);from AthenaCommon.BeamFlags import jobproperties;jobproperties.Beam.numberOfCollisions.set_Value_and_Lock(20.0);from LArROD.LArRODFlags import larRODFlags;larRODFlags.NumberOfCollisions.set_Value_and_Lock(20);larRODFlags.nSamples.set_Value_and_Lock(4);larRODFlags.doOFCPileupOptimization.set_Value_and_Lock(True);larRODFlags.firstSample.set_Value_and_Lock(0);larRODFlags.useHighestGainAutoCorr.set_Value_and_Lock(True);from InDetRecExample.InDetJobProperties import InDetFlags;InDetFlags.doSlimming.set_Value_and_Lock(False)' 'RAWtoESD:from CaloRec.CaloCellFlags import jobproperties;jobproperties.CaloCellFlags.doLArCellEmMisCalib=False' 'ESDtoAOD:TriggerFlags.AODEDMSet=\"AODSLIM\"' 'ESDtoDPD:from InDetPrepRawDataToxAOD.InDetDxAODJobProperties import InDetDxAODFlags;InDetDxAODFlags.DumpSctInfo.set_Value_and_Lock(False);InDetDxAODFlags.DumpPixelRdoInfo.set_Value_and_Lock(True);InDetDxAODFlags.DumpPixelNNInfo.set_Value_and_Lock(True);InDetDxAODFlags.ThinHitsOnTrack.set_Value_and_Lock(True)' --triggerConfig 'RDOtoRDOTrigger=MCRECO:DBF:TRIGGERDBMC:2046,20,56' --geometryVersion 'default:ATLAS-R2-2015-03-01-00' --numberOfCavernBkg '0' --inputHITSFile %IN --outputRDOFile %OUT.RDO.root --outputESDFile %OUT.ESD.root --outputAODFile %OUT.AOD.root --outputDAOD_IDTRKVALIDFile %OUT.DAOD_IDTRKVALID.root" --inDS ${INDS} --outDS user.${USER}.${OUTDS} --nFilesPerJob=1