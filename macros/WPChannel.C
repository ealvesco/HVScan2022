#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include "TCanvas.h"
#include "TAxis.h"
#include <TCanvas.h>
#include <TAxis.h>
#include <TFile.h>
#include <TMath.h>
#include <TTree.h>
#include <TChain.h>

void Title(TH1F* ,const char* ,const char* ,const char* , float);
void Format(TH1F* , int , int , int , int);
void Legend2(float, float, float, float,const char*, TH1F*,const char*, TH1F*,const char*, float);
void Legend3(float, float, float, float, const char*, TH1F*, const char*, TH1F*, const char*, TH1F*, const  char*, float);
TCanvas* DrawOFUF(TH1F*, TH1F*, TH1F*, bool, int, const char*, const char*, const char*);
std::vector<std::string> blacklist(const char*);

using namespace TMath;
using namespace std;

//int cpu = 2;

void WPChannel()
{
 
    //---Define the Roll Names and variables      
    vector<string> rollBAR, rollEND; 
    vector<Float_t> wpBAR, wpEND, wp_chBAR, wp_chEND;
    Float_t wp_chEC, wp_chB;
    Float_t MAX=0.0; 
    Float_t MIN=1000000.0;
    Float_t MEAN,sum;
    int nrolls;   
 
   TH1F* hmaxEC = new TH1F ("WPmaxEC","#Delta_{wp}(MAX_{ROLL} - WP_{CH})",100,0.,600.);
   TH1F* hmaxB = new TH1F ("WPmaxB","#Delta_{wp}(MAX_{ROLL} - WP_{CH})",100,0.,600.);
   TH1F* hminEC = new TH1F ("WPminEC","#Delta_{wp}(WP_{CH} - MIN_{ROLL})",100,0,500.);  
   TH1F* hminB = new TH1F ("WPminB","#Delta_{wp}(WP_{CH} - MIN_{ROLL})",100,0,600.);
   TH1F* hwpEC = new TH1F ("WPEC","(WP(CH)",100,8800,10200.);  
   TH1F* hwpB = new TH1F ("WPB","WP(CH)",50,8800,10200.);
   TH1F* hwpRE4 = new TH1F ("WPRE4","WP(CH)",50,8800,10200.);
   
   ofstream wp_ch_EC, wp_ch_B; 
   
   //--- Get the Name of the Chambers
    ifstream map_channel; 
    //map_channel.open("ChambersName_EndCap_noRE4.txt");  
    map_channel.open("ChambersName_EndCap.txt");  
    string CH0, CH1;
    vector<string> ch_roll0,ch_roll1;      
    while (map_channel.good()) {
        map_channel >> CH0
                    >> CH1;
        if(map_channel.eof()) break; 
        ch_roll0.push_back(CH0+"_"); ch_roll1.push_back(CH1+"_"); 
    }      
    map_channel.close();
     
   
    map_channel.open("ChambersName_Barrel.txt");
    string CHB;
    vector<string> chB_roll;
    while (map_channel.good()) {
        map_channel >> CHB; 
  	if (map_channel.eof())break;        
        chB_roll.push_back(CHB+"_"); 
    }
    map_channel.close();    
      

    //--- Get the data summary   
    vector<std::string> removedRolls; 
    removedRolls = blacklist("../data/blacklist_2017.txt"); 

    TChain *ch = new TChain("filtered");
  
    ch->Add("../summary/barrel_summary_2017.root/filtered"); }
    ch->Add("../summary/endcap_summary_2017.root/filtered"); }

    TTree *filtered = (TTree*)ch;

  	cout<<" Entries : "<<filtered->GetEntries()<<endl;
  	Long64_t nentries = filtered->GetEntries();
  	filtered->SetBranchAddress("RollName", &RollName);
  	filtered->SetBranchAddress("WorkingPoint", &WorkingPoint);
