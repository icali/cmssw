#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TLegend.h>

void draw()
{
//  TFile* f = new TFile("efficiency_track_PU1_centmin16_centmax40.root");
  TFile* f = new TFile("Eta2.0_cent1640.root");
//  TH2D* h = (TH2D*)f->Get("track_curve");
  TH2D* h = (TH2D*)f->Get("MaxEtMaxPt");
  h->SetTitle("");

  TH1D* h_20 = (TH1D*)h->ProjectionX("h_20",h->GetYaxis()->FindBin(60.0),h->GetYaxis()->FindBin(1000.0),"e");
  TH1D* h_30 = (TH1D*)h->ProjectionX("h_30",h->GetYaxis()->FindBin(50.0),h->GetYaxis()->FindBin(1000.0),"e");
  TH1D* h_40 = (TH1D*)h->ProjectionX("h_40",h->GetYaxis()->FindBin(40.0),h->GetYaxis()->FindBin(1000.0),"e");
  TH1D* h_all = (TH1D*)h->ProjectionX("h_all",h->GetYaxis()->FindBin(0.0),h->GetYaxis()->FindBin(1000.0),"e");

  h_20->Rebin(4);
  h_30->Rebin(4);
  h_40->Rebin(4);
  h_all->Rebin(4);

//  h_20->Divide(h_20, h_all, 1, 1, "b");
  TGraphAsymmErrors* gr_20 = new TGraphAsymmErrors(h_all->GetNbinsX()); 
  gr_20->BayesDivide(h_20,h_all);
  h_20->SetXTitle("Leading Track Pt (GeV)");
  h_20->SetYTitle("Fraction Accepted");
  h_20->GetYaxis()->SetRangeUser(0,1.05);
  gr_20->SetMarkerColor(1);
  h_20->Reset();

//  h_30->Divide(h_30, h_all, 1, 1, "b");
  TGraphAsymmErrors* gr_30 = new TGraphAsymmErrors(h_all->GetNbinsX());
  gr_30->BayesDivide(h_30,h_all);
  h_30->SetXTitle("Leading Track Pt (GeV)");
  h_30->SetYTitle("Fraction Accepted");
  h_30->GetYaxis()->SetRangeUser(0,1.05);
  gr_30->SetMarkerColor(2);
  h_30->Reset();

//  h_40->Divide(h_40, h_all, 1, 1, "b");
  TGraphAsymmErrors* gr_40 = new TGraphAsymmErrors(h_all->GetNbinsX());
  gr_40->BayesDivide(h_40,h_all);
  h_40->SetXTitle("Leading track p_{T} (GeV)");
  h_40->SetYTitle("Fraction Accepted");
  h_40->GetYaxis()->SetRangeUser(0,1.05);
  gr_40->SetMarkerColor(4);
  h_40->Reset();

  TCanvas* c1 = new TCanvas("c1","c1",600,500);
  h_40->SetAxisRange(14,80,"X");
  h_40->Draw("");
  gr_40->Draw("PESAME");
  gr_30->Draw("PESAME");
  gr_20->Draw("PESAME");
//  h_40->Draw("PE");
//  h_30->Draw("PESAME");
//  h_20->Draw("PESAME");

  TLegend* legend = new TLegend(0.5,0.25,0.9,0.45);
  legend->SetTextFont(42);
  legend->SetFillColor(0);
  legend->SetHeader("40-100% centrality");
  legend->AddEntry(gr_40,"L1 jet E_{T} > 40 GeV","P");
  legend->AddEntry(gr_30,"L1 jet E_{T} > 50 GeV","P");
  legend->AddEntry(gr_20,"L1 jet E_{T} > 60 GeV","P");
  legend->Draw("same");

  TCanvas* c = new TCanvas("c","c",600,500);
  c->SetLogz();
  h->SetAxisRange(14,80,"X");
  h->Draw("colz");

 // SaveCanvas(c1,"L1Trigger/upgrade","efficiency_trackpt_PU1_centmin16_centmax40");
 // SaveCanvas(c,"L1Trigger/upgrade","JetvsTrack_PU1_centmin16_centmax40");
}
