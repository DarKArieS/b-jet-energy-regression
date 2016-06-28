#!/bin/bash

# stop on first error
set -e

mkdir -p output
mkdir -p TestTree

echo "Training b jet TMVA... 6/24"
echo "Selection: |eta|<2.4 , jet_genJetPt>20"
echo "add neutrinos"

infile=/home/cyeh/bjetregression/minitree_bJetRegression_v3_nu.root

#================================================================================================
echo "Target: jet_GenjetPt/Jet_Pt"

#================================================================================================
#			js run
#================================================================================================
# 20 variable training with jetGenJet
echo "Training TMVA with 20 var, leading jet"
output=TestTree/tmva_20_jetGenJet_nu_leading_6_24.root
echo "
    .x train_TMVA.C+(\"${infile}\",\"${output}\",1,0,1)
    .q" | root -b -l > output/train_tmva.log
mv weights/_BDTG.weights.xml weights/BDTG_20_jetGenJet_nu_leading_6_24.weights.xml

echo "Training TMVA with 20 var, trailing jet"
output=TestTree/tmva_20_jetGenJet_nu_trailing_6_24.root
echo "
    .x train_TMVA.C+(\"${infile}\",\"${output}\",1,0,2)
    .q" | root -b -l > output/train_tmva.log
mv weights/_BDTG.weights.xml weights/BDTG_20_jetGenJet_nu_trailing_6_24.weights.xml


# 20 variable training with jetGenJet v2 (setting as Caterina)
echo "Training TMVA with 20 var, leading jet"
output=TestTree/tmva_20_jetGenJet_nu_leading_6_24_v2.root
echo "
    .x train_TMVA_v2.C+(\"${infile}\",\"${output}\",1,0,1)
    .q" | root -b -l > output/train_tmva.log
mv weights/_BDTG.weights.xml weights/BDTG_20_jetGenJet_nu_leading_6_24_v2.weights.xml

echo "Training TMVA with 20 var, trailing jet"
output=TestTree/tmva_20_jetGenJet_nu_trailing_6_24_v2.root
echo "
    .x train_TMVA_v2.C+(\"${infile}\",\"${output}\",1,0,2)
    .q" | root -b -l > output/train_tmva.log
mv weights/_BDTG.weights.xml weights/BDTG_20_jetGenJet_nu_trailing_6_24_v2.weights.xml

#================================================================================================
#			low mass run
#================================================================================================
infile_low=/home/cyeh/bjetregression/minitree_bJetRegression_v3_low_nu.root

output=TestTree/tmva_16_jetGenJet_nu_lowmass_6_24.root
echo "Training TMVA with 20 var low mass"
echo "
    .x train_TMVA.C+(\"${infile_low}\",\"${output}\",1,3,0)
    .q" | root -b -l > output/train_tmva.log
mv weights/_BDTG.weights.xml weights/BDTG_16_jetGenJet_nu_lowmass_6_24.weights.xml

#================================================================================================
#			16 var run
#================================================================================================

output=TestTree/tmva_16_jetGenJet_nu_6_24.root
echo "Training TMVA with 16 var"
echo "
    .x train_TMVA.C+(\"${infile}\",\"${output}\",1,3,0)
    .q" | root -b -l > output/train_tmva.log
mv weights/_BDTG.weights.xml weights/BDTG_16_jetGenJet_nu_6_24.weights.xml


echo "Finish!"