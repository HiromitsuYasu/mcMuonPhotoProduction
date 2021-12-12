#ifndef SensitiveDetector_h
#define SensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"

#include "TTree.h"
#include "TFile.h"
#include <vector>
class G4Step;

class SensitiveDetector : public G4VSensitiveDetector
{
    public:
        SensitiveDetector(G4String);
        ~SensitiveDetector();

        void Initialize(G4HCofThisEvent*);
        G4bool ProcessHits(G4Step*, G4TouchableHistory*);
        void EndOfEvent(G4HCofThisEvent*);

        void setOutFileName(G4String outFileName_arg);
        G4String getOutFileName();

        void saveTTreeAsRootFile();

    private:
        G4String outFileName;
        TTree* tree;
        TFile* tfile;
        std::vector<G4int> trackID;
        std::vector<G4double> globalTime;
        std::vector<G4String> particleName;
        std::vector<G4double> prePosX;
        std::vector<G4double> prePosY;
        std::vector<G4double> prePosZ;
        std::vector<G4double> postPosX;
        std::vector<G4double> postPosY;
        std::vector<G4double> postPosZ;
        G4int i_event;

};

#endif 