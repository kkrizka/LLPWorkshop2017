#include <LLPWorkshop2017/PixelClusterHists.h>

#include <iostream>

PixelClusterHists::PixelClusterHists(const std::string& name, const std::string& detailStr)
  : HistogramManager(name, detailStr)
{ }

PixelClusterHists::~PixelClusterHists()
{ }

StatusCode PixelClusterHists::initialize()
{
  h_nRDO  =book(m_name,"nRDO", "N_{RDO}", 51 ,-0.5,50.5);

  h_ToT   =book(m_name,"ToT",   "PixelCluster ToT"        ,200,  0.  ,2000.);
  h_ToTavg=book(m_name,"ToTavg","PixelCluster ToT/N_{RDO}", 30,  0.  ,  15.);
  h_ToTmin=book(m_name,"ToTmin","min(Pixel ToT)"     , 16, -0.5 ,  15.5);
  h_ToTmax=book(m_name,"ToTmax","max(Pixel ToT)"     , 16, -0.5 ,  15.5);

  h_nED      =book(m_name,"nED",  "N_{hits}"                   ,  21, -0.5 ,20.5);
  h_ED       =book(m_name,"ED",   "PixelCluster ED"                 , 100,  0.  , 2.);
  h_EDavg    =book(m_name,"EDavg","PixelCluster ED/N_{hits}"        , 100,  0.  , 2.);

  h_EDmin    =book(m_name,"EDmin","min(PixelCluster ED)"            , 100,  0.  , 2.);
  h_EDmax    =book(m_name,"EDmax","max(PixelCluster ED)"            , 100,  0.  , 2.);
  h_EDminnorm=book(m_name,"EDminnorm","min(PixelCluster ED)/size Z" , 100,  0.  , 2.);
  h_EDmaxnorm=book(m_name,"EDmaxnorm","max(PixelCluster ED)/size Z" , 100,  0.  , 2.);

  h_ToTvsED          =book(m_name, "ToTvsED",          "PixelCluster ED",             100, 0. ,2., "PixelCluster ToT"   , 200,  0.  , 2000.);
  h_ToTmaxvsEDmaxnorm=book(m_name, "ToTmaxvsEDmaxnorm","max(PixelCluster ED)/size Z", 100, 0. ,2., "max(Pixel ToT)",  16, -0.5 ,   15.5);
  h_ToTminvsEDminnorm=book(m_name, "ToTminvsEDminnorm","min(PixelCluster ED)/size Z", 100, 0. ,2., "min(Pixel ToT)",  16, -0.5 ,   15.5);

  return StatusCode::SUCCESS;
}

StatusCode PixelClusterHists::execute(const xAOD::TrackMeasurementValidation& cluster, double eventWeight)
{ 
  static const SG::AuxElement::ConstAccessor<int> a_ToT("ToT");
  static const SG::AuxElement::ConstAccessor<int> a_nRDO("nRDO");
  static const SG::AuxElement::ConstAccessor<int> a_sizeZ("sizeZ");
  static const SG::AuxElement::ConstAccessor<std::vector<int> > a_rdo_tot("rdo_tot");
  static const SG::AuxElement::ConstAccessor<std::vector<float> > a_sihit_energyDeposit("sihit_energyDeposit");

  int ToT =a_ToT (cluster);
  int nRDO=a_nRDO(cluster);
  int sizeZ=a_sizeZ(cluster);

  h_ToT   ->Fill(ToT                        ,eventWeight);
  h_nRDO  ->Fill(nRDO                       ,eventWeight);
  h_ToTavg->Fill(((float)ToT)/((float)nRDO) ,eventWeight);

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
	  h_ToTmin->Fill(ToTmin ,eventWeight);
	  h_ToTmax->Fill(ToTmax ,eventWeight);
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

	  h_nED  ->Fill(nED ,eventWeight);
	  h_ED   ->Fill(ED ,eventWeight);
	  h_EDavg->Fill(ED/nED ,eventWeight);
	  h_EDmin->Fill(EDmin ,eventWeight);
	  h_EDmax->Fill(EDmax ,eventWeight);
	  h_EDminnorm->Fill(EDmin/sizeZ ,eventWeight);
	  h_EDmaxnorm->Fill(EDmax/sizeZ ,eventWeight);
	}
    }

  //
  // 2D
  if(nED==1)
    {
      h_ToTvsED->Fill(ED, ToT, eventWeight);
      if(sizeZ>0)
	{
	  h_ToTmaxvsEDmaxnorm->Fill(EDmax/sizeZ, ToTmax, eventWeight);
	  h_ToTminvsEDminnorm->Fill(EDmin/sizeZ, ToTmin, eventWeight);
	}
    }

  return StatusCode::SUCCESS;
}

StatusCode PixelClusterHists::execute(const std::vector<int>& rdo_tot, double eventWeight)
{ 
  int ToT =std::accumulate(rdo_tot.begin(), rdo_tot.end(), 0);
  int nRDO=rdo_tot.size();

  h_ToT   ->Fill(ToT                        ,eventWeight);
  h_nRDO  ->Fill(nRDO                       ,eventWeight);
  h_ToTavg->Fill(((float)ToT)/((float)nRDO) ,eventWeight);

  // pixel hit info
  if(rdo_tot.size()==0)
    {
      h_ToTmin->Fill(0. ,eventWeight);
      h_ToTmax->Fill(0. ,eventWeight);
    }
  else
    {
      auto minmax_tot=std::minmax_element(rdo_tot.begin(), rdo_tot.end());
      h_ToTmin->Fill(*minmax_tot.first ,eventWeight);
      h_ToTmax->Fill(*minmax_tot.second,eventWeight);
    }

  return StatusCode::SUCCESS;
}

