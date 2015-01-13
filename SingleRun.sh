nevents=1
# nevents=1000
# nevents=2000
ptype="Electrons"
# ptype="Pions"
Case=1				# 1: lookuptable, 2: riemann sum
# nparticles=3

smear=0.01
t=$nevents$ptype

runGen=true
runSim=true
runSmear=false
runPixelate=false
runRec=false
runphotoncomp=false
runEff=false

print=true
quiet=true

g="matchfalse_graphs"
f="fits"
scripts="scripts"
b=build

S=simulator
G=generator
R=reconstructor
H=histmaker
F=fitter

appendage=".root"
smearsettingsfile="smearsettings.txt"
gen_out="generation.root"
cheat_out="cheat.root"
sim="simulation"
sim_out=$sim$appendage
rec_out="reconstruction.root"

# make master directory
if [ ! -d $t ]; then 
	mkdir $t
fi

if [ "$runGen" = true ] ; then
	echo "generator"
	gen=$t/$gen_out
	if [ -e $gen ]; then	
		rm $gen
	fi
	if [ "$print" = true ] ; then
		generator -e $nevents -f $t/$gen_out -c parameters.txt -v
	elif [ "$quiet" = true ] ; then		
		generator -e $nevents -f $t/$gen_out -c parameters.txt -q
	else
		generator -e $nevents -f $t/$gen_out -c parameters.txt
	fi
fi

# directory with files
s="for_smear_"$smear
## make directories
# for files for this smear
if [ ! -d $t/$s ]; then	
	mkdir $t/$s
fi

# for efficiency and false positive rate plots
if [ ! -d $t/$s/$Case/$g ]; then	
	mkdir $t/$s/$Case/$g
fi
# for fits to histogram
if [ ! -d $t/$s/$Case/$f ]; then	
	mkdir $t/$s/$Case/$f
fi

if [ "$runSim" = true ] ; then
	echo "simulator"
	SIM=$t/$sim_out
	if [ -e $SIM ]; then	
		rm $SIM
	fi
	if [ "$print" = true ] ; then
		simulator -i $t/$gen_out -W $sim_out -S $smear --file-write-off -v
	elif [ "$quiet" = true ] ; then		
		simulator -i $t/$gen_out -W $sim_out -S $smear --file-write-off -q
	else
		simulator -i $t/$gen_out -W $sim_out -S $smear --file-write-off
	fi
fi

sim_out_smeared=$s/$sim"_"$smear$appendage
if [ "$runSmear" = true ] ; then
	echo "smear"
	rm $smearsettingsfile
	touch $smearsettingsfile
	echo $t/$sim_out >> $smearsettingsfile
	echo $t/$sim_out_smeared >> $smearsettingsfile
	echo $smear >> $smearsettingsfile
	./$scripts/smear
fi

## if photons have been smeared, set sim file to smeared file
if [ -e $t/$sim_out_smeared ]; then	
	sim_out=$sim_out_smeared
fi

sim_out_pix=$s/$sim"_"$smear"pix"$appendage
if [ "$runPixelate" = true ] ; then
	echo "pixelate"
	./$scripts/"pixelate" -S$t/$sim_out_smeared -o$t/$sim_out_pix -T$smear
fi

## if photons have been pixelated, set sim file to pixelated file
if [ -e $t/$sim_out_pix ]; then	
	sim_out=$sim_out_pix
fi


if [ ! -d $t/$s/$Case ]; then
	mkdir $t/$s/$Case
fi

recon=$t/$s/$Case/"reconstruction.root"
if [ "$runRec" = true ] ; then
	## FIXME: when efficiency header accepts variable for fitresults change this to variable
	if [ -e $recon ]; then	
		rm $recon
	fi
	echo "reconstructor"
	if [ "$print" = true ] ; then
		reconstructor -i $t/$sim_out -e$Case -D $t/$s/$Case/ -v
	elif [ "$quiet" = true ] ; then		
		reconstructor -i $t/$sim_out -e$Case -D $t/$s/$Case/ -q
	else
		reconstructor -i $t/$sim_out -e$Case -D $t/$s/$Case/
	fi
fi

if [ "$runphotoncomp" = true ] ; then
	echo "photon comparison"
	graphdir=$t/$s/$Case/"photon_comp_graphs/"
	if [ ! -d $graphdir ]; then 
		mkdir $graphdir
	fi	
	./scripts/photoncomp -S$t/$cheat_out -R$recon -g$graphdir
fi

if [ "$runEff" = true ] ; then
	echo "efficiency script"
	rm directories
	touch directories
	echo $t"/" >> directories 
	echo $s/$Case"/" >> directories
	echo $g"/" >> directories
	echo $f"/" >> directories
	./$scripts/eff

	# open "$t"/"$s"
fi
