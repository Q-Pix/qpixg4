import os
import subprocess as sp

def haddFiles(in_files, outf_name):
    start_i = 0
    stop_i = len(in_files)
    interval = 10000
    while start_i < stop_i:
        prog = "hadd"
        if start_i+interval-1 > stop_i:
            inp = in_files[start_i:]
        else:
            inp = in_files[start_i:start_i+interval]
        sp.run([prog, "-a", "-j", "10", outf_name, *inp])
        start_i += interval

def main():

    in_dir = "./neutrinos_rtd"
    in_files = os.listdir(in_dir)
    in_files = [os.path.abspath(os.path.join(in_dir, i)) for i in in_files]
    assert len(in_files) > 10000, "should expect more infiles than this"

    # get 8 output files, muon / amuon / electron / aelectron and FHC / RHC separated
    muon_rhc_files = [i for i in in_files if "pdg-14_" in i and "FHC-0" in i]
    amuon_rhc_files = [i for i in in_files if "pdg--14_" in i and "FHC-0" in i]
    electron_rhc_files = [i for i in in_files if "pdg-12_" in i and "FHC-0" in i]
    aelectron_rhc_files = [i for i in in_files if "pdg--12_" in i and "FHC-0" in i]
    # fhc
    muon_fhc_files = [i for i in in_files if "pdg-14_" in i and "FHC-1" in i]
    amuon_fhc_files = [i for i in in_files if "pdg--14_" in i and "FHC-1" in i]
    electron_fhc_files = [i for i in in_files if "pdg-12_" in i and "FHC-1" in i]
    aelectron_fhc_files = [i for i in in_files if "pdg--12_" in i and "FHC-1" in i]

    print("pdg-14 RHC cnt: ", len(muon_rhc_files))
    print("pdg--14 RHC cnt: ", len(amuon_rhc_files))
    print("pdg-12 RHC cnt: ", len(electron_rhc_files))
    print("pdg--12 RHC cnt: ", len(aelectron_rhc_files))
    print("pdg-14 cnt: ", len(muon_fhc_files))
    print("pdg-14 cnt: ", len(amuon_fhc_files))
    print("pdg-12 cnt: ", len(electron_fhc_files))
    print("pdg--12 cnt: ", len(aelectron_fhc_files))

    haddFiles(muon_rhc_files, "muon_nu_rhc_files.root")
    haddFiles(amuon_rhc_files, "amuon_nu_rhc_files.root") 
    haddFiles(electron_rhc_files, "electron_nu_rhc_files.root") 
    haddFiles(aelectron_rhc_files, "aelectron_nu_rhc_files.root") 
    haddFiles(muon_fhc_files, "muon_nu_fhc_files.root") 
    haddFiles(amuon_fhc_files, "amuon_nu_fhc_files.root") 
    haddFiles(electron_fhc_files, "electron_nu_fhc_files.root") 
    haddFiles(aelectron_fhc_files, "aelectron_nu_fhc_files.root") 
    print('haddin complete..')

if __name__ == "__main__":
    main()
