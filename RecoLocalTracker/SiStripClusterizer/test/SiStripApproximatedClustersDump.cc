#include "RecoLocalTracker/SiStripClusterizer/test/SiStripApproximatedClustersDump.h"


SiStripApproximatedClustersDump::SiStripApproximatedClustersDump(const edm::ParameterSet& conf) {
  inputTagClusters = conf.getParameter< edm::InputTag >("approximatedClustersTag");
  clusterToken = consumes< edmNew::DetSetVector<SiStripApproximateClusterv2>>(inputTagClusters);
  
  usesResource("TFileService");

  outNtuple = fs_->make<TNtuple>("Pedestals", "Pedestals", 2048, -1023.5, 1023.5);
  
}

SiStripApproximatedClustersDump::~SiStripApproximatedClustersDump() {}

void SiStripApproximatedClustersDump::analyze(const edm::Event& e, const edm::EventSetup& es) {
  
  edm::Handle<edmNew::DetSetVector<SiStripApproximateClusterv2>> inClusters;
  event.getByToken(clusterToken, inClusters);




  TFileDirectory sdProcessedRawDigis_ = fs_->mkdir("ProcessedRawDigis");
  TFileDirectory sdBaseline_ = fs_->mkdir("Baseline");
  TFileDirectory sdBaselinePoints_ = fs_->mkdir("BaselinePoints");
  TFileDirectory sdClusters_ = fs_->mkdir("Clusters");
  TFileDirectory sdDigis_ = fs_->mkdir("Digis");

  edm::DetSetVector<SiStripProcessedRawDigi>::const_iterator itDSBaseline;
  if (plotBaseline_)
    itDSBaseline = moduleBaseline->begin();
  edm::DetSetVector<SiStripRawDigi>::const_iterator itRawDigis = moduleRawDigi->begin();

  uint32_t NBabAPVs = moduleRawDigi->size();
  std::cout << "Number of module with HIP in this event: " << NBabAPVs << std::endl;
  h1BadAPVperEvent_->Fill(NBabAPVs);

  for (; itRawDigis != moduleRawDigi->end(); ++itRawDigis) {
    if (actualModule_ > nModuletoDisplay_)
      return;
    uint32_t detId = itRawDigis->id;

    if (plotBaseline_) {
      //std::cout << "bas id: " << itDSBaseline->id << " raw id: " << detId << std::endl;
      if (itDSBaseline->id != detId) {
        std::cout << "Collections out of Synch. Something of fishy is going on ;-)" << std::endl;
        return;
      }
    }

    actualModule_++;
    edm::RunNumber_t const run = e.id().run();
    edm::EventNumber_t const event = e.id().event();
    //std::cout << "processing module N: " << actualModule_<< " detId: " << detId << " event: "<< event << std::endl;

    edm::DetSet<SiStripRawDigi>::const_iterator itRaw = itRawDigis->begin();
    bool restAPV[6] = {false, false, false, false, false, false};
    int strip = 0, totADC = 0;
    int minAPVRes = 7, maxAPVRes = -1;
    for (; itRaw != itRawDigis->end(); ++itRaw, ++strip) {
      float adc = itRaw->adc();
      totADC += adc;
      if (strip % 127 == 0) {
        //std::cout << "totADC " << totADC << std::endl;
        int APV = strip / 128;
        if (totADC != 0) {
          restAPV[APV] = true;
          totADC = 0;
          if (APV > maxAPVRes)
            maxAPVRes = APV;
          if (APV < minAPVRes)
            minAPVRes = APV;
        }
      }
    }

    uint16_t bins = 768;
    float minx = -0.5, maxx = 767.5;
    if (minAPVRes != 7) {
      minx = minAPVRes * 128 - 0.5;
      maxx = maxAPVRes * 128 + 127.5;
      bins = maxx - minx;
    }

    sprintf(detIds, "%ul", detId);
    sprintf(evs, "%llu", event);
    sprintf(runs, "%u", run);
    char* dHistoName = Form("Id:%s_run:%s_ev:%s", detIds, runs, evs);
    h1ProcessedRawDigis_ = sdProcessedRawDigis_.make<TH1F>(dHistoName, dHistoName, bins, minx, maxx);

    if (plotBaseline_) {
      h1Baseline_ = sdBaseline_.make<TH1F>(dHistoName, dHistoName, bins, minx, maxx);
      h1Baseline_->SetXTitle("strip#");
      h1Baseline_->SetYTitle("ADC");
      h1Baseline_->SetMaximum(1024.);
      h1Baseline_->SetMinimum(-300.);
      h1Baseline_->SetLineWidth(2);
      h1Baseline_->SetLineStyle(2);
      h1Baseline_->SetLineColor(2);
    }

    if (plotClusters_) {
      h1Clusters_ = sdClusters_.make<TH1F>(dHistoName, dHistoName, bins, minx, maxx);

      h1Clusters_->SetXTitle("strip#");
      h1Clusters_->SetYTitle("ADC");
      h1Clusters_->SetMaximum(1024.);
      h1Clusters_->SetMinimum(-300.);
      h1Clusters_->SetLineWidth(2);
      h1Clusters_->SetLineStyle(2);
      h1Clusters_->SetLineColor(3);
    }

    h1ProcessedRawDigis_->SetXTitle("strip#");
    h1ProcessedRawDigis_->SetYTitle("ADC");
    h1ProcessedRawDigis_->SetMaximum(1024.);
    h1ProcessedRawDigis_->SetMinimum(-300.);
    h1ProcessedRawDigis_->SetLineWidth(2);

    std::vector<int16_t> ProcessedRawDigis(itRawDigis->size());
    subtractorPed_->subtract(*itRawDigis, ProcessedRawDigis);

    edm::DetSet<SiStripProcessedRawDigi>::const_iterator itBaseline;
    if (plotBaseline_)
      itBaseline = itDSBaseline->begin();
    std::vector<int16_t>::const_iterator itProcessedRawDigis;

    strip = 0;
    for (itProcessedRawDigis = ProcessedRawDigis.begin(); itProcessedRawDigis != ProcessedRawDigis.end();
         ++itProcessedRawDigis) {
      if (restAPV[strip / 128]) {
        float adc = *itProcessedRawDigis;
        h1ProcessedRawDigis_->Fill(strip, adc);
        if (plotBaseline_) {
          h1Baseline_->Fill(strip, itBaseline->adc());
          ++itBaseline;
        }
      }
      ++strip;
    }

    if (plotBaseline_)
      ++itDSBaseline;
    if (plotClusters_) {
      edmNew::DetSetVector<SiStripCluster>::const_iterator itClusters = clusters->begin();
      for (; itClusters != clusters->end(); ++itClusters) {
        for (edmNew::DetSet<SiStripCluster>::const_iterator clus = itClusters->begin(); clus != itClusters->end();
             ++clus) {
          if (itClusters->id() == detId) {
            int firststrip = clus->firstStrip();
            //std::cout << "Found cluster in detId " << detId << " " << firststrip << " " << clus->amplitudes().size() << " -----------------------------------------------" << std::endl;
            strip = 0;
            for (auto itAmpl = clus->amplitudes().begin(); itAmpl != clus->amplitudes().end(); ++itAmpl) {
              h1Clusters_->Fill(firststrip + strip, *itAmpl);
              ++strip;
            }
          }
        }
      }
    }
  }
}

DEFINE_FWK_MODULE(SiStripApproximatedClustersDump);
