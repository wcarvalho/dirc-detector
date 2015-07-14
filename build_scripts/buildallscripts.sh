#!/usr/bin/bash

echo "\nsmearer\n"
source buildsmearer.sh
echo "\nparticleembedder\n"
source buildparticleembedder.sh


echo "\ncalibrator\n"
source buildcalibrator.sh
echo "\ngeneratecalibrationdata\n"
source buildgeneratecalibrationdata.sh


echo "\nnewefficiencyplotter\n"
source buildnewefficiencyplotter.sh


echo "\nhistogramprinter\n"
source buildhistogramprinter.sh
echo "\nreducedhistogramprinter\n"
source buildreducedhistogramprinter.sh
echo "\ncombinationprinter\n"
source buildcombinationprinter.sh


echo "\nplotfakevsefficiency\n"
source buildplotfakevsefficiency.sh


echo "\nlookupinfo\n"
source buildlookupinfo.sh
echo "\neventscanner\n"
source buildeventscanner.sh
