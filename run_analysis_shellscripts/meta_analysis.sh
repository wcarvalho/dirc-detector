

#!/bin/bash

source shell_functions.sh
source setup_resultsfolders.sh

if [ "$runphotoncomp" = true ] ; then	
	source make_photoncomparison_graphs.sh 
fi

if [ "$runsig" = true ] ; then	
	source make_deltasigma_pt_graphs.sh
fi

if [ "$runEff" = true ] ; then
	source make_efficiency_graphs.sh
fi

## FIXME: plays sound to notify me of when executables are finished running. deletion or replacement suggested. can also be found in meta-analysis.sh
source ~/finish_sound.sh