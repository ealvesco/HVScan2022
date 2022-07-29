#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cmath>
//Root 
//
#include "TVector3.h"
#include <TMath.h>
#include <TTree.h>
#include "TROOT.h"
#include "TFile.h"
#include "TKey.h"
#include "TBrowser.h"
#include "TH2.h"
#include "TBranch.h"
#include "TChain.h"
#include "TUnixSystem.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TH2Poly.h"
#include "DrawGeometry.C"

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"

using namespace RooFit ;
using namespace std; 
std::vector<std::string> blacklist(const char*);
void fillWorkspace(RooWorkspace&,  const float, const float) ;
bool isMatch(const char *, const char*);



void differenceEstimation(const char* yearA, const char* yearB, const char* module ){


  vector<Color_t> colors;
  colors.push_back(kRed); colors.push_back(kGreen);  
  colors.push_back(kBlue); colors.push_back(kMagenta);  
  colors.push_back(kOrange); colors.push_back(kCyan);  
  colors.push_back(kViolet); colors.push_back(kTeal);  
  
	
  /////////////////////////////////////7/////
  //  Retrieve the data 	        /////
  /////////////////////////////////////7/////
  std::vector<tuple<string,Double_t , Double_t> > valuesA;
  std::vector<tuple<string,Double_t , Double_t> > valuesB;

  Char_t  RollName[38];
  Double_t WorkingPoint;
  Double_t EffWP;
  for (int k=0; k<2; ++k) {
	TChain *ch = new TChain("ch","");
	if ( k==0 ) ch->Add(("../summary/"+string(module)+"_summary_"+yearA+".root/filtered").c_str());
	if ( k==1 ) ch->Add(("../summary/"+string(module)+"_summary_"+yearB+".root/filtered").c_str());
  	TTree *filtered = (TTree*)ch;
  	cout<<" Entries : "<<filtered->GetEntries()<<endl;
  	Long64_t nentries = filtered->GetEntries();
  	filtered->SetBranchAddress("RollName", &RollName);
  	filtered->SetBranchAddress("WorkingPoint", &WorkingPoint);
  	filtered->SetBranchAddress("EffWP", &EffWP);
		
	 for(Long64_t i=0;i<nentries;i++){
           filtered->GetEntry(i);
           std::string chamber = RollName; 

  	   bool matchRolls=false;   
	   matchRolls = isMatch(RollName, "blacklist_2018_global.txt" ); 
           if(matchRolls)continue;
 	   
 	   if ( k==0 ){
			auto tupleA = std::make_tuple (chamber, 1000.0*WorkingPoint, EffWP); 
			valuesA.push_back(tupleA);
                      }  

           if ( k==1 ){ 
			auto tupleB = make_tuple(chamber, 1000.0*WorkingPoint, EffWP); 
		 	valuesB.push_back(tupleB);
                      } 	 
	 }
	
   }

  ////////////////////////////////////////////////////////
  //   Fitting  and  drawing 
    
  ////////////////////////////////////////////////////////
  int W = 800;
  int H = 800; 
  int H_ref = 800; 
  int W_ref = 800;

  float T = 0.08*H_ref;
  float B = 0.14*H_ref; 
  float L = 0.10*W_ref;
  float R = 0.08*W_ref;


  gStyle->SetNumberContours(800);
  gStyle->SetPaintTextFormat("2.3f ");
  gStyle->SetLineWidth(2);
  gStyle->SetLineColor(1);
  gStyle->SetHistLineWidth(2); 
  gStyle->SetHistLineColor(1); 
  gStyle->SetFrameLineWidth(3); 
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1,0); 
  const float effMin = -4.0; 
  const float effMax = 4.0; 
  const float wpMin = -60; 
  const float wpMax = 60; 
  
  RooWorkspace* w = new RooWorkspace("w",kTRUE) ;
  fillWorkspace(*w, effMin, effMax); 
  
  
 
  if (strcmp(module,"endcap")==0){
    std::vector<std::string> disks; 
    disks.push_back("RE-4");
    disks.push_back("RE-3");
    disks.push_back("RE-2");
    disks.push_back("RE-1");
    disks.push_back("RE+1");
    disks.push_back("RE+2");
    disks.push_back("RE+3");
    disks.push_back("RE+4");
    TFile* file = new TFile(("Endcap_DiffEffWP_"+string(yearB)+"_"+string(yearA)+".root").c_str(),"RECREATE"); 
    TDirectory *TopEndcap = file->mkdir("Endcap");
    
    const int st = (int)disks.size() ; 
    TCanvas* canvas[st]; 
    RooRealVar* eff = w->var("diff"); 
    RooRealVar* mean= w->var("mean");
    RooRealVar* s1  = w->var("sigma1");
    RooRealVar* s2  = w->var("sigma2");
    RooRealVar* f   = w->var("f");
    RooAbsPdf * model= w->pdf("dgauss");
    
   for (int k=0, l=-4 ; k < disks.size(); ++k, ++l){ // for over disk
     std::stringstream ssw; 
     std::string nameDisk; 
   
     if (l<0) {ssw<<(-1)*l; nameDisk = "REm"+ssw.str();  } 	
     else {ssw<<l+1; nameDisk = "REp"+ssw.str();  } 	
     TDirectory* diskDir = TopEndcap->mkdir(nameDisk.c_str());    
     diskDir->cd();     
   
    /* for (int j = 2; j < 4; ++j  ){// for over each station 
        std::stringstream ssR; 
  	ssR << j; 
        std::string ring = "_R"+ssR.str();   
       */ RooDataSet* data = new RooDataSet("data","data", RooArgSet(*eff));   

	for (std::vector< tuple<string,double ,double> >::const_iterator itA=valuesA.begin(); itA!= valuesA.end(); ++itA){//loop over era A 
 	    if ( !((std::get<0>(*itA)).find(disks.at(k)) !=std::string::npos)   )continue; // find the wheel 
 	    //if ( !((std::get<0>(*itA)).find(string(wheelOrdisk)) !=std::string::npos)   )continue; // find the wheel 
	    //if  ((std::get<0>(*itA)).find(ring) ==std::string::npos )continue; // find the station
    
 	    for (std::vector< tuple<string,double ,double> >::const_iterator  itB=valuesB.begin(); itB!= valuesB.end(); ++itB){ //loop over era B
		 if (std::get<0>(*itA) != std::get<0>(*itB) )continue; 
		 else {
                         double d =  std::get<2>(*itB) - std::get<2>(*itA); 
			 *eff = d;
			 data->add(RooArgSet(*eff));
		      }
	    }// end loop eraAB 	
        }//end  loop eraA
     // w->import(*data,Rename( ("data"+nameDisk+ring).c_str()  ) ); 
     
     //} 
     canvas[k] = new TCanvas(("canvas"+disks.at(k)+string(yearB)+string(yearA)).c_str()," ",50,50,W,H);
     canvas[k]->cd();
     canvas[k]->SetFillColor(0);
     canvas[k]->SetBorderMode(0);
     canvas[k]->SetFrameFillStyle(0);
     canvas[k]->SetFrameBorderMode(0);
     canvas[k]->SetLeftMargin( L/W );
     canvas[k]->SetRightMargin( R/W );
     canvas[k]->SetTopMargin( T/H );
     canvas[k]->SetBottomMargin( B/H );
     
     RooPlot* frame = eff->frame(""); 
     frame->GetXaxis()->SetTitle(("Difference eff% ("+string(yearB)+"-"+string(yearA)+")").c_str());
     frame->GetYaxis()->SetTitle("Number of rolls");
     frame->SetTitle( (disks.at(k)).c_str());

     //RooAbsData* dt2 = w->data(  ("data"+nameDisk+"_R2").c_str() );
     //RooAbsData* dt3 = w->data(  ("data"+nameDisk+"_R3").c_str() );
   
     RooFitResult* r1 = model->fitTo(*data,Save()) ;	   
     r1->Print("v"); 
     r1->Write("Results");      
     data->plotOn(frame,Binning(15) /*,XErrorSize(0)*/); 
     model->plotOn(frame, FillColor(colors.at(0) ));
     cout << disks.at(k) <<  ": Mean: " << mean->getVal() << " +/- " << sqrt(s1->getVal()*s1->getVal()*f->getVal() + (1-f->getVal())*s2->getVal()*s2->getVal()) << std::endl;       	
         model->plotOn(frame/*, FillColor(colors.at(k) ), DrawOption("LF"),FillStyle(3008+k)*/ );
     //cout << disks.at(k)+"_R3"  <<  ": Mean: " << mean->getVal() << " +/- " << sqrt(s1->getVal()*s1->getVal()*f->getVal() + (1-f->getVal())*s2->getVal()*s2->getVal()) << std::endl;       	
       
     canvas[k]->cd();  frame->Draw(); 	 
     canvas[k]->Update(); 
     canvas[k]->Modified(); 
     canvas[k]->Write(("canvas"+string(yearB)+string(yearA)+"_"+disks.at(k)+".pdf").c_str());;
     	   
     TopEndcap->cd();  
  } 
  // Fit and drawing 
 
  file->Write();
  delete file; 
 
 }
  
 // +++++++++++++++++++++++++++++++++  
 // +++++++++++ B A R R E L +++++++++ 
 // +++++++++++++++++++++++++++++++++  
 
 else {
  TFile* file = new TFile(("Barrel_DiffEffWP_"+string(yearB)+"_"+string(yearA)+".root").c_str(),"RECREATE"); 
  TDirectory *TopBarrel = file->mkdir("Barrel");
 
  TopBarrel->cd(); 
  TH2F* diffMap = new TH2F("diffMap","diffMap",5,1.,6.,6,1,7); 
  diffMap->SetContour(500);
  diffMap->GetZaxis()->SetRangeUser(-0.5,0.0);
  
  
  std::vector<std::string> stations;
  stations.push_back("RB1in");
  stations.push_back("RB1out");
  stations.push_back("RB2in");
  stations.push_back("RB2out");
  stations.push_back("RB3");
  stations.push_back("RB4");
 
  vector<tuple<string, float, float> > fitresults_barrel;
  
  	
  const int st = (int)stations.size() ; 
  TCanvas* canvas[st]; 
  RooRealVar* eff = w->var("diff"); 
  RooRealVar* mean= w->var("mean");
  RooRealVar* s1  = w->var("sigma1");
  RooRealVar* s2  = w->var("sigma2");
  RooRealVar* f   = w->var("f");
  RooAbsPdf * model= w->pdf("dgauss");

  // Data 

 for (int l = -2, m=0 ; l <= 2; ++l,++m){ // for over disk
   std::stringstream ssw; 
   std::string wheel;
   std::string nameWheel; 
   if (l<0) {ssw<<(-1)*l; wheel = "W-"+ssw.str(); nameWheel = "Wm"+ssw.str();  } 	
   else {ssw<<l; wheel = "W+"+ssw.str(); nameWheel = "Wp"+ssw.str();  } 	
   diffMap->GetXaxis()->SetBinLabel(m+1, (wheel).c_str());
 
   TDirectory* wheelDir = TopBarrel->mkdir(nameWheel.c_str());    
   wheelDir->cd();     
   

   TH1F* h_diff = new TH1F((nameWheel+"_fitResDiff").c_str(),("Difference ("+string(yearB)+"-"+string(yearA)+") "+wheel).c_str(),6,1,7);
   h_diff->GetYaxis()->SetRangeUser(-0.9,0.9);     
   for(size_t kk=0; kk < st; ++kk){ 
	h_diff->GetXaxis()->SetBinLabel(kk+1, (stations.at(kk)).c_str());
	diffMap->GetYaxis()->SetBinLabel(kk+1, (stations.at(kk)).c_str());
   } 
   h_diff->GetYaxis()->SetTitle("Difference eff[%] at wp"); 
   

   for (size_t j = 0; j < st; ++j  ){// for over each station 
  	RooDataSet* data = new RooDataSet("data","data", RooArgSet(*eff));   

	for (std::vector< tuple<string,double ,double> >::const_iterator itA=valuesA.begin(); itA!= valuesA.end(); ++itA){//loop over era A 
 	    if ( !((std::get<0>(*itA)).find(string(wheel)) !=std::string::npos)   )continue; // find the wheel 
	    if  ((std::get<0>(*itA)).find(stations.at(j)) ==std::string::npos )continue; // find the station
    
 	    for (std::vector< tuple<string,double ,double> >::const_iterator  itB=valuesB.begin(); itB!= valuesB.end(); ++itB){ //loop over era B
		 if (std::get<0>(*itA) != std::get<0>(*itB) )continue; 
		 else {
                         double d =  std::get<2>(*itB) - std::get<2>(*itA); 
			 *eff = d;
			 data->add(RooArgSet(*eff));
		      }
	    }// end loop eraAB 	
        }//end  loop eraA
     
    canvas[j] = new TCanvas(("canvas"+string(yearB)+string(yearA)+"_"+stations.at(j)).c_str()," ",50,50,W,H);
    canvas[j]->cd();
    canvas[j]->SetFillColor(0);
    canvas[j]->SetBorderMode(0);
    canvas[j]->SetFrameFillStyle(0);
    canvas[j]->SetFrameBorderMode(0);
    canvas[j]->SetLeftMargin( L/W );
    canvas[j]->SetRightMargin( R/W );
    canvas[j]->SetTopMargin( T/H );
    canvas[j]->SetBottomMargin( B/H );
    
    RooPlot* frame = eff->frame(""); 
    frame->GetXaxis()->SetTitle(("Difference eff% ("+string(yearB)+"-"+string(yearA)+")").c_str());
    frame->GetYaxis()->SetTitle("Number of rolls");
    frame->SetTitle( (wheel+"_"+stations.at(j)).c_str());

   
    RooFitResult* r1 = model->fitTo(*data,Save()) ;	   
    r1->Print("v"); 
    r1->Write(("Results"+stations.at(j)).c_str());      
    data->plotOn(frame,Binning(15) /*,XErrorSize(0)*/); 
    model->plotOn(frame/*, FillColor(colors.at(k) ), DrawOption("LF"),FillStyle(3008+k)*/ );
//    cout << wheel+stations.at(j)  <<  ": Mean: " << mean->getVal() << " +/- " << sqrt(s1->getVal()*s1->getVal()*f->getVal() + (1-f->getVal())*s2->getVal()*s2->getVal()) << std::endl;       	
    float sigmaSum = sqrt(s1->getVal()*s1->getVal()*f->getVal() + (1-f->getVal())*s2->getVal()*s2->getVal()); 
    auto resultsTuple = std::make_tuple(wheel+"_"+stations.at(j) , mean->getVal(), sigmaSum);
    fitresults_barrel.push_back(resultsTuple);  

    canvas[j]->cd();  frame->Draw(); 	 
    canvas[j]->Update(); 
    canvas[j]->Modified(); 
    canvas[j]->Write(("canvas"+string(yearB)+string(yearA)+"_"+wheel+"_"+stations.at(j)+".pdf").c_str());;
    	   
    }// end loop station 
   
   for(int l=0+m*st, k=0; l< fitresults_barrel.size(); ++l, ++k){ 
 	h_diff->SetBinContent(k+1,std::get<1>(fitresults_barrel.at(l)));
 	h_diff->SetBinError(k+1,std::get<2>(fitresults_barrel.at(l))); 
        diffMap->SetBinContent(m+1,k+1,std::get<1>(fitresults_barrel.at(l)));
      }
   
   h_diff->Write(); 
 
   TopBarrel->cd();  
  }// loop over wheel 
  // Fit and drawing 
  diffMap->Write(); 
  file->Write();
  delete file; 
  

 }//isBarrel 
 

}




