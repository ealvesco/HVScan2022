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

using namespace TMath;
using namespace std;
std::vector<std::string> blacklist(const char*);

void compare_ages_granul_effi(const char* eraA, const char* eraB, const char* module)
{

   // Parameters Z axis Plots 
 
   float  mineffi=80, maxeffi=100;
   float  diffeffi=0.50;


    // -------- D E F I N E   T H E    H I S T O G R A M S  

   vector<tuple<string,int,int> > ew_histosB;
   vector<tuple<string,int,int> > ew_histosEC;

   //BARREL
   auto histoB_1=std::make_tuple ("effi_B_Wheel_vs_Station", 5, 4);
   auto histoB_2=std::make_tuple ("effi_B_Wheel_vs_Roll", 5, 21);
   auto histoB_3=std::make_tuple ("effi_B_Sector_vs_Roll", 12 , 21);
   auto histoB_4=std::make_tuple ("effi_B_Sector_vs_Station", 12, 4);
   auto histoB_5=std::make_tuple ("effi_B_Sector_vs_Wheel", 12, 5);

   ew_histosB.push_back(histoB_1);  ew_histosB.push_back(histoB_2);
   ew_histosB.push_back(histoB_3);  ew_histosB.push_back(histoB_4);
   ew_histosB.push_back(histoB_5);

   //ENDCAP
   auto histoEC_1=std::make_tuple ("effi_EC_Disk_vs_Roll", 8, 6);
   auto histoEC_2=std::make_tuple ("effi_EC_Disk_vs_Ring", 8, 2);
   auto histoEC_3=std::make_tuple ("effi_EC_Segment_vs_Roll", 36, 6);
   auto histoEC_4=std::make_tuple ("effi_EC_Segment_vs_Ring", 36, 2);
   auto histoEC_5=std::make_tuple ("effi_EC_Segment_vs_Disk", 36, 8);

   ew_histosEC.push_back(histoEC_1);   ew_histosEC.push_back(histoEC_2);
   ew_histosEC.push_back(histoEC_3);   ew_histosEC.push_back(histoEC_4);
   ew_histosEC.push_back(histoEC_5);

   TH2F* HistosB[15];
   TH2F* HistosEC[15];

   string yearA(eraA);
   string yearB(eraB);

 
   for(int i=0;i<5;i++){

   int nbinxB=std::get<1>(ew_histosB.at(i));
   int nbinyB=std::get<2>(ew_histosB.at(i));
   int nbinxEC=std::get<1>(ew_histosEC.at(i));
   int nbinyEC=std::get<2>(ew_histosEC.at(i));

   string nameB=std::get<0>(ew_histosB.at(i));
   string nameEC=std::get<0>(ew_histosEC.at(i));
   
   //BARREL

   HistosB[i]=new TH2F(nameB.c_str(),"",nbinxB,0,nbinxB,nbinyB,0,nbinyB);
   HistosB[i]->SetTitle(("Efficiency at  Point Difference Between "+yearB+"-"+yearA).c_str());
 //  HistosB[i]->GetYaxis()->SetTitleOffset(1.25);  
   HistosB[i]->GetYaxis()->SetTitleSize(0.05);  
  // HistosB[i]->GetZaxis()->SetTitleOffset(1.45);  
  // HistosB[i]->GetZaxis()->SetTitle(1.45);  
      HistosB[i]->GetZaxis()->SetTitle("[%]");
      HistosB[i]->GetXaxis()->SetLabelSize(.04);
      HistosB[i]->GetYaxis()->SetLabelSize(.04);
      HistosB[i]->GetZaxis()->SetLabelSize(.04);
      HistosB[i]->GetYaxis()->SetTitleOffset(1.35);
      HistosB[i]->GetXaxis()->SetTitleOffset(1.1);
      HistosB[i]->GetZaxis()->SetTitleOffset(1.1);

   HistosB[i+5]=new TH2F((nameB+"A").c_str(),"",nbinxB,0,nbinxB,nbinyB,0,nbinyB);
   HistosB[i+5]->SetTitle(("Efficiency at Working Point "+yearA).c_str());
   HistosB[i+5]->GetZaxis()->SetTitle("[%]");
      HistosB[i+5]->GetXaxis()->SetLabelSize(.04);
      HistosB[i+5]->GetYaxis()->SetLabelSize(.04);
      HistosB[i+5]->GetZaxis()->SetLabelSize(.04);
      HistosB[i+5]->GetYaxis()->SetTitleOffset(1.35);
      HistosB[i+5]->GetXaxis()->SetTitleOffset(1.1);
      HistosB[i+5]->GetZaxis()->SetTitleOffset(1.1);


   HistosB[i+10]=new TH2F((nameB+"B").c_str(),"",nbinxB,0,nbinxB,nbinyB,0,nbinyB);
   HistosB[i+10]->SetTitle(("Efficiency at Working Point "+yearB).c_str());
   HistosB[i+10]->GetZaxis()->SetTitle("[%]");
      HistosB[i+10]->GetXaxis()->SetLabelSize(.04);
      HistosB[i+10]->GetYaxis()->SetLabelSize(.04);
      HistosB[i+10]->GetZaxis()->SetLabelSize(.04);
      HistosB[i+10]->GetYaxis()->SetTitleOffset(1.35);
      HistosB[i+10]->GetXaxis()->SetTitleOffset(1.1);
      HistosB[i+10]->GetZaxis()->SetTitleOffset(1.1);


   //ENDCAP

   HistosEC[i]=new TH2F(nameEC.c_str(),"",nbinxEC,0,nbinxEC,nbinyEC,0,nbinyEC);
   HistosEC[i]->SetTitle(("Efficiency at Working Point Difference Between "+yearB+"-"+yearA).c_str());
   HistosEC[i]->SetMarkerSize(1.3);
   HistosEC[i]->GetYaxis()->SetTitleSize(0.05);  
   HistosEC[i]->GetYaxis()->SetLabelSize(0.05);  
   HistosEC[i]->GetYaxis()->SetTitleOffset(1.2);  
   HistosEC[i]->GetZaxis()->SetTitleOffset(1.1);  
   HistosEC[i]->GetZaxis()->SetTitle("[%]");
   HistosEC[i]->GetZaxis()->SetTitleSize(0.05);
   
   HistosEC[i+5]=new TH2F((nameEC+"A").c_str(),"",nbinxEC,0,nbinxEC,nbinyEC,0,nbinyEC);
   HistosEC[i+5]->SetTitle(("Efficiency at Working Point "+yearA).c_str());
   HistosEC[i+5]->SetMarkerSize(1.3);
   HistosEC[i+5]->GetYaxis()->SetTitleSize(0.05);  
   HistosEC[i+5]->GetYaxis()->SetLabelSize(0.05);  
   HistosEC[i+5]->GetYaxis()->SetTitleOffset(1.2);  
   HistosEC[i+5]->GetZaxis()->SetTitleOffset(1.1);  
   HistosEC[i+5]->GetZaxis()->SetTitle("[%]");
   HistosEC[i]->GetZaxis()->SetTitleSize(0.05);
   
   HistosEC[i+10]=new TH2F((nameEC+"B").c_str(),"",nbinxEC,0,nbinxEC,nbinyEC,0,nbinyEC);
   HistosEC[i+10]->SetTitle(("Efficiency at Working Point "+yearB).c_str());
   HistosEC[i+10]->SetMarkerSize(1.3);
   HistosEC[i+10]->GetYaxis()->SetTitleSize(0.05);  
   HistosEC[i+10]->GetYaxis()->SetLabelSize(0.05);  
   HistosEC[i+10]->GetYaxis()->SetTitleOffset(1.2);  
   HistosEC[i+10]->GetZaxis()->SetTitleOffset(1.1);  
   HistosEC[i+10]->GetZaxis()->SetTitle("[%]");
   HistosEC[i]->GetZaxis()->SetTitleSize(0.05);
   
   }


  
   //+++++++++++ D e f i n e   a n d   c r e a t e   y o u r   m a p s   
    vector <string> sector, layer; 
    vector <string> channel, eta_part; 
    vector<string> disk, ring, ring2;
    vector<string> wheel, stations;
    
    //++++ E n d   C a p

    for(int i=1;i<=36;i++){
         ostringstream s;
	 string name;
	 s << i; 
	 if (i<10) name = "0"+s.str();
         else      name = s.str();
	channel.push_back(name);
    }
  
    eta_part.push_back("C");eta_part.push_back("B");eta_part.push_back("A");
  
    std::vector< std::pair<std::string,std::string >  > EndCap_CH;
    for(int i=0;i<channel.size();i++){
        for(int j=0;j<eta_part.size();j++){
             std::pair<std::string,std::string > pair_map;
             pair_map.first  = channel.at(i);
             pair_map.second = eta_part.at(j);
	     EndCap_CH.push_back(pair_map);
        }
    }
 

    std::vector< std::pair<std::string,std::string >  > EndCap_Map; 
    std::vector< std::pair<std::string,std::string >  > EndCap2_Map;
    std::vector< std::pair<std::string,std::string >  > EndCap3_Map;
    std::vector< std::pair<std::string,std::string >  > EndCap4_Map;
   
    for(int k= -4;k <= 4; k++){
	 if (k!=0){
             ostringstream s;  string st;
	     s << k; 
             st=s.str();
	     if (k<0)disk.push_back("RE"+st);
	     else disk.push_back("RE+"+st);
         }
    }
   
    ring.push_back("R2"); ring.push_back("R3");  
    ring2.push_back("R2_C"); ring2.push_back("R2_B"); ring2.push_back("R2_A");
    ring2.push_back("R3_C"); ring2.push_back("R3_B"); ring2.push_back("R3_A");

    for (int i=0; i < disk.size(); i++){
         for (int j=0; j <ring.size(); j++){
             std::pair<std::string,std::string > pair_map;
             pair_map.first = disk.at(i);
             pair_map.second = ring.at(j);
             EndCap_Map.push_back(pair_map);  
         }
    }



    for (int i=0; i < disk.size(); i++){
         for (int j=0; j <ring2.size(); j++){
            std::pair<std::string,std::string > pair_map;
            pair_map.first = disk.at(i);
            pair_map.second = ring2.at(j);
            EndCap2_Map.push_back(pair_map);
         }
    }


    for (int i=0; i < channel.size(); i++){
         for (int j=0; j <ring2.size(); j++){
             std::pair<std::string,std::string > pair_map;
             pair_map.first = channel.at(i);
             pair_map.second = ring2.at(j);
             EndCap3_Map.push_back(pair_map);
         }
    }


    for (int i=0; i < channel.size(); i++){
         for (int j=0; j <ring.size(); j++){
             std::pair<std::string,std::string > pair_map;
             pair_map.first = channel.at(i);
             pair_map.second = ring.at(j);
             EndCap4_Map.push_back(pair_map);
         }
    }


    std::vector< std::pair<std::string,std::string >  > EndCap_ZPhi;
    for (int i=0; i < channel.size(); i++){
        for (int j=0; j <disk.size(); j++){
            std::pair<std::string,std::string > pair_map;
            pair_map.first = channel.at(i);
            pair_map.second = disk.at(j);
            EndCap_ZPhi.push_back(pair_map);
        }
    }


    //+++++ B a r r e l 
    //
  
    wheel.push_back("W-2");
    wheel.push_back("W-1");
    wheel.push_back("W+0");
    wheel.push_back("W+1");
    wheel.push_back("W+2");
  
    stations.push_back("RB1");
    stations.push_back("RB2");
    stations.push_back("RB3");
    stations.push_back("RB4");
  
    layer.push_back("RB1in_B");
    layer.push_back("RB1in_F");
    layer.push_back("RB1out_B");
    layer.push_back("RB1out_F");
    layer.push_back("RB2in_B");
    layer.push_back("RB2in_F");
    layer.push_back("RB2in_M");
    layer.push_back("RB2out_B");
    layer.push_back("RB2out_F");
    layer.push_back("RB3-_B");
    layer.push_back("RB3-_F");
    layer.push_back("RB3+_B");
    layer.push_back("RB3+_F");
    layer.push_back("RB4-_B");
    layer.push_back("RB4-_F");
    layer.push_back("RB4+_B");
    layer.push_back("RB4+_F");
    layer.push_back("RB4--_B");
    layer.push_back("RB4--_F");
    layer.push_back("RB4++_B");
    layer.push_back("RB4++_F");

  
    for(int i=1;i<=12;i++){
         ostringstream s;
	 string name;
	 s << i; 
	 if (i<10) name = "S0"+s.str();
         else      name = "S"+s.str();
	 sector.push_back(name);
    }

  
    std::vector< std::pair<std::string,std::string >  > Barrel_S;
    for(int i=0;i<sector.size();i++){
         for(int j=0;j<layer.size();j++){
               std::pair<std::string,std::string > pair_map;
	       pair_map.first  = sector.at(i);
	       pair_map.second = layer.at(j);
               Barrel_S.push_back(pair_map);
	 }
    }
   
 
    std::vector< std::pair<std::string,std::string >  > Barrel_S2;
    for(int i=0;i<sector.size();i++){
         for(int j=0;j<stations.size();j++){
              std::pair<std::string,std::string > pair_map;
              pair_map.first  = sector.at(i);
              pair_map.second = stations.at(j);
              Barrel_S2.push_back(pair_map);
         }
    }


    std::vector< std::pair<std::string,std::string >  > Barrel_Map;
    for (int i=0; i < wheel.size(); i++){
        for (int j=0; j <stations.size(); j++){
           std::pair<std::string,std::string > pair_map;
           pair_map.first = wheel.at(i);
           pair_map.second = stations.at(j);
           Barrel_Map.push_back(pair_map);  
        }
    }
   

    std::vector< std::pair<std::string,std::string >  > Barrel_lW;
    for (int i=0; i < wheel.size(); i++){
        for (int j=0; j <layer.size(); j++){
            std::pair<std::string,std::string > pair_map;
            pair_map.first = wheel.at(i);
            pair_map.second = layer.at(j);
            Barrel_lW.push_back(pair_map);  
        }
    }
   

    std::vector< std::pair<std::string,std::string >  > Barrel_WS;
    for (int i=0; i < sector.size(); i++){
        for (int j=0; j <wheel.size(); j++){
           std::pair<std::string,std::string > pair_map;
           pair_map.first = sector.at(i);
           pair_map.second = wheel.at(j);
           Barrel_WS.push_back(pair_map);  
        }
    }
    
    
     //--- G e t   t h e   d a t a -------------------   
   
     std::vector<double> wpBARa,  wpBARb, wpENDa,  wpENDb;
     std::vector<double> hv50BARa, hv50BARb, hv50ENDa, hv50ENDb ;
     std::vector<double> ewBARa, ewBARb, ewENDa, ewENDb;
     vector<string> RollBARa, RollENDa;
     vector<string> RollBARb, RollENDb;
     vector<int> samples;
  
     if (strcmp(module,"barrel")==0){
        samples.push_back(1);
        samples.push_back(2);
     }
     else if (strcmp(module,"endcap")==0){
          samples.push_back(-1);
          samples.push_back(-2);
     }


    vector<string> removedRolls; 
    removedRolls = blacklist("blacklist_2018_global.txt"); 
    bool matchRolls=false; 
    Char_t   RollName[38];
    Double_t WorkingPoint;
    Double_t slope50, emax, hv50, chi2, EffWP, clsWP, chi2cls;
    Int_t nentries=0; 
   
    for (std::vector<int>::const_iterator Sample = samples.begin(); Sample != samples.end(); Sample++) {
   
   	TChain *ch = new TChain("filtered");
        if ( (*Sample)==-1 ) { ch->Add(("endcap_summary_"+string(eraA)+".root/filtered").c_str()); }
        if ( (*Sample)==-2 ) { ch->Add(("endcap_summary_"+string(eraB)+".root/filtered").c_str()); }
        
        if ( (*Sample)==1 ) { ch->Add(("barrel_summary_"+string(eraA)+".root/filtered").c_str()); }
        if ( (*Sample)==2 ) { ch->Add(("barrel_summary_"+string(eraB)+".root/filtered").c_str()); }

   	TTree *filtered = (TTree*)ch;

  	cout<<" Entries : " << filtered->GetEntries()<<endl;
 	Long64_t nentries = filtered->GetEntries();
  	filtered->SetBranchAddress("RollName", &RollName);
  	filtered->SetBranchAddress("WorkingPoint", &WorkingPoint);
  	filtered->SetBranchAddress("hv50", &hv50);
  	filtered->SetBranchAddress("EffWP", &EffWP);
  
 	for(Long64_t i=0;i<nentries;i++){
           filtered->GetEntry(i);
           std::string chamber = RollName; 
  	   matchRolls=false;   
    
           if(removedRolls.size()!=0){ 
			for (size_t l=0; l < removedRolls.size();l++){	
	         	if (chamber==removedRolls.at(l))matchRolls = true;
                	else continue; 
         		}
 	       }
                if(matchRolls)continue; 	   
  
                    if ( (*Sample)==-1 ) {
        			   RollENDa.push_back(RollName);
                                   wpENDa.push_back(1000*WorkingPoint);
                                   hv50ENDa.push_back(1000*hv50);  
                                   ewENDa.push_back(EffWP);  
                     }

                    if ( (*Sample)==-2 ) { 
 				   RollENDb.push_back(RollName);
                                   wpENDb.push_back(1000*WorkingPoint);
                                   hv50ENDb.push_back(1000*hv50);  
                                   ewENDb.push_back(EffWP);  
                     } 
                    if ( (*Sample)==1 ) { 
 				  RollBARa.push_back(RollName);
                                  wpBARa.push_back(1000*WorkingPoint);
				  hv50BARa.push_back(1000*hv50);
                                  ewBARa.push_back(EffWP);  
                     }
                    if ( (*Sample)==2 ) { 
                                  RollBARb.push_back(RollName);
                                  wpBARb.push_back(1000*WorkingPoint);
				  hv50BARb.push_back(1000*hv50);
                                  ewBARb.push_back(EffWP);  
                     }
        }
    }
    
  
    //--- Do the operations whatever you want roll by roll
   
    //B A R R E L
    
    // Plot Wheel vs Station
    Int_t y=0;
    nentries = 0;
    Int_t bin=1;
  
    if(strcmp(module,"barrel")==0){ 
 
        for (vector <std::pair<std::string,std::string>>::const_iterator it = Barrel_Map.begin() ;it != Barrel_Map.end(); it++  ){
          TString nameEW = "ewBM_"+it->first+"_"+it->second;   
          TString nameEWa = "ewBMa_"+it->first+"_"+it->second;
          TString nameEWb = "ewBMb_"+it->first+"_"+it->second;
    
          TH1F *h1 = new TH1F(nameEW," ",200,-100,100);
          TH1F *h2 = new TH1F(nameEWa," ",200,0,100);
          TH1F *h3 = new TH1F(nameEWb," ",200,0,100);
       
          TString cond=it->first+"_"+it->second;

            for (int i = 0; i<RollBARb.size(); i++) {
                for (int j = 0; j<RollBARa.size(); j++){   
                   if (RollBARb.at(i)!=RollBARa.at(j))continue; 
                   if((RollBARa.at(j)).find(cond) != std::string::npos ){
            	      h1->Fill(ewBARb.at(i) - ewBARa.at(j));
                      h2->Fill(ewBARa.at(i));
                      h3->Fill(ewBARb.at(j));
	           } 
                }
            }
          y++;
          HistosB[0]->SetBinContent(bin,y,h1->GetMean());
          HistosB[5]->SetBinContent(bin,y,h2->GetMean());
          HistosB[10]->SetBinContent(bin,y,h3->GetMean());
 
          if (y == 4 ){y=0;   bin++; }  
        }

     // Plot Wheel vs Roll

      y=0;
      bin=1;
      for (vector <std::pair<std::string,std::string>>::const_iterator it = Barrel_lW.begin() ;it != Barrel_lW.end(); it++  ){
          TString nameEW = "ewBlW_"+it->first+"_"+it->second;
          TString nameEWa = "ewBlWa_"+it->first+"_"+it->second;
          TString nameEWb = "ewBlWb_"+it->first+"_"+it->second;

          TH1F *h1 = new TH1F(nameEW," ",200,-100,100);
          TH1F *h2 = new TH1F(nameEWa," ",200,0,100);
          TH1F *h3 = new TH1F(nameEWb," ",200,0,100);

          int tam=(it->second).length();
          std::string  name=it->second;
          std::string  name2=name.erase(tam-1);
          std::string  names=it->second;
          std::string  letter=names.erase(0,tam-1);
          std::string  name3="_"+letter;
          std::string  cond=it->first+"_"+name2;

              for (int i = 0; i<RollBARb.size(); i++) {
                  for (int j = 0; j<RollBARa.size(); j++){
                        if (RollBARb.at(i)!=RollBARa.at(j))continue;
                        if((RollBARa.at(j)).find(cond) != std::string::npos) {
                             if((RollBARa.at(j)).find(name3) != std::string::npos) {
                                 h1->Fill(ewBARb.at(i) - ewBARa.at(j));
                                 h2->Fill(ewBARa.at(j));
                                 h3->Fill(ewBARb.at(i));
                             }
                        }
                   }
               }

          y++;
          int entries1=h1->GetEntries();
          if (entries1==0){ HistosB[1]->SetBinContent(bin,y,-500);}
          else{HistosB[1]->SetBinContent(bin,y,h1->GetMean());}
          HistosB[6]->SetBinContent(bin,y,h2->GetMean());
          HistosB[11]->SetBinContent(bin,y,h3->GetMean());

          if (y == 21 ){y=0; bin++;}
      }


    // Plot Sector vs Roll

      y=0;
      bin=1;
      for (vector <std::pair<std::string,std::string>>::const_iterator it = Barrel_S.begin() ;it != Barrel_S.end(); it++  ){

         TString nameWP = "ewBS_"+it->second+"_"+it->first;
         TString nameWPa = "ewBSa_"+it->second+"_"+it->first;
         TString nameWPb = "ewBSb_"+it->second+"_"+it->first;

         TH1F *h1 = new TH1F(nameWP," ",200,-100,100);
         TH1F *h2 = new TH1F(nameWPa," ",200,0,100);
         TH1F *h3 = new TH1F(nameWPb," ",200,0,100);

         int tam=(it->second).length();
         std::string  name=it->second;
         std::string  name3=name;
         std::string  name4=name;
         std::string  name2=it->first;
         std::string  letter=name4.erase(0,tam-1);
         std::string  second2=name3.erase(tam-1)+name2+"_"+letter;

          for (int i = 0; i<RollBARb.size(); i++) {
	      if(RollBARb.at(i).find("W-1") == std::string::npos)continue;
              for (int j = 0; j<RollBARa.size(); j++){
                 if (RollBARb.at(i)!=RollBARa.at(j))continue;
                 if((RollBARa.at(j)).find(second2)!= std::string::npos) {
                    h1->Fill(ewBARb.at(i)-ewBARa.at(j));
                    //h2->Fill(ewBARa.at(j));cout <<"value: " << ewBARa.at(j) << "Mean: "<<  h2->GetMean()<<std::endl;
                    h3->Fill(ewBARb.at(i));
                 }
              }  
          }
        y++;
        h2->Print();
        //cout << "X "<< bin << " Y "<< y <<  endl;  
        int mentries=h1->GetEntries();
        if (mentries==0){ HistosB[2]->SetBinContent(bin,y,-500);}   
        else{HistosB[2]->SetBinContent(bin,y,h1->GetMean());}
        HistosB[7]->SetBinContent(bin,y,h2->GetMean());
        HistosB[12]->SetBinContent(bin,y,h3->GetMean());
        if (y == 21 ){y=0;bin++;}
     }

    // Plot Sector vs Station

     y=0;
     bin=1;
     for (vector <std::pair<std::string,std::string>>::const_iterator it = Barrel_S2.begin() ;it != Barrel_S2.end(); it++  ){

       TString nameWP = "ewBS2_"+it->second+"_"+it->first;
       TString nameWPa = "ewBS2a_"+it->second+"_"+it->first;
       TString nameWPb = "ewBS2b_"+it->second+"_"+it->first;

       TH1F *h1 = new TH1F(nameWP," ",200,-100,100);
       TH1F *h2 = new TH1F(nameWPa," ",200,0,100);
       TH1F *h3 = new TH1F(nameWPb," ",200,0,100);


       for (int i = 0; i<RollBARb.size(); i++) {
           for (int j = 0; j<RollBARa.size(); j++){
                if (RollBARb.at(i)!=RollBARa.at(j))continue;
                if(((RollBARa.at(j)).find(it->second)!= std::string::npos) && ( (RollBARa.at(j)).find(it->first)!= std::string::npos)) {
                    h1->Fill(ewBARb.at(i)-ewBARa.at(j));
                    h2->Fill(ewBARb.at(j));
                    h3->Fill(ewBARa.at(i));
                }
           }
       }
       
      y++;
      int mentries=h1->GetEntries();
      if (mentries==0){ HistosB[3]->SetBinContent(bin,y,-500);}
      else{HistosB[3]->SetBinContent(bin,y,h1->GetMean());}
      HistosB[8]->SetBinContent(bin,y,h2->GetMean());
      HistosB[13]->SetBinContent(bin,y,h3->GetMean());
      if (y == 4 ){y=0;bin++;}
    }

    // Plot Sector vs Wheel

     y=0;
     bin=1;
     for (vector <std::pair<std::string,std::string>>::const_iterator it = Barrel_WS.begin() ;it != Barrel_WS.end(); it++  ){

       TString nameWP = "ewBWS_"+it->second+"_"+it->first;
       TString nameWPa = "ewBWSa_"+it->second+"_"+it->first;
       TString nameWPb = "ewBWSb_"+it->second+"_"+it->first;

       TH1F *h1 = new TH1F(nameWP," ",0,-100,100);
       TH1F *h2 = new TH1F(nameWPa," ",0,0,100);
       TH1F *h3 = new TH1F(nameWPb," ",0,0,100);


       for (int i = 0; i<RollBARb.size(); i++) {
            for (int j = 0; j<RollBARa.size(); j++){
                if (RollBARb.at(i)!=RollBARa.at(j))continue;
                if(((RollBARa.at(j)).find(it->second)!= std::string::npos) && ( (RollBARa.at(j)).find(it->first)!= std::string::npos)) {
                   h1->Fill(ewBARb.at(i)-ewBARa.at(j));
                   h2->Fill(ewBARb.at(j));
                   h3->Fill(ewBARa.at(i));
                }
            }
       }
       y++;
       int mentries=h1->GetEntries();
       if (mentries==0){ HistosB[4]->SetBinContent(bin,y,-500);}
       else{HistosB[4]->SetBinContent(bin,y,h1->GetMean());}
       HistosB[9]->SetBinContent(bin,y,h2->GetMean());
       HistosB[14]->SetBinContent(bin,y,h3->GetMean());
       if (y == 5 ){y=0;bin++;}
    }

  }
 


    // E N D C A P

   else {  
	

   // Plot Disk vs Roll

       y=0;
       bin=1;
       nentries=0;
       size_t pos = 10;  

       for (vector <std::pair<std::string,std::string>>::const_iterator it = EndCap2_Map.begin() ;it!= EndCap2_Map.end(); it++  ){
        
           TString nameEW = "ewECM2_"+it->first+"_"+it->second;
           TString nameEWa = "ewECM2A_"+it->first+"_"+it->second;
           TString nameEWb = "ewECM2B_"+it->first+"_"+it->second;

           TH1F *h1 = new TH1F(nameEW," ",200,-100,100);
           TH1F *h2 = new TH1F(nameEWa," ",200,0,100);
           TH1F *h3 = new TH1F(nameEWb," ",200,0,100);
         
           int tam=(it->second).length();
           std::string name=(it->second); 
           std::string name2=name;           
           std::string first2=(it->first);
           std::string second2=name.erase(tam-1);
           std::string letter=name2.erase(0,tam-1);        

           for (int i = 0; i<RollENDb.size(); i++) {

		 for (int j = 0; j<RollENDa.size(); j++){
	             if (RollENDb.at(i) != RollENDa.at(j))continue;   
	             if(((RollENDa.at(j)).find(first2+"_"+second2) != std::string::npos) && ((RollENDa.at(j)).find(letter,pos) != std::string::npos ) ){
                         h1->Fill(ewENDb.at(i) - ewENDa.at(j));
                         h2->Fill(ewENDa.at(j));
                         h3->Fill(ewENDb.at(i));
                     }
                 }
           }   
           y++;
           int entries1=h1->GetEntries();
           int entries2=h2->GetEntries();
           if(entries1==0 && entries2==0){HistosEC[0]->SetBinContent(bin,y,-10000);} //To fix the plot
           else{ HistosEC[0]->SetBinContent(bin,y,h1->GetMean());}
           HistosEC[5]->SetBinContent(bin,y,h2->GetMean());
           HistosEC[10]->SetBinContent(bin,y,h3->GetMean());
   
	   if (y == 6 ){y=0;bin++;}
       }


   // Plot Disk vs Ring

       y=0;
       bin=1;
       nentries=0;

       for (vector <std::pair<std::string,std::string>>::const_iterator it = EndCap_Map.begin() ;it!= EndCap_Map.end(); it++  ){

           TString nameEW = "ewECM_"+it->first+"_"+it->second;
           TString nameEWa = "ewECMA_"+it->first+"_"+it->second;
           TString nameEWb = "ewECMB_"+it->first+"_"+it->second;

           TH1F *h1 = new TH1F(nameEW," ",200,-100,100);
           TH1F *h2 = new TH1F(nameEWa," ",200,0,100);
           TH1F *h3 = new TH1F(nameEWb," ",200,0,100);

           for (int i = 0; i<RollENDb.size(); i++) {

                 for (int j = 0; j<RollENDa.size(); j++){
                     if (RollENDb.at(i) != RollENDa.at(j))continue;
                     if((RollENDa.at(j)).find(it->first+"_"+it->second) != std::string::npos)  {
                         h1->Fill(ewENDb.at(i) - ewENDa.at(j));
                         h2->Fill(ewENDa.at(j));
                         h3->Fill(ewENDb.at(i));
                     }
                 }
           }
           y++;
           HistosEC[1]->SetBinContent(bin,y,h1->GetMean());
           HistosEC[6]->SetBinContent(bin,y,h2->GetMean());
           HistosEC[11]->SetBinContent(bin,y,h3->GetMean());

           if (y == 2 ){y=0;bin++;}
       }



   // Plot Segment vs Roll

       y=0;
       bin=1;
       nentries=0;
       for (vector <std::pair<std::string,std::string>>::const_iterator it = EndCap3_Map.begin() ;it!= EndCap3_Map.end(); it++  ){
          TString nameEW = "ewEC3M_"+it->first+"_"+it->second;
          TString nameEWa = "ewEC3Ma_"+it->first+"_"+it->second;
          TString nameEWb = "ewEC3Mb_"+it->first+"_"+it->second;
       
          TH1F *h1 = new TH1F(nameEW," ",200,-100,100);
          TH1F *h2 = new TH1F(nameEWa," ",200,0,100);
          TH1F *h3 = new TH1F(nameEWb," ",200,0,100);

          int tam=(it->second).length();
          std::string name=(it->second);
          std::string name2=name;
          std::string first2=(it->first);
          std::string second2=name.erase(tam-1);
          std::string letter=name2.erase(0,tam-1);

          for (int i = 0; i<RollENDb.size(); i++) {
               for (int j = 0; j<RollENDa.size(); j++){
                   if (RollENDb.at(i) != RollENDa.at(j))continue;
                   if(((RollENDa.at(j)).find(first2+"_"+letter) != std::string::npos) && ((RollENDa.at(j)).find(second2) != std::string::npos ) ){
                       h1->Fill(ewENDb.at(i) - ewENDa.at(j));
                       h2->Fill(ewENDa.at(j));
                       h3->Fill(ewENDb.at(i));
                   }
              }
          }
          y++;
          HistosEC[2]->SetBinContent(bin,y,h1->GetMean());
          HistosEC[7]->SetBinContent(bin,y,h2->GetMean());
          HistosEC[12]->SetBinContent(bin,y,h3->GetMean());
         
          if (y == 6 ){y=0;bin++;}
       }



   // Plot Segment vs Ring

       y=0;
       bin=1;
       nentries=0;
       for (vector <std::pair<std::string,std::string>>::const_iterator it = EndCap4_Map.begin() ;it!= EndCap4_Map.end(); it++  ){
          TString nameEW = "ewEC4M_"+it->first+"_"+it->second;
          TString nameEWa = "ewEC4Ma_"+it->first+"_"+it->second;
          TString nameEWb = "ewEC4Mb_"+it->first+"_"+it->second;

          TH1F *h1 = new TH1F(nameEW," ",200,-100,100);
          TH1F *h2 = new TH1F(nameEWa," ",200,0,100);
          TH1F *h3 = new TH1F(nameEWb," ",200,0,100);

          for (int i = 0; i<RollENDb.size(); i++) {
               for (int j = 0; j<RollENDa.size(); j++){
                   if (RollENDb.at(i) != RollENDa.at(j))continue;
                   if((RollENDa.at(j)).find(it->second+"_CH"+it->first) != std::string::npos  ){
                       h1->Fill(ewENDb.at(i) - ewENDa.at(j));
                       h2->Fill(ewENDa.at(j));
                       h3->Fill(ewENDb.at(i));
                   }
              }
          }
          y++;
          HistosEC[3]->SetBinContent(bin,y,h1->GetMean());
          HistosEC[8]->SetBinContent(bin,y,h2->GetMean());
          HistosEC[13]->SetBinContent(bin,y,h3->GetMean());

          if (y == 2 ){y=0;bin++;}
       }

	

   // Plot Segment vs Disk
 
       y=0;
       bin=1;  
       for (vector <std::pair<std::string,std::string>>::const_iterator it = EndCap_ZPhi.begin() ;it != EndCap_ZPhi.end(); it++  ){

           TString nameEW = "ewECZP_"+it->first+"_"+it->second;
           TString nameEWa = "ewECZPa_"+it->first+"_"+it->second;
           TString nameEWb = "ewECZPb_"+it->first+"_"+it->second;

           TH1F *h1 = new TH1F(nameEW," ",200,-100,100);
           TH1F *h2 = new TH1F(nameEWa," ",200,0,100);
           TH1F *h3 = new TH1F(nameEWb," ",200,0,100);
         

           for (int i = 0; i<RollENDb.size(); i++) {
	      for (int j = 0; j<RollENDa.size(); j++){
                if (RollENDb.at(i) != RollENDa.at(j))continue;   
                if(((RollENDa.at(j)).find(it->second) != std::string::npos) && ((RollENDa.at(j)).find(it->first) != std::string::npos) ){
                	h1->Fill(ewENDb.at(i) - ewENDa.at(j));
                        h2->Fill(ewENDa.at(j));
                        h3->Fill(ewENDb.at(i));
	        }
             }
          }
          y++; 
          HistosEC[4]->SetBinContent(bin,y,h1->GetMean());
          HistosEC[9]->SetBinContent(bin,y,h2->GetMean());
          HistosEC[14]->SetBinContent(bin,y,h3->GetMean());
          if (y == 8 ){y=0;bin++;}
       }
       
   }
//========== P r i n t i n g   t h e   p l o t s
  
   if(strcmp(module,"barrel")==0){
  
      for(int k=0;k<2;k++){
          for (size_t i=0;i<wheel.size();i++){ 
	     const char* label= wheel.at(i).c_str();
	     HistosB[k]->GetXaxis()->SetBinLabel(i+1,label);
             HistosB[k]->GetXaxis()->SetTitle("Wheel");

             HistosB[k+5]->GetXaxis()->SetBinLabel(i+1,label);
             HistosB[k+5]->GetXaxis()->SetTitle("Wheel");

             HistosB[k+10]->GetXaxis()->SetBinLabel(i+1,label);
             HistosB[k+10]->GetXaxis()->SetTitle("Wheel");
          }
      }


      for (size_t i=0;i<wheel.size();i++){ 
           const char* label= wheel.at(i).c_str();
           HistosB[4]->GetYaxis()->SetBinLabel(i+1,label);
           HistosB[4]->GetYaxis()->SetTitle("Wheel");
           
           HistosB[9]->GetYaxis()->SetBinLabel(i+1,label);
           HistosB[9]->GetYaxis()->SetTitle("Wheel");
           
           HistosB[14]->GetYaxis()->SetBinLabel(i+1,label);
           HistosB[14]->GetYaxis()->SetTitle("Wheel");
      }


      for(int k=0;k<4;k++){
          if(k==0||k==3){
              for (size_t i=0;i<stations.size();i++){ 
	         const char* label= stations.at(i).c_str();
      	         HistosB[k]->GetYaxis()->SetBinLabel(i+1,label);
                 HistosB[k]->GetYaxis()->SetTitle("Station");
               
                 HistosB[k+5]->GetYaxis()->SetBinLabel(i+1,label);
                 HistosB[k+5]->GetYaxis()->SetTitle("Station");
           
                 HistosB[k+10]->GetYaxis()->SetBinLabel(i+1,label);
                 HistosB[k+10]->GetYaxis()->SetTitle("Station");
              }
         }
      }
   

      for(int k=2;k<5;k++){
         for (size_t i=0;i<sector.size();i++){
	     const char* label= sector.at(i).c_str();
	     HistosB[k]->GetXaxis()->SetBinLabel(i+1,label);
             HistosB[k]->GetXaxis()->SetTitle("Sector");
 
             HistosB[k+5]->GetXaxis()->SetBinLabel(i+1,label);
             HistosB[k+5]->GetXaxis()->SetTitle("Sector");
 
             HistosB[k+10]->GetXaxis()->SetBinLabel(i+1,label);
             HistosB[k+10]->GetXaxis()->SetTitle("Sector");   
         }
      }
 

      for (int k=1;k<3;k++){  
         for (size_t i=0;i<layer.size();i++){ 
           const char* label= layer.at(i).c_str();
           HistosB[k]->GetYaxis()->SetBinLabel(i+1,label);
           HistosB[k]->GetYaxis()->SetTitleOffset(1.35);
           HistosB[k]->GetYaxis()->SetTitle("Layer");
          
           HistosB[k+5]->GetYaxis()->SetBinLabel(i+1,label);
           HistosB[k+5]->GetYaxis()->SetTitleOffset(1.35);
           HistosB[k+5]->GetYaxis()->SetTitle("Layer");
   
           HistosB[k+10]->GetYaxis()->SetBinLabel(i+1,label);
           HistosB[k+10]->GetYaxis()->SetTitleOffset(1.35);
           HistosB[k+10]->GetYaxis()->SetTitle("Layer");
        }
     }

   }
  
    //Plots EndCap

   else {
   
       for (int k=0;k<2;k++){
          for (size_t i=0;i<disk.size();i++){ 
             const char* label= disk.at(i).c_str();

             HistosEC[k]->GetXaxis()->SetBinLabel(i+1,label);
             HistosEC[k]->GetXaxis()->SetTitle("Disk");

             HistosEC[k+5]->GetXaxis()->SetBinLabel(i+1,label);
             HistosEC[k+5]->GetXaxis()->SetTitle("Disk");
 
             HistosEC[k+10]->GetXaxis()->SetBinLabel(i+1,label);
             HistosEC[k+10]->GetXaxis()->SetTitle("Disk");
    
             if (k==0){
         
                HistosEC[k+4]->GetYaxis()->SetBinLabel(i+1,label);
                HistosEC[k+4]->GetYaxis()->SetTitle("Disk");

                HistosEC[k+9]->GetYaxis()->SetBinLabel(i+1,label);
                HistosEC[k+9]->GetYaxis()->SetTitle("Disk");
 
                HistosEC[k+14]->GetYaxis()->SetBinLabel(i+1,label);
                HistosEC[k+14]->GetYaxis()->SetTitle("Disk");


             }
          }
      }

   
      for (int k=0;k<3;k++){
           for (size_t i=0;i<ring2.size();i++){ 
             const char* label=  ring2.at(i).c_str();
             
             if (k==0||k==2){     
                HistosEC[k]->GetYaxis()->SetBinLabel(i+1,label);
                HistosEC[k]->GetYaxis()->SetTitle("R");

                HistosEC[k+5]->GetYaxis()->SetBinLabel(i+1,label);
                HistosEC[k+5]->GetYaxis()->SetTitle("R");
 
                HistosEC[k+10]->GetYaxis()->SetBinLabel(i+1,label);
                HistosEC[k+10]->GetYaxis()->SetTitle("R");
             }
          }
      }




       for (int k=1;k<4;k++){
            for (size_t i=0;i<ring.size();i++){
               const char* label=ring.at(i).c_str();
                
               if (k==1||k==3){
                  HistosEC[k]->GetYaxis()->SetBinLabel(i+1,label);
                  HistosEC[k]->GetYaxis()->SetTitle("Ring");

                  HistosEC[k+5]->GetYaxis()->SetBinLabel(i+1,label);
                  HistosEC[k+5]->GetYaxis()->SetTitle("Ring");
 
                  HistosEC[k+10]->GetYaxis()->SetBinLabel(i+1,label);
                  HistosEC[k+10]->GetYaxis()->SetTitle("Ring");
               }
           }
      }



      for (int k=2;k<5;k++){
           for (size_t i=0;i<channel.size();i++){ 
              const  char* label= channel.at(i).c_str();

              HistosEC[k]->GetXaxis()->SetBinLabel(i+1,label);
              HistosEC[k]->GetXaxis()->SetTitle("Segment");

              HistosEC[k+5]->GetXaxis()->SetBinLabel(i+1,label);
              HistosEC[k+5]->GetXaxis()->SetTitle("Segment");

              HistosEC[k+10]->GetXaxis()->SetBinLabel(i+1,label);
              HistosEC[k+10]->GetXaxis()->SetTitle("Segment");

           }
      }


   }
  
 
  // Draw the Map in a histogram         

   gStyle->SetOptStat(0); 
   gStyle->SetPalette(1); 
   gStyle->SetPadTickX(1); 
   gStyle->SetPadTickY(1); 
//   gStyle->SetPalette(1);
  Int_t palette[6];
     palette[0] = 1;
     palette[1] = 4;
     palette[2] = 6;
     palette[3] = 3;
     palette[4] = 5;
     palette[5] = 2;
     //palette[6] = 7;
 //    palette[7] = 9;
 //    gStyle->SetNdivisions(602, "Z");
      gStyle->SetPalette(6,palette); 
 //    gStyle->SetPalette(55);


   TCanvas* MyCanvas[15];

   int W = 800;
   int H = 600;

  //  
  // Simple example of macro: plot with CMS name and lumi text
  //  (this script does not pretend to work in all configurations)
  // iPeriod = 1*(0/1 7 TeV) + 2*(0/1 8 TeV)  + 4*(0/1 13 TeV) 
  // For instance: 
  //               iPeriod = 3 means: 7 TeV + 8 TeV
  //               iPeriod = 7 means: 7 TeV + 8 TeV + 13 TeV 
  // Initiated by: Gautier Hamel de Monchenault (Saclay)
  // Updated by:   Dinko Ferencek (Rutgers)
  //  
   int H_ref = 600; 
   int W_ref = 800; 

  // references for T, B, L, R
   float T = 0.08*H_ref;
   float B = 0.12*H_ref; 
   float L = 0.12*W_ref;
   float R = 0.14*W_ref;

   gStyle->SetLineWidth(2);
   gStyle->SetHistLineWidth(2); 
   gStyle->SetFrameLineWidth(2); 
   gStyle->SetOptStat(0);
   gStyle->SetPaintTextFormat("2.3f "); 
   gStyle->SetNumberContours(18); 
  //TCanvas* canv = new TCanvas(canvName,canvName,50,50,W,H);
   std::string title;

   for(int i=0;i<15;i++){
     stringstream ss;
     ss<<i;
     string canvas="canvas"+ss.str();
     MyCanvas[i]= new TCanvas(canvas.c_str()," ",50,50,W,H); 
     MyCanvas[i]->SetFillColor(0);
     MyCanvas[i]->SetBorderMode(0);
     MyCanvas[i]->SetFrameFillStyle(0);
     MyCanvas[i]->SetFrameBorderMode(0);
     MyCanvas[i]->SetLeftMargin( L/W );
     MyCanvas[i]->SetRightMargin( R/W );
     MyCanvas[i]->SetTopMargin( T/H );
     MyCanvas[i]->SetBottomMargin( B/H );
     //MyCanvas[i]->SetTickx(0);
     //MyCanvas[i]->SetTicky(0);
     MyCanvas[i]->SetGrid();
      //TCanvas(canvas.c_str(),"",1000,800);
   }

/*
   for(int i=0;i<15;i++){ 
     MyCanvas[i]->SetFillColor(0);
     MyCanvas[i]->SetBorderMode(0);
     MyCanvas[i]->SetBorderSize(2);
     MyCanvas[i]->SetLeftMargin(0.155);
     MyCanvas[i]->SetRightMargin(0.155);
     MyCanvas[i]->SetFrameBorderMode(0);
     MyCanvas[i]->SetFrameBorderMode(0);
   }
*/
  
  // for (int k=0;k<15;k++){
  // }



   if(strcmp(module,"barrel")==0){

      TFile *f = new TFile(("Plots_ew_Barrel"+yearB+"-"+yearA+".root").c_str(),"RECREATE");

      for (int k=0;k<5;k++){
         HistosB[k]->GetZaxis()->SetRangeUser(-diffeffi,diffeffi);
         HistosB[k+5]->GetZaxis()->SetRangeUser(mineffi,maxeffi);
         HistosB[k+10]->GetZaxis()->SetRangeUser(mineffi,maxeffi);
      }

         
    
     
      for(int i=0;i<15;i++){
         MyCanvas[i]->cd();
         HistosB[i]->Draw("COLZ TEXT"); 
         MyCanvas[i]->Update(); 
         MyCanvas[i]->Modified();  
      }

      //S A V E   T H E   R E S U L T S   I N   A   P N G   F I L E  
      gSystem->mkdir("Plots_ew_Barrel");

      for(int i=0;i<5;i++){

        const char* axisx;
        const char* axisy;
        axisx=HistosB[i]->GetXaxis()->GetTitle();
        axisy=HistosB[i]->GetYaxis()->GetTitle();
        string axisxs(axisx);
        string axisys(axisy);

        MyCanvas[i]->SaveAs(("Plots_ew_Barrel/B_"+axisxs+"_vs_"+axisys+"_"+yearB+"-"+yearA+".png").c_str());
        MyCanvas[i]->Write(("B_"+axisxs+"_vs_"+axisys+"_"+yearB+"-"+yearA).c_str());
        MyCanvas[i]->Clear();
        MyCanvas[i]->Close();
       
        MyCanvas[i+5]->SaveAs(("Plots_ew_Barrel/B_"+axisxs+"_vs_"+axisys+"_"+yearA+".png").c_str());
        MyCanvas[i+5]->Write(("B_"+axisxs+"_vs_"+axisys+"_"+yearA).c_str());
        MyCanvas[i+5]->Clear();
        MyCanvas[i+5]->Close();
       
        MyCanvas[i+10]->SaveAs(("Plots_ew_Barrel/B_"+axisxs+"_vs_"+axisys+"_"+yearB+".png").c_str());
        MyCanvas[i+10]->Write(("B_"+axisxs+"_vs_"+axisys+"_"+yearB).c_str());
        MyCanvas[i+10]->Clear();
        MyCanvas[i+10]->Close();
      }
    f->Close(); 
   } 

   else {
 
      TFile *f = new TFile(("Plots_ew_EndCap"+yearB+"-"+yearA+".root").c_str(),"RECREATE");
      
      for (int k=0;k<5;k++){
         HistosEC[k]->GetZaxis()->SetRangeUser(-diffeffi,diffeffi);
         HistosEC[k+5]->GetZaxis()->SetRangeUser(mineffi,maxeffi);
         HistosEC[k+10]->GetZaxis()->SetRangeUser(mineffi,maxeffi);
      }


      for(int i=0;i<15;i++){
         MyCanvas[i]->cd();
         if(i==2||i==7||i==12||i==3||i==4||i==8||i==9||i==13||i==14){
            HistosEC[i]->Draw("COLZ TEXT89");
            MyCanvas[i]->Update();
            MyCanvas[i]->Modified();
         }else{
            HistosEC[i]->Draw("COLZ TEXT");
            MyCanvas[i]->Update();
            MyCanvas[i]->Modified();
         }
      }
 
     //S A V E   T H E   R E S U L T S   I N   A   P N G   F I L E  
     // gSystem->mkdir("Plots_ew_EndCap");
      for(int i=0;i<5;i++){

        const char* axisx;
        const char* axisy;
        axisx=HistosEC[i]->GetXaxis()->GetTitle();
        axisy=HistosEC[i]->GetYaxis()->GetTitle();
        string axisxs(axisx);
        string axisys(axisy);
        MyCanvas[i]->Write(("EC_"+axisxs+"_vs_"+axisys+"_"+yearB+"-"+yearA).c_str());
        MyCanvas[i+5]->Write(("EC_"+axisxs+"_vs_"+axisys+"_"+yearA).c_str());
        MyCanvas[i+10]->Write(("EC_"+axisxs+"_vs_"+axisys+"_"+yearB).c_str());
/*
        MyCanvas[i]->SaveAs(("Plots_ew_EndCap/EC_"+axisxs+"_vs_"+axisys+"_"+yearB+"-"+yearA+".pdf").c_str());
        MyCanvas[i]->Clear();
        MyCanvas[i]->Close();

        MyCanvas[i+5]->SaveAs(("Plots_ew_EndCap/EC_"+axisxs+"_vs_"+axisys+"_"+yearA+".pdf").c_str());
        MyCanvas[i+5]->Clear();
        MyCanvas[i+5]->Close();

        MyCanvas[i+10]->SaveAs(("Plots_ew_EndCap/EC_"+axisxs+"_vs_"+axisys+"_"+yearB+".pdf").c_str());
        */MyCanvas[i]->Clear();
        MyCanvas[i]->Close();
        
     }
   //delete  palette;
   f->Close(); 
  
   }

   
   wheel.clear();
   layer.clear();
   sector.clear();
   stations.clear();
   disk.clear();
   ring.clear();
   ring2.clear();
   eta_part.clear();
   EndCap_CH.clear();  
   EndCap_Map.clear();
   EndCap2_Map.clear();   
   EndCap3_Map.clear(); 
   EndCap4_Map.clear(); 
   Barrel_S.clear();  
   Barrel_S2.clear(); 
   Barrel_WS.clear();  
   Barrel_lW.clear();  
   Barrel_Map.clear();  
 
   return;

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
