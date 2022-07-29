#HV Scan Tool

The HV Scan Tool it is a series of codes that you can run depending on the output you need. 

1. To start the analysis you need to get the input root files from: 

eos ls /store/group/dpg_rpc/comm_rpc/Run-II/data2016/HVScan_May9/ForHVscanAna

the name of files are: AnalyzeEfficiency_272818_RPCMon_p\*.root 
They are 12 in total, also you have to create a file 'hvEffective.txt' with the values of the Effective High Voltages in the taking data. The values must to be in kV  
You can get more information in the twiki page: https://twiki.cern.ch/twiki/bin/view/CMS/RPCHvscan2016
Put the input files in the data directory. 

2. Once you have the input files in the data directory go to macro directory, the next step is run the FitData.C macro. This macro needs a input parameter depending on where you want 
   to do the fit "barrel" or "endcap", for instance you can do in the prompt line $ root -l -q 'FitData.C("barrel")'. The output is the analysis of the data, this creates the results directory 
   and 1 subdirectory per chamber in the RPC with the information of the fit results of the efficiency and the cluster size in two txt Files "fitData.txt" and "fitDataCls.txt". 
   Basically the macro takes the root files, next it does two distributions, efficiency vs HV and clustersize vs HV, and then fit them using a Sigmoid function and a polynomial function 
   respectively. 

3. You can see the plots of the distributions and the fit running the pngProducer.C. This macro accept a input parameter "barrel" or "endcap" as well, you can do in the prompt line
   for example $ root -l -b -q pngProducer.C("barrel")'. The macro create two png's per chamber the efficiency and the cluster size distributions with the respectively fits. 

4. If you need to get a summary of the fit results for all chambers, you can run the MakeASummary.C macro   just do $ root -l 'MakeASummary.C("barrel", false)',   and you will get a root file in the summary directory with all relevant parameters from the fit. The second bool is the "black-list" parameter, most probably you have to define a black list with problematic rolls in the run and create a txt file "blacklist.txt" with them, if that was the case and you define a black-list with the  help of the experts, and you need a summary only with the filtered rolls(no problematic rolls) so you can put 'true'. 

NOTE:  The blacklist.txt file have to be in the data directory. 

5.The 7 main parameters. To create the 7 main plots in a special format you need the summary root files ("barrel" and "endcap") and run DrawingOUFlow.C. This macro create 7 png files from 7 TCanvas. Do root -l DrawingOUFlow.C and you get it. 

6.The Working Point (WP) per channel is computed from the set of rolls in the HV channel and it is defined as   

    if  (MAX(wp)_roll - MIN(wp)_roll)_ch < 100 V -> WP_ch = < WP_rolls >_ch 
    
    else (MAX(wp)_roll - MIN(wp)_roll)_ch > 100 V -> WP_ch = (MIN(wp) + 100V)_ch 


You can run the macro 'WPChannel.C' and compute de values of the WP per channel. To do you need two txt files named ChambersName_X.txt (X = Barrel or EndCap ) being in data directory. 

===================================================================================================================

-- This tool could be improved adding classes performance. In particular add two classes, one to define the objects "rolls" with their data members HV, Efficiency, ClusterSize, NumExtrapol(Number of extrapolations), and other one a dictionary defining the objects "chamber" with the data members 'ChannelName', 'DipId', human_name (RE+4_R2_CH13_B for example) and logic_name (RE+4_R2_CH13_B = '637600102')   

===================================================================================================================
#Other studies 
 
 1. Compare the effciency in different ages. Probably you have to look at last years and compare them with the actual year. For this there is a macro named compare_ages_granul.C. To run it you need two input summary root files barrel_summary_ageB.root and barrel_summary_ageA.root  in the summary directory. 

Do root -l -q "compare_ages_granul.C(\"eraA\",\"eraB\",\"barrel\")" and you will get maps of the detector, those maps are filled with the means of the difference (eraB-eraA)[efficiency at wp] for every different sector.
 
 2. Miniscan study. To get a properly definition of the WP probably you need the miniscan macro 'EffAtKneeDist.C' this macro draws the distribution of efficiency when the knee is changing (Knee + x[V] ), we will see 3 different distributions Barrel, EndCap, RE4. To run it  you need the summary root files for barrel and endcap and do $root -l "EffAtKneeDist.C(false)". 
 
Probably you want to see the histograms of the efficiency every 5 points if you need those plots change the bool to 'true' and you get them. 

3. History Plot. The history plot could be done using  the history.C macro, to run it you need the different summaries for all years (2011, 2012a, 2012b ,2016) and do root -l 'history.C("barrel")' or "endcap" and you get a plot of the means of the efficiency and the the wp for all years. 


# HVScan2022
