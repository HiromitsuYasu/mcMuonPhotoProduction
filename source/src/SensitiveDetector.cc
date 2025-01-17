#include "SensitiveDetector.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4HCofThisEvent.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include "TTree.h"
#include "TFile.h"
#include <vector>
#include <string>

SensitiveDetector::SensitiveDetector(G4String name)
:G4VSensitiveDetector(name)
{   
    i_event = 0;
    outFileName = "tmp.root";
    tree = new TTree("tree", "mcMuonPhotoProduction Output");
    tree->Branch("trackID", &trackID);
    tree->Branch("parentID", &parentID);
    tree->Branch("globalTime", &globalTime);
    tree->Branch("particleName", &particleName);
    tree->Branch("charge", &charge);
    tree->Branch("eDep", &eDep);
    tree->Branch("prePosX", &prePosX);
    tree->Branch("prePosY", &prePosY);
    tree->Branch("prePosZ", &prePosZ);
    tree->Branch("postPosX", &postPosX);
    tree->Branch("postPosY", &postPosY);
    tree->Branch("postPosZ", &postPosZ);
    tree->Branch("preCopyNo", &preCopyNo);
    tree->Branch("postCopyNo", &postCopyNo);
}

SensitiveDetector::~SensitiveDetector(){}

G4String SensitiveDetector::getOutFileName(){ return outFileName; }
void SensitiveDetector::setOutFileName(G4String outFileName_arg)
{
    outFileName = outFileName_arg;
}

void SensitiveDetector::saveTTreeAsRootFile()
{
    tfile = new TFile(outFileName, "recreate");
    tree->Write();
    tfile->Close();
    delete tree;
}

void SensitiveDetector::Initialize(G4HCofThisEvent*)
{
    trackID = {};
    parentID = {};
    globalTime = {};
    particleName = {};
    charge = {};
    eDep = {};
    prePosX = {};
    prePosY = {};
    prePosZ = {};
    postPosX = {};
    postPosY = {};
    postPosZ = {};
    preCopyNo = {};
    postCopyNo = {};
}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{   
    // ignore charge = 0
    if (aStep->GetTrack()->GetDefinition()->GetPDGCharge() == 0)
        return true;

    G4Track* aTrack = aStep->GetTrack();
    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint* postStepPoint = aStep->GetPostStepPoint();
    G4ThreeVector preStepPos = preStepPoint->GetPosition();
    G4ThreeVector poststepPos = postStepPoint->GetPosition();

    trackID.push_back(aTrack->GetTrackID());
    parentID.push_back(aTrack->GetParentID());
    globalTime.push_back(aTrack->GetGlobalTime()/ns);
    particleName.push_back((std::string)aTrack->GetDynamicParticle()->GetParticleDefinition()->GetParticleName());
    charge.push_back(aTrack->GetDefinition()->GetPDGCharge()/eplus);
    eDep.push_back(aStep->GetTotalEnergyDeposit()/MeV);
    prePosX.push_back(preStepPos.x()/cm);
    prePosY.push_back(preStepPos.y()/cm);
    prePosZ.push_back(preStepPos.z()/cm);
    postPosX.push_back(poststepPos.x()/cm);
    postPosY.push_back(poststepPos.y()/cm);
    postPosZ.push_back(poststepPos.z()/cm);
    preCopyNo.push_back(preStepPoint->GetPhysicalVolume()->GetCopyNo());
    postCopyNo.push_back(postStepPoint->GetPhysicalVolume()->GetCopyNo());
    return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    tree->Fill();
    if (i_event % 1000 == 0){ G4cout << i_event << G4endl; }
    i_event++;
}