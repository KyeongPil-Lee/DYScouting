#include <Test/SingleMuon/dataMCRatio_RunDep/OfflineZPeakHistContainer.h>
#include <NLOReweight/NLOWeightTool.h>

class DataHistContainerPerRun
{
public:
  DataHistContainerPerRun() { Setup(); }

  void Fill(DYTool::DYTree* ntuple, DYTool::OffMuPair DYMuPair, Double_t totWeight )
  {
    Int_t runIndex = FindRunIndex(ntuple->runNum);
    if( runIndex != -1 ) vec_histContainer_[runIndex]->Fill( DYMuPair, totWeight );
    else                 hist_unknownRun_->Fill( DYMuPair, totWeight );
  }

  void Save(TFile *f_output)
  {
    for(auto histContainer : vec_histContainer_)
      histContainer->Write( f_output );

    hist_unknownRun_->Write( f_output );
  }

private:
  OfflineZPeakHistContainer* hist_unknownRun_;
  vector<OfflineZPeakHistContainer*> vec_histContainer_;
  vector<Int_t> vec_run_     = {315257, 315259, 315264, 315265, 315267, 315270, 315322, 315339, 315357, 315361, 315363, 315366, 315420, 315488, 315489, 315490, 315506, 315510, 315512, 315543, 315555, 315556, 315557, 315640, 315641, 315642, 315644, 315645, 315646, 315647, 315648, 315689, 315690, 315702, 315703, 315704, 315705, 315713, 315721, 315741, 315764, 315770, 315784, 315785, 315786, 315790, 315800, 315801, 315840, 315973, 315974, 316058, 316059, 316060, 316061, 316062, 316082, 316110, 316111, 316113, 316114, 316153, 316186, 316187, 316199, 316200, 316201, 316202, 316216, 316217, 316218, 316219, 316239, 316240, 316241, 316271, 316361, 316362, 316363, 316377, 316378, 316379, 316380, 316455, 316457, 316469, 316470, 316472, 316505, 316569, 316590, 316613, 316615, 316666, 316667, 316700, 316701, 316702, 316715, 316716, 316717, 316718, 316719, 316720, 316721, 316722, 316723, 316758, 316766, 316876, 316877, 316879, 316928, 316985, 316993, 316994, 316995, 317080, 317087, 317089, 317182, 317212, 317213, 317279, 317291, 317292, 317297, 317319, 317320, 317338, 317339, 317340, 317382, 317383, 317391, 317392, 317435, 317438, 317475, 317478, 317484, 317488, 317527, 317591, 317626, 317640, 317641, 317648, 317649, 317650, 317661, 317663, 317683, 317696, 318733, 318828, 318872, 318874, 318876, 318877, 319077, 319337, 319347, 319348, 319349, 319449, 319450, 319456, 319459, 319486, 319503, 319524, 319526, 319528, 319579, 319625, 319639, 319656, 319657, 319658, 319659, 319678, 319687, 319697, 319698, 319756, 319840, 319841, 319847, 319848, 319849, 319851, 319853, 319854, 319908, 319909, 319910, 319912, 319913, 319914, 319915, 319941, 319942, 319950, 319991, 319992, 319993, 320002, 320006, 320010, 320011, 320012, 320023, 320024, 320025, 320026, 320038, 320039, 320040, 320059, 320060, 320061, 320062, 320063, 320064, 320065, 320673, 320674, 320688, 320712, 320757, 320804, 320807, 320809, 320821, 320822, 320823, 320824, 320838, 320840, 320841, 320853, 320854, 320855, 320856, 320857, 320858, 320859, 320887, 320888, 320916, 320917, 320920, 320933, 320934, 320936, 320941, 320980, 320995, 320996, 321004, 321005, 321006, 321007, 321009, 321010, 321011, 321012, 321051, 321055, 321067, 321068, 321069, 321119, 321121, 321122, 321124, 321126, 321134, 321138, 321140, 321149, 321165, 321166, 321167, 321177, 321178, 321218, 321219, 321221, 321230, 321231, 321232, 321233, 321262, 321283, 321294, 321295, 321296, 321305, 321311, 321312, 321313, 321393, 321396, 321397, 321414, 321415, 321431, 321432, 321433, 321434, 321436, 321457, 321461, 321475, 321710, 321712, 321730, 321732, 321735, 321755, 321758, 321760, 321773, 321774, 321775, 321776, 321777, 321778, 321780, 321781, 321813, 321815, 321817, 321818, 321820, 321831, 321832, 321833, 321834, 321879, 321880, 321887, 321908, 321909, 321917, 321919, 321933, 321960, 321961, 321973, 321975, 321988, 321990, 322013, 322014, 322022, 322040, 322057, 322068, 322079, 322106, 322113, 322118, 322179, 322201, 322204, 322222, 322252, 322317, 322319, 322322, 322324, 322332, 322348, 322355, 322356, 322381, 322407, 322430, 322431, 322480, 322492, 322510, 322599, 322602, 322603, 322605, 322617, 322625, 322633, 323414, 323423, 323470, 323471, 323472, 323473, 323474, 323475, 323487, 323488, 323492, 323493, 323495, 323524, 323525, 323526, 323693, 323696, 323702, 323725, 323726, 323727, 323755, 323775, 323778, 323790, 323794, 323841, 323857, 323940, 323954, 323976, 323978, 323980, 323983, 323997, 324021, 324022, 324077, 324201, 324202, 324205, 324206, 324207, 324209, 324237, 324245, 324293, 324315, 324318, 324420, 324729, 324747, 324764, 324765, 324769, 324772, 324785, 324791, 324835, 324840, 324841, 324846, 324878, 324897, 324970, 324980, 324997, 324998, 324999, 325000, 325001, 325022, 325057, 325097, 325098, 325099, 325100, 325101, 325110, 325117, 325159, 325168, 325169, 325170, 325172};
  // -- /fb
  vector<Double_t> vec_lumi_ = {0.007929497, 0.013918864, 0.039703388, 0.006454507, 0.037143311, 0.078981117, 0.180062920, 0.093349061, 0.151930725, 0.118531849, 0.014489096, 0.100182760, 0.258566396, 0.190563067, 0.103438191, 0.002520994, 0.027983158, 0.103172328, 0.236871400, 0.036886267, 0.021968881, 0.008395188, 0.084134393, 0.010512485, 0.001237309, 0.031431097, 0.063159684, 0.161090551, 0.191964439, 0.008018192, 0.015226137, 0.329958838, 0.119001932, 0.022853106, 0.158162019, 0.014739015, 0.132814780, 0.353734703, 0.210579169, 0.023717251, 0.114011154, 0.121796189, 0.051799834, 0.106003338, 0.021848143, 0.173123579, 0.206215401, 0.098534603, 0.354111468, 0.278586191, 0.018532205, 0.137197208, 0.170426968, 0.198479234, 0.006146917, 0.000698283, 0.141359586, 0.081969483, 0.015161958, 0.020491266, 0.367994967, 0.256240431, 0.015153677, 0.512428751, 0.401961320, 0.002226180, 0.110093155, 0.075106131, 0.171479086, 0.088158861, 0.243232708, 0.048978441, 0.209319054, 0.283682524, 0.053194396, 0.029698080, 0.026413888, 0.053916088, 0.004611485, 0.001986070, 0.003336673, 0.008222157, 0.175280166, 0.009607083, 0.326273293, 0.148877856, 0.116869505, 0.066237151, 0.401220030, 0.485569309, 0.189247294, 0.031466211, 0.049127397, 0.279381290, 0.042140099, 0.114304211, 0.143933326, 0.095461886, 0.003000536, 0.061792771, 0.063385896, 0.090222571, 0.032449482, 0.041732114, 0.003260078, 0.134760529, 0.009621132, 0.455682721, 0.561052774, 0.216379022, 0.111051479, 0.036513199, 0.044417240, 0.172546110, 0.074473713, 0.004777100, 0.186194514, 0.007844260, 0.243200389, 0.212904926, 0.406207245, 0.051172376, 0.125489076, 0.065891594, 0.274028870, 0.085006647, 0.123798905, 0.053514561, 0.460767275, 0.013631072, 0.065749760, 0.140596348, 0.024139434, 0.022451239, 0.002717287, 0.490656629, 0.360612921, 0.049709578, 0.025761150, 0.007937242, 0.160355997, 0.181128073, 0.452099029, 0.111116517, 0.528939540, 0.261179265, 0.268315019, 0.033881916, 0.193211576, 0.255996426, 0.375651112, 0.153301355, 0.117851782, 0.228786857, 0.002792197, 0.013229261, 0.091854514, 0.099565217, 0.046587025, 0.160548836, 0.016371875, 0.232388818, 0.210856031, 0.005006475, 0.040843828, 0.249173726, 0.207045612, 0.052015625, 0.017212101, 0.020314797, 0.090959188, 0.456701499, 0.069659575, 0.058410906, 0.818490479, 0.070596043, 0.409340341, 0.086471826, 0.051824096, 0.061716393, 0.021787667, 0.095424824, 0.016052913, 0.060561287, 0.036295265, 0.490204543, 0.049718970, 0.020280330, 0.000903537, 0.019081513, 0.173552091, 0.001137038, 0.071203488, 0.053716327, 0.018295828, 0.001120242, 0.304865167, 0.013437223, 0.012566235, 0.006879860, 0.086802323, 0.094417188, 0.017468906, 0.065935971, 0.282111415, 0.065586502, 0.182675815, 0.050203468, 0.114140140, 0.074676692, 0.061138036, 0.018060690, 0.098834607, 0.126440979, 0.030113234, 0.050085703, 0.218712390, 0.008200272, 0.177428533, 0.040908654, 0.015725006, 0.018270053, 0.006956471, 0.022166104, 0.064866603, 0.227086186, 0.064592146, 0.033749177, 0.130855199, 0.069579800, 0.116683595, 0.319033307, 0.000348408, 0.106230704, 0.060432928, 0.149814403, 0.080025310, 0.164020053, 0.092366066, 0.141043050, 0.050468155, 0.118065454, 0.039644924, 0.146559259, 0.033490847, 0.052859822, 0.037521088, 0.004926672, 0.104361547, 0.009500190, 0.006635005, 0.477801747, 0.024013428, 0.051965819, 0.210460582, 0.078320032, 0.015638078, 0.038407528, 0.025796208, 0.048288004, 0.046906478, 0.019195813, 0.051704227, 0.202763102, 0.014732928, 0.057206632, 0.028948830, 0.004974448, 0.311577321, 0.125495500, 0.172706969, 0.147136741, 0.049557775, 0.058668389, 0.015831462, 0.120346156, 0.185307787, 0.078957429, 0.008164269, 0.141976762, 0.116949370, 0.434674653, 0.000389898, 0.003380938, 0.274126989, 0.151674214, 0.023034517, 0.282500585, 0.181189493, 0.005906206, 0.022670378, 0.014523657, 0.006057920, 0.231852526, 0.001669120, 0.119780879, 0.020610486, 0.201490287, 0.011876596, 0.601904551, 0.000473243, 0.060265688, 0.026586081, 0.049689179, 0.365018162, 0.052262513, 0.380801771, 0.142213696, 0.055907925, 0.016577963, 0.042731538, 0.180882406, 0.138441929, 0.482761896, 0.021013094, 0.471912521, 0.017235399, 0.084378140, 0.102670881, 0.344691952, 0.023661290, 0.213437556, 0.037517590, 0.146153421, 0.021058783, 0.043723139, 0.004807240, 0.010304681, 0.058616866, 0.036058295, 0.113674716, 0.068009102, 0.120243809, 0.005120206, 0.148473508, 0.138015405, 0.032068958, 0.248042890, 0.115961844, 0.074190773, 0.049809506, 0.090170659, 0.041375820, 0.276941304, 0.145314737, 0.340569377, 0.261219957, 0.001488823, 0.101007248, 0.007304486, 0.132126192, 0.358666936, 0.145783784, 0.384021716, 0.081031948, 0.001146111, 0.006669868, 0.452968644, 0.013692424, 0.008235478, 0.232599447, 0.140244760, 0.281859052, 0.040789720, 0.268228934, 0.469749616, 0.084335391, 0.311166308, 0.092744223, 0.420357700, 0.019245910, 0.055502234, 0.292464047, 0.069259748, 0.313995681, 0.414112209, 0.033876145, 0.234583361, 0.178729681, 0.169958460, 0.156140903, 0.214230370, 0.109337400, 0.287515085, 0.002228890, 0.094350581, 0.024109024, 0.002693892, 0.089623887, 0.136645564, 0.335273291, 0.050945617, 0.006204142, 0.021037074, 0.080200218, 0.075495647, 0.019315198, 0.054519627, 0.077807771, 0.014194080, 0.154166173, 0.186197456, 0.005823197, 0.027375069, 0.032346526, 0.172934401, 0.242648282, 0.064950697, 0.040083352, 0.088847729, 0.145958433, 0.118533090, 0.014606471, 0.246903637, 0.296639246, 0.046767743, 0.270935867, 0.276737455, 0.016056603, 0.155038205, 0.095513180, 0.426706208, 0.015140930, 0.014398816, 0.021951778, 0.067731897, 0.056791397, 0.128791789, 0.252485124, 0.127785561, 0.242582801, 0.362857357, 0.039241165, 0.022963460, 0.017522834, 0.004054579, 0.016078704, 0.074991780, 0.417199145, 0.513035432, 0.070436667, 0.099840257, 0.199727029, 0.014796593, 0.308246151, 0.030065379, 0.087679541, 0.046770044, 0.018782623, 0.185797535, 0.245007227, 0.085856011, 0.027721689, 0.301008029, 0.068552956, 0.403016934, 0.050228783, 0.479465581, 0.513294960, 0.038586760, 0.114235368, 0.003883479, 0.094663582, 0.119138042, 0.433385704, 0.120048335, 0.017770097, 0.002038523, 0.139170738, 0.074467205, 0.118270296, 0.003259474, 0.085216684, 0.061439969, 0.004288854, 0.005577620, 0.263142487, 0.067635724};

