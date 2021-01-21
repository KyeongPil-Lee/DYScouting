#!/bin/bash

if [ $DY_ANALYZER_PATH ]; then
    echo "DY_ANALYZER_PATH is already defined: use a clean shell!"
    return 1
fi

export DY_ANALYZER_PATH=$(pwd)

# -- root setup -- #
export ROOT_INCLUDE_PATH=${DY_ANALYZER_PATH}:${ROOT_INCLUDE_PATH}
export PYTHONPATH=${DY_ANALYZER_PATH}:${PYTHONPATH}

export DY_NTUPLE_PATH=""
if [ $HOSTNAME == "tamsa2" ]; then
  DY_NTUPLE_PATH="/gv0/Users/kplee/DYScoutingTree"

elif [ $HOSTNAME == "muon" ]; then
  DY_NTUPLE_PATH="/scratch/kplee/DYScoutingTree"

elif [ $HOSTNAME == "cms.knu.ac.kr" ]; then
  DY_NTUPLE_PATH="/u/user/kplee/SE_UserHome"

# elif [ $USER == "KyeongPil_Lee" ]; then
#   DY_NTUPLE_PATH="/Users/KyeongPil_Lee/temp/DYntuple"

fi

if [ $HOSTNAME == "tamsa2" ]; then
  source /cvmfs/cms.cern.ch/cmsset_default.sh
  cd /cvmfs/cms.cern.ch/slc7_amd64_gcc700/cms/cmssw/CMSSW_10_2_6
  eval `scramv1 runtime -sh`
  cd $DY_ANALYZER_PATH
  echo "CMSSW enviornment under " $CMSSW_VERSION

elif [ $HOSTNAME == "cms.knu.ac.kr" ]; then
  source /cvmfs/cms.cern.ch/cmsset_default.sh
  cd /cvmfs/cms.cern.ch/slc7_amd64_gcc700/cms/cmssw/CMSSW_10_2_6
  eval `scramv1 runtime -sh`
  cd $DY_ANALYZER_PATH
  echo "CMSSW enviornment under " $CMSSW_VERSION

fi

echo "========================================"
echo "Enviornment variables"
echo "DY_ANALYZER_PATH: "$DY_ANALYZER_PATH
echo "DY_NTUPLE_PATH:   "$DY_NTUPLE_PATH
echo "Setup for DY analyzer is finished."
echo "========================================"