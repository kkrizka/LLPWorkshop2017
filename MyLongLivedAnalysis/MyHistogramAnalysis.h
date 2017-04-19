#ifndef MyLongLivedAnalysis_MyHistogramAnalysis_H
#define MyLongLivedAnalysis_MyHistogramAnalysis_H

#include <EventLoop/Algorithm.h>

#include <AsgTools/AnaToolHandle.h>

#include <TrigConfxAOD/xAODConfigTool.h>
#include <TrigDecisionTool/TrigDecisionTool.h>


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
  TH1 *h_histIBLToT; //!
  TH1 *h_histIBLnRDO; //!
  TH1 *h_histIBLToTavg; //!


  // this is needed to distribute the algorithm to the workers
  ClassDef(MyHistogramAnalysis, 1);
};

#endif
