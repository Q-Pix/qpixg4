import multiprocessing as mp
import subprocess
import os
import sys
import argparse

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
        ret = subprocess.run([prog, input_file], stdout=subprocess.DEVNULL)
        if ret.returncode != 0:
            msg = f"geant4 {input_file} process exited with:", ret.returncode
            print(msg)
            file1 = open("/home/argon/Projects/Kevin/err/g4_err.txt", "a")
            file1.write(msg[0])
            file1.close()
    else:
        return -1 

def run_chain(input_files, dest_file, delete=False):
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
        if delete:
            for f in input_files:
                subprocess.run([prog, f])
    else:
        return -1 

def run_sort(input_file, dest_file, delete=True):
    """
    once the geant files are created, we need to perform the following:
    sort geant ttree data
    move and rename the sort_event_tree and move metadata tree into a new tfile
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
    if delete:
        prog = "rm"
        subprocess.run([prog, input_file])
        # print("file moved.. ")

def createGeantData(time, cores, seed, outputPath):
    """
    Wrapper function to make geant data from macros.
    function simply runs the script LongDecay.sh macro with arguments supplied
    above.
    """

    decay_script =  "./LongDecay.sh"
    if not os.path.isfile(decay_script):
        print("WARNING unable to find decay script!")
        return None 
    subprocess.run(["bash", decay_script, str(time), str(cores), str(seed), str(outputPath)])

    if not os.path.isdir("./macros/long_macros/"):
        print("did not find macro directory!!")
        return None

    mac_files = os.listdir("./macros/long_macros/")
    mac_files = ["./macros/long_macros/"+f for f in mac_files]
    pool = mp.Pool(30)
    r = pool.map_async(run_g4, mac_files)
    r.wait()
    print("Geant4 Files created.")
    return outputPath

def createSortData(time, input_path):
    """
    Wrapper function to run sort on set of files after geant4 build
    """

    # now combine everything in terms of core
    sort_path = input_path+"/sorted/"
    if not os.path.isdir(sort_path):
        print("could not find input file directory")
        return -1

    sort_files = [os.path.join(input_path, f) for f in os.listdir(input_path) if ".root" in f and "_" in f]
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

    return sort_path

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

def createRTD(sorted_files, output_path):
    """
    finally, run the RTD code on each sorted file

    EXAMPLE-RTD program should create a new tree of the resets
    
    but change the postfix to _rtd.root.
    """
    path = output_path
    if not os.path.isdir(path):
        print("unable to find output RTD path!")
        return -1

    rtd_files = []
    for f in sorted_files:
        # take only the base name from this path
        outf = f.split("/")[-1] 
        assert ".root" == outf[-5:], "rtd code should read sorted .root files"
        rtd_files.append(path+outf[:-5] + "_rtd.root")
    nCores = len(sorted_files)
    if nCores > 50: # don't go too hard
        pool = mp.Pool()
    else:
        pool = mp.Pool(nCores)
    r = pool.starmap_async(run_rtd, zip(sorted_files, rtd_files))
    r.wait()

def chainCombineData(cores, input_path="/media/argon/NVME1/Kevin/qpix/output/sorted/"):
    """
    Read from input_files from /media/argon/NVME1/Kevin/qpix/output/sorted/ to group all of the 
    cores together in long tchains
    """
    if not os.path.isdir(input_path):
        print("unable to find input sorted path!")
        return -1

    # build all of the og chain files
    chain_files = []
    for core in range(cores):
        chain_files.append([os.path.join(input_path, f) for f in
            os.listdir(input_path) if f"core-{core}" in f and ".root" in f])

    steps = set()
    for chain in chain_files:
        for f in chain:
            seed = f.split('_')[-3]
            steps.add(int(seed))

    # build input and output files for final chain
    int_files, step_files = [], []
    for core in range(cores):
        for step in steps:
            files = os.listdir(input_path)
            ifs = [os.path.join(input_path, f) for f in
                    files if f"core-{core}" in f and ".root" in f and f"_{step}_" in f]
            if len(ifs) != 10:
                continue
            int_files.append(ifs)
            step_files.append(os.path.join(input_path, f"core-{core}_step-{step}_input.root"))

    # iterate through the chain files slowly grouping them down
    pool = mp.Pool()
    input("run the step chain")
    r = pool.starmap_async(run_chain, zip(int_files, step_files, [False]*len(steps)))
    r.wait()

    # build input and output files for final chain
    chain_files, rtd_files = [], []
    for core in range(cores):
        chain_files.append([os.path.join(input_path, f) for f in
            os.listdir(input_path) if f"core-{core}" in f and ".root" in f and 'step-' in f])
        rtd_files.append(os.path.join(input_path, f"core_{core}_rtd_input.root"))

    # final chain
    pool = mp.Pool(5)
    r = pool.starmap_async(run_chain, zip(chain_files, rtd_files))
    r.wait()

    return rtd_files

def run_neutrino(inp):
    inp = [str(arg) for arg in inp]
    ret = subprocess.run(["bash", "./NeutrinoMac.sh", *inp], stdout=subprocess.DEVNULL)
    if ret.returncode != 0:
        print("neutrino process exited with:", ret.returncode)

def makeNeutrinoArgsOdyssey(args):
    """
    List of neutrino script arguments
    1 - xpos
    2 - ypos
    3 - zpos
    4 - seed
    5 - input file
    6 - output directory
    7 - angle select [1,2,3,4,5]
    8 - nEvt, number within event file
    9 - fsPdg
    10 - fsEnergy
    11 - isFHC
    """

    outputPath = args.outDir
    outputPath = os.path.abspath(outputPath) + "/"
    inputPath = args.srcDir
    xpos = args.xpos
    ypos = args.ypos
    assert xpos >= 1 and xpos < 575, f"x position outside of APA: {xpos}"
    assert ypos >= 1 and ypos < 1500, f"y position outside of APA: {ypos}"

    path = inputPath
    fs = [os.path.join(path, f) for f in os.listdir(path)]
    fs = [good_file for good_file in fs if os.path.getsize(good_file) > 1e6 and "root" in good_file]
    print(f"found {len(fs)} input files: {fs}")
    seed = args.seed
    if len(fs) == 0:
        return -1
    neutrino_args = []
    for z in args.zpos:
        for f in fs:
            for t in ['1', '2', '3', '4', '5']:
                for nEvt in range(100):
                    for nEng in range(250, 10000, 250):
                        if 'aelectron' in f:
                            pdg = -12
                        elif 'electron' in f:
                            pdg = 12
                        elif 'amuon' in f:
                            pdg = -14
                        elif 'muon' in f:
                            pdg = 14
                        isFHC = int("fhc" in f)
                        f = os.path.abspath(f)
                        neutrino_args.append([xpos,ypos,z,seed,f,outputPath,t,nEvt,pdg,nEng,isFHC])

    return neutrino_args

def makeNeutrinoArgsSrc(args):
    """
    Make neutrino args from the source files based on nEvents
    """
    outputPath = args.outDir
    outputPath = os.path.abspath(outputPath) + "/"
    inputPath = args.srcDir
    xpos = args.xpos
    ypos = args.ypos
    ncores = args.cores
    assert xpos > 5 and xpos < 575, f"x position outside of APA: {xpos}"
    assert ypos > 5 and ypos < 1500, f"y position outside of APA: {ypos}"

    fs = []
    path = inputPath
    f = [os.path.join(path, f) for f in os.listdir(path) if ".root" in f ]
    f = [good_file for good_file in f if os.path.getsize(good_file) > 1e6]
    fs.extend(f)
    seed = args.seed
    if len(fs) == 0:
        return -1
    neutrino_args = []
    for evt in range(args.nEvts):
        for z in args.zpos:
            for f in fs:
                neutrino_args.append([xpos,ypos,z,seed,f,outputPath,0,evt])
                neutrino_args.append([xpos,ypos,z,seed,f,outputPath,1,evt])

    return neutrino_args

def makeNeutrinos(args):
    """
    Wrapper function to control the creation->RTD sequence for
    generating RTD data from the FHC/RHC data. Use the nvme2 dir
    for the output directory.

    Controlled via argparse nu subparser.
    """
    neutrino_args = makeNeutrinoArgsOdyssey(args)
    msg = f"making {len(neutrino_args)} neutrino macro files. enter to continue"
    print(msg)

    pool = mp.Pool()
    r = pool.map(run_neutrino, neutrino_args)

    # read in macro files and make files in source dir
    g4_args = [os.path.join("./macros/neutrino_macros", f) for f in os.listdir("./macros/neutrino_macros/") if ".mac" in f]
    msg = f"found a total of {len(g4_args)} geant4 files to make enter to continue.."
    print(msg)

    # do the total number of neutrino files in batches..
    pool = mp.Pool(50)
    r = pool.map(run_g4, g4_args)

    sort_path = os.path.abspath(args.outDir)
    sort_files = [os.path.join(sort_path, f) for f in os.listdir(sort_path) if ".root" in f and "_" in f]
    dest_files = []
    for f in sort_files:
        outf = f.split("/")[-1] 
        assert ".root" == outf[-5:], "sort input code should only read .root files"
        outf = outf[:-5]
        dest_files.append(sort_path+f"/neutrinos_sort/{outf}_sorted.root")
    assert len(dest_files) == len(sort_files), "mismatch between dest sort files"

    # sort and move files to sorted output directory
    msg = f"found {len(dest_files)} dest files."
    print(msg)
    pool = mp.Pool(50)
    r = pool.starmap(run_sort, zip(sort_files, dest_files))
    # if you don't want to delete the inbetween data files use line below
    # r = pool.starmap(run_sort, zip(sort_files, dest_files, [False]*len(dest_files)))

    msg = f"len dest files for rtd: {len(dest_files)}, {dest_files[0]}"
    print(msg)
    createRTD(dest_files, output_path=args.outDir+"/neutrinos_rtd/")

def main(args):
    """
    Look for the macro files in ./macros/long_macros and create the output
    files for each of them.

    at the conclusion of this run, there should only be base hit files in
    output/sorted and rtd files in output/rtd.

    The output directory should itself contain only there directories.
    """
    # "/media/argon/NVME1/Kevin/qpix/output/"
    time, cores, seed, geant4_data_path = args.time, args.cores, args.seed, args.outDir
    print(f"running: time={time}, cores={cores}, seed={seed}")

    # Run the Macro to create all of the base output files in ./output/
    # output_path = createGeantData(time, cores, seed, geant4_data_path)
    # if output_path is None:
    #     return -1

    # sort and combine all of the output files
    # sort_path = createSortData(time, input_path=geant4_data_path)

    # all sorted data is now in /media/argon/NVME1/Kevin/qpix/output/sorted/ we can now create a tchain
    # to combine them all
    chain_files = chainCombineData(cores, input_path="/media/argon/NVME1/Kevin/qpix/output/sorted/")
    if len(chain_files) == 0:
        return -1

    # input_path = "/media/argon/NVME1/Kevin/qpix/output/sorted/1ks/"
    createRTD(chain_files, output_path="/media/argon/NVME1/Kevin/qpix/output/rtd/")
    print("qpix rtd creation complete..\n")

## ArgParse Config ##
def get_args():
    """
    define arguments to be set into the parser here
    """
    parser = argparse.ArgumentParser()
    subParsers = get_subParsers(parser)
    args = parser.parse_args()
    return args, parser

def get_subParsers(parser):
    """
    define list of sub commands here to be parsed:
    Command List:
    g4   - make output of geant4 events from radiogenic data
    """
    subParsers = parser.add_subparsers(dest="mods")

    # make g4 files
    g4 = subParsers.add_parser("g4", description="create geant4 data")
    g4.set_defaults(func=main)
    g4.add_argument("-t", "--time", default=10, type=int, help="time in s to run g4 simulation for")
    g4.add_argument("-c", "--cores", default=1, type=int, help="number of cores to produce")
    g4.add_argument("-s", "--seed", default=420, type=int, help="random seed")
    g4.add_argument("-o", "--outDir", required=True, type=str, help="output ROOT file location for radiogenic generation")

    # make neutrino files
    nu = subParsers.add_parser("nu", description="create geant4 data")
    nu.set_defaults(func=makeNeutrinos)
    nu.add_argument("-i", "--srcDir", default="/media/argon/NVME2/Kevin/qpix/", type=str, help="source file where FHC and RHC files are found")
    nu.add_argument("-o", "--outDir", required=True, type=str, help="output ROOT file location for hit generation")
    nu.add_argument("-c", "--cores", default=50, type=int, help="number of cores to help produce")
    nu.add_argument("-s", "--seed", default=420, type=int, help="random seed")
    nu.add_argument("-z", "--zpos", nargs="+", default=[10, 80, 180, 280, 350], help="list of z position values")
    nu.add_argument("-x", "--xpos", default=120, type=int, help="x position within APA, default near center")
    nu.add_argument("-y", "--ypos", default=320, type=int, help="y position within APA, default near center")
    nu.add_argument("-e", "--nEvts", default=-1, type=int, help="construct event to read from a source file")

    return subParsers

def run_args(args):
    """
    implent the arguments from the user here if they pass all of the
    conditions from testing
    """
    print("running args:", args)
    args.func(args)


if __name__ == "__main__":
    # if len(sys.argv) != 4:
    #     print("required to have time and number of cores supplied.")
    #     sys.exit(-2)
    # time = int(sys.argv[1])
    # core = int(sys.argv[2])
    # assert core <= 10, "can't have more than 10 cores without exceeding RAM"
    # assert time <= 5000, "can't store more than 5k seconds of data.."
    # # ensure that time is a multiple of 10s
    # assert time%10==0, "must run in time multiples of 10s"
    # seed = int(sys.argv[3])
    # main(time, core, seed)

    # argparse ctrl
    args, parser = get_args()
    run_args(args)
