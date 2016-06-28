// Trainer of PFCluster energy correction MVAs with TMVA.

#include <TFile.h>
#include <TTree.h>
#include <TMVA/Factory.h>
#include <TCut.h>

//Use the opt as Caterina!

void train_TMVA_v2(const char* infile, const char* outfile, int Target_type = 0, int Training_type = 0, int jet_type = 0)
{
  //Training type: 0 = 20var, 1=17var, 21,22,23=18var, 3=16var(ttbar)
  // Steering function.

  // TMVA output
  //TFile fo("output/tmva_RunI.root", "RECREATE");
  TFile fo(outfile, "RECREATE");

  TMVA::Factory factory("", &fo, "!Silent:!DrawProgressBar");

  // FIXME: find out how to train on 0, 2, 4, ... events
  //factory.PrepareTrainingAndTestTree(TCut(""), "nTrain_Regression=0:nTest_Regression=100");
  //factory.PrepareTrainingAndTestTree(TCut(""), "SplitMode=Alternate");
  factory.PrepareTrainingAndTestTree(TCut(""), "SplitMode=Random");

  // apply pre-filtering
  TCut cuts("Jet_genjetPt > 20");
  cuts += "Jet_eta < 2.4 && Jet_eta > -2.4";
  if(jet_type == 1)cuts += "Jet_type == 0"; // for leading jet
  if(jet_type == 2)cuts += "Jet_type == 1"; // for trailing jet
  factory.AddCut(cuts, "Regression");

  // set input
  TFile fi(infile);
  TTree* tree = (TTree*) fi.Get("jet");
  factory.AddRegressionTree(tree);

  // add variables for training
  factory.AddVariable("Jet_pt", 'F');
  factory.AddVariable("Jet_corr", 'F');
  if(Training_type!=3) factory.AddVariable("rho", 'F');
  factory.AddVariable("Jet_eta", 'F');
  factory.AddVariable("Jet_mt", 'F');
  factory.AddVariable("Jet_leadTrackPt", 'F');
  factory.AddVariable("Jet_leptonPtRel", 'F');
  factory.AddVariable("Jet_leptonPt", 'F');
  factory.AddVariable("Jet_leptonDeltaR", 'F');
  if(Training_type!=3) factory.AddVariable("Jet_neHEF", 'F');
  if(Training_type==3) factory.AddVariable("Jet_totHEF", 'F');
  factory.AddVariable("Jet_neEmEF", 'F');
  if(Training_type!=3) factory.AddVariable("Jet_chMult", 'F');
  factory.AddVariable("Jet_vtxPt", 'F');
  factory.AddVariable("Jet_vtxMass", 'F');
  factory.AddVariable("Jet_vtx3dL", 'F');
  factory.AddVariable("Jet_vtxNtrk", 'F');
  factory.AddVariable("Jet_vtx3deL", 'F');
  if(Training_type==3) factory.AddVariable("nPVs", 'F');
  if(Training_type==0){
	factory.AddVariable("Jet_PFMET", 'F');
	factory.AddVariable("Jet_METDPhi", 'F');
	factory.AddVariable("Jet_JetDR", 'F');
  }
  if(Training_type==21){
	factory.AddVariable("Jet_PFMET", 'F');
  }
  if(Training_type==22){
	factory.AddVariable("Jet_METDPhi", 'F');
  }
  if(Training_type==23){
	factory.AddVariable("Jet_JetDR", 'F');
  }
  // target variable
  if(Target_type == 0) factory.AddTarget("Jet_TruePt/Jet_pt"); //b quark
  if(Target_type == 1) factory.AddTarget("Jet_genjetPt/Jet_pt"); //gen jet

  // BDTG,  Boosted Decision Trees with gradient boosting
  TString opts = "NTrees=200:MaxDepth=3:nCuts=20:BoostType=Grad:UseBaggedGrad:GradBaggingFraction=0.5";
  opts += ":Shrinkage=0.1";
  opts += ":PruneStrength=30:PruneMethod=CostComplexity";
  //opts += ":NegWeightTreatment=IgnoreNegWeightsInTraining";
  //New option
  opts += ":V=False:H=False";
  opts += ":UseNvars=4";
  factory.BookMethod(TMVA::Types::kBDT, "BDTG", opts);

  factory.TrainAllMethods();
  factory.TestAllMethods();
  factory.EvaluateAllMethods();
}