  void Setup()
  {
    Int_t nRun = (Int_t)vec_run_.size();
    for(Int_t i_run=0; i_run<nRun; i_run++)
    {
      TString str_run = TString::Format("%d", vec_run_[i_run]);
      OfflineZPeakHistContainer* hist_temp = new OfflineZPeakHistContainer(str_run);
      vec_histContainer_.push_back(hist_temp);
    }

    hist_unknownRun_ = new OfflineZPeakHistContainer("unknownRun"); // -- for the events not included in vec_run
  }

  Int_t FindRunIndex(Int_t runNum)
  {
    Int_t theIndex = -1;
    for(UInt_t i_run=0; i_run<vec_run_.size(); i_run++)
    {
      if( runNum == vec_run_[i_run] )
      {
        theIndex = i_run;
        break;
      }
    }

    if( theIndex == -1 )
      cout << "[FindRunIndex] The run = " << runNum << " is not found in vec_run!! ... need to check" << endl;

    return theIndex;
  }

};


class HistProducer: public DYTool::ClassTemplate
{
public:
  HistProducer(): ClassTemplate()
  {

  }

  void Run()
  {
    CheckSampleInfo();
    StartTimer();

    TChain *chain = new TChain("DYTree/ntuple");
    DYTool::AddNtupleToChain(chain, sampleInfo_.ntuplePathFile);

    DYTool::DYTree *ntuple = new DYTool::DYTree( chain );
    ntuple->TurnOnBranches_OffMuon();

    Int_t nEvent = chain->GetEntries();
    cout << "\t[Total Events: " << nEvent << "]" << endl;

    DYTool::PUReweightTool* PUTool = new DYTool::PUReweightTool("2018");
    NLOWeightTool* nloWeightTool = new NLOWeightTool();

    // -- histogram
    DataHistContainerPerRun* hist_perRun_IsoMu24Z = new DataHistContainerPerRun();
    DataHistContainerPerRun* hist_perRun_L1       = new DataHistContainerPerRun();
    DataHistContainerPerRun* hist_perRun_HLT      = new DataHistContainerPerRun();

    for(Int_t i=0; i<nEvent; i++)
    {
      DYTool::loadBar(i+1, nEvent, 100, 100);
      
      ntuple->GetEvent(i);

      Double_t genWeight;
      ntuple->genWeight < 0 ? genWeight = -1 : genWeight = 1;

      // Double_t totWeight = sampleInfo_.normFactor * genWeight;
      Double_t totWeight = genWeight;
      if( sampleInfo_.isMC ) totWeight *= PUTool->Weight( ntuple->truePU );

      // -- only DY->mumu or DY->ee events according to its name -- //
      if( DYTool::SelectGenEventBySampleType(sampleInfo_.type, ntuple) )
      {
        // -- DY low mass sample: apply NLO/LO k-factor
        if( sampleInfo_.type.Contains("DYMuMu_M10to50"))
          totWeight *= nloWeightTool->GetWeight(ntuple);

        // -- test: does it pass IsoMu24+Dimuon selection+Z mass window?
        Bool_t isDYEvent_IsoMu24Z = kFALSE;
        DYTool::OffMuPair DYMuPair = OfflineEventSelection_IsoMu24_ZMassWindow(ntuple, isDYEvent_IsoMu24Z);

        if( isDYEvent_IsoMu24Z )
        {
          hist_perRun_IsoMu24Z->Fill( ntuple, DYMuPair, totWeight );
          if( IsFired_L1(ntuple) )
          {
            hist_perRun_L1->Fill( ntuple, DYMuPair, totWeight );

            if( IsFired_DoubleMu3(ntuple) )
              hist_perRun_HLT->Fill(ntuple, DYMuPair, totWeight);
          }

        } // -- end of if( isDYEvent_IsoMu24Z )
      } // -- SelectGenEventBySampleType      
    } // -- end of event iteration

    TString outputName = GetOutputFileName("DataDimuonMass_PerRun");
    TFile *f_output = TFile::Open(outputName, "RECREATE");
    f_output->cd();
    f_output->mkdir("IsoMu24Z");
    f_output->cd("IsoMu24Z");
    hist_perRun_IsoMu24Z->Save(f_output);

    f_output->cd();
    f_output->mkdir("L1");
    f_output->cd("L1");
    hist_perRun_L1->Save(f_output);

    f_output->cd();
    f_output->mkdir("HLT");
    f_output->cd("HLT");
    hist_perRun_HLT->Save(f_output);

    f_output->cd();
    f_output->Close();

    PrintRunTime();
  }

private:
  Bool_t IsFired_L1(DYTool::DYTree* ntuple)
  {
    Bool_t doPass_L1 = kFALSE;
    if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPass_L1 = kTRUE; // -- no L1 w/ mass cut

    return doPass_L1;
  }

