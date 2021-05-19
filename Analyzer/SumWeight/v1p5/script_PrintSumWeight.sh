#!/bin/bash

root -l -b -q ROOTFile_MakeHist_SumWeight_DYMuMu_M10to50.root Print_SumWeight.cxx
root -l -b -q ROOTFile_MakeHist_SumWeight_DYMuMu_M50toInf.root Print_SumWeight.cxx
root -l -b -q ROOTFile_MakeHist_SumWeight_DYTauTau_M10to50.root Print_SumWeight.cxx
root -l -b -q ROOTFile_MakeHist_SumWeight_DYTauTau_M50toInf.root Print_SumWeight.cxx
root -l -b -q ROOTFile_MakeHist_SumWeight_WJets.root Print_SumWeight.cxx
root -l -b -q ROOTFile_MakeHist_SumWeight_ttbar.root Print_SumWeight.cxx

