## v1.6 -> v1.7

* Add L3Muon_NoVtx object information (pt, eta, phi)
  * It will be saved for AOD and miniAOD samples (not RAW)



## v1.5 -> v1.6

* Add L1 muon information (L1 pt, eta, phi, charge and quality)
* Add eta and phi information of the offline muon after the propagation to the 2nd muon station
  * They will be used to properly calculate dR(offline muon, L1 muon)
* Save trigger object for AOD also
  * Previously, the trigger objects are saved only for miniAOD case



## v1.4 -> v1.5

* Add more offline muon information
  * muon's inner track  pt
  * dimuon vertex information



## v1.3 -> v1.4

* Add offline muon information
* Make it run on miniAOD samples



Workspace:

```
/afs/cern.ch/user/k/kplee/work/private/Physics/DYScouting/CMSSW_10_6_12
```



## v1.2 -> v1.3

* Add calo jet information

* NOTE

  * DYLL, WJets sample: the site where the sample is stored doesn't work on slc6 setting

    ```
    /DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v2/AODSIM
    /WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18DRPremix-102X_upgrade2018_realistic_v15-v2/AODSIM
    ```

  * Need to submit CRAB jobs under slc7 setup

    ```
    /afs/cern.ch/user/k/kplee/work/private/Physics/DYScouting/CMSSW_10_2_6/src/DYScouting
    ```

    



## v1.1 -> v1.2

* add MET information (MET pt, phi)
* rho information
* more scouting muon information
  * Vtx index
  * add nStripHit
* add more vertex information
  * Pixel vertices from HLT tracking
  * Pixel vertcies made by tracks near L3 muons





## v1.0 -> v1.1

* Initialization of genWeight: -999 -> 0 (to be easy for analysis code)
* Explicitly write the process name of triggerResults
  * Without this, vec_firedTrigger is not filled for MC
* Add L1 seed information
* GenParticle: muon -> electron, muon and tau (to select DYTauTau events)

