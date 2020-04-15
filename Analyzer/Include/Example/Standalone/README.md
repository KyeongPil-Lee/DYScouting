# Example analyzer to produce histograms

## Header files

### DYTree.h

```DYTree``` class: have variables from branches from input files



### Object.h

Define objects (e.g. muon) using the information from ```DYTree```



### DYTool.h

have useful functions commonly used for the analysis

* Get all objects in a given event (e.g. ```GetAllMuons```)
* Event selection
* PU reweighting tool
* Others: color code, progress bar, etc.



### TMVAClassification_BDTG.class.C

for BDT classification

* Latest one at 15 April 2020
  * Copy of ```BDT/Trial/v06_noOS/dataset/weights/TMVAClassification_BDTG.class.C```



## Main code: ```ExampleAnalyzer.cxx```

Please update ```ntuplePath```  in ```ExampleAnalyzer()``` before running

Running: ```root -l -b -q ExampleAnalyzer.cxx```

* Output: ```ROOTFile_DimuonMass.root``` with ```h_diMuM``` histogram (dimuon mass distribution)

  * Gen weights are taken into account
  * PU reweghting is applied
  * Latest event selection (BDT classification) is used
    * Working point: 90% QCD rejection with 78% DY efficiency

  

## Cross sections & sum of weights

For the normalization of the histograms to the integrated luminosity

* Cross sections & sum of weights for each dataset: [link](https://github.com/KyeongPil-Lee/DYScouting/blob/master/Analyzer/BkgEst/SSPair/ReversedMVACut/QCDEstimation/EstimateQCD.cxx#L136-L199)