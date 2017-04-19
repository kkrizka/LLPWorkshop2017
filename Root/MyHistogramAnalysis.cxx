#include <MyLongLivedAnalysis/MyHistogramAnalysis.h>

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <xAODCore/ShallowCopy.h>
#include <xAODCutFlow/CutBookkeeper.h>
#include <xAODCutFlow/CutBookkeeperContainer.h>
#include <xAODEventInfo/EventInfo.h>
#include <xAODRootAccess/Init.h>
#include <xAODTracking/TrackMeasurementValidationContainer.h>
#include <xAODTracking/TrackParticleContainer.h>
#include <SampleHandler/MetaFields.h>

// this is needed to distribute the algorithm to the workers
ClassImp(MyHistogramAnalysis)



MyHistogramAnalysis :: MyHistogramAnalysis ()
: m_event(0), m_store(0),
  m_trigDecisionTool(0), m_trigConfigTool(0)
{
}

EL::StatusCode MyHistogramAnalysis :: setupJob (EL::Job& job)
{
  // let's initialize the algorithm to use the xAODRootAccess package
  job.useXAOD ();
  xAOD::Init(); // call before opening first file
  xAOD::TReturnCode::enableFailure();
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MyHistogramAnalysis :: histInitialize ()
{
  // metadata
  m_histEventCount = new TH1D("MetaData_EventCount", "MetaData_EventCount", 6, 0.5, 6.5);
  m_histEventCount -> GetXaxis() -> SetBinLabel(1, "nEvents initial");
  m_histEventCount -> GetXaxis() -> SetBinLabel(2, "nEvents selected");
  m_histEventCount -> GetXaxis() -> SetBinLabel(3, "sumOfWeights initial");
  m_histEventCount -> GetXaxis() -> SetBinLabel(4, "sumOfWeights selected");
  m_histEventCount -> GetXaxis() -> SetBinLabel(5, "sumOfWeightsSquared initial");
  m_histEventCount -> GetXaxis() -> SetBinLabel(6, "sumOfWeightsSquared selected");
  wk()->addOutput(m_histEventCount);

  // Interesting histograms
  h_histIBLToT=new TH1F("IBLToT", "IBL ToT",200,0.,2000.);
  wk()->addOutput(h_histIBLToT);
  h_histIBLnRDO=new TH1F("IBLnRDO", "IBL N_{RDO}",51,-0.5,50.5);
  wk()->addOutput(h_histIBLnRDO);
  h_histIBLToTavg=new TH1F("IBLToTavg", "IBL ToT/N_{RDO}",30,0.,15.);
  wk()->addOutput(h_histIBLToTavg);
  
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MyHistogramAnalysis :: fileExecute ()
{
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  //
  // Initial event count
  const xAOD::CutBookkeeperContainer* completeCBC = 0;
  if(!m_event->retrieveMetaInput(completeCBC, "CutBookkeepers").isSuccess()){
    Error("initializeEvent()","Failed to retrieve CutBookkeepers from MetaData! Exiting.");
    return EL::StatusCode::FAILURE;
  }

  const xAOD::CutBookkeeper* allEventsCBK = 0;
  int maxCycle = -1;
  for (const auto& cbk: *completeCBC) {
    std::cout << cbk->name() << " " << cbk->inputStream() << std::endl;
    if (cbk->cycle() > maxCycle && cbk->name() == "AllExecutedEvents" && cbk->inputStream() == "StreamAOD") {
      allEventsCBK = cbk;
      maxCycle = cbk->cycle();
    }
  }
  std::cout << "GOT ALL EVENTS " << allEventsCBK << std::endl;
  // m_histEventCount->Fill(1, allEventsCBK->nAcceptedEvents());
  // m_histEventCount->Fill(3, allEventsCBK->sumOfEventWeights());
  // m_histEventCount->Fill(5, allEventsCBK->sumOfEventWeightsSquared());

  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MyHistogramAnalysis :: changeInput (bool firstFile)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MyHistogramAnalysis :: initialize ()
{
  //
  // Event information
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  //
  // Initialize trigger information
  m_trigConfigTool = new TrigConf::xAODConfigTool("xAODConfigTool"); // gives us access to the meta-data
  ANA_CHECK( m_trigConfigTool->initialize() );
  ToolHandle< TrigConf::ITrigConfigTool > trigConfigHandle( m_trigConfigTool );
  m_trigDecisionTool = new Trig::TrigDecisionTool("TrigDecisionTool");
  ANA_CHECK(m_trigDecisionTool->setProperty( "ConfigTool", trigConfigHandle ) ); // connect the TrigDecisionTool to the ConfigTool
  ANA_CHECK(m_trigDecisionTool->setProperty( "TrigDecisionKey", "xTrigDecision" ) );
  ANA_CHECK(m_trigDecisionTool->initialize() );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MyHistogramAnalysis :: execute ()
{
  static const SG::AuxElement::ConstAccessor<int> a_bec("bec");
  static const SG::AuxElement::ConstAccessor<int> a_layer("layer");
  static const SG::AuxElement::ConstAccessor<int> a_ToT("ToT");
  static const SG::AuxElement::ConstAccessor<int> a_nRDO("nRDO");

  //
  // EventInfo
  const xAOD::EventInfo* eventInfo = 0;
  ANA_CHECK(m_event->retrieve( eventInfo, "EventInfo"));
  bool isMC=eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION );

  //
  // Determine weight
  double mcweight= (isMC) ? eventInfo->mcEventWeight() : 1.;
  double xs      = wk()->metaData()->castDouble(SH::MetaFields::crossSection    ,1);
  double eff     = wk()->metaData()->castDouble(SH::MetaFields::filterEfficiency,1);
  double kfactor = wk()->metaData()->castDouble(SH::MetaFields::kfactor         ,1);
  double weight  = mcweight * xs * eff * kfactor;

  //
  // Fill histogram
  m_histEventCount->Fill(2, 1.);
  m_histEventCount->Fill(4, mcweight);
  m_histEventCount->Fill(6, mcweight*mcweight);

  const xAOD::TrackParticleContainer* tracks=0;
  ANA_CHECK(m_event->retrieve(tracks, "InDetTrackParticles"));

  const xAOD::TrackMeasurementValidationContainer* clusters=0;
  ANA_CHECK(m_event->retrieve(clusters, "PixelClusters"));

  int ToT, nRDO;
  for(const auto& cluster : *clusters)
    {
      if(a_layer(*cluster)!=0 || a_bec(*cluster)!=0) continue;
      //if(a_nRDO(*cluster)==0) { std::cout << "nRDO==0" << std::endl; continue; }
      ToT =a_ToT(*cluster);
      nRDO=a_nRDO(*cluster);

      h_histIBLToT   ->Fill(ToT      ,weight);
      h_histIBLnRDO  ->Fill(nRDO     ,weight);
      h_histIBLToTavg->Fill(ToT/nRDO ,weight);
    }

  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.
  return EL::StatusCode::SUCCESS;
}






EL::StatusCode MyHistogramAnalysis :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MyHistogramAnalysis :: finalize ()
{
  //
  // Cleanup trigger information
  if( m_trigConfigTool ) {
    delete m_trigConfigTool;
    m_trigConfigTool = 0;
  }
  if( m_trigDecisionTool ) {
    m_trigDecisionTool->finalize();
    delete m_trigDecisionTool;
    m_trigDecisionTool = 0;
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode MyHistogramAnalysis :: histFinalize ()
{
  // This method is the mirror image of histInitialize(), meaning it
  // gets called after the last event has been processed on the worker
  // node and allows you to finish up any objects you created in
  // histInitialize() before they are written to disk.  This is
  // actually fairly rare, since this happens separately for each
  // worker node.  Most of the time you want to do your
  // post-processing on the submission node after all your histogram
  // outputs have been merged.  This is different from finalize() in
  // that it gets called on all worker nodes regardless of whether
  // they processed input events.
  return EL::StatusCode::SUCCESS;
}