std::vector<std::string> blacklist(const char* filename){
  ifstream infile; 
  infile.open(filename); 
  std::vector<std::string> rolls; 
  while (infile.good()){
   	std::string name; 
   	infile >> name; 
   	if (infile.eof())break;
   	rolls.push_back(name); 
  }   
  
  return rolls;  
  
}

bool isMatch(const char *roll, const char* file){
  /////////////////////////////////////7/////
  //  Get the list of problematic rolls /////
  /////////////////////////////////////7/////
	
  vector<string> removedRolls; 
  removedRolls = blacklist(file); 
  bool matchRolls=false; 
  if(removedRolls.size()!=0){ 
      for (size_t l=0; l < removedRolls.size();l++){	
	if ( strcmp(roll, removedRolls.at(l).c_str() ) == 0) matchRolls = true;
        else continue; 
        }
   }
  return matchRolls; 
}



void fillWorkspace(RooWorkspace& w, const float min, const float max) 
{
  // C r e a t e   m o d e l
  // -----------------------
  // Declare observable x
  RooRealVar diff("diff","diff",min,max) ;

  // Create two Gaussian PDFs g1(x,mean1,sigma) anf g2(x,mean2,sigma) and their parameters
  RooRealVar mean("mean","mean",min,max);
  RooRealVar s1("sigma1","sigma",0.0,0.80);
  RooRealVar s2("sigma2","sigma",0.0,0.20);
  RooGaussian g1("gauss1","gauss",diff,mean,s1);
  RooGaussian g2("gauss2","gauss",diff,mean,s2);
  RooRealVar f("f","f",0.0,1.0); 
  RooAddPdf  dgauss("dgauss","",RooArgList(g1,g2),f); 



  // Import model into p.d.f.
  w.import(dgauss) ;
  RooArgSet* params = (RooArgSet*) dgauss.getParameters(diff) ;
  w.defineSet("parameters",*params) ;
  w.defineSet("observables",diff) ;
}
 
