#ifndef CLUSTERHISTS_H_
#define CLUSTERHISTS_H_

#include <xAODAnaHelpers/HistogramManager.h>

#include <EventLoop/Worker.h>
#include <xAODTracking/TrackMeasurementValidation.h>

#include <string>

class PixelClusterHists : public HistogramManager
{
public:
  PixelClusterHists(const std::string &name, const std::string &detailStr="");
  virtual ~PixelClusterHists();

  virtual StatusCode initialize();
  StatusCode execute(const xAOD::TrackMeasurementValidation& cluster, double eventWeight);
  StatusCode execute(const std::vector<int>& rdo_tot, double eventWeight);
  using HistogramManager::book;    // make other overloaded version of book() to show up in subclass
  using HistogramManager::execute; // make other overloaded version of execute() to show up in subclass

private:
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
