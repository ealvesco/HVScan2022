#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cmath>
// ROOT 
#include "TPad.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TLegend.h"
#include "TText.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TMath.h"
#include "TTree.h"
#include "TROOT.h"
#include "TFile.h"
#include "TBrowser.h"
#include "TH2.h"
#include "TBranch.h"
#include "TChain.h"
#include "TUnixSystem.h"
#include "THStack.h"


void Title(TH1F* ,const char* ,const char* ,const char* , float);
void Format(TH1F* , int , int , int , int);
void Legend2(float, float, float, float,const char*, TH1F*,const char*, TH1F*,const char*, float);
void Legend3(float, float, float, float, const char*, TH1F*, const char*, TH1F*, const char*, TH1F*, const  char*, float);
TCanvas* DrawOFUF(TH1F*, TH1F*, TH1F*, bool, int, const char*, const char*, const char*);
std::vector<std::string> blacklist(const char*);

using namespace std; 

void DrawingOUFlow(bool BL=false)
{
  
  const int NUM = 9; 
  TCanvas* MyCanvas[NUM];  
  
  //TPad* MyPad[NUM];  
  TH1F* HistosB[NUM];
  TH1F* HistosEC[NUM];
  TH1F* HistosRE4[NUM];
  std::string TitlesHistos[9]; 
  std::string TitlesPNG[9]; 
  // Histos
  HistosB[0] 	= new TH1F("wp_Barrel", "Working Point", 60, 8.6, 10);
  HistosEC[0] 	= new TH1F("wp_Endcap", "Working Point", 60, 8.6, 10);
  HistosRE4[0]	= new TH1F("wp_RE4", "Working Point ", 60, 8.6, 10);
  TitlesHistos[0] = "Working Point [kV]";   
  TitlesPNG[0] = "wp";   


  HistosB[1] 	= new TH1F("hv50_Barrel", "HV50", 70, 8.6, 10);
  HistosEC[1] 	= new TH1F("hv50_Endcap", "HV50", 70, 8.6, 10); 
  HistosRE4[1] 	= new TH1F("hv50_RE4", "HV50 ", 70, 8.6, 10);
  TitlesHistos[1] = "HV50 [kV]";
  TitlesPNG[1] = "hv50";


  HistosB[2] = new TH1F("knee_Barrel", "Knee", 70, 8.6, 10);
  HistosEC[2] = new TH1F("knee_Endcap", "Knee", 70, 8.6, 10);
  HistosRE4[2] = new TH1F("knee_RE4", "Knee ", 70, 8.6, 10);
  TitlesHistos[2] = "Knee [kV]";
  TitlesPNG[2] = "knee";


  HistosB[3] = new TH1F("emax_Barrel", "emax", 25, 70, 120); 
  HistosEC[3] = new TH1F("emax_Endcap", "emax", 25, 70, 120);
  HistosRE4[3] = new TH1F("emax_RE4", "emax ", 25, 70, 120);
  TitlesHistos[3] = "Emax [%]";
  TitlesPNG[3] = "emax";

  HistosB[4] = new TH1F("slope50_Barrel", "slope50", 50, 0, 400);
  HistosEC[4] = new TH1F("slope50_Endcap", "slope50", 50, 0, 400);
  HistosRE4[4] = new TH1F("slope50_RE4", "slope50 ", 50, 0, 400);
  TitlesHistos[4] = "Solpe50 [%]";
  TitlesPNG[4] = "slope50";

  HistosB[5] = new TH1F("Chi2_Barrel", "chi2", 44, -2, 20);
  HistosEC[5] = new TH1F("Chi2_Endcap", "chi2", 44, -2, 20);
  HistosRE4[5] = new TH1F("Chi2_RE4", "chi2 ", 44, -2, 20);
  TitlesHistos[5] = "Chi2";
  TitlesPNG[5] = "chi2";
  
  HistosB[6] = new TH1F("effwp_Barrel", "effwp", 100, 60, 110);
  HistosEC[6] = new TH1F("effwp_Endcap", "effwp", 100, 60, 110);
  HistosRE4[6] = new TH1F("effwp_RE4", "effwp ", 100, 60, 110);
  TitlesHistos[6] = "Efficiency at WP [%]";
  TitlesPNG[6] = "effwp";

  HistosB[7] = new TH1F("cls_Barrel", "Cluster Size", 48, -2, 6);
  HistosEC[7] = new TH1F("cls_Endcap", "Cluster Size", 48, -2, 6);
  HistosRE4[7] = new TH1F("cls_RE4", "Cluster Size", 48, -2, 6);
  TitlesHistos[7] = "Cluster Size";       
  TitlesPNG[7] = "clswp";       


  HistosB[8] = new TH1F("Chi2Cls_Barrel", "effwp", 44, -2, 20);
  HistosEC[8] = new TH1F("Chi2Cls_Endcap", "effwp", 44, -2, 20);
  HistosRE4[8] = new TH1F("Chi2Cls_RE4", "effwp ", 44, -2, 20);
  TitlesHistos[8] = "Chi2 ClusterSize";  
  TitlesPNG[8] = "chi2clswp";  

  vector <double> wp, effmax, HV50, slope;
  vector <double> effwp, chi2eff, clswp, chi2clswp;
  vector <std::string> roll;
  vector <std::string> removedRolls; 
  //if (BL)removedRolls = blacklist("../data/blacklist_reRunSpt2018.txt"); 
  //if (BL)removedRolls = blacklist("../data/blacklist_reRunMay2018.txt"); 
  if (BL)removedRolls = blacklist("../data/blacklist_2018.txt"); 
  //if (BL)removedRolls = blacklist("../data/blacklist_2018b_v4.txt"); 
  //if (BL)removedRolls = blacklist("../data/blacklist_2018b.txt"); 
  //if (BL)removedRolls = blacklist("../data/blacklist_global_NonIn2011.txt"); 
  //if (BL)removedRolls = blacklist("../data/blacklist_2018a_v3.txt"); 
  bool matchRolls=false; 
  
  Char_t   RollName[38];
  Double_t WorkingPoint;
  Double_t slope50, emax, hv50, chi2, EffWP, clsWP, chi2cls, EffWPch;
  TChain *ch = new TChain("ch","");
 
   
 
// if (BL){
//	   ch->Add("../summary/barrel_summary_2018BlackList.root/filtered");
//	   ch->Add("../summary/endcap_summary_2018BlackList.root/filtered");
//  }
//else {
	  ch->Add("../summary/barrel_summary_2018.root/filtered");
	  ch->Add("../summary/endcap_summary_2018.root/filtered");
//  }

  TTree *filtered = (TTree*)ch;

  cout<<" Entries : "<<filtered->GetEntries()<<endl;
  Long64_t nentries = filtered->GetEntries();
  filtered->SetBranchAddress("RollName", &RollName);
  filtered->SetBranchAddress("WorkingPoint", &WorkingPoint);
  filtered->SetBranchAddress("emax", &emax);
  filtered->SetBranchAddress("hv50", &hv50);
  filtered->SetBranchAddress("chi2", &chi2);
  filtered->SetBranchAddress("slope50", &slope50);
  filtered->SetBranchAddress("EffWP", &EffWP);
  filtered->SetBranchAddress("EffWPch", &EffWPch);
  filtered->SetBranchAddress("clsWP", &clsWP);
  filtered->SetBranchAddress("chi2cls", &chi2cls);

 for(Long64_t i=0;i<nentries;i++){
           filtered->GetEntry(i);
           std::string chamber = RollName; 
           matchRolls=false; 
           if(BL){
              for (size_t l=0; l < removedRolls.size();l++){	
	   	        if (chamber==removedRolls.at(l))matchRolls = true;
          		else continue; 
		} 
	   }
 	   if(matchRolls)continue; 	   
           roll.push_back(RollName);
           wp.push_back(WorkingPoint);
           effmax.push_back(emax);
           HV50.push_back(hv50);
           slope.push_back(slope50);  
           effwp.push_back(EffWP);  
           clswp.push_back(clsWP);  
           chi2eff.push_back(chi2);  
           chi2clswp.push_back(chi2cls);  

         }
  gROOT->Reset(); 
  for (int k=0;k<int(roll.size()); k++){      
     if((roll.at(k)).find("W")!= std::string::npos){
              HistosB[0]->Fill(wp.at(k));
              HistosB[1]->Fill(HV50.at(k));
              HistosB[2]->Fill(wp.at(k) - .100);//Knee 
              HistosB[3]->Fill(effmax.at(k));
              HistosB[4]->Fill(slope.at(k));
              HistosB[5]->Fill(chi2eff.at(k));  
              HistosB[6]->Fill(effwp.at(k));  
              HistosB[7]->Fill(clswp.at(k)); 
              HistosB[8]->Fill(chi2clswp.at(k)); 
            }
            else if ((roll.at(k)).find("RE+4")!= std::string::npos || (roll.at(k)).find("RE-4")!= std::string::npos ){
              HistosRE4[0]->Fill(wp.at(k));
              HistosRE4[1]->Fill(HV50.at(k));
              HistosRE4[2]->Fill(wp.at(k) - .120);//Knee 
              HistosRE4[3]->Fill(effmax.at(k));
              HistosRE4[4]->Fill(slope.at(k));
              HistosRE4[5]->Fill(chi2eff.at(k));  
              HistosRE4[6]->Fill(effwp.at(k));  
              HistosRE4[7]->Fill(clswp.at(k)); 
              HistosRE4[8]->Fill(chi2clswp.at(k)); 
                   }
             else {
              HistosEC[0]->Fill(wp.at(k));
              HistosEC[1]->Fill(HV50.at(k));
              HistosEC[2]->Fill(wp.at(k) - .120);//Knee 
              HistosEC[3]->Fill(effmax.at(k));
              HistosEC[4]->Fill(slope.at(k));
              HistosEC[5]->Fill(chi2eff.at(k));  
              HistosEC[6]->Fill(effwp.at(k));  
              HistosEC[7]->Fill(clswp.at(k)); 
              HistosEC[8]->Fill(chi2clswp.at(k)); 
                 }
           
      }
     
      // create your canvas and the pads 
     for(int j=0; j< NUM; j++){
     	std::stringstream ss;
        ss << j;
        std::string canvasstr = "canvas"+ss.str(); 
        std::string padstr = "pad"+ss.str(); 
        std::string stacstr = "stack"+ss.str(); 
        const char* name1 = canvasstr.c_str();
        const char* name2 = stacstr.c_str();
        
        Title(HistosEC[j], TString(TitlesHistos[j]), TString(TitlesHistos[j]), "Number of Rolls", 1.2);
        Title(HistosRE4[j], TString(TitlesHistos[j]), TString(TitlesHistos[j]), "Number of Rolls", 1.2);
        Format(HistosB[j], 2, 1, 1001, 2);
        Format(HistosEC[j], 2, 1, 3001, 4);
        Format(HistosRE4[j], 2, 1, 1001, 3);
        MyCanvas[j] =  DrawOFUF(HistosB[j], HistosEC[j],HistosRE4[j], 0, 1, name1, name2,  TString(TitlesHistos[j]) );
        MyCanvas[j]->Modified(); 
        MyCanvas[j]->Update();
        MyCanvas[j]->cd();        
      /*  Legend3(0.12, 0.70, 0.30, 0.88,
                  TString(TitlesHistos[j]),
                   HistosB[j], "Barrel",
                   HistosEC[j], "Endcap",
                   HistosRE4[j], "RE4",
                   0.028);
        MyCanvas[j]->Modified(); 
        MyCanvas[j]->Update();
        MyCanvas[j]->cd();*/        
        std::string pngname= TitlesPNG[j];
        if (!BL) MyCanvas[j]->Print("../summary/"+TString(pngname)+"_May2018.C");
        else  MyCanvas[j]->Print("../summary/"+TString(pngname)+"_May2018_bl.C");
        MyCanvas[j]->Modified(); 
        MyCanvas[j]->Update();
            
       }
}



