import multiprocessing as mp
import subprocess
import os
import sys

def run_g4(cmd):
    """
    main function here takes in all of the macro files, then looks for a built
    G4_QPIX, if found it should run this program to begin making the output G4
    files
    """
    path = "./build/app"
    if not os.path.isdir(path):
        print("did not find build directory!!")
    found = False
    f = None
    for root, dirs, files in os.walk(path):
        if "G4_QPIX" in files:
            found = True
            break;
    if found:
        prog = path+"/"+"G4_QPIX"
        subprocess.run([prog, cmd])

def run_hadd(combFiles):
    """
    main function here looks for the output ROOT files, finds the important
    core counts, seed numbers, and isotope types.

    It should call hadd -a iteratively continually appending to the base file
    from all of the other output files to condense everything into a single
    file.
    """
    assert len(combFiles) > 1, "combine files need more than one file to combine"
    base_file = combFiles[0]
    move_files = combFiles[1:]
    print(f"move {len(move_files)} files into {base_file}")
    for mf in move_files:
        proc = subprocess.run(["hadd", "-a", base_file, mf])
        if proc.returncode != 0:
            print(f"WARNING hadd error failed at {mf} into {base_file}")
            return
        else:
            proc = subprocess.run(["rm", mf])

def main():
    """
    Look for the macro files in ./macros/long_macros and create the output
    files for each of them.
    """
    # if not os.path.isdir("./macros/long_macros/"):
    #     print("did not find macro directory!!")
    #     return -1
    # mac_files = os.listdir("./macros/long_macros/")
    # mac_files = ["./macros/long_macros/"+f for f in mac_files]
    # # print(mac_files)
    # pool = mp.Pool(50)
    # r = pool.map_async(run_g4, mac_files)
    # r.wait()
    # print("Geant4 Files created.")

    path = "./test_had/"
    # if not os.path.isdir(path):
    #     print("did not find output directory!!")
    #     return -1

    files = [os.path.join(path, f) for f in os.listdir(path) if ".root" in f]
    isotopes = set([f.split("_")[0] for f in os.listdir(path) if ".root" in f])
    cores = set([f.split("_")[1] for f in os.listdir(path) if ".root" in f])
    seeds = set([int(f.split("_")[2][:-5])  for f in os.listdir(path) if ".root" in f])
    print("found isotopes: ", isotopes)
    print("found cores: ", cores)
    print("found seeds: ", seeds)

    # combFiles = []
    # for iso in isotopes:
    #     for core in cores:
    #         combFiles.append([f for f in files if iso in f and core in f])

    # pool = mp.Pool(len(combFiles))
    # r = pool.map_async(run_hadd, combFiles)
    # r.wait()
    # print("files combined into isotopes.")

    # files after the hadd
    files = [os.path.join(path, f) for f in os.listdir(path) if ".root" in f]
    # sorting ensures that Ar39, the largest root file is the base
    files.sort()

    # now combine everything in terms of core
    combFiles = []
    for core in cores:
        combFiles.append([f for f in files if core in f])

    pool = mp.Pool(len(combFiles))
    r = pool.map_async(run_hadd, combFiles)
    r.wait()
    print("files combined into cores. Ready for RTD!")

if __name__ == "__main__":
    if len(sys.argv) != 1:
        print("need only a single arg")
        sys.exit(-2)
    main()
