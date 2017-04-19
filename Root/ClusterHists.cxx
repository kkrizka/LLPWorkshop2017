#include <MyLongLivedAnalysis/ClusterHists.h>

ClusterHists::ClusterHists(const std::string& name)
  : m_name(name)
{ }

void ClusterHists::record(EL::Worker *wk)
{
  wk->addOutput(h_ToT);
  wk->addOutput(h_nRDO);
  wk->addOutput(h_ToTavg);
}

void ClusterHists::initialize()
{ 
  h_ToT   =new TH1F((m_name+"/ToT").c_str(),   "ToT"        ,200, 0. ,2000.);
  h_nRDO  =new TH1F((m_name+"/nRDO").c_str(),  "N_{RDO}"    ,51 ,-0.5,50.5);
  h_ToTavg=new TH1F((m_name+"/ToTavg").c_str(),"ToT/N_{RDO}",30 , 0. ,15.);
}

void ClusterHists::execute(const xAOD::TrackMeasurementValidation& cluster, double weight)
{ 
  static const SG::AuxElement::ConstAccessor<int> a_ToT("ToT");
  static const SG::AuxElement::ConstAccessor<int> a_nRDO("nRDO");

  int ToT =a_ToT (cluster);
  int nRDO=a_nRDO(cluster);

  h_ToT   ->Fill(ToT                        ,weight);
  h_nRDO  ->Fill(nRDO                       ,weight);
  h_ToTavg->Fill(((float)ToT)/((float)nRDO) ,weight);

}

void ClusterHists::finalize()
{ }
