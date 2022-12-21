// -----------------------------------------------------------------------------
//  MARLEYManager.cpp
//
//
//   * Author: Everybody is an author!
//   * Creation date: 7 August 2020
// -----------------------------------------------------------------------------

// Class includes
#include "MARLEYManager.h"

// Qpix includes
#include "ConfigManager.h"

// MARLEY includes
#include "marley/JSONConfig.hh"

MARLEYManager * MARLEYManager::instance_ = 0;

//-----------------------------------------------------------------------------
MARLEYManager::MARLEYManager()
  : marley_json_(ConfigManager::GetMarleyJson())
{
}

//-----------------------------------------------------------------------------
MARLEYManager::~MARLEYManager()
{
}

//-----------------------------------------------------------------------------
MARLEYManager * MARLEYManager::Instance()
{
    if (instance_ == 0) instance_ = new MARLEYManager();
    return instance_;
}

//-----------------------------------------------------------------------------
marley::Generator & MARLEYManager::Generator()
{
    return marley_generator_;
}

//-----------------------------------------------------------------------------
void MARLEYManager::Initialize()
{
    if (!marley_json_.empty())
    {
        std::cout << "Configuring MARLEY..." << std::endl;
        marley::JSONConfig marley_config(marley_json_);
        marley_generator_ = marley_config.create_generator();
    }
    else
    {
        std::cout << "MARLEY configuration file not found!  Continuing..."
                  << std::endl;
    }
}

