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
if [ $HOSTNAME == "tamsa2.snu.ac.kr" ]; then
  DY_NTUPLE_PATH="DUMMY"

elif [ $HOSTNAME == "muon" ]; then
  DY_NTUPLE_PATH="/scratch/kplee/DYScoutingTree"

# elif [ $USER == "KyeongPil_Lee" ]; then
#   DY_NTUPLE_PATH="/Users/KyeongPil_Lee/temp/DYntuple"

fi
echo "========================================"
echo "Enviornment variables"
echo "DY_ANALYZER_PATH: "$DY_ANALYZER_PATH
echo "DY_NTUPLE_PATH:   "$DY_NTUPLE_PATH
echo "Setup for DY analyzer is finished."
echo "========================================"