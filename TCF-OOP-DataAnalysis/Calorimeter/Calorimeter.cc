//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
// * This code from the TestEm7 example of Geant4 software has been   *
// * modified for the integration in the software [PROJECTNAME] for   *
// * the exam project of the course "Tecniche Informatiche per la     *
// * Fisica" - University of Turin - 2022 - V.Pagliarino, F. Calisto  *
// * L. Cane.                                                         *
// ********************************************************************
//
/// \file electromagnetic/Calorimeter/Calorimeter.cc
/// \brief Main program of the electromagnetic/Calorimeter example
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"

#include "RunAction.hh"
#include "TrackingAction.hh"
#include "SteppingAction.hh"
#include "SteppingVerbose.hh"

#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "G4UIterminal.hh"
#include "G4GraphicsSystemList.hh"
#include "G4OpenGL.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
int main(int argc,char** argv) {

  //detect interactive mode (if no arguments) and define UI session
  G4UIExecutive* ui = nullptr;
  if (argc == 1) ui = new G4UIExecutive(argc,argv);

  //choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  
  //my Verbose output class
  G4VSteppingVerbose::SetInstance(new SteppingVerbose);
    
  //Construct the default run manager
  G4RunManager * runManager = new G4RunManager;

  //set mandatory initialization classes
  //
  DetectorConstruction*   det  = new DetectorConstruction();
  PhysicsList*            phys = new PhysicsList();
  
  runManager->SetUserInitialization(det);
  runManager->SetUserInitialization(phys);
  
  //set user action classes
  //
  PrimaryGeneratorAction* kin   = new PrimaryGeneratorAction(det);  
  RunAction*              run   = new RunAction(det,phys,kin);
  TrackingAction*         track = new TrackingAction(det,run);
  SteppingAction*         step  = new SteppingAction(det,run);
  
  runManager->SetUserAction(kin); 
  runManager->SetUserAction(run); 
  runManager->SetUserAction(track);  
  runManager->SetUserAction(step);

  //initialize visualization
  G4VisManager* visManager = nullptr;

  //get the pointer to the User Interface manager 
  G4UImanager* UImanager = G4UImanager::GetUIpointer();  

  if (ui)  {
   //interactive mode
   visManager = new G4VisExecutive;
   visManager->Initialize();
   ui->SessionStart();
   delete ui;
  }
  else  {
   //batch mode
   visManager = new G4VisExecutive;
   visManager->Initialize();
   G4String command = "/control/execute ";
   G4String fileName = argv[1];
   UImanager->ApplyCommand(command+fileName);
   //UImanager->ExecuteMacroFile(fileName.c_str());
  }

  //job termination 
  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
