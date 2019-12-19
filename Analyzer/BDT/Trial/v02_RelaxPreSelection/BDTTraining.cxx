// -- reference: https://github.com/root-project/root/blob/master/tutorials/tmva/TMVAClassification.C

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

int TMVAClassification( TString myMethodList = "" )
{
  //---------------------------------------------------------------
  // This loads the library
  TMVA::Tools::Instance();

  // Default MVA methods to be trained + tested
  // only run BDT
  std::map<std::string,int> Use;

  // Cut optimisation
  Use["Cuts"]            = 0;
  Use["CutsD"]           = 0;
  Use["CutsPCA"]         = 0;
  Use["CutsGA"]          = 0;
  Use["CutsSA"]          = 0;
  //
  // 1-dimensional likelihood ("naive Bayes estimator")
  Use["Likelihood"]      = 0;
  Use["LikelihoodD"]     = 0; // the "D" extension indicates decorrelated input variables (see option strings)
  Use["LikelihoodPCA"]   = 0; // the "PCA" extension indicates PCA-transformed input variables (see option strings)
  Use["LikelihoodKDE"]   = 0;
  Use["LikelihoodMIX"]   = 0;
  //
  // Mutidimensional likelihood and Nearest-Neighbour methods
  Use["PDERS"]           = 0;
  Use["PDERSD"]          = 0;
  Use["PDERSPCA"]        = 0;
  Use["PDEFoam"]         = 0;
  Use["PDEFoamBoost"]    = 0; // uses generalised MVA method boosting
  Use["KNN"]             = 0; // k-nearest neighbour method
  //
  // Linear Discriminant Analysis
  Use["LD"]              = 0; // Linear Discriminant identical to Fisher
  Use["Fisher"]          = 0;
  Use["FisherG"]         = 0;
  Use["BoostedFisher"]   = 0; // uses generalised MVA method boosting
  Use["HMatrix"]         = 0;
  //
  // Function Discriminant analysis
  Use["FDA_GA"]          = 0; // minimisation of user-defined function using Genetics Algorithm
  Use["FDA_SA"]          = 0;
  Use["FDA_MC"]          = 0;
  Use["FDA_MT"]          = 0;
  Use["FDA_GAMT"]        = 0;
  Use["FDA_MCMT"]        = 0;
  //
  // Neural Networks (all are feed-forward Multilayer Perceptrons)
  Use["MLP"]             = 0; // Recommended ANN
  Use["MLPBFGS"]         = 0; // Recommended ANN with optional training method
  Use["MLPBNN"]          = 0; // Recommended ANN with BFGS training method and bayesian regulator
  Use["CFMlpANN"]        = 0; // Depreciated ANN from ALEPH
  Use["TMlpANN"]         = 0; // ROOT's own ANN
#ifdef R__HAS_TMVAGPU
  Use["DNN_GPU"]         = 1; // CUDA-accelerated DNN training.
#else
  Use["DNN_GPU"]         = 0;
#endif

#ifdef R__HAS_TMVACPU
  Use["DNN_CPU"]         = 1; // Multi-core accelerated DNN.
#else
  Use["DNN_CPU"]         = 0;
#endif
  //
  // Support Vector Machine
  Use["SVM"]             = 0;
  //
  // Boosted Decision Trees
  Use["BDT"]             = 0; // uses Adaptive Boost
  Use["BDTG"]            = 1; // uses Gradient Boost
  Use["BDTB"]            = 0; // uses Bagging
  Use["BDTD"]            = 0; // decorrelation + Adaptive Boost
  Use["BDTF"]            = 0; // allow usage of fisher discriminant for node splitting
  //
  // Friedman's RuleFit method, ie, an optimised series of cuts ("rules")
  Use["RuleFit"]         = 0;
  // ---------------------------------------------------------------

  std::cout << std::endl;
  std::cout << "==> Start TMVAClassification" << std::endl;

  // Select methods (don't look at this code - not of interest)
  if (myMethodList != "") {
    for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

    std::vector<TString> mlist = TMVA::gTools().SplitString( myMethodList, ',' );
    for (UInt_t i=0; i<mlist.size(); i++) {
      std::string regMethod(mlist[i]);

      if (Use.find(regMethod) == Use.end()) {
        std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
        for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
        std::cout << std::endl;
        return 1;
      }
      Use[regMethod] = 1;
    }
  }

  // --------------------------------------------------------------------------------------------------

  // Here the preparation phase begins

  // Read training and test data
  TString analyzerPath = gSystem->Getenv("DY_ANALYZER_PATH");
  TFile *f_input_signal = TFile::Open(analyzerPath+"/BDT/Trial/v02_RelaxPreSelection/TreeMaker/ROOTFile_BDTInputTree_DYJetsToLL_M10to50_Madgraph.root");
  TFile *f_input_bkg    = TFile::Open(analyzerPath+"/BDT/Trial/v02_RelaxPreSelection/TreeMaker/ROOTFile_BDTInputTreeProducer_QCDMuEnriched.root");

  TTree *signalTree     = (TTree*)f_input_signal->Get("BDTInput");
  TTree *background     = (TTree*)f_input_bkg->Get("BDTInput");

  // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
  TString outfileName( "TMVA.root" );
  TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

  // Create the factory object. Later you can choose the methods
  // whose performance you'd like to investigate. The factory is
  // the only TMVA object you have to interact with
  //
  // The first argument is the base of the name of all the
  // weightfiles in the directory weight/
  //
  // The second argument is the output file for the training results
  // All TMVA output can be suppressed by removing the "!" (not) in
  // front of the "Silent" argument in the option string
  TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
                                "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );

  TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");
  // If you wish to modify default settings
  // (please check "src/Config.h" to see all available global options)
  //
  //    (TMVA::gConfig().GetVariablePlotting()).fTimesRMS = 8.0;
  //    (TMVA::gConfig().GetIONames()).fWeightFileDir = "myWeightDirectory";

  // Define the input variables that shall be used for the MVA training
  // note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
  // [all types of expressions that can also be parsed by TTree::Draw( "expression" )]
  dataloader->AddVariable( "diMuNormVtxChi2", "Normalized dimuon vertex chi2", "", 'F' );
  dataloader->AddVariable( "diMuDEta", "eta difference between two muons", "", 'F' );
  dataloader->AddVariable( "diMuDPhi", "phi difference between two muons", "", 'F' );
  dataloader->AddVariable( "diMuOS",   "Opposite sign", "", 'I' );

  dataloader->AddVariable( "caloMET_pt", "Calo MET", "GeV", 'F' );

  dataloader->AddVariable( "nMuonHit_lead",        "# muon hits (leading muon)", "", 'I' );
  dataloader->AddVariable( "nMatchedStation_lead", "# matched station (leading muon)", "", 'I' );
  dataloader->AddVariable( "nPixelHit_lead",       "# pixel hits (leading muon)", "", 'I' );
  dataloader->AddVariable( "nTrackerLayer_lead",   "# tracker layers (leading muon)", "", 'I' );
  dataloader->AddVariable( "normChi2_lead",        "Normalized chi2 (leading muon)", "", 'F' );
  dataloader->AddVariable( "relTrkIso_lead",       "Relative tracker isolation (leading muon)", "", 'F' );

  dataloader->AddVariable( "nMuonHit_sub",        "# muon hits (sub-leading muon)", "", 'I' );
  dataloader->AddVariable( "nMatchedStation_sub", "# matched station (sub-leading muon)", "", 'I' );
  dataloader->AddVariable( "nPixelHit_sub",       "# pixel hits (sub-leading muon)", "", 'I' );
  dataloader->AddVariable( "nTrackerLayer_sub",   "# tracker layers (sub-leading muon)", "", 'I' );
  dataloader->AddVariable( "normChi2_sub",        "Normalized chi2 (sub-leading muon)", "", 'F' );
  dataloader->AddVariable( "relTrkIso_sub",       "Relative tracker isolation (sub-leading muon)", "", 'F' );
  

  // You can add so-called "Spectator variables", which are not used in the MVA training,
  // but will appear in the final "TestTree" produced by TMVA. This TestTree will contain the
  // input variables, the response values of all trained MVAs, and the spectator variables

  dataloader->AddSpectator( "diMuM",   "Dimuon mass",     "GeV", 'F' );
  dataloader->AddSpectator( "diMuRap", "Dimuon rapidity", "",    'F' );
  dataloader->AddSpectator( "diMuPt",  "Dimuon pT",       "GeV", 'F' );

  // global event weights per tree (see below for setting event-wise weights)
  Double_t signalWeight     = 1.0;
  Double_t backgroundWeight = 1.0;

  // You can add an arbitrary number of signal or background trees
  dataloader->AddSignalTree    ( signalTree,     signalWeight );
  dataloader->AddBackgroundTree( background, backgroundWeight );

  // Set individual event weights (the variables must exist in the original TTree)
  // -  for signal    : `dataloader->SetSignalWeightExpression    ("weight1*weight2");`
  // -  for background: `dataloader->SetBackgroundWeightExpression("weight1*weight2");`
  // dataloader->SetBackgroundWeightExpression( "weight" );

  // Apply additional cuts on the signal and background samples (can be different)
  TCut mycuts = ""; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";

  dataloader->PrepareTrainingAndTestTree( mycuts, "nTrain_Signal=50000:nTrain_Background=50000:nTest_Signal=50000:nTest_Background=50000:SplitMode=Random:!V" );

  // ### Book MVA methods
  //
  // Please lookup the various method configuration options in the corresponding cxx files, eg:
  // src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
  // it is possible to preset ranges in the option string in which the cut optimisation should be done:
  // "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable

  // Cut optimisation
  if (Use["Cuts"])
    factory->BookMethod( dataloader, TMVA::Types::kCuts, "Cuts",
                  "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );

  if (Use["CutsD"])
    factory->BookMethod( dataloader, TMVA::Types::kCuts, "CutsD",
                  "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=Decorrelate" );

  if (Use["CutsPCA"])
    factory->BookMethod( dataloader, TMVA::Types::kCuts, "CutsPCA",
                  "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=PCA" );

  if (Use["CutsGA"])
    factory->BookMethod( dataloader, TMVA::Types::kCuts, "CutsGA",
                  "H:!V:FitMethod=GA:CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95" );

  if (Use["CutsSA"])
    factory->BookMethod( dataloader, TMVA::Types::kCuts, "CutsSA",
                  "!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );

  // Likelihood ("naive Bayes estimator")
  if (Use["Likelihood"])
    factory->BookMethod( dataloader, TMVA::Types::kLikelihood, "Likelihood",
                  "H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" );

  // Decorrelated likelihood
  if (Use["LikelihoodD"])
    factory->BookMethod( dataloader, TMVA::Types::kLikelihood, "LikelihoodD",
                  "!H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=Decorrelate" );

  // PCA-transformed likelihood
  if (Use["LikelihoodPCA"])
    factory->BookMethod( dataloader, TMVA::Types::kLikelihood, "LikelihoodPCA",
                  "!H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=PCA" );

  // Use a kernel density estimator to approximate the PDFs
  if (Use["LikelihoodKDE"])
    factory->BookMethod( dataloader, TMVA::Types::kLikelihood, "LikelihoodKDE",
                  "!H:!V:!TransformOutput:PDFInterpol=KDE:KDEtype=Gauss:KDEiter=Adaptive:KDEFineFactor=0.3:KDEborder=None:NAvEvtPerBin=50" );

  // Use a variable-dependent mix of splines and kernel density estimator
  if (Use["LikelihoodMIX"])
    factory->BookMethod( dataloader, TMVA::Types::kLikelihood, "LikelihoodMIX",
                  "!H:!V:!TransformOutput:PDFInterpolSig[0]=KDE:PDFInterpolBkg[0]=KDE:PDFInterpolSig[1]=KDE:PDFInterpolBkg[1]=KDE:PDFInterpolSig[2]=Spline2:PDFInterpolBkg[2]=Spline2:PDFInterpolSig[3]=Spline2:PDFInterpolBkg[3]=Spline2:KDEtype=Gauss:KDEiter=Nonadaptive:KDEborder=None:NAvEvtPerBin=50" );

  // Test the multi-dimensional probability density estimator
  // here are the options strings for the MinMax and RMS methods, respectively:
  //
  //      "!H:!V:VolumeRangeMode=MinMax:DeltaFrac=0.2:KernelEstimator=Gauss:GaussSigma=0.3" );
  //      "!H:!V:VolumeRangeMode=RMS:DeltaFrac=3:KernelEstimator=Gauss:GaussSigma=0.3" );
  if (Use["PDERS"])
    factory->BookMethod( dataloader, TMVA::Types::kPDERS, "PDERS",
                  "!H:!V:NormTree=T:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600" );

  if (Use["PDERSD"])
    factory->BookMethod( dataloader, TMVA::Types::kPDERS, "PDERSD",
                  "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=Decorrelate" );

  if (Use["PDERSPCA"])
    factory->BookMethod( dataloader, TMVA::Types::kPDERS, "PDERSPCA",
                  "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=PCA" );

  // Multi-dimensional likelihood estimator using self-adapting phase-space binning
  if (Use["PDEFoam"])
    factory->BookMethod( dataloader, TMVA::Types::kPDEFoam, "PDEFoam",
                  "!H:!V:SigBgSeparate=F:TailCut=0.001:VolFrac=0.0666:nActiveCells=500:nSampl=2000:nBin=5:Nmin=100:Kernel=None:Compress=T" );

  if (Use["PDEFoamBoost"])
    factory->BookMethod( dataloader, TMVA::Types::kPDEFoam, "PDEFoamBoost",
                  "!H:!V:Boost_Num=30:Boost_Transform=linear:SigBgSeparate=F:MaxDepth=4:UseYesNoCell=T:DTLogic=MisClassificationError:FillFoamWithOrigWeights=F:TailCut=0:nActiveCells=500:nBin=20:Nmin=400:Kernel=None:Compress=T" );

  // K-Nearest Neighbour classifier (KNN)
  if (Use["KNN"])
    factory->BookMethod( dataloader, TMVA::Types::kKNN, "KNN",
                  "H:nkNN=20:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:UseWeight=T:!Trim" );

  // H-Matrix (chi2-squared) method
  if (Use["HMatrix"])
    factory->BookMethod( dataloader, TMVA::Types::kHMatrix, "HMatrix", "!H:!V:VarTransform=None" );

  // Linear discriminant (same as Fisher discriminant)
  if (Use["LD"])
    factory->BookMethod( dataloader, TMVA::Types::kLD, "LD", "H:!V:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

  // Fisher discriminant (same as LD)
  if (Use["Fisher"])
    factory->BookMethod( dataloader, TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

  // Fisher with Gauss-transformed input variables
  if (Use["FisherG"])
    factory->BookMethod( dataloader, TMVA::Types::kFisher, "FisherG", "H:!V:VarTransform=Gauss" );

  // Composite classifier: ensemble (tree) of boosted Fisher classifiers
  if (Use["BoostedFisher"])
    factory->BookMethod( dataloader, TMVA::Types::kFisher, "BoostedFisher",
                  "H:!V:Boost_Num=20:Boost_Transform=log:Boost_Type=AdaBoost:Boost_AdaBoostBeta=0.2:!Boost_DetailedMonitoring" );

  // Function discrimination analysis (FDA) -- test of various fitters - the recommended one is Minuit (or GA or SA)
  if (Use["FDA_MC"])
    factory->BookMethod( dataloader, TMVA::Types::kFDA, "FDA_MC",
                  "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:SampleSize=100000:Sigma=0.1" );

  if (Use["FDA_GA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
    factory->BookMethod( dataloader, TMVA::Types::kFDA, "FDA_GA",
                  "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:PopSize=100:Cycles=2:Steps=5:Trim=True:SaveBestGen=1" );

  if (Use["FDA_SA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
    factory->BookMethod( dataloader, TMVA::Types::kFDA, "FDA_SA",
                  "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=SA:MaxCalls=15000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );

  if (Use["FDA_MT"])
    factory->BookMethod( dataloader, TMVA::Types::kFDA, "FDA_MT",
                  "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=2:UseImprove:UseMinos:SetBatch" );

  if (Use["FDA_GAMT"])
    factory->BookMethod( dataloader, TMVA::Types::kFDA, "FDA_GAMT",
                  "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:Cycles=1:PopSize=5:Steps=5:Trim" );

  if (Use["FDA_MCMT"])
    factory->BookMethod( dataloader, TMVA::Types::kFDA, "FDA_MCMT",
                  "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:SampleSize=20" );

  // TMVA ANN: MLP (recommended ANN) -- all ANNs in TMVA are Multilayer Perceptrons
  if (Use["MLP"])
    factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:!UseRegulator" );

  if (Use["MLPBFGS"])
    factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLPBFGS", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:!UseRegulator" );

  if (Use["MLPBNN"])
    factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLPBNN", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=60:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:UseRegulator" ); // BFGS training with bayesian regulators


  // Multi-architecture DNN implementation.
  if (Use["DNN_CPU"] or Use["DNN_GPU"]) {
    // General layout.
    TString layoutString ("Layout=TANH|128,TANH|128,TANH|128,LINEAR");

    // Training strategies.
    TString training0("LearningRate=1e-2,Momentum=0.9,Repetitions=1,"
                "ConvergenceSteps=30,BatchSize=256,TestRepetitions=10,"
                "WeightDecay=1e-4,Regularization=None,"
                "DropConfig=0.0+0.5+0.5+0.5, Multithreading=True");
    TString training1("LearningRate=1e-2,Momentum=0.9,Repetitions=1,"
                "ConvergenceSteps=20,BatchSize=256,TestRepetitions=10,"
                "WeightDecay=1e-4,Regularization=L2,"
                "DropConfig=0.0+0.0+0.0+0.0, Multithreading=True");
    TString training2("LearningRate=1e-3,Momentum=0.0,Repetitions=1,"
                "ConvergenceSteps=20,BatchSize=256,TestRepetitions=10,"
                "WeightDecay=1e-4,Regularization=L2,"
                "DropConfig=0.0+0.0+0.0+0.0, Multithreading=True");
    TString trainingStrategyString ("TrainingStrategy=");
    trainingStrategyString += training0 + "|" + training1 + "|" + training2;

    // General Options.
    TString dnnOptions ("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=N:"
                  "WeightInitialization=XAVIERUNIFORM");
    dnnOptions.Append (":"); dnnOptions.Append (layoutString);
    dnnOptions.Append (":"); dnnOptions.Append (trainingStrategyString);

    // // Cuda implementation.
    // if (Use["DNN_GPU"]) {
    //   TString gpuOptions = dnnOptions + ":Architecture=GPU";
    //   factory->BookMethod(dataloader, TMVA::Types::kDL, "DNN_GPU", gpuOptions);
    // }
    // // Multi-core CPU implementation.
    // if (Use["DNN_CPU"]) {
    //   TString cpuOptions = dnnOptions + ":Architecture=CPU";
    //   factory->BookMethod(dataloader, TMVA::Types::kDL, "DNN_CPU", cpuOptions);
    // }
  }

  // CF(Clermont-Ferrand)ANN
  if (Use["CFMlpANN"])
    factory->BookMethod( dataloader, TMVA::Types::kCFMlpANN, "CFMlpANN", "!H:!V:NCycles=200:HiddenLayers=N+1,N"  ); // n_cycles:#nodes:#nodes:...

  // Tmlp(Root)ANN
  if (Use["TMlpANN"])
    factory->BookMethod( dataloader, TMVA::Types::kTMlpANN, "TMlpANN", "!H:!V:NCycles=200:HiddenLayers=N+1,N:LearningMethod=BFGS:ValidationFraction=0.3"  ); // n_cycles:#nodes:#nodes:...

  // Support Vector Machine
  if (Use["SVM"])
    factory->BookMethod( dataloader, TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001:VarTransform=Norm" );

  // Boosted Decision Trees
  if (Use["BDTG"]) // Gradient Boost
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG",
                  "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2" );

  if (Use["BDT"])  // Adaptive Boost
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT",
                  "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );

  if (Use["BDTB"]) // Bagging
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTB",
                  "!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=20" );

  if (Use["BDTD"]) // Decorrelation + Adaptive Boost
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTD",
                  "!H:!V:NTrees=400:MinNodeSize=5%:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:VarTransform=Decorrelate" );

  if (Use["BDTF"])  // Allow Using Fisher discriminant in node splitting for (strong) linearly correlated variables
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTF",
                  "!H:!V:NTrees=50:MinNodeSize=2.5%:UseFisherCuts:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20" );

  // RuleFit -- TMVA implementation of Friedman's method
  if (Use["RuleFit"])
    factory->BookMethod( dataloader, TMVA::Types::kRuleFit, "RuleFit",
                  "H:!V:RuleFitModule=RFTMVA:Model=ModRuleLinear:MinImp=0.001:RuleMinDist=0.001:NTrees=20:fEventsMin=0.01:fEventsMax=0.5:GDTau=-1.0:GDTauPrec=0.01:GDStep=0.01:GDNSteps=10000:GDErrScale=1.02" );

  // For an example of the category classifier usage, see: TMVAClassificationCategory
  //
  // --------------------------------------------------------------------------------------------------
  //  Now you can optimize the setting (configuration) of the MVAs using the set of training events
  // STILL EXPERIMENTAL and only implemented for BDT's !
  //
  //     factory->OptimizeAllMethods("SigEffAtBkg0.01","Scan");
  //     factory->OptimizeAllMethods("ROCIntegral","FitGA");
  //
  // --------------------------------------------------------------------------------------------------

  // Now you can tell the factory to train, test, and evaluate the MVAs
  //
  // Train MVAs using the set of training events
  factory->TrainAllMethods();

  // Evaluate all MVAs using the set of test events
  factory->TestAllMethods();

  // Evaluate and compare performance of all configured MVAs
  factory->EvaluateAllMethods();

  // --------------------------------------------------------------

  // Save the output
  outputFile->Close();

  std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
  std::cout << "==> TMVAClassification is done!" << std::endl;

  delete factory;
  delete dataloader;
  // Launch the GUI for the root macros
  if (!gROOT->IsBatch()) TMVA::TMVAGui( outfileName );

  return 0;
}

void BDTTraining()
{
  TMVAClassification();
}

// int main( int argc, char** argv )
// {
//   // Select methods (don't look at this code - not of interest)
//   TString methodList;
//   for (int i=1; i<argc; i++) {
//     TString regMethod(argv[i]);
//     if(regMethod=="-b" || regMethod=="--batch") continue;
//     if (!methodList.IsNull()) methodList += TString(",");
//     methodList += regMethod;
//   }
//   return TMVAClassification(methodList);
// }