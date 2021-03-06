Bool_t ConfigKstarLeading(AliRsnMiniAnalysisTask *task, Bool_t isMC = kFALSE, Bool_t isPP = kFALSE, Double_t nSigmaKaon = -1)
{

    // -- Values ------------------------------------------------------------------------------------
    /* invariant mass   */ Int_t imID = task->CreateValue(AliRsnMiniValue::kInvMass, kFALSE);
    /* transv. momentum */ Int_t ptID = task->CreateValue(AliRsnMiniValue::kPt, kFALSE);
    /* angel to leading */ Int_t alID = task->CreateValue(AliRsnMiniValue::kAngleLeading, kFALSE);
    /* pt of leading    */ Int_t ptlID = task->CreateValue(AliRsnMiniValue::kLeadingPt, kFALSE);
    /* multiplicity     */ Int_t multID = task->CreateValue(AliRsnMiniValue::kMult,kFALSE);

     
   // integrated pion cut
   AliRsnCutDaughterKStar2010PP *cutPi = new AliRsnCutDaughterKStar2010PP("cutPionForKStar", AliPID::kPion);
   // cut set
   AliRsnCutSet *cutSetPi = new AliRsnCutSet("setPionForKStar", AliRsnTarget::kDaughter);
   cutSetPi->AddCut(cutPi);
   cutSetPi->SetCutScheme(cutPi->GetName());
   // add to task
   Int_t iCutPi = task->AddTrackCuts(cutSetPi);
   
   // integrated kaon cut
   AliRsnCutDaughterKStar2010PP *cutK = new AliRsnCutDaughterKStar2010PP("cutKaonForKStar", AliPID::kKaon);
   // cut set
   AliRsnCutSet *cutSetK = new AliRsnCutSet("setKaonForKStar", AliRsnTarget::kDaughter);
   cutSetK->AddCut(cutK);
   cutSetK->SetCutScheme(cutK->GetName());
   // add to task
   Int_t iCutK = task->AddTrackCuts(cutSetK);

    // Defining output objects
    const Int_t dims = 8;
    Int_t useIM[dims] = {          1,           1,          1,          1,               1,          1,           isMC,          isMC};
    TString name[dims] = {        "UnlikePM", "UnlikeMP", "MixingPM",   "MixingMP",   "LikePP",   "LikeMM",    "True",       "Mother"};
    TString comp[dims] = {        "PAIR",     "PAIR",      "MIX",      "MIX",         "PAIR",     "PAIR",      "TRUE",       "MOTHER"};
    TString output[dims] = {      "SPARSE",   "SPARSE",   "SPARSE",   "SPARSE",       "SPARSE",   "SPARSE",    "SPARSE",     "SPARSE"};
    Char_t charge1[dims] = {          '+',       '-',        '+',       '-',            '+',         '-',         '+',          '+'};
    Char_t charge2[dims] = {          '-',       '+',        '-',       '+',            '+',         '-',         '-',          '-'};
    Int_t pdgCode[dims] = {           313,        313,         313,     313,             313,         313,        313,          313};
    Double_t motherMass[dims] = {   0.896,      0.896,       0.896,     0.896,          0.896,       0.896,      0.896,        0.896};

    for (Int_t i = 0; i < dims; i++)
    {
        if (!useIM[i])
            continue;
        AliRsnMiniOutput *out = task->CreateOutput(name[i].Data(), output[i].Data(), comp[i].Data());
        out->SetCutID(0, iCutK);
        out->SetCutID(1, iCutPi);
        out->SetDaughter(0, AliRsnDaughter::kKaon);
        out->SetDaughter(1, AliRsnDaughter::kPion);
        out->SetCharge(0, charge1[i]);
        out->SetCharge(1, charge2[i]);
        out->SetMotherPDG(pdgCode[i]);
        out->SetMotherMass(motherMass[i]);

        out->AddAxis(imID, 215, 0.6, 1.3);
        out->AddAxis(ptID, 40, 0., 20.);
        if(!isPP ) out->AddAxis(multID,100,0.,100.);
        else out->AddAxis(multID, 20, 0., 200.); 

        out->AddAxis(alID, 72, -0.5 * TMath::Pi(), 1.5 * TMath::Pi()); 
        out->AddAxis(ptlID, 40, 0., 20.); 
        
    }
    return kTRUE;
}
