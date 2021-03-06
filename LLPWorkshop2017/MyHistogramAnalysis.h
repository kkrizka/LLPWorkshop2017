#ifndef LLPWorkshop2017_MyHistogramAnalysis_H
#define LLPWorkshop2017_MyHistogramAnalysis_H

#include <EventLoop/Algorithm.h>

#include <AsgTools/AnaToolHandle.h>

#include <TrigConfxAOD/xAODConfigTool.h>
#include <TrigDecisionTool/TrigDecisionTool.h>

#include <LLPWorkshop2017/PixelClusterHists.h>

#include <TH1.h>

class MyHistogramAnalysis : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  // float cutValue;



  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!



  // this is a standard constructor
  MyHistogramAnalysis ();

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

private:
  xAOD::TEvent* m_event; //!
  xAOD::TStore* m_store; //!

  Trig::TrigDecisionTool *m_trigDecisionTool; //!
  TrigConf::xAODConfigTool *m_trigConfigTool; //!

  TH1* m_histEventCount; //!

  //
  // Histograms
  PixelClusterHists *h_IBL; //!
  PixelClusterHists *h_IBL_eta_module[20]; //!
  PixelClusterHists *h_IBL_abseta_module[10]; //!

  PixelClusterHists *h_perIBLchip; //!
  PixelClusterHists *h_perIBLchip_eta_module[20]; //!
  PixelClusterHists *h_perIBLchip_abseta_module[10]; //!

  PixelClusterHists *h_RHad_IBL; //!
  PixelClusterHists *h_RHad_IBL_eta_module[20]; //!
  PixelClusterHists *h_RHad_IBL_abseta_module[10]; //!

  PixelClusterHists *h_nonRHad_IBL; //!
  PixelClusterHists *h_nonRHad_IBL_eta_module[20]; //!
  PixelClusterHists *h_nonRHad_IBL_abseta_module[10]; //!

  TH1 *h_nRhadron; //!
  TH1 *h_nChRhadron; //!
  TH1 *h_nNeRhadron; //!

  TH1 *h_Rhadron_pt; //!
  TH1 *h_Rhadron_eta; //!
  TH1 *h_Rhadron_phi; //!
  TH1 *h_Rhadron_m; //!
  TH1 *h_Rhadron_p; //!
  TH1 *h_Rhadron_decVtx_x; //!
  TH1 *h_Rhadron_decVtx_y; //!
  TH1 *h_Rhadron_decVtx_z; //!
  TH1 *h_Rhadron_decVtx_t; //!
  TH1 *h_Rhadron_decVtx_eta; //!
  TH1 *h_Rhadron_decVtx_phi; //!

  TH1 *h_nRhadtrk; //!
  TH1 *h_Rhadtrk_pt; //!
  TH1 *h_Rhadtrk_eta; //!
  TH1 *h_Rhadtrk_phi; //!

  TH1 *h_Rhadtrk_IBL_eta_module; //!
  TH1 *h_Rhadtrk_IBL_abseta_module; //!

  // this is needed to distribute the algorithm to the workers
  ClassDef(MyHistogramAnalysis, 1);
};

#endif
