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

def main():
    """
    Look for the macro files in ./macros/long_macros and create the output
    files for each of them.
    """
    if not os.path.isdir("./macros/long_macros/"):
        print("did not find macro directory!!")
        return -1
    mac_files = os.listdir("./macros/long_macros/")
    mac_files = ["./macros/long_macros/"+f for f in mac_files if "Pb" in f]
    # print(mac_files)
    pool = mp.Pool(10)
    r = pool.map_async(run_g4, mac_files)
    r.wait()
    print("done.")


if __name__ == "__main__":
    if len(sys.argv) != 1:
        print("need only a single arg")
        sys.exit(-2)
    main()