void Title(TH1F* Hist,const char* Title,const char* XTitle,const char* YTitle, float TitleOffset)
{
	Hist->SetTitle(Title);
	Hist->GetXaxis()->SetTitle(XTitle);
	Hist->GetXaxis()->CenterTitle();	
	// Hist->GetXaxis()->SetTitleOffset(TitleOffset);
	Hist->GetYaxis()->SetTitle(YTitle);
	Hist->GetYaxis()->CenterTitle();
	Hist->GetYaxis()->SetTitleOffset(TitleOffset);
}

void Format(TH1F* Hist, int LineWidth, int LineColor, int FillStyle, int FillColor)
{
	Hist->SetLineWidth(LineWidth);
	Hist->SetLineColor(1);
	Hist->SetFillStyle(FillStyle);
	Hist->SetFillColor(FillColor);
}

void Legend3(float x1, float y1, float x2, float y2, const char* Header, TH1F* Entry1, const char* Desc1, TH1F* Entry2, const char* Desc2, TH1F* Entry3,const  char* Desc3, float TextSize)
{
	TLegend *Leg; 
        Leg = new TLegend(x1, y1, x2, y2);
	Leg->AddEntry(Entry1, Desc1, "f");
	Leg->AddEntry(Entry2, Desc2, "f");	
	Leg->AddEntry(Entry3, Desc3, "f");
	Leg->SetBorderSize(0); 
        //Leg->SetLegendFillColor(0);
	//Leg->SetLegendFont(42);
	//Leg->SetLegendTextSize(0.);
        Leg->SetTextSize(TextSize);
	Leg->Draw();
}

