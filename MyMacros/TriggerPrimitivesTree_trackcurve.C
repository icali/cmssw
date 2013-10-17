#define TriggerPrimitivesTree_trackcurve_cxx
#include "TriggerPrimitivesTree_trackcurve.h"
#include <TH2.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TString.h>
#include <TProfile.h>
#include <iostream>
#include <sstream>
//#include "TriggerPrimitivesTree_towerMEN.h"
#include "FindRegionJet.C"


using namespace std;

TFile* TriggerPrimitivesTree_trackcurve::Loop(int total_events,
					   int threshold,
					   bool PHI_AVERAGE,
					   int minCentBin,
					   int maxCentBin)
{
  TH1::SetDefaultSumw2();

  if (fChain == 0) return(0);
  
  Long64_t nentries = fChain->GetEntriesFast();
  
  const int nbins = 100; //number of track pt bins
  const int MAX_JET_EN = 200; //max energy shown by the histogram, in GeV
  const int NETA_REGIONS = 22;
  const int NPHI_REGIONS = 18;  

  TH2D* track_curve;
  TH2D* MaxEtMaxPt = new TH2D("MaxEtMaxPt","Max Region Et vs Max Track Pt",200,0,50,100,0,50);
// = new TH2D("track_curve",";Leading Track p_{T} (GeV/c); Leading L1 jet E_{T} (GeV)",nbins,0,200,100,0,200);
  TH2D* MaxEtEtaMaxPtEta = new TH2D("MaxEtEtaMaxPtEta","Max Region Et Eta vs Max Track Pt Eta",100,-2.5,2.5,100,-2.5,2.5);
  TH2D* MaxEtPhiMaxPtPhi = new TH2D("MaxEtPhiMaxPtPhi","Max Region Et Phi vs Max Track Pt Phi",100,-3.145,3.145,100,-3.145,3.145);
  TH2D* deltaEtadeltaPhi = new TH2D("deltaEtadeltaPhi","RegionEta-TrackEta vs. RegionPhi-TrackPhi",200,-4.0,4.0,100,-3.15,3.15);
  TH2D* NumAccEtThresh = new TH2D("NumAccEtThresh","Fraction of events acc vs Leading Region Et Threshold",100,-5.0,55.0,100,0.0,1.1);
  TH2D* RateSigEff = new TH2D("RateSigEff","Rate vs. Signal Efficiency",100, 0.0, 1.1, 100,0.0,1.1);
  TH1I* total_in_bin;

  TString method;
  TString method_s;

  if(PHI_AVERAGE)
  {
    method = "2015 System";
    method_s = "phisub";
  }
  else
  {
    method = "Current L1 System";
    method_s = "nosub";
  }

  stringstream name1,name2,name3;
  name1 << "Track Turn-On. "<< method <<  " Threshold:" << threshold << ";Leading Track p_{T} (GeV);Leading L1 jet E_{T} (GeV)";
//  name1 << "Track Turn-On. "<< method <<  " Threshold:" << threshold << ";Leading Track p_{T} (GeV/c);Leading HLT jet E_{T} (GeV)";
  name2 << "total_in_bin";
  name3 << "track_curve";
  track_curve = new TH2D(name3.str().c_str(), name1.str().c_str(),
		       nbins,0,MAX_JET_EN,nbins,0,MAX_JET_EN);

  total_in_bin = new TH1I(name2.str().c_str(),name2.str().c_str(),
			  nbins,0,MAX_JET_EN);

  int evts = 0;
  bool break_early = total_events != -1;
  int numaccet = 0;  
  int numaccpt = 0;
  int numaccptet = 0;
  for (int itol = 0; itol < 55; itol=itol+5)
 {
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    
    fhlt->GetEntry(jentry);
    fhiinfo->GetEntry(jentry);

    // If the event has halo, no tracks, or energy less than 3GeV in
    // both forward calorimeters than skip it.
    if( !(!(fhlt->L1Tech_BSC_halo_beam2_inner_v0 ||
	    fhlt->L1Tech_BSC_halo_beam2_outer_v0 ||
	    fhlt->L1Tech_BSC_halo_beam1_inner_v0 ||
	    fhlt->L1Tech_BSC_halo_beam1_outer_v0 )
	  && fhiinfo->hiNtracks>0
	  && fhiinfo->hiHFplus>3
	  && fhiinfo->hiHFminus>3))
      continue;

    trackTree->GetEntry(jentry);
    fjet->GetEntry(jentry);

    // If there are no tracks in the event skip it.
    if (ftracks.nTrk <= 0) continue;
    float_t realTrackPt = ftracks.GetMaxPt();
    float_t realTrackPtEta = ftracks.GetMaxPtEta();
    float_t realTrackPtPhi = ftracks.GetMaxPtPhi();
    float_t realJetPt = fjet->jtpt[0];
    int centBin = fhiinfo->hiBin;
    if(centBin < minCentBin || centBin > maxCentBin) continue;

    evts++;
    if(break_early && (evts > total_events)) break;
   // cout << evts << " " << realTrackPt << " " << realTrackPtEta << " " << realTrackPtPhi << endl;

    fChain->GetEntry(jentry);

    double fulldetector[NETA_REGIONS][NPHI_REGIONS]; //[eta][phi]
    for (int i = 0; i < NETA_REGIONS; i++)
    {
	for (int j = 0; j < NPHI_REGIONS; j++)
	{
		fulldetector[i][j] = 0.0;
	}
    }	

    for(int i = 0; i < numRegions; i++)
    {
      fulldetector[caloRegionEtaIndex[i]][caloRegionPhiIndex[i]] = caloRegionEt[i];
//	cout << i << " " << caloRegionEtaIndex[i] << " " << caloRegionPhiIndex[i] << endl;
    }
      
//------Fills in the phi-eta matrix for total Et values using eta and phi indexes------
    //Stolen from Doga
/*    for (int i=0; i<hcalDetectorMapSize;i++){
      if(hcalEtaIndex[i]<0 && hcalEtaIndex[i]>-29){
        for (int j=0;j<ecalDetectorMapSize;j++){
          if(ecalEtaIndex[j]==hcalEtaIndex[i] && ecalPhiIndex[j]==hcalPhiIndex[i]){
            int towertotal=hcalEt[i]+ecalEt[j];
            fulldetector[hcalEtaIndex[i]+28+16][hcalPhiIndex[i]-1]=towertotal;
            break;
          }
        }
      }
      if(hcalEtaIndex[i]>0 && hcalEtaIndex[i]<29){
        for (int j=0;j<ecalDetectorMapSize;j++){
          if(ecalEtaIndex[j]==hcalEtaIndex[i] && ecalPhiIndex[j]==hcalPhiIndex[i]){
            int towertotal=hcalEt[i]+ecalEt[j];
            fulldetector[hcalEtaIndex[i]+27+16][hcalPhiIndex[i]-1]=towertotal;
            break;
          }
        }
      }
      if (hcalEtaIndex[i]<-28){
        for (int k=0; k<4; k++){
          for (int l=0; l<4; l++){
            int towertotal=hcalEt[i]/16.0;
            fulldetector[(hcalEtaIndex[i]+32)*4+k][hcalPhiIndex[i]-1+l]=towertotal;
          }
        }
      }
      if (hcalEtaIndex[i]>28){
        for (int k=0; k<4; k++){
          for (int l=0; l<4; l++){
            int towertotal=hcalEt[i]/16.0;
            fulldetector[(hcalEtaIndex[i]-29)*4+72+k][hcalPhiIndex[i]-1+l]=towertotal;
          }
        }
      }
    }
    
*/

double tempnew = 0.0;
double tempold = 0.0;
double MaxEt = 0.0;
int MaxRegionEtaIndex = 0.0;
int MaxRegionPhiIndex = 0.0;
int MaxRegionEtaIndexMin = 0.0;


/* for(int i = 0; i < numRegions; i++)
    {
        if (!(regionEta(caloRegionEtaIndex[i]) < 2.0 && regionEta(caloRegionEtaIndex[i]) > -2.0)) continue;
        tempnew = fulldetector[caloRegionEtaIndex[i]][caloRegionPhiIndex[i]];
        if (tempnew > tempold)
        {
		MaxRegionEtaIndex = caloRegionEtaIndex[i];
		MaxRegionPhiIndex = caloRegionPhiIndex[i];
                MaxEt = tempnew;
                tempold = tempnew;
        }
    }
*/  

RegionJet *highestjet = findRegionJet(fulldetector);  

MaxEt = highestjet[0].sumEt;

  if (MaxEt > itol) 
  {
    	numaccet++;
//	cout << itol << " " << MaxEt << " " << numacc << " " << evts << endl;
  }
  if (realTrackPt > 20.0)
  {
        numaccpt++;
  }
  if (MaxEt > itol && realTrackPt > 20.0)
  {
	numaccptet++;
  }

for (int i = 0; i < numRegions; i++)
{
	if (regionEta(caloRegionEtaIndex[i]) == (-1)*regionEta(MaxRegionEtaIndex))
	{
//		cout << "Opp Region Number: " << i << endl;
//		cout << "Opp Eta Index: " << caloRegionEtaIndex[i] << endl;
//		cout << "Opp Calo Eta: " << regionEta(caloRegionEtaIndex[i]) << endl;
//		cout << "Max Region Eta Index: " << MaxRegionEtaIndex << endl;
//		cout << "Max Region Eta: " << regionEta(MaxRegionEtaIndex) << endl;
		MaxRegionEtaIndexMin = caloRegionEtaIndex[i];
//		cout << MaxRegionEtaIndex << " " << regionEta(MaxRegionEtaIndex) << " " << MaxRegionEtaIndexMin << " " << regionEta(MaxRegionEtaIndexMin) << endl; 
		break;
	}
}
/*   if(PHI_AVERAGE)
    {

       	
      double phi_average[NETA_REGIONS];
      for(int ieta = 0; ieta < NETA_REGIONS; ieta++){
	phi_average[ieta] = 0;
	for(int iphi = 0; iphi < NPHI_REGIONS; iphi++){
	  phi_average[ieta] += fulldetector[ieta][iphi];
//	  cout << ieta << " " << iphi << " " << fulldetector[ieta][iphi] << endl;
	}
	phi_average[ieta] /= NPHI_REGIONS;
    //	cout << "Aver " << ieta << " " <<  phi_average[ieta] << endl; 
     }
      
      for(int ieta = 0; ieta < NETA_REGIONS; ieta++)
	for(int iphi = 0; iphi < NPHI_REGIONS; iphi++){
	  fulldetector[ieta][iphi] -= phi_average[ieta];
	  if(fulldetector[ieta][iphi] < 0)
	    fulldetector[ieta][iphi] = 0;
	}
    }
*/

if(PHI_AVERAGE)
    {
	double rings_average = 0.0;      
	double phi_average[NETA_REGIONS];
      for(int ieta = 0; ieta < NETA_REGIONS; ieta++){
        phi_average[ieta] = 0;
        for(int iphi = 0; iphi < NPHI_REGIONS; iphi++){
          phi_average[ieta] += fulldetector[ieta][iphi];
//        cout << ieta << " " << iphi << " " << fulldetector[ieta][iphi] << endl;
        }
        phi_average[ieta] /= NPHI_REGIONS;
    //  cout << "Aver " << ieta << " " <<  phi_average[ieta] << endl; 
     }

      rings_average = (phi_average[MaxRegionEtaIndexMin] + phi_average[MaxRegionEtaIndexMin+1] + phi_average[MaxRegionEtaIndexMin-1])/3;	
	MaxEt -= rings_average;
//	cout << rings_average << " " << MaxEt << endl;
      for(int ieta = 0; ieta < NETA_REGIONS; ieta++)
        for(int iphi = 0; iphi < NPHI_REGIONS; iphi++){
          fulldetector[ieta][iphi] -= phi_average[ieta];
          if(fulldetector[ieta][iphi] < 0)
            fulldetector[ieta][iphi] = 0;
        }
    }
    

/*double tempnew = 0.0;
double tempold = 0.0;
double MaxEt = 0.0;  

 for(int i = 0; i < numRegions; i++)
    {
	if (!(regionEta(caloRegionEtaIndex[i]) < 2.4 && regionEta(caloRegionEtaIndex[i]) > -2.4)) continue;    
	tempnew = fulldetector[caloRegionEtaIndex[i]][caloRegionPhiIndex[i]];  
      if (tempnew > tempold)
	{
		MaxEt = tempnew;
		tempold = tempnew;
	}
    }	*/ 	    
//	cout << realTrackPtPhi << " " << regionPhi(MaxRegionPhiIndex) << endl;
	MaxEtEtaMaxPtEta->Fill(realTrackPtEta, regionEta(MaxRegionEtaIndex));
	if (realTrackPtPhi > 0.0)
	{
	MaxEtPhiMaxPtPhi->Fill(realTrackPtPhi, regionPhi(MaxRegionPhiIndex)+3.14159);
        deltaEtadeltaPhi->Fill(regionEta(MaxRegionEtaIndex) - realTrackPtEta, regionPhi(MaxRegionPhiIndex)+3.14159 - realTrackPtPhi);
	}
	else	
	{
        MaxEtPhiMaxPtPhi->Fill(realTrackPtPhi, regionPhi(MaxRegionPhiIndex)-3.14159);
        deltaEtadeltaPhi->Fill(regionEta(MaxRegionEtaIndex) - realTrackPtEta, regionPhi(MaxRegionPhiIndex)-3.14159 - realTrackPtPhi);
	}
//	deltaEtadeltaPhi->Fill(regionEta(MaxRegionEtaIndex) - realTrackPtEta, regionPhi(MaxRegionPhiIndex) - realTrackPtPhi);	
	MaxEtMaxPt->Fill(realTrackPt,MaxEt);   
        //cout << MaxEt << " " << MaxPt << endl;                                        
   //  RegionJet *highestJet = findRegionJet(fulldetector);
    
//    if(highestJet[0].sumEt > threshold)
 //     track_curve->Fill(realTrackPt,highestJet[0].sumEt);
//     track_curve->Fill(realTrackPt,realJetPt);
 //     track_curve->Fill(realJetPt,highestJet[0].sumEt);
//    total_in_bin->Fill(realTrackPt);
  }
 NumAccEtThresh->Fill((double)itol,(double)numaccet/(double)evts); 
 RateSigEff->Fill((double)numaccptet/(double)numaccpt,(double)numaccet/(double)evts);
 cout << (double)numaccptet/(double)numaccpt << " " << (double)numaccet/(double)evts << endl;
 numaccet = 0;
 numaccpt = 0;
 numaccptet = 0;
 evts = 0;
}  
//TCanvas *plot;
/*
  stringstream filename;
  //filename << "trackto_" << method_s << "_" << threshold << ".gif";
//plot = new TCanvas();
  //track_curve->Divide(total_in_bin);
  track_curve->Divide(track_curve, total_in_bin, 1, 1, "b");
  track_curve->SetXTitle("Leading Track Pt (GeV)");
  track_curve->SetYTitle("Fraction Accepted");

  track_curve->GetYaxis()->SetRangeUser(0,1);
  //track_curve->Draw("E");

//plots[i]->SaveAs(filename.str().c_str());
*/
TFile* myfile = new TFile("HistFile.root","RECREATE");
MaxEtMaxPt->Write();
MaxEtEtaMaxPtEta->Write();
MaxEtPhiMaxPtPhi->Write();
deltaEtadeltaPhi->Write();
NumAccEtThresh->Write();
RateSigEff->Write();
myfile->Close();  
  return(myfile);
}
