#include <LLPWorkshop2017/ClusterHists.h>

#include <iostream>

ClusterHists::ClusterHists(const std::string& name)
  : m_name(name)
{ }

void ClusterHists::record(EL::Worker *wk)
{
  wk->addOutput(h_nRDO);

  wk->addOutput(h_ToT);
  wk->addOutput(h_ToTavg);
  wk->addOutput(h_ToTmin);
  wk->addOutput(h_ToTmax);

  wk->addOutput(h_ED);
  wk->addOutput(h_EDavg);
  wk->addOutput(h_EDmin);
  wk->addOutput(h_EDmax);

  wk->addOutput(h_ToTvsED);
}

void ClusterHists::initialize()
{ 
  h_nRDO  =new TH1F((m_name+"/nRDO").c_str(),  ";N_{RDO}"    ,51 ,-0.5,50.5);

  h_ToT   =new TH1F((m_name+"/ToT").c_str(),   ";Cluster ToT"        ,200,  0.  ,2000.);
  h_ToTavg=new TH1F((m_name+"/ToTavg").c_str(),";Cluster ToT/N_{RDO}", 30,  0.  ,  15.);
  h_ToTmin=new TH1F((m_name+"/ToTmin").c_str(),";min(Pixel ToT)"     , 16, -0.5 ,  15.5);
  h_ToTmax=new TH1F((m_name+"/ToTmax").c_str(),";max(Pixel ToT)"     , 16, -0.5 ,  15.5);

  h_ED   =new TH1F((m_name+"/ED").c_str(),   ";Cluster ED"        , 100, 0. ,5.);
  h_EDavg=new TH1F((m_name+"/EDavg").c_str(),";Cluster ED/N_{RDO}", 100, 0. ,5.);
  h_EDmin=new TH1F((m_name+"/EDmin").c_str(),";min(Pixel ED)"     , 100, 0. ,5.);
  h_EDmax=new TH1F((m_name+"/EDmax").c_str(),";max(Pixel ED)"     , 100, 0. ,5.);

  h_ToTvsED=new TH2F((m_name+"/ToTvsED").c_str(),";Pixel ED;Pixel ToT", 1000, -5. ,5., 200, 0., 2000.);
}

void ClusterHists::execute(const xAOD::TrackMeasurementValidation& cluster, double weight)
{ 
  static const SG::AuxElement::ConstAccessor<int> a_ToT("ToT");
  static const SG::AuxElement::ConstAccessor<int> a_nRDO("nRDO");
  static const SG::AuxElement::ConstAccessor<std::vector<int> > a_rdo_tot("rdo_tot");
  static const SG::AuxElement::ConstAccessor<std::vector<float> > a_sihit_energyDeposit("sihit_energyDeposit");

  int ToT =a_ToT (cluster);
  int nRDO=a_nRDO(cluster);

  h_ToT   ->Fill(ToT                        ,weight);
  h_nRDO  ->Fill(nRDO                       ,weight);
  h_ToTavg->Fill(((float)ToT)/((float)nRDO) ,weight);

  // pixel hit info
  if(a_rdo_tot.isAvailable(cluster))
    {
      std::vector<int> rdo_tot=a_rdo_tot(cluster);
      if(rdo_tot.size()>0)
	{
	  auto minmax_tot=std::minmax_element(rdo_tot.begin(), rdo_tot.end());
	  h_ToTmin->Fill(*minmax_tot.first ,weight);
	  h_ToTmax->Fill(*minmax_tot.second,weight);
	}
    }

  //
  // energy deposits
  float ED=0.;
  if(a_sihit_energyDeposit.isAvailable(cluster))
    {
      std::vector<float> sihit_energyDeposit=a_sihit_energyDeposit(cluster);
      if(sihit_energyDeposit.size()>0)
	{
	  ED=std::accumulate(sihit_energyDeposit.begin(), sihit_energyDeposit.end(), 0.);
	  auto minmax_ed=std::minmax_element(sihit_energyDeposit.begin(), sihit_energyDeposit.end());
	  h_ED->Fill(ED ,weight);
	  h_EDavg->Fill(ED/sihit_energyDeposit.size() ,weight);
	  h_EDmin->Fill(*minmax_ed.first ,weight);
	  h_EDmax->Fill(*minmax_ed.second,weight);
	}
    }

  if(a_rdo_tot.isAvailable(cluster))
    {
      std::vector<int> rdo_tot=a_rdo_tot(cluster);
      if(rdo_tot.size()>0)
	{
	  auto minmax_tot=std::minmax_element(rdo_tot.begin(), rdo_tot.end());
	  h_ToTmin->Fill(*minmax_tot.first ,weight);
	  h_ToTmax->Fill(*minmax_tot.second,weight);
	}
    }

  //
  // 2d 
  h_ToTvsED->Fill(ED, ToT, weight);
}

void ClusterHists::execute(const std::vector<int>& rdo_tot, double weight)
{ 
  int ToT =std::accumulate(rdo_tot.begin(), rdo_tot.end(), 0);
  int nRDO=rdo_tot.size();

  h_ToT   ->Fill(ToT                        ,weight);
  h_nRDO  ->Fill(nRDO                       ,weight);
  h_ToTavg->Fill(((float)ToT)/((float)nRDO) ,weight);

  // pixel hit info
  if(rdo_tot.size()==0)
    {
      h_ToTmin->Fill(0. ,weight);
      h_ToTmax->Fill(0. ,weight);
    }
  else
    {
      auto minmax_tot=std::minmax_element(rdo_tot.begin(), rdo_tot.end());
      h_ToTmin->Fill(*minmax_tot.first ,weight);
      h_ToTmax->Fill(*minmax_tot.second,weight);
    }
}

void ClusterHists::finalize()
{ }
