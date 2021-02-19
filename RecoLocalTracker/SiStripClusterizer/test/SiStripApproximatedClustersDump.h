#ifndef RecoLocalTracker_ClusterRefinerTagMCmerged_h
#define RecoLocalTracker_ClusterRefinerTagMCmerged_h


// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"
#include "DataFormats/SiStripApproximateClusterv2/interface/SiStripApproximateClusterv2.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CommonTools/Utils/interface/TFileDirectory.h"

#include <memory>
#include <iostream>

//ROOT inclusion
#include "TROOT.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TList.h"
#include "TString.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "THStack.h"

//
// class decleration
//

class SiStripApproximatedClustersDump : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit SiStripApproximatedClustersDump(const edm::ParameterSet&);
  ~SiStripApproximatedClustersDump() override;

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;

  edm::InputTag inputClusters;
  edm::EDGetTokenT< edmNew::DetSetVector<SiStripApproximateClusterv2> > clusterToken;  

  TNtuple* outNtuple;
  edm::Service<TFileService> fs;

};
 #endif
