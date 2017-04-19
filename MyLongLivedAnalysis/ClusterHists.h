#ifndef CLUSTERHISTS_H_
#define CLUSTERHISTS_H_

#include <TH1.h>

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
  void finalize();

private:
  std::string m_name;

  TH1 *h_ToT;
  TH1 *h_nRDO;
  TH1 *h_ToTavg;
};

#endif // CLUSTERHISTS_H_
