import multiprocessing as mp
import subprocess
import os
import sys

def find_prog(path, prog):
    """
    helper function to find programs with a given path
    """
    f = None
    if not os.path.isdir(path):
        print("did not find prog directory!!")
        return f
    for root, dirs, files in os.walk(path):
        if prog in files:
            f = os.path.join(root, prog)
            break;
    return f

def run_g4(cmd):
    """
    main function here takes in all of the macro files, then looks for a built
    G4_QPIX, if found it should run this program to begin making the output G4
    files
    """
    prog = find_prog("./build/app", "G4_QPIX")
    if found is not None:
        subprocess.run([prog, cmd])
    else:
        return -1 

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

def run_sort(input_file, core, dest_file):
    """
    once the files are combined, we need to perform the following:
    sort combined hadd ttree of all of the hits
    move the sort_event_tree into a new tfile
    remove the old, unsorted tree
    """

    # find and run sort
    prog = find_prog("./build/sorter", "sorter")
    if found is not None:
        subprocess.run([prog, cmd, input_file])
    else:
        return -1 

    # move
    path = "./output/sorted/"
    if not os.path.isdir(path):
        print("did not find output sorted directory!!")
        return
    prog = "rootmv"
    subprocess.run([prog, input_file+":sort_event_tree", dest_file+":event_tree"])
    subprocess.run([prog, input_file+":metadata", dest_file])

    # remove
    print("file moved.. deteling..")
    prog = "rm"
    subprocess.run([prog, input_file])

def createGeantData(time, cores):
    """
    Wrapper function to make geant data from macros
    """

    decay_script =  "LongDecay.sh"
    if not os.path.isfile(decay_script):
        print("WARNING unable to find decay script!")
        return -1 
    subprocess.run(["bash", decay_script, time, cores])

    if not os.path.isdir("./macros/long_macros/"):
        print("did not find macro directory!!")
        return -1

    mac_files = os.listdir("./macros/long_macros/")
    mac_files = ["./macros/long_macros/"+f for f in mac_files]
    # print(mac_files)
    pool = mp.Pool(50)
    r = pool.map_async(run_g4, mac_files)
    r.wait()
    print("Geant4 Files created.")
    return 1

def hadd_files():
    """
    Wrapper function to hadd files after geant4 creation.

    final output of these files should be a single, unsorted ROOT file per
    core in the ./output/ directory
    """
    path = "./output/"

    if not os.path.isdir(path):
        print("did not find output directory!!")
        return -1

    files = [os.path.join(path, f) for f in os.listdir(path) if ".root" in f and "_" in f]
    isotopes = set([f.split("_")[0] for f in files if ".root" in f])
    cores = set([f.split("_")[1] for f in files if ".root" in f])

    combFiles = []
    for iso in isotopes:
        for core in cores:
            combFiles.append([f for f in files if iso in f and core in f])

    pool = mp.Pool(len(combFiles))
    r = pool.map_async(run_hadd, combFiles)
    r.wait()
    print("files combined into isotopes.")

    # files after the hadd
    files = [os.path.join(path, f) for f in os.listdir(path) if ".root" in f and "_" in f]
    files.sort() # Ar39 is base

    # now combine everything in terms of core
    coreFiles = []
    for core in cores:
        coreFiles.append([f for f in files if core in f])

    pool = mp.Pool(len(coreFiles))
    r = pool.map_async(run_hadd, coreFiles)
    r.wait()
    print("files combined into cores. Ready for RTD!")

    return 1

def createSortData(time):
    """
    Wrapper function to run sort on set of files after hadd
    """

    # now combine everything in terms of core
    path="./output/"
    if not os.path.isdir(path+"sorted"):
        print("could not find sorted directory")
        return -1

    files = [os.path.join(path, f) for f in os.listdir(path) if ".root" in f and "_" in f]
    cores = set([f.split("_")[1] for f in files if ".root" in f])
    output_cores, coreFiles, dest_files = [], [], []
    for core in cores:
        coreFiles.append([f for f in files if core in f])
        output_cores.append(core)
        dest_files.append(path+"/sorted/"+f"{core}_{time}_sorted.root")

    ## sort and move cores to sorted output directory
    sortFiles = [c[0] for c in coreFiles]
    print("sorting files:", sortFiles, output_cores)
    pool = mp.Pool(len(sortFiles))
    r = pool.starmap_async(run_sort, zip(sortFiles, output_cores, dest_files))
    r.wait()

    return dest_files

def run_rtd(sorted_file, rtd_file):
    """
    Finally, look for the built EXAMPLE rtd program, 
    give it the sorted hit file and the output rtd_file to build
    """
    prog = find_prog("../qpixrtd/build/", "EXAMPLE")
    if prog is not None:
        subprocess.run([prog, sorted_file, rtd_file])
    else:
        return -1 

def createRTD(sorted_files):
    """
    finally, run the RTD code on each sorted file

    EXAMPLE-RTD program should create a new tree of the resets
    
    but change the postfix to _rtd.root.
    """
    path = "./output/rtd/"
    if not os.path.isdir(path):
        print("unable to find output RTD path!")
        return -1

    rtd_files = []
    for f in sorted_files:
        # take only the base name from this path
        outf = f.split("/")[-1] 
        assert ".root" == outf[-5:], "rtd code should read sorted .root files"
        rtd_files.append(path+outf[:-5] + "_rtd.root")
    pool = mp.Pool(len(sorted_files))
    r = pool.starmap_async(run_rtd, zip(sorted_files, rtd_files))
    r.wait()


def main(time, cores):
    """
    Look for the macro files in ./macros/long_macros and create the output
    files for each of them.

    at the conclusion of this run, there should only be base hit files in
    output/sorted and rtd files in output/rtd.

    The output directory should itself contain only there directories.
    """

    val = createGeantData(time, cores)
    if val < 0:
        return val

    val = hadd_files()
    if val < 0:
        return val

    sorted_files = createSortData(time)

    createRTD(sorted_files)
    print("qpix rtd creation complete..\n")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("required to have time and number of cores supplied.")
        sys.exit(-2)
    time = int(sys.argv[1])
    core = int(sys.argv[2])
    assert core <= 10, "can't have more than 10 cores without exceeding RAM"
    assert time <= 5000, "can't store more than 5k seconds of data.."
    # ensure that time is a multiple of 10s
    assert time%10==0, "must run in time multiples of 10s"
    main(time, core)
