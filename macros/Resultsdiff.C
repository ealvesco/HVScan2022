#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "TCanvas.h"
#include "TAxis.h"
#include <TCanvas.h>
#include <TAxis.h>
#include <TFile.h>
#include <TMath.h>
#include <TTree.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <TChain.h>
#include <tuple>
#include "TH1.h"
#include "THStack.h"

using namespace TMath;
using namespace std;

void Resultsdiff(){
 
  
  vector<Color_t> colors;
  colors.push_back(kRed); colors.push_back(kGreen);  
  colors.push_back(kBlue); colors.push_back(kMagenta);  
  colors.push_back(kOrange); colors.push_back(kCyan);  
  colors.push_back(kViolet); colors.push_back(kTeal);  
     
  vector<tuple<int, std::string> > stations;
  auto RB1in_=std::make_tuple (1,"RB1in");
  auto RB1out_=std::make_tuple(2,"RB1out");
  auto RB2in_=std::make_tuple (3,"RB2in");
  auto RB2out_=std::make_tuple (4,"RB2out");
  auto RB3_=std::make_tuple (5,"RB3");
  auto RB4_=std::make_tuple (6,"RB4");
  stations.push_back(RB1in_);   
  stations.push_back(RB1out_);   
  stations.push_back(RB2in_);   
  stations.push_back(RB2out_);   
  stations.push_back(RB3_);   
  stations.push_back(RB4_);   
 
  const int points = 6;  
  
//  const char* stations[points] = {"RB1in","RB1out", "RB2in","RB2out", "RB3", "RB4"};    
    
  float Wm2[points] = {-0.397048,-0.387255,-0.386199,-0.3854,-0.089721,-0.187302};  
  float Wm2err[points] = {0.233673,0.344189,0.353627,0.296629,0.412432,0.456829};
    
  float Wm1[points] ={-0.370031,-0.221744,-0.160472,-0.405744,-0.258178,-0.0997029 };  
  float Wm1err[points] ={0.376412,0.232803,0.408128,0.447436,0.251746,0.380697};
  
  float Wp0[points] = {-0.357515,-0.0922255,-0.152,-0.242375,-0.120461,-0.0885681};
  float Wp0err[points] = {0.24853,0.252065,0.133574,0.265156,0.187005,0.277443};

  float Wp1[points] = {-0.400977,-0.218549,-0.254704,-0.361715,-0.119338,-0.144984 };
  float Wp1err[points] = {0.364436,0.140793,0.302485,0.469145,0.243986,0.34466 };

  float Wp2[points] = {-0.387997,-0.259349,-0.292329,-0.208911,-0.138744,-0.114901 };
  float Wp2err[points] = {0.337557,0.23384,0.260155,0.210064,0.382468,0.491738};
 
  TH1F* h_18b17b[5];
  THStack* s_18b17b = new THStack("s_18b17b","") ;  
   
for (int l=-2, k=0; l <= 2; ++l, k++ ){
     stringstream ss; 
     string name; 
     if (l<0) {ss << -1*l; name = "Wm"+ss.str();} 
     else  {ss << l; name = "Wp"+ss.str();} 
     h_18b17b[k] = new TH1F((name+"_18b17b").c_str(),"",6,1,7); 
     h_18b17b[k]->SetLineColor(colors.at(k));
     h_18b17b[k]->SetMarkerColor(colors.at(k));
     h_18b17b[k]->SetMarkerStyle(33);
     h_18b17b[k]->GetYaxis()->SetRangeUser(-0.9,0.9);     
     s_18b17b->Add( h_18b17b[k]);     
     for(size_t kk=0; kk < points; ++kk) h_18b17b[k]->GetXaxis()->SetBinLabel(kk+1, (std::get<1>(stations.at(kk))).c_str() ); 
  }
  

   
  for (size_t k=0; k < points; ++k ){
	  
          h_18b17b[0]->SetBinContent(std::get<0>(stations.at(k)), Wm2[k]); 
          h_18b17b[0]->SetBinError(std::get<0>(  stations.at(k))  , Wm2err[k]); 
          h_18b17b[1]->SetBinContent(std::get<0>(stations.at(k)), Wm1[k]); 
          h_18b17b[1]->SetBinError(std::get<0>(  stations.at(k))  , Wm1err[k]); 
          h_18b17b[2]->SetBinContent(std::get<0>(stations.at(k)), Wp0[k]); 
          h_18b17b[2]->SetBinError(std::get<0>(  stations.at(k))  , Wp0err[k]); 
          h_18b17b[3]->SetBinContent(std::get<0>(stations.at(k)), Wp1[k]); 
          h_18b17b[3]->SetBinError(std::get<0>(  stations.at(k))  , Wp1err[k]); 
          h_18b17b[4]->SetBinContent(std::get<0>(stations.at(k)), Wp2[k]); 
          h_18b17b[4]->SetBinError(std::get<0>(  stations.at(k))  , Wp2err[k]); 
 	            
     } 
  gStyle->SetErrorX(0.0001); 
  s_18b17b->Draw("nostack");      
  
  
}
