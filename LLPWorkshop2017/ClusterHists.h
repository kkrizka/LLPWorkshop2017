#ifndef CLUSTERHISTS_H_
#define CLUSTERHISTS_H_

#include <TH1.h>
#include <TH2.h>

#include <EventLoop/Worker.h>
#include <xAODTracking/TrackMeasurementValidation.h>

#include <string>

class ClusterHists
{
public:
  ClusterHists(const std::string &name);
  void record(EL::Worker *wk);
  void initialize();
  void execute(const xAOD::TrackMeasurementValidation& cluster, double weight);
  void execute(const std::vector<int>& rdo_tot, double weight);
  void finalize();

private:
  std::string m_name;

  TH1 *h_nRDO;

  TH1 *h_ToT;
  TH1 *h_ToTavg;
  TH1 *h_ToTmin;
  TH1 *h_ToTmax;

  TH1 *h_nED;
  TH1 *h_ED;
  TH1 *h_EDavg;
  TH1 *h_EDmin;
  TH1 *h_EDmax;
  TH1 *h_EDminnorm;
  TH1 *h_EDmaxnorm;

  TH2 *h_ToTvsED;
  TH2 *h_ToTmaxvsEDmaxnorm;
  TH2 *h_ToTminvsEDminnorm;
};

#endif // CLUSTERHISTS_H_