TCanvas* DrawOFUF(TH1F* HistB, TH1F* HistEC, TH1F* HistRE4, bool Norm, int Same, const char* nameCanvas,const char* nameS, const char* title )
{
	Int_t Nx1    = HistB->GetNbinsX()+2;
	Int_t Nx2    = HistEC->GetNbinsX()+2;
	Int_t Nx3    = HistRE4->GetNbinsX()+2;
	Double_t x1B = 	HistB->GetBinLowEdge(1)  - HistB->GetBinWidth(0);;
	Double_t x1EC = HistEC->GetBinLowEdge(1) - HistEC->GetBinWidth(0);;
	Double_t x1RE = HistRE4->GetBinLowEdge(1) - HistRE4->GetBinWidth(0);;
        Double_t x2B  = HistB->GetBinLowEdge(HistB->GetNbinsX()+1)  + HistB->GetBinWidth(HistB->GetNbinsX()+1);;
	Double_t x2EC = HistEC->GetBinLowEdge(HistEC->GetNbinsX()+1)+ HistEC->GetBinWidth(HistEC->GetNbinsX()+1);;
	Double_t x2RE = HistRE4->GetBinLowEdge(HistRE4->GetNbinsX()+1)+ HistRE4->GetBinWidth(HistRE4->GetNbinsX()+1);;
	
        // Book a temporary histogram having extra bins_overflows and underflows
        TH1F *HTmp1 = new TH1F("hTmp1", " ", HistB->GetNbinsX()+2, x1B, x2B);
        TH1F *HTmp2 = new TH1F("hTmp2", " ", HistEC->GetNbinsX()+2, x1EC, x2EC);
        TH1F *HTmp3 = new TH1F("hTmp3", " ", HistRE4->GetNbinsX()+2, x1RE, x2RE);
      /* 
        HTmp1->SetName(HistB->GetName());
        HTmp2->SetName(HistEC->GetName());
        HTmp3->SetName(HistRE4->GetName());
        */
        /*TH1F *HTmp1 = (TH1F*)HistB->Clone("hTmp1");      
        TH1F *HTmp2 = (TH1F*)HistEC->Clone("hTmp2");      
        TH1F *HTmp3 = (TH1F*)HistRE4->Clone("hTmp3");      
        */
        //TH1F *HTmp = (TH1F*)Hist->Clone("hTmp");
        
       // Fill the new hitogram including the extra bin_overflows
	for (Int_t i=1; i<=Nx1; i++)
		HTmp1->Fill(HTmp1->GetBinCenter(i), HistB->GetBinContent(i-1));
	for (Int_t i=1; i<=Nx2; i++)
		HTmp2->Fill(HTmp2->GetBinCenter(i), HistEC->GetBinContent(i-1));
	for (Int_t i=1; i<=Nx3; i++)
		HTmp3->Fill(HTmp3->GetBinCenter(i), HistRE4->GetBinContent(i-1));
 	// Restore the number of entries
	HTmp1->SetEntries(HistB->GetEntries());
	HTmp2->SetEntries(HistEC->GetEntries());
	HTmp3->SetEntries(HistRE4->GetEntries());
	  // Restore the Format 
        Format(HTmp1, HistB->GetLineWidth(), HistB->GetLineColor(), HistB->GetFillStyle(), HistB->GetFillColor());
	Format(HTmp2, HistEC->GetLineWidth(), HistEC->GetLineColor(), HistEC->GetFillStyle(), HistEC->GetFillColor());
	Format(HTmp3, HistRE4->GetLineWidth(), HistRE4->GetLineColor(), HistRE4->GetFillStyle(), HistRE4->GetFillColor());

        
         
       //gROOT->ForceStyle();   

        



      int H = 600; 
      int W = 800; 
      int H_ref = 600; 
      int W_ref = 800; 

  // references for T, B, L, R
  float T = 0.077*H_ref;
  float B = 0.125*H_ref; 
  float L = 0.14*W_ref;
  float R = 0.028*W_ref;


       gStyle->SetLineWidth(3);
       gStyle->SetHistLineWidth(3); 
       gStyle->SetOptStat(1111);
       gStyle->SetFrameLineWidth(3); 
     
       
       //TCanvas *canvas = new TCanvas(nameCanvas," ", 1024, 768); 
       TCanvas* canvas = new TCanvas(nameCanvas,nameCanvas,50,50,W,H);
       canvas->SetFillColor(0);
       canvas->SetBorderMode(0);
       canvas->SetFrameFillStyle(0);
       canvas->SetFrameBorderMode(0);
       canvas->SetLeftMargin( L/W );
       canvas->SetRightMargin( R/W );
       canvas->SetTopMargin( T/H );
       canvas->SetBottomMargin( B/H );
       canvas->SetTickx(0);
       canvas->SetTicky(0);      
       canvas->cd();
     
     
             
       THStack *hs = new THStack(nameS, title);
       hs->Add(HTmp1,"sames");
       hs->Add(HTmp2,"sames");
       hs->Add(HTmp3,"sames");
       hs->Draw("hist nostack");
       hs->GetYaxis()->SetTitle("Number of Rolls");  
       hs->GetXaxis()->SetTitle(title);  
       hs->GetXaxis()->SetTitleSize(0.06);  
       hs->GetYaxis()->SetTitleSize(0.06);  
       canvas->Update();
       canvas->cd();
//     //the following lines will force the stats for h[1] and h[2]
//     //to be drawn at a different position to avoid overlaps
       //TLatex *dex = new TLatex(0.1, 0.92,"Data May 2018");
       //dex->SetNDC();
       //dex->SetTextAngle(0);	//Tex->SetLineWidth(2);
       //dex->SetTextFont(92);
       //dex->Draw(); 
       TPaveStats *st1 =  (TPaveStats*)HTmp1->GetListOfFunctions()->FindObject("stats");
       TPaveStats *st2 =  (TPaveStats*)HTmp2->GetListOfFunctions()->FindObject("stats");
       TPaveStats *st3 =  (TPaveStats*)HTmp3->GetListOfFunctions()->FindObject("stats");
      // return canvas;

	st3->SetX1NDC(0.82-R/W);
	st3->SetX2NDC(0.98-R/W);
	st3->SetY1NDC(0.45);
	st3->SetY2NDC(0.58);
	st2->SetX1NDC(0.82-R/W);
	st2->SetX2NDC(0.98-R/W);
	st2->SetY1NDC(0.60);
	st2->SetY2NDC(0.73);
        st1->SetX1NDC(0.82-R/W);
        st1->SetX2NDC(0.98-R/W);
        st1->SetY1NDC(0.75);
        st1->SetY2NDC(0.88);
 
    canvas->Modified();
    canvas->Update();
    canvas->cd();
   
/*    int n_ = 4;

    float x1_l = 0.3;
    float y1_l = 0.88;

    float dx_l = 0.35;
    float dy_l = 0.2;
    float x0_l = x1_l-dx_l;
    float y0_l = y1_l-dy_l;

    float ar_l = dy_l/dx_l;

    float gap_ = 1./(n_+1);
    float bwx_ = 0.12;
    float bwy_ = gap_/1.5;

    float xx_ = 1.2*bwx_;
    float yy_ = 1-gap_;

    TLegend *legend=new TLegend(x0_l,y0_l,x1_l, y1_l);
    legend->SetBorderSize(0);
    legend->SetFillColor(0);
    legend->SetFillStyle(0);
    legend->SetTextFont(42);
    legend->SetTextSize(0.05);
    legend->SetBorderSize(0); 
    legend->AddEntry(HTmp1,"Barrel","f");
    legend->AddEntry(HTmp2,"Endcap","f");
    legend->AddEntry(HTmp3,"RE4","f");
    canvas->Modified();
    canvas->Update();
    canvas->cd();	
*/

        TText *Tex = new TText(x2B-(HistEC->GetBinWidth(HistEC->GetNbinsX()+1))/2, HistEC->GetBinContent(HistEC->GetNbinsX()+1)+20, "Overflow");
        Tex->SetTextAngle(90);
	Tex->SetTextAlign(12);
	Tex->SetTextSize(0.03);
	Tex->SetTextColor(1);
	Tex->Draw();
	// Underflow Text
	Tex = new TText(x1B+(HistEC->GetBinWidth(0))/2, HistEC->GetBinContent(0)+20, "Underflow");
	Tex->SetTextAngle(90);
	Tex->SetTextAlign(12);
	Tex->SetTextSize(0.03);
	Tex->SetTextColor(1);
	Tex->Draw();
               
  
        canvas->Modified();
        canvas->Update();
        canvas->cd();
       
        HTmp1->SetName(HistB->GetName());
        HTmp2->SetName(HistEC->GetName());
        HTmp3->SetName(HistRE4->GetName());
        
       return canvas;        

}

std::vector<std::string> blacklist(const char* filename){
  ifstream infile; 
  
  infile.open(filename); 
  std::vector<std::string> rolls; 
  while (1){
   	std::string name; 
   	infile >> name; 
   	if (infile.eof())break;
   	rolls.push_back(name); 
   }   
  
  return rolls;  
  
}