  Bool_t IsFired_DoubleMu3(DYTool::DYTree* ntuple)
  {
    Bool_t doPass_HLT = kFALSE;
    for(const auto& firedTrigger : *(ntuple->vec_firedTrigger) )
    {
      TString tstr_firedTrig = firedTrigger;
      if( tstr_firedTrig.Contains("DST_DoubleMu3_noVtx_CaloScouting_v") ) doPass_HLT = kTRUE;

      if( doPass_HLT ) break;
    }

    return doPass_HLT;
  }

  DYTool::OffMuPair OfflineEventSelection_IsoMu24_ZMassWindow(DYTool::DYTree* ntuple, Bool_t& doPass)
  {
    DYTool::OffMuPair muPair_dummy;

    DYTool::OffMuPair DYMuPair = OfflineEventSelection_IsoMu24(ntuple, doPass);
    Double_t diMuM = DYMuPair.mass;

    if( 81 < diMuM && diMuM < 101 )
    {
      doPass = kTRUE;
      return DYMuPair;
    }
    else
    {
      doPass = kFALSE;
      return muPair_dummy;
    }

    return muPair_dummy;
  }

  DYTool::OffMuPair OfflineEventSelection_IsoMu24(DYTool::DYTree* ntuple, Bool_t& doPass)
  {
    doPass = kFALSE;

    DYTool::OffMuPair muPair_dummy;

    // -- pass unprescaled L1 (only for 2018)
    // -- 4 = L1_DoubleMu_15_7
    // -- 11 = L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18
    // -- 16 = L1_DoubleMu4p5_SQ_OS_dR_Max1p2
    // Bool_t doPassL1 = kFALSE;
    // if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(11) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE;
    // if( ntuple->vec_L1Bit->at(4) || ntuple->vec_L1Bit->at(16) ) doPassL1 = kTRUE; // -- no L1 w/ mass cut

    // if( !doPassL1 ) return muPair_dummy;

    // -- pass HLT
    // Bool_t doPassTrig1 = kFALSE;
    // Bool_t doPassTrig2 = kFALSE;
    // for(const auto& firedTrigger : *(ntuple->vec_firedTrigger) )
    // {
    //   TString tstr_firedTrig = firedTrigger;
    //   if( tstr_firedTrig.Contains("HLT_IsoMu24_v") )                      doPassTrig1 = kTRUE;
    //   if( tstr_firedTrig.Contains("DST_DoubleMu3_noVtx_CaloScouting_v") ) doPassTrig2 = kTRUE;

    //   if( doPassTrig1 && doPassTrig2 ) break;
    // }

    // if( !(doPassTrig1 && doPassTrig2) ) return muPair_dummy;

    Bool_t doPassTrig = kFALSE;
    for(const auto& firedTrigger : *(ntuple->vec_firedTrigger) )
    {
      TString tstr_firedTrig = firedTrigger;
      if( tstr_firedTrig.Contains("HLT_IsoMu24_v") ) doPassTrig = kTRUE;

      if( doPassTrig ) break;
    }

    if( !doPassTrig ) return muPair_dummy;

    // -- dimuon selection
    vector< DYTool::OffMuPair > vec_muPair = DYTool::GetAllOffMuPairs(ntuple);

    vector< DYTool::OffMuPair > vec_goodMuPair;
    for( auto& muPair : vec_muPair )
      if( IsDYCandidate_BDTInput_noOS_Offline(muPair) ) vec_goodMuPair.push_back( muPair );

    Int_t nGoodPair = (Int_t)vec_goodMuPair.size();
    if( nGoodPair == 0 )
    {
      doPass = kFALSE;
      return muPair_dummy;
    }
    else if( nGoodPair == 1) // -- exactly one pair: no sort is needed
    {
      doPass = kTRUE;
      return vec_goodMuPair[0];
    }
    else // -- multiple pair: take the pair with the smallest chi2
    {
      doPass = kTRUE;
      std::sort(vec_goodMuPair.begin(), vec_goodMuPair.end(), DYTool::CompareOffMuonPair_SmallerVtxChi2);
      return vec_goodMuPair[0]; // -- pair with smallest vertex chi2 value
    }
  }

  Bool_t IsDYCandidate_BDTInput_noOS_Offline(DYTool::OffMuPair& muPair)
  {
    Bool_t flag = kFALSE;

    Bool_t isWithinAcc = kFALSE;
    if( muPair.first_.pt > 26  && fabs(muPair.first_.eta) < 2.4 &&
        muPair.second_.pt > 10 && fabs(muPair.second_.eta) < 2.4 ) 
      isWithinAcc = kTRUE;

    Bool_t isGoodMuon = kFALSE;
    if( muPair.first_.nPixelHit_inner > 0  && muPair.first_.nTrackerLayer_inner > 5  && muPair.first_.normChi2_global < 10  && muPair.first_.relTrkIso < 0.15 &&
        muPair.second_.nPixelHit_inner > 0 && muPair.second_.nTrackerLayer_inner > 5 && muPair.second_.normChi2_global < 10 && muPair.second_.relTrkIso < 0.15 )
      isGoodMuon = kTRUE;

    if( isWithinAcc && isGoodMuon && muPair.mass > 10.0 ) flag = kTRUE;

    return flag;
  }


};
