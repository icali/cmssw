//#include "TriggerPrimitivesTree_tower.C"
//#include "TriggerPrimitivesTree_towerMEN.C"
#include "TriggerPrimitivesTree_trackcurve.C"

void run_tracktrigger(int nevents=10000, int threshhold=0, bool isPhiAv=0 /*, bool cut_noise_events = 0*/, int minCentBin = 0, int maxCentBin = 40)
//void run_tracktrigger(int nevents=10000, bool isPU=1, int centmin=0, int centmax=40)
{
//  TriggerPrimitivesTree_tower* minbias = 0;
  TriggerPrimitivesTree_trackcurve* minbias = 0;
  minbias = new TriggerPrimitivesTree_trackcurve(TFile::Open("root://eoscms//eos/cms/store/caf/user/davidlw/HIHighPt_Skim_HLT_HIFullTrack14_FOREST.root"));
//  minbias = new TriggerPrimitivesTree_trackcurve(TFile::Open("root://xrootd1.cmsaf.mit.edu//store/user/davidlw/HIHighPt_Skim_HLT_HIFullTrack14_FOREST.root"));
// new TriggerPrimitivesTree_trackcurve(new TFile("root://xrootd1.cmsaf.mit.edu/mnt/hadoop/cms/store/user/davidlw/HIHighPt_Skim_HLT_HIFullTrack14_FOREST.root"));
//  minbias = new TriggerPrimitivesTree_trackcurve(new TFile("/mnt/hadoop/cms/store/user/luck/HiMinbias_L1_tracks_forest/HiMinbias_L1_track_forest_all.root"));
//  TH2D *efficiency_curve = minbias->Loop(nevents,0,isPU,centmin,centmax);
   TFile *efficiency_curve = minbias->Loop(nevents,threshhold,isPhiAv, minCentBin, maxCentBin);
//   minbias->Loop(nevents,threshhold,isPhiAv, minCentBin, maxCentBin);   
//   TCanvas* c = new TCanvas("c","c",550,500);
//   efficiency_curve->Draw("colz");
//   efficiency_curve->Draw();
//   TFile* fout = new TFile(Form("efficiency_track_PU%d_centmin%d_centmax%d.root",isPU,centmin,centmax),"recreate");
//   TFile* fout = new TFile(Form("MaxEtvsMaxPt_alleta.root",threshhold,nevents),"recreate");
//  TFile* fout = new TFile(Form("minbias_tree"),"recreate");	
//  minbias->Write();
//   efficiency_curve->Write();
//    fout->Close();
} 
