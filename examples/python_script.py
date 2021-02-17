import ROOT
​
if __name__ == "__main__":
​
    # Read in the root file
   inFileName = "/n/home04/sruppert/Q_PIX_GEANT4/gxe/output/electon.root"
   inFile = ROOT.TFile.Open(inFileName, "READ")
