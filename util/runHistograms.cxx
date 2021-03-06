#include "xAODRootAccess/Init.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ScanDir.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "SampleHandler/DiskListLocal.h"
#include <TSystem.h>

#include "LLPWorkshop2017/MyHistogramAnalysis.h"

int main( int argc, char* argv[] ) {

  // Take the submit directory from the input if provided:
  std::string submitDir = "submitDir";
  if( argc > 1 ) submitDir = argv[ 1 ];

  // Set up the job for xAOD access:
  xAOD::Init().ignore();

  // Construct the samples to run on:
  SH::SampleHandler sh;

  // use SampleHandler to scan all of the subdirectories of a directory for particular MC single file:
  //SH::ScanDir().filePattern("DAOD_IDTRKVALID.*.root").scan(sh,"/global/projecta/projectdirs/atlas/spgriso/data/LLPWorkshop17/scratch/r20p7p8p5PixClusters/output_EnhancedBias_r8631_r8632");
  //SH::ScanDir().scan(sh,"/global/projecta/projectdirs/atlas/spgriso/data/LLPWorkshop17/DAOD_IDTRKVALID/data16_13TeV.00299315.calibration_PixelBeam.recon.DAOD_IDTRKVALID.r8077");
  //SH::ScanDir().scan(sh,"/global/projecta/projectdirs/atlas/spgriso/data/LLPWorkshop17/DAOD_IDTRKVALID/mc15_13TeV.361203.Pythia8_A2_MSTW2008LO_ND_minbias.recon.DAOD_IDTRKVALID.e3639_s2601_s2132_r8383");
  SH::ScanDir().filePattern("*.DAOD_IDTRKVALID.root").scan(sh,"/global/projecta/projectdirs/atlas/kkrizka/data/LLPWorkshop17/DAOD_IDTRKVALID");

  // Set the name of the input TTree. It's always "CollectionTree"
  // for xAOD files.
  sh.setMetaString( "nc_tree", "CollectionTree" );

  // Print what we found:
  sh.print();

  // Create an EventLoop job:
  EL::Job job;
  job.sampleHandler( sh );
  job.options()->setString (EL::Job::optXaodAccessMode, EL::Job::optXaodAccessMode_athena);
  //job.options()->setDouble (EL::Job::optMaxEvents, 500);

  // Add our analysis to the job:
  MyHistogramAnalysis* alg = new MyHistogramAnalysis();
  job.algsAdd( alg );

  // Run the job using the local/direct driver:
  EL::DirectDriver driver;
  driver.submit( job, submitDir );

  return 0;
}
