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

def run_g4(input_file):
    """
    main function here takes in all of the macro files, then looks for a built
    G4_QPIX, if found it should run this program to begin making the output G4
    files
    """
    prog = find_prog("./build/app", "G4_QPIX")
    if prog is not None:
        subprocess.run([prog, input_file])
    else:
        return -1 

def run_chain(input_files, dest_file):
    """
    Run chain on sorted input files to create a single output file 
    which are sorted all of the inputs
    """
    prog = find_prog("./build/chainsort", "chainsort")
    if prog is not None:
        print("chaining files: ", *input_files, dest_file)
        subprocess.run([prog, *input_files, dest_file])
        prog = "rootcp"
        subprocess.run([prog, input_files[0]+":metadata", dest_file])
        # remove all of the base sorted files to preserve SSD space
        prog = "rm"
        for f in input_files:
            subprocess.run([prog, f])
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

def run_sort(input_file, dest_file):
    """
    once the files are combined, we need to perform the following:
    sort combined hadd ttree of all of the hits
    move the sort_event_tree into a new tfile
    remove the old, unsorted tree
    """

    # find and run sort
    prog = find_prog("./build/sorter", "sorter")
    if prog is not None:
        subprocess.run([prog, input_file])
    else:
        return -1 

    # move
    prog = "rootmv"
    subprocess.run([prog, input_file+":sort_event_tree", dest_file+":event_tree"])
    prog = "rootcp"
    subprocess.run([prog, input_file+":metadata", dest_file])

    # remove
    prog = "rm"
    subprocess.run([prog, input_file])
    print("file moved.. ")

def createGeantData(time, cores, seed):
    """
    Wrapper function to make geant data from macros.
    function simply runs the script LongDecay.sh macro with arguments supplied
    above.
    """

    decay_script =  "./LongDecay.sh"
    if not os.path.isfile(decay_script):
        print("WARNING unable to find decay script!")
        return -1 
    outputPath="/mnt/nvme1/Kevin/qpix/output/"
    subprocess.run(["bash", decay_script, str(time), str(cores), str(seed), str(outputPath)])

    if not os.path.isdir("./macros/long_macros/"):
        print("did not find macro directory!!")
        return -1

    mac_files = os.listdir("./macros/long_macros/")
    mac_files = ["./macros/long_macros/"+f for f in mac_files]
    pool = mp.Pool()
    r = pool.map_async(run_g4, mac_files)
    r.wait()
    print("Geant4 Files created.")
    return 1

def hadd_files():
    """
    Wrapper function to hadd files after geant4 creation.

    final output of these files should be a single, unsorted ROOT file per
    core in the /mnt/nvme1/Kevin/qpix/output/ directory
    """
    path = "/mnt/nvme1/Kevin/qpix/output/"

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
    print("files combined into cores. Ready for sort!")

    return 1

def createSortData(time):
    """
    Wrapper function to run sort on set of files after geant4 build
    """

    # now combine everything in terms of core
    path="/mnt/nvme1/Kevin/qpix/output/"
    if not os.path.isdir(path+"sorted"):
        print("could not find input file directory")
        return -1

    sort_path = "/mnt/nvme1/Kevin/qpix/output/sorted/"
    sort_files = [os.path.join(path, f) for f in os.listdir(path) if ".root" in f and "_" in f]
    dest_files = []
    for f in sort_files:
        outf = f.split("/")[-1] 
        assert ".root" == outf[-5:], "sort input code should only read .root files"
        outf = outf[:-5]
        dest_files.append(sort_path+f"{outf}_{time}_sorted.root")

    ## sort and move files to sorted output directory
    print("sorting files:", sort_files)
    pool = mp.Pool() 
    r = pool.starmap_async(run_sort, zip(sort_files, dest_files))
    r.wait()

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
    path = "/mnt/nvme1/Kevin/qpix/output/rtd/"
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

def chainCombineData(cores):
    """
    Read from input_files from /mnt/nvme1/Kevin/qpix/output/sorted/ to group all of the 
    cores together in long tchains
    """
    path = "/mnt/nvme1/Kevin/qpix/output/sorted/"
    if not os.path.isdir(path):
        print("unable to find input sorted path!")
        return -1

    # build input and output files
    chain_files, rtd_files = [], []
    for core in range(cores):
        chain_files.append([os.path.join(path, f) for f in os.listdir(path) if f"core-{core}" in f and ".root" in f])
        rtd_files.append(os.path.join(path, f"core_{core}_rtd_input.root"))

    # attempt chaining on every core
    pool = mp.Pool(3)
    r = pool.starmap_async(run_chain, zip(chain_files, rtd_files))
    r.wait()

    return rtd_files


def main(time, cores, seed):
    """
    Look for the macro files in ./macros/long_macros and create the output
    files for each of them.

    at the conclusion of this run, there should only be base hit files in
    output/sorted and rtd files in output/rtd.

    The output directory should itself contain only there directories.
    """

    # Run the Macro to create all of the base output files in ./output/
    val = createGeantData(time, cores, seed)
    if val < 0:
        return val

    # sort and combine all of the output files
    createSortData(time)

    # all sorted data is now in /mnt/nvme1/Kevin/qpix/output/sorted/ we can now create a tchain
    # to combine them all
    chain_files = chainCombineData(cores)
    if len(chain_files) == 0:
        return -1

    createRTD(chain_files)
    print("qpix rtd creation complete..\n")

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("required to have time and number of cores supplied.")
        sys.exit(-2)
    time = int(sys.argv[1])
    core = int(sys.argv[2])
    assert core <= 10, "can't have more than 10 cores without exceeding RAM"
    assert time <= 5000, "can't store more than 5k seconds of data.."
    # ensure that time is a multiple of 10s
    assert time%10==0, "must run in time multiples of 10s"
    seed = int(sys.argv[3])
    main(time, core, seed)
