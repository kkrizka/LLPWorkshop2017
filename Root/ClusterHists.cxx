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

  wk->addOutput(h_nED);
  wk->addOutput(h_ED);
  wk->addOutput(h_EDavg);
  wk->addOutput(h_EDmin);
  wk->addOutput(h_EDmax);
  wk->addOutput(h_EDminnorm);
  wk->addOutput(h_EDmaxnorm);

  wk->addOutput(h_ToTvsED);
  wk->addOutput(h_ToTminvsEDminnorm);
  wk->addOutput(h_ToTmaxvsEDmaxnorm);
}

void ClusterHists::initialize()
{ 
  h_nRDO  =new TH1F((m_name+"/nRDO").c_str(),  ";N_{RDO}"    ,51 ,-0.5,50.5);

  h_ToT   =new TH1F((m_name+"/ToT").c_str(),   ";Cluster ToT"        ,200,  0.  ,2000.);
  h_ToTavg=new TH1F((m_name+"/ToTavg").c_str(),";Cluster ToT/N_{RDO}", 30,  0.  ,  15.);
  h_ToTmin=new TH1F((m_name+"/ToTmin").c_str(),";min(Pixel ToT)"     , 16, -0.5 ,  15.5);
  h_ToTmax=new TH1F((m_name+"/ToTmax").c_str(),";max(Pixel ToT)"     , 16, -0.5 ,  15.5);

  h_nED  =new TH1F((m_name+"/nED").c_str(),  ";N_{hits}"           ,  21, -0.5 ,20.5);
  h_ED   =new TH1F((m_name+"/ED").c_str(),   ";Cluster ED"         , 100,  0.   ,5.);
  h_EDavg=new TH1F((m_name+"/EDavg").c_str(),";Cluster ED/N_{hits}", 100,  0.   ,5.);
  h_EDmin=new TH1F((m_name+"/EDmin").c_str(),";min(Cluster ED)"    , 100,  0.   ,5.);
  h_EDmax=new TH1F((m_name+"/EDmax").c_str(),";max(Cluster ED)"    , 100,  0.   ,5.);
  h_EDminnorm=new TH1F((m_name+"/EDminnorm").c_str(),";min(Cluster ED)/size Z"    , 100,  0.   ,5.);
  h_EDmaxnorm=new TH1F((m_name+"/EDmaxnorm").c_str(),";max(Cluster ED)/size Z"    , 100,  0.   ,5.);

  h_ToTvsED=new TH2F((m_name+"/ToTvsED").c_str(),";Cluster ED;Cluster ToT", 500, 0. ,5., 200, 0., 2000.);
  h_ToTmaxvsEDmaxnorm=new TH2F((m_name+"/ToTmaxvsEDmaxnorm").c_str(),";max(Cluster ED)/size Z;max(Pixel ToT)", 500, 0. ,5., 16, -0.5 ,  15.5);
  h_ToTminvsEDminnorm=new TH2F((m_name+"/ToTminvsEDminnorm").c_str(),";min(Cluster ED)/size Z;min(Pixel ToT)", 500, 0. ,5., 16, -0.5 ,  15.5);
}

void ClusterHists::execute(const xAOD::TrackMeasurementValidation& cluster, double weight)
{ 
  static const SG::AuxElement::ConstAccessor<int> a_ToT("ToT");
  static const SG::AuxElement::ConstAccessor<int> a_nRDO("nRDO");
  static const SG::AuxElement::ConstAccessor<int> a_sizeZ("sizeZ");
  static const SG::AuxElement::ConstAccessor<std::vector<int> > a_rdo_tot("rdo_tot");
  static const SG::AuxElement::ConstAccessor<std::vector<float> > a_sihit_energyDeposit("sihit_energyDeposit");

  int ToT =a_ToT (cluster);
  int nRDO=a_nRDO(cluster);
  int sizeZ=a_sizeZ(cluster);

  h_ToT   ->Fill(ToT                        ,weight);
  h_nRDO  ->Fill(nRDO                       ,weight);
  h_ToTavg->Fill(((float)ToT)/((float)nRDO) ,weight);

  //
  // pixel hits
  float ToTmax=0.;
  float ToTmin=0.;
  if(a_rdo_tot.isAvailable(cluster))
    {
      std::vector<int> rdo_tot=a_rdo_tot(cluster);
      if(rdo_tot.size()>0)
	{
	  auto minmax_tot=std::minmax_element(rdo_tot.begin(), rdo_tot.end());
	  ToTmin=*minmax_tot.first;
	  ToTmax=*minmax_tot.second;
	  h_ToTmin->Fill(ToTmin ,weight);
	  h_ToTmax->Fill(ToTmax ,weight);
	}
    }

  //
  // energy deposits
  float ED=0.;
  float EDmin=0.;
  float EDmax=0.;
  int nED=0;
  if(a_sihit_energyDeposit.isAvailable(cluster))
    {
      std::vector<float> sihit_energyDeposit=a_sihit_energyDeposit(cluster);
      if(sihit_energyDeposit.size()>0)
	{
	  ED=std::accumulate(sihit_energyDeposit.begin(), sihit_energyDeposit.end(), 0.);
	  auto minmax_ed=std::minmax_element(sihit_energyDeposit.begin(), sihit_energyDeposit.end());

	  EDmin=*minmax_ed.first;
	  EDmax=*minmax_ed.second;

	  nED=sihit_energyDeposit.size();

	  h_nED  ->Fill(nED ,weight);
	  h_ED   ->Fill(ED ,weight);
	  h_EDavg->Fill(ED/nED ,weight);
	  h_EDmin->Fill(EDmin ,weight);
	  h_EDmax->Fill(EDmax ,weight);
	  h_EDminnorm->Fill(EDmin/sizeZ ,weight);
	  h_EDmaxnorm->Fill(EDmax/sizeZ ,weight);
	}
    }

  //
  // 2D
  if(nED==1)
    {
      h_ToTvsED->Fill(ED, ToT, weight);
      if(sizeZ>0)
	{
	  h_ToTmaxvsEDmaxnorm->Fill(EDmax/sizeZ, ToTmax, weight);
	  h_ToTminvsEDminnorm->Fill(EDmin/sizeZ, ToTmin, weight);
	}
    }
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
