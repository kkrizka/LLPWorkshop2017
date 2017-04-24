#include <LLPWorkshop2017/MyHistogramAnalysis.h>

#include <LLPWorkshop2017/Helpers.h>

#include <AthLinks/ElementLink.h>
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
#include <xAODTracking/TrackStateValidationContainer.h>
#include <xAODTruth/TruthParticleContainer.h>
#include <xAODTruth/TruthVertex.h>
#include <SampleHandler/MetaFields.h>

#include <sstream>

typedef std::vector<ElementLink< xAOD::TrackStateValidationContainer > > MeasurementsOnTrackLink;
typedef ElementLink< xAOD::TrackMeasurementValidationContainer > TrackMeasurementValidationLink;
typedef ElementLink< xAOD::TruthParticleContainer > TruthParticleContainerLink;

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
  h_IBL=new ClusterHists("IBL");
  h_IBL->initialize();
  h_IBL->record(wk());

  std::stringstream ss_name;
  for(int i=-10; i<=9; i++)
    {
      ss_name << "IBL_eta_module_" << i;
      ClusterHists *IBL_eta_module=new ClusterHists(ss_name.str());
      IBL_eta_module->initialize();
      IBL_eta_module->record(wk());
      h_IBL_eta_module[i+10]=IBL_eta_module;
      ss_name.str("");
    }
  
  for(int i=0; i<=9; i++)
    {
      ss_name << "IBL_abseta_module_" << i;
      ClusterHists *IBL_abseta_module=new ClusterHists(ss_name.str());
      IBL_abseta_module->initialize();
      IBL_abseta_module->record(wk());
      h_IBL_abseta_module[i]=IBL_abseta_module;
      ss_name.str("");
    }

  //
  // per pixel chip
  h_perIBLchip=new ClusterHists("perIBLchip");
  h_perIBLchip->initialize();
  h_perIBLchip->record(wk());

  for(int i=-10; i<=9; i++)
    {
      ss_name << "perIBLchip_eta_module_" << i;
      ClusterHists *perIBLchip_eta_module=new ClusterHists(ss_name.str());
      perIBLchip_eta_module->initialize();
      perIBLchip_eta_module->record(wk());
      h_perIBLchip_eta_module[i+10]=perIBLchip_eta_module;
      ss_name.str("");
    }
  
  for(int i=0; i<=9; i++)
    {
      ss_name << "perIBLchip_abseta_module_" << i;
      ClusterHists *perIBLchip_abseta_module=new ClusterHists(ss_name.str());
      perIBLchip_abseta_module->initialize();
      perIBLchip_abseta_module->record(wk());
      h_perIBLchip_abseta_module[i]=perIBLchip_abseta_module;
      ss_name.str("");
    }

  //
  // matched to track
  h_RHad_IBL=new ClusterHists("RHad_IBL");
  h_RHad_IBL->initialize();
  h_RHad_IBL->record(wk());

  for(int i=-10; i<=9; i++)
    {
      ss_name << "RHad_IBL_eta_module_" << i;
      ClusterHists *RHad_IBL_eta_module=new ClusterHists(ss_name.str());
      RHad_IBL_eta_module->initialize();
      RHad_IBL_eta_module->record(wk());
      h_RHad_IBL_eta_module[i+10]=RHad_IBL_eta_module;
      ss_name.str("");
    }
  
  for(int i=0; i<=9; i++)
    {
      ss_name << "RHad_IBL_abseta_module_" << i;
      ClusterHists *RHad_IBL_abseta_module=new ClusterHists(ss_name.str());
      RHad_IBL_abseta_module->initialize();
      RHad_IBL_abseta_module->record(wk());
      h_RHad_IBL_abseta_module[i]=RHad_IBL_abseta_module;
      ss_name.str("");
    }

  //
  // matched to track
  h_nonRHad_IBL=new ClusterHists("nonRHad_IBL");
  h_nonRHad_IBL->initialize();
  h_nonRHad_IBL->record(wk());

  for(int i=-10; i<=9; i++)
    {
      ss_name << "nonRHad_IBL_eta_module_" << i;
      ClusterHists *nonRHad_IBL_eta_module=new ClusterHists(ss_name.str());
      nonRHad_IBL_eta_module->initialize();
      nonRHad_IBL_eta_module->record(wk());
      h_nonRHad_IBL_eta_module[i+10]=nonRHad_IBL_eta_module;
      ss_name.str("");
    }
  
  for(int i=0; i<=9; i++)
    {
      ss_name << "nonRHad_IBL_abseta_module_" << i;
      ClusterHists *nonRHad_IBL_abseta_module=new ClusterHists(ss_name.str());
      nonRHad_IBL_abseta_module->initialize();
      nonRHad_IBL_abseta_module->record(wk());
      h_nonRHad_IBL_abseta_module[i]=nonRHad_IBL_abseta_module;
      ss_name.str("");
    }


  //
  // Truth histograms
  h_nRhadron =new TH1F("nRhadron",  ";N_{R-hadron}",6,-0.5,5.5);
  h_nChRhadron =new TH1F("nChRhadron",  ";N_{Charged R-hadron}",6,-0.5,5.5);
  h_nNeRhadron =new TH1F("nNeRhadron",  ";N_{Neutral R-hadron}",6,-0.5,5.5);

  h_Rhadron_pt =new TH1F("Rhadron_pt",  ";R-hadron p_{T} [GeV]",100,0,1000);
  h_Rhadron_eta=new TH1F("Rhadron_eta", ";R-hadron #eta"       ,20,-5,5);
  h_Rhadron_phi=new TH1F("Rhadron_phi", ";R-hadron #phi"       ,20,-2*TMath::Pi(),2*TMath::Pi());
  h_Rhadron_m  =new TH1F("Rhadron_m",   ";R-hadron m [GeV]"    ,500,0,5000);
  h_Rhadron_p  =new TH1F("Rhadron_p",   ";R-hadron p [GeV]"    ,500,0,1000);

  h_Rhadron_decVtx_x  =new TH1F("Rhadron_decVtx_x", ";R-hadron Decay Vertex x [mm]",500,0,1000);
  h_Rhadron_decVtx_y  =new TH1F("Rhadron_decVtx_y", ";R-hadron Decay Vertex y [mm]",500,0,1000);
  h_Rhadron_decVtx_z  =new TH1F("Rhadron_decVtx_z", ";R-hadron Decay Vertex z [mm]",500,0,1000);
  h_Rhadron_decVtx_t  =new TH1F("Rhadron_decVtx_t", ";R-hadron Decay Vertex Time [ps]",500,0,1000);
  h_Rhadron_decVtx_eta  =new TH1F("Rhadron_decVtx_eta", ";R-hadron Decay Vertex #eta",20,-5,5);
  h_Rhadron_decVtx_phi  =new TH1F("Rhadron_decVtx_phi", ";R-hadron Decay Vertex #phi",20,-2*TMath::Pi(),2*TMath::Pi());

  wk()->addOutput(h_nRhadron);
  wk()->addOutput(h_nChRhadron);
  wk()->addOutput(h_nNeRhadron);

  wk()->addOutput(h_Rhadron_pt);
  wk()->addOutput(h_Rhadron_eta);
  wk()->addOutput(h_Rhadron_phi);
  wk()->addOutput(h_Rhadron_m);
  wk()->addOutput(h_Rhadron_p);
  wk()->addOutput(h_Rhadron_decVtx_x);
  wk()->addOutput(h_Rhadron_decVtx_y);
  wk()->addOutput(h_Rhadron_decVtx_z);
  wk()->addOutput(h_Rhadron_decVtx_t);
  wk()->addOutput(h_Rhadron_decVtx_eta);
  wk()->addOutput(h_Rhadron_decVtx_phi);

  h_nRhadtrk   =new TH1F("nRhadtrk",    ";N_{R-hadron tracks}",6,-0.5,5.5);
  h_Rhadtrk_pt =new TH1F("Rhadtrk_pt",  ";R-hadron track p_{T} [GeV]",100,0,1000);
  h_Rhadtrk_eta=new TH1F("Rhadtrk_eta", ";R-hadron track #eta"       ,20,-5,5);
  h_Rhadtrk_phi=new TH1F("Rhadtrk_phi", ";R-hadron track #phi"       ,20,-2*TMath::Pi(),2*TMath::Pi());

  h_Rhadtrk_IBL_eta_module   =new TH1F("Rhadtrk_IBL_eta_module"   , ";R-hadron track IBL #eta module",21,-10.5,10.5);
  h_Rhadtrk_IBL_abseta_module=new TH1F("Rhadtrk_IBL_abseta_module", ";R-hadron track IBL #eta module",11, -0.5,10.5);

  wk()->addOutput(h_nRhadtrk);
  wk()->addOutput(h_Rhadtrk_pt);
  wk()->addOutput(h_Rhadtrk_eta);
  wk()->addOutput(h_Rhadtrk_phi);

  wk()->addOutput(h_Rhadtrk_IBL_eta_module);
  wk()->addOutput(h_Rhadtrk_IBL_abseta_module);

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
    if (cbk->cycle() > maxCycle && cbk->name() == "AllExecutedEvents" && cbk->inputStream() == "StreamESD") {
      allEventsCBK = cbk;
      maxCycle = cbk->cycle();
    }
  }
  m_histEventCount->Fill(1, allEventsCBK->nAcceptedEvents());
  m_histEventCount->Fill(3, allEventsCBK->sumOfEventWeights());
  m_histEventCount->Fill(5, allEventsCBK->sumOfEventWeightsSquared());

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
  static const SG::AuxElement::ConstAccessor<int> a_eta_module("eta_module");
  static const SG::AuxElement::ConstAccessor<int> a_phi_module("phi_module");
  static const SG::AuxElement::ConstAccessor<int> a_nRDO("nRDO");
  static const SG::AuxElement::ConstAccessor<std::vector<int> > a_rdo_tot("rdo_tot");

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

  //
  // Truth histograms
  //std::cout << "TRUTH HISTOGRAMS" << std::endl;
  if(isMC)
    {
      int nRhadron=0;
      int nChRhadron=0;
      int nNeRhadron=0;

      const xAOD::TruthParticleContainer* truths=0;
      ANA_CHECK(m_event->retrieve(truths, "TruthParticles"));
      //std::cout << "New Event" << std::endl;
      for(const auto& truth : *truths)
	{
	  if(isRHadron(*truth))
	    {
	      //std::cout << "FOUND R HADRON " << truth->decayVtx() << " and " << truth->hasDecayVtx() << std::endl;
	      if(truth->hasDecayVtx() && truth->decayVtx()->t()==0) continue;
	      // std::cout << "FOUND ONE " << truth->p4().M()/1e3 << " with PDGID=" << truth->pdgId() << std::endl;
	      // std::cout << "pT = " << truth->p4().Pt()/1e3 << " eta = " << truth->p4().Eta() << " phi = " << truth->p4().Phi() << std::endl;
	      // std::cout << "Decay Vertex (x,y,z)     = (" << truth->decayVtx()->x() << "," << truth->decayVtx()->y() << "," << truth->decayVtx()->z() << ")" << std::endl;
	      // std::cout << "             (t,eta,phi) = (" << truth->decayVtx()->t() << "," << truth->decayVtx()->eta() << "," << truth->decayVtx()->phi() << ")" << std::endl;

	      nRhadron++;
	      if(isChargedRHadron(*truth)) nChRhadron++;
	      else nNeRhadron++;

	      h_Rhadron_pt ->Fill(truth->p4().Pt() /1000., weight);
	      h_Rhadron_eta->Fill(truth->p4().Eta()      , weight);
	      h_Rhadron_phi->Fill(truth->p4().Phi()      , weight);
	      h_Rhadron_m  ->Fill(truth->p4().M()  /1000., weight);
	      h_Rhadron_p  ->Fill(truth->p4().P()  /1000., weight);

	      if(truth->hasDecayVtx())
		{
		  //std::cout << "DECAY " << truth->decayVtx() << std::endl;
		  h_Rhadron_decVtx_x  ->Fill(truth->decayVtx()->x(), weight);
		  h_Rhadron_decVtx_y  ->Fill(truth->decayVtx()->y(), weight);
		  h_Rhadron_decVtx_z  ->Fill(truth->decayVtx()->z(), weight);

		  h_Rhadron_decVtx_t  ->Fill(truth->decayVtx()->t()  , weight);
		  h_Rhadron_decVtx_eta->Fill(truth->decayVtx()->eta(), weight);
		  h_Rhadron_decVtx_phi->Fill(truth->decayVtx()->phi(), weight);
		}
	      //std::cout << "DONE" << std::endl;
	    }
	}
      h_nRhadron->Fill(nRhadron, weight);
      h_nChRhadron->Fill(nChRhadron, weight);
      h_nNeRhadron->Fill(nNeRhadron, weight);
    }

  //
  // Cluster histograms
  //std::cout << "CLUSTER HISTOGRAMS" << std::endl;
  const xAOD::TrackMeasurementValidationContainer* clusters=0;
  ANA_CHECK(m_event->retrieve(clusters, "PixelClusters"));

  int nRDO, phi_module, eta_module, abseta_module;
  std::vector<xAOD::TrackMeasurementValidation*> clustersIn      [20][14];
  for(const auto& cluster : *clusters)
    {
      if(a_layer(*cluster)!=0 || a_bec(*cluster)!=0) continue;
      //if(a_nRDO(*cluster)==0) { std::cout << "nRDO==0" << std::endl; continue; }
      phi_module=a_phi_module(*cluster);
      eta_module=a_eta_module(*cluster);
      abseta_module=(eta_module>=0)?eta_module:-eta_module-1;
      nRDO=a_nRDO(*cluster);

      clustersIn[10+eta_module][phi_module].push_back(cluster);

      if(nRDO<=3) continue;

      h_IBL->execute(*cluster, weight);
      h_IBL_eta_module   [eta_module+10]->execute(*cluster, weight);
      h_IBL_abseta_module[abseta_module]->execute(*cluster, weight);
    }

  // Calculate per chip stuff
  std::vector<int> rdo_tot;
  //std::cout << "PER CHIP" << std::endl;
  for(abseta_module=0; abseta_module<10; abseta_module++)
    {
      for(phi_module=0; phi_module<14; phi_module++)
	{
	  std::vector<int> tot_rdo_tot;

	  // right
	  tot_rdo_tot.clear();
	  for(const auto& cluster : clustersIn[10+abseta_module][phi_module])
	    {
	      if(!a_rdo_tot.isAvailable(*cluster)) continue;
	      rdo_tot=a_rdo_tot(*cluster);
	      tot_rdo_tot.insert(tot_rdo_tot.end(),rdo_tot.begin(), rdo_tot.end());
	    }
	  h_perIBLchip->execute(tot_rdo_tot, weight);
	  h_perIBLchip_eta_module[10+abseta_module]->execute(tot_rdo_tot, weight);
	  h_perIBLchip_abseta_module[abseta_module]->execute(tot_rdo_tot, weight);

	  // left
	  tot_rdo_tot.clear();
	  for(const auto& cluster : clustersIn[10-abseta_module-1][phi_module])
	    {
	      if(!a_rdo_tot.isAvailable(*cluster)) continue;
	      rdo_tot=a_rdo_tot(*cluster);
	      tot_rdo_tot.insert(tot_rdo_tot.end(),rdo_tot.begin(), rdo_tot.end());
	    }
	  h_perIBLchip->execute(tot_rdo_tot, weight);
	  h_perIBLchip_eta_module[10-abseta_module-1]->execute(tot_rdo_tot, weight);
	  h_perIBLchip_abseta_module[abseta_module]  ->execute(tot_rdo_tot, weight);
	}
    }

  //
  // Track histograms
  //std::cout << "TRACK HISTOGRAMS" << std::endl;
  const xAOD::TrackParticleContainer* tracks=0;
  ANA_CHECK(m_event->retrieve(tracks, "InDetTrackParticles"));

  uint nRhadtrk=0;
  // std::cout << "TRACK LIST" << std::endl;
  for(const auto& track : *tracks)
    {
      // Determine if is Rhadron
      const xAOD::TruthParticle *truth=getTruthParticle(track);
      bool isRhad=(truth!=0 && isChargedRHadron(*truth));

      // std::cout << "TRACK pt,eta,phi,d0,z0 = " << track->p4().Pt() << "," << track->p4().Eta() << "," << track->p4().Phi() << "," << track->d0() << "," << track->z0() << std::endl;
      // std::cout << "\tradiusOfFirstHit = " << track->radiusOfFirstHit() << std::endl;
      // std::cout << "\thitPattern = b" << std::bitset<32>(track->hitPattern()) << std::endl;
      
      if(isRhad)
	{
	  nRhadtrk++;
	  h_Rhadtrk_pt ->Fill(track->p4().Pt() , weight);
	  h_Rhadtrk_eta->Fill(track->p4().Eta(), weight);
	  h_Rhadtrk_phi->Fill(track->p4().Phi(), weight);
	}

      // std::cout << "FOUND ONE " << truth->p4().M()/1e3 << " with PDGID=" << truth->pdgId() << std::endl;
      // std::cout << "pT = " << truth->p4().Pt()/1e3 << " eta = " << truth->p4().Eta() << " phi = " << truth->p4().Phi() << std::endl;
      // std::cout << "Decay Vertex (x,y,z)     = (" << truth->decayVtx()->x() << "," << truth->decayVtx()->y() << "," << truth->decayVtx()->z() << ")" << std::endl;
      // std::cout << "             (t,eta,phi) = (" << truth->decayVtx()->t() << "," << truth->decayVtx()->eta() << "," << truth->decayVtx()->phi() << ")" << std::endl;

      // Loop over clusters
      if( track->isAvailable< MeasurementsOnTrackLink >( "msosLink" ) ) 
  	{
  	  for ( const auto& msos : track->auxdataConst< MeasurementsOnTrackLink > ("msosLink") ) 
  	    {
  	      if( (*msos)->detType() == 1 )
  		{
  		  //it's a measurement from pixel detector
		  TrackMeasurementValidationLink clusterLink= (*msos)->trackMeasurementValidationLink();
		  if(!clusterLink.isValid()) continue;
		  const xAOD::TrackMeasurementValidation* cluster = *clusterLink;
		  if(a_layer(*cluster)!=0 || a_bec(*cluster)!=0) continue; // Not IBL

		  eta_module   =a_eta_module(*cluster);
		  abseta_module=(eta_module>=0)?eta_module:-eta_module-1;

		  if(isRhad)
		    {
		      h_RHad_IBL->execute(*cluster, weight);
		      h_RHad_IBL_eta_module   [eta_module+10]->execute(*cluster, weight);
		      h_RHad_IBL_abseta_module[abseta_module]->execute(*cluster, weight);

		      h_Rhadtrk_IBL_eta_module   ->Fill(eta_module   , weight);
		      h_Rhadtrk_IBL_abseta_module->Fill(abseta_module, weight);
		    }
		  else
		    {
		      h_nonRHad_IBL->execute(*cluster, weight);
		      h_nonRHad_IBL_eta_module   [eta_module+10]->execute(*cluster, weight);
		      h_nonRHad_IBL_abseta_module[abseta_module]->execute(*cluster, weight);
		    }
  		}
  	    }
  	}
    }

  h_nRhadtrk->Fill(nRhadtrk, weight);
  //std::cout << "nRhadtrk = " << nRhadtrk << std::endl;

  //std::cout << "DONE" << std::endl;
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

  //
  // Cleanup histograms
  h_IBL->finalize();
  for(int i=-10; i<=9; i++) h_IBL_eta_module[i+10]->finalize();
  for(int i=0; i<=9; i++) h_IBL_abseta_module[i]->finalize();

  h_perIBLchip->finalize();
  for(int i=-10; i<=9; i++) h_perIBLchip_eta_module[i+10]->finalize();
  for(int i=0; i<=9; i++) h_perIBLchip_abseta_module[i]->finalize();

  h_RHad_IBL->finalize();
  for(int i=-10; i<=9; i++) h_RHad_IBL_eta_module[i+10]->finalize();
  for(int i=0; i<=9; i++) h_RHad_IBL_abseta_module[i]->finalize();

  h_nonRHad_IBL->finalize();
  for(int i=-10; i<=9; i++) h_nonRHad_IBL_eta_module[i+10]->finalize();
  for(int i=0; i<=9; i++) h_nonRHad_IBL_abseta_module[i]->finalize();

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
