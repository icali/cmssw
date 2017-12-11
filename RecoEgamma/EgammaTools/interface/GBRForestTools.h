//--------------------------------------------------------------------------------------------------
//
// GRBForestTools
//
// Utility to read a TMVA weights file with a BDT into a GRBForest.
//
// Author: Jonas Rembser
//--------------------------------------------------------------------------------------------------


#ifndef EgammaTools_GBRForestTools_h
#define EgammaTools_GBRForestTools_h

#include <vector>
#include <string>

#include "CondFormats/EgammaObjects/interface/GBRForest.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "TMVA/MethodBDT.h"
#include "TMVA/Reader.h"

class GBRForestTools
{
  public:
    GBRForestTools() {}

    static std::unique_ptr<const GBRForest> createGBRForest(const std::string &weightFile);
    static std::unique_ptr<const GBRForest> createGBRForest(const edm::FileInPath &weightFile);

};

#endif