/*  	filtered->SetBranchAddress("emax", &emax);
  	filtered->SetBranchAddress("hv50", &hv50);
  	filtered->SetBranchAddress("chi2", &chi2);
  	filtered->SetBranchAddress("slope50", &slope50);
  	filtered->SetBranchAddress("EffWP", &EffWP);
  	filtered->SetBranchAddress("clsWP", &clsWP);
  	filtered->SetBranchAddress("chi2cls", &chi2cls);
*/
      for(Long64_t i=0;i<nentries;i++){
           filtered->GetEntry(i);
           std::string chamber = RollName; 
           matchRolls=false; 
           for (size_t l=0; l < removedRolls.size();l++){	
	   	      if (chamber==removedRolls.at(l))matchRolls = true;
          	      else continue; 
	   } 
 	   if(matchRolls)continue; 	   
           if(chamber.find("W")!= std::string::npos){                
                                  rollBAR.push_back(t.RollName);
                                  wpBAR.push_back(1000*t.WorkingPoint)
			}
 	    else {             
		rollEND.push_back(t.RollName);
                wpEND.push_back(1000*t.WorkingPoint);
                                 }


          }
 
   //--- Define the HV applied to each chamber 
 
   string stroll1, stroll2;   
   //+++ For the End Cap 

     std::vector< std::pair<std::string,std::string >> MapWPCHEC;
     std::vector< std::pair<std::string,std::string >> MapRollCHEC;

   string::size_type match1, matchmaxEC;
   string::size_type matchREp4, matchREm4;
   Int_t MAXLESS_100, MAXGREATER_100; 
   Int_t MINLESS_100, MINGREATER_100; 
   for (int i=0; i<ch_roll0.size(); i++){
     	stroll1="c_";
     	stroll2="c_";
        MIN=100000.0; 
     	MAX=0.0;
     	sum = 0.0;
     	nrolls = 0;
        std::pair<std::string,std::string > pair_mapWP;
        std::pair<std::string,std::string > pair_mapROLL;
 	for (int j=0; j<rollEND.size(); j++){ 
       	   match1= (rollEND.at(j)).find(ch_roll0.at(i)); 
           matchmaxEC= (rollEND.at(j)).find(ch_roll1.at(i));
          if(match1!=std::string::npos || matchmaxEC!=std::string::npos) {
		   std::stringstream ss2;
           	   ss2<<wpEND.at(j);  
		  
		  if (match1!=std::string::npos){
			  if ((rollEND.at(j)).find("_A")!=std::string::npos)stroll2 +="_wpCH1A_"+ss2.str();
		          else if ((rollEND.at(j)).find("_B")!=std::string::npos)stroll2 +="_wpCH1B_"+ss2.str();
		          else stroll2 +="_wpCH1C_"+ss2.str();
		  }
		  else{

			  if ((rollEND.at(j)).find("_A")!=std::string::npos)stroll2 +="_wpCH2A_"+ss2.str();
		          else if ((rollEND.at(j)).find("_B")!=std::string::npos)stroll2 +="_wpCH2B_"+ss2.str();
		          else stroll2 +="_wpCH2C_"+ss2.str();

		  }
	   	   stroll1 +=rollEND.at(j)+"_";
	   	   if(wpEND.at(j)<MIN)MIN=wpEND.at(j);
           	   if(wpEND.at(j)>MAX)MAX=wpEND.at(j);
	   	   sum +=wpEND.at(j);
           	   ++nrolls;      
	   }
      
        }

      	if (nrolls == 0 )MEAN = 0;
      	else MEAN = sum/nrolls;
        std::stringstream ss0, ss1;
        ss0 << nrolls; 
          
    	
        wp_chEC = (MAX-MIN >= 100.) ? MIN+100.0: MEAN ;  
        //++++++++++++ 
        pair_mapROLL.first =ch_roll0.at(i)+ch_roll1.at(i);
        pair_mapROLL.second =stroll2;
	MapRollCHEC.push_back(pair_mapROLL);
	
	pair_mapWP.first =ch_roll0.at(i)+ch_roll1.at(i);
	ss1<<wp_chEC;  
        pair_mapWP.second =ss0.str()+"_wp"+ss1.str();  
	MapWPCHEC.push_back(pair_mapWP);

	matchREp4= (ch_roll0.at(i)).find("RE+4");
        matchREm4= (ch_roll0.at(i)).find("RE-4");   	
        if(nrolls !=0){ 
              if (matchREm4!=std::string::npos || matchREp4!=std::string::npos  )hwpRE4->Fill(wp_chEC);
  	      else hwpEC->Fill(wp_chEC); 
              
              hmaxEC->Fill(MAX-wp_chEC);
     	      hmaxEC->Fill(wp_chEC-MIN);
      	}
        
        if(MAX-wp_chEC<100){
        	++MAXLESS_100;}
    	else ++MAXGREATER_100;
        if(wp_chEC-MIN<100)++MINLESS_100;
        else ++MINGREATER_100;
          
   
   }
   //+++ For the Barrel  
   std::vector< std::pair<std::string,std::string >> MapWPCHB;
   std::vector< std::pair<std::string,std::string >> MapRollCHB;
   string::size_type match;
   Int_t MAXLESSB_100, MAXGREATERB_100;
   Int_t MINLESSB_100, MINGREATERB_100;
   for (int i=0; i<chB_roll.size(); i++){
        stroll1="c_";
	stroll2="c_";
	MIN=100000.0;
        MAX=0.0;
        sum = 0.0;
        nrolls = 0;
        std::pair<std::string,std::string > pair_mapWPB;
        std::pair<std::string,std::Int_t > pair_mapNumB;
        std::pair<std::string,std::string > pair_mapROLLB;
        for (int j=0; j<rollBAR.size(); j++){
           std::stringstream ss2;
           ss2<<wpBAR.at(j);  
           match= (rollBAR.at(j)).find(chB_roll.at(i));
           if(match!=std::string::npos){
            if ((rollBAR.at(j)).find("_Forward")!=std::string::npos)stroll2 +="_wpFor_"+ss2.str();
            else if ((rollBAR.at(j)).find("_Backward")!=std::string::npos)stroll2 +="_wpBac_"+ss2.str();
            else stroll2 +="_wpMid_"+ss2.str();


           if(wpBAR.at(j)<MIN)MIN=wpBAR.at(j);
           if(wpBAR.at(j)>MAX)MAX=wpBAR.at(j);
           sum +=wpBAR.at(j);
           ++nrolls;
           }

        }

        if (nrolls == 0 )MEAN = 0.;
        else MEAN = sum/nrolls;
        
       wp_chB = (MAX-MIN >= 100.) ? MIN+100.0: MEAN !=0 ? MEAN : 0 ;
       
       if(nrolls!=0){ 
       hwpB->Fill(wp_chB);   
       hmaxB->Fill(MAX-wp_chB);
       hmaxB->Fill(wp_chB-MIN);
       }
 
        pair_mapROLLB.first =chB_roll.at(i);
        pair_mapROLLB.second =stroll2;
	MapRollCHB.push_back(pair_mapROLLB);
	
        std::stringstream ss0, ss1;
        ss0 << nrolls; 
	pair_mapWPB.first =chB_roll.at(i);
	ss1<<wp_chB;  
        pair_mapWPB.second =ss0.str()+"_wp"+ss1.str();  
	MapWPCHB.push_back(pair_mapWPB);
       
       
       if(MAX-wp_chB<=100)++MAXLESSB_100;
       else  ++MAXGREATERB_100;
       hminB->Fill(wp_chB-MIN);
       if(wp_chB-MIN<100)++MINLESSB_100;
       else ++MINGREATERB_100;
       //wp_ch_B << chB_roll.at(i) << "   wpCH=" << wp_chB<< endl;        

  }
  // wp_ch_B.close();
  //ofstream WPCHout;
  //WPCHout.open("WPpCH_out.txt");
  for (vector <std::pair<std::string,std::string>>::const_iterator it =  MapRollCHEC.begin() ;it != MapRollCHEC.end(); it++ ){
       //cout <<it->first<<" "<<it->second<<"  "<< endl;
      for (vector <std::pair<std::string,std::string>>::const_iterator itr =  MapWPCHEC.begin() ;itr != MapWPCHEC.end(); itr++  ){
     		if(it->first != itr->first)continue;
	      	   //WPCHout <<it->first<<"  "<<itr->second <<" "<<it->second<<endl;  
	      	   cout  <<it->first<<"  "<<itr->second <<" "<<it->second<<endl;  
      }
  }
 /* 
  //WPCHout.seekp(0L,ios::end); 
  for (vector <std::pair<std::string,std::string>>::const_iterator it =  MapRollCHB.begin() ;it != MapRollCHB.end(); it++ ){
       //cout <<it->first<<" "<<it->second<<"  "<< endl;
      for (vector <std::pair<std::string,std::string>>::const_iterator itr =  MapWPCHB.begin() ;itr != MapWPCHB.end(); itr++  ){
     		if(it->first != itr->first)continue;
	      	   cout <<it->first<<"  "<<itr->second <<" "<<it->second<<endl;  
      }
  }*/
  /*
  WPCHout.close();
  system("sed -e 's:_wp: :g' -e 's:_A_:_A :g' -e 's:_B_:_B :g' -e 's:_C_:_C :g' WPpCH_out.txt > WPpCH_out_.txt");
  system("mv WPpCH_out_.txt WPpCH_out.txt");
 */
  
  
  
  // ---- Plot your results 
  
  gStyle->SetOptStat(111111);
  TCanvas* c = new TCanvas("c","c", 1024, 768);
  c->SetTicks(1, 1);
  TPad *Pad1 = new TPad("Pad1", "The pad 100% of the height", 0.0, 0.0, 1.0, 1.0, 0);
  Pad1->Draw();
  Pad1->SetTicks(1, 1);

  Pad1->cd();

  Title(hwpB,"WP_{CH} Black-list "," WP_{ch}[V] ", "Events", 1.2);
  Format(hwpB, 2, 2, 3001, 2);
  DrawWithOFUF(hwpB, 0, 0); 
  Format(hwpEC, 2, 4, 3001, 4);
  DrawWithOFUF(hwpEC, 0, 1);
  Format(hwpRE4, 2, 3, 3001, 3);
  DrawWithOFUF(hwpRE4, 0, 2);

  Legend3(0.12, 0.75, 0.28, 0.88,
       "hwp",
        hwpB, "WP_{CH} Barrel",
        hwpEC,"WP_{CH} EnCap",
        hwpRE4,"WP_{CH} RE4",
        0.028);  

 c->Update();
 Pad1->cd();
 Title(hmaxEC,"Diff MAX MIN wrt WP_{CH}"," WP_{ch}[V] ", "Events", 1.2);
 Format(hmaxEC, 2, 4, 3001, 4);
 DrawWithOFUF(hmaxEC, 0, 0);
 Format(hmaxB, 2, 2, 3001, 4);
 DrawWithOFUF(hmaxB, 0, 1);

 Legend2(0.12, 0.75, 0.28, 0.88,
       "Diff MAX MIN wrt WP_{CH}",
        hmaxB, "Barrel",
        hmaxEC,"EnCap",
        0.028);

			 
 return;

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
	Hist->SetLineColor(LineColor);
	Hist->SetFillStyle(FillStyle);
	Hist->SetFillColor(FillColor);
}

void Legend3(float x1, float y1, float x2, float y2, const char* Header, TH1F* Entry1, const char* Desc1, TH1F* Entry2, const char* Desc2, TH1F* Entry3,const  char* Desc3, float TextSize)
{
	TLegend *Leg; 
        Leg = new TLegend(x1, y1, x2, y2);
	Leg->AddEntry(Entry1, Desc1, "lep");
	Leg->AddEntry(Entry2, Desc2, "lep");	
	Leg->AddEntry(Entry3, Desc3, "lep");
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
        
        /*TH1F *HTmp1 = (TH1F*)HistB->Clone("hTmp1");      
        TH1F *HTmp2 = (TH1F*)HistEC->Clone("hTmp2");      
        TH1F *HTmp3 = (TH1F*)HistRE4->Clone("hTmp3");      
        */
        //TH1F *HTmp = (TH1F*)Hist->Clone("hTmp");
        HTmp1->SetName(HistB->GetName());
        HTmp2->SetName(HistEC->GetName());
        HTmp3->SetName(HistRE4->GetName());
        
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

        THStack *hs = new THStack(nameS, title);
        
         
       //gROOT->ForceStyle();   

       hs->Add(HTmp1,"sames");
       hs->Add(HTmp2,"sames");
       hs->Add(HTmp3,"sames");
        
       gStyle->SetOptStat(1111);
       TCanvas *canvas = new TCanvas(nameCanvas," ", 1024, 768); 
        
       hs->Draw("hist nostack");
       hs->GetYaxis()->SetTitle("Number of Rolls");  
       hs->GetXaxis()->SetTitle(title);  
       //canvas->Modified();
//     //the following lines will force the stats for h[1] and h[2]
//     //to be drawn at a different position to avoid overlaps
       canvas->Update(); //to for the generation of the 'stat" boxes
       canvas->cd();
       TLatex *dex = new TLatex(0.1, 0.92,"Data May 2016");
       dex->SetNDC();
       dex->SetTextAngle(0);	//Tex->SetLineWidth(2);
       dex->SetTextFont(92);
       dex->Draw(); 
       TPaveStats *st1 =  (TPaveStats*)HTmp1->GetListOfFunctions()->FindObject("stats");
       TPaveStats *st2 =  (TPaveStats*)HTmp2->GetListOfFunctions()->FindObject("stats");
       TPaveStats *st3 =  (TPaveStats*)HTmp3->GetListOfFunctions()->FindObject("stats");
	st3->SetX1NDC(0.74);
	st3->SetX2NDC(0.88);
	st3->SetY1NDC(0.45);
	st3->SetY2NDC(0.58);
	st2->SetX1NDC(0.74);
	st2->SetX2NDC(0.88);
	st2->SetY1NDC(0.60);
	st2->SetY2NDC(0.73);
        st1->SetX1NDC(0.74);
        st1->SetX2NDC(0.88);
        st1->SetY1NDC(0.75);
        st1->SetY2NDC(0.88);
 
  	
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
        canvas->cd();
        canvas->Update();
        
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
