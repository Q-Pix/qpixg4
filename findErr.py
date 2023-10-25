import os
import sys
import multiprocessing as mp
import subprocess

from qpixg4mp import find_prog

def countIn(t, fileList):
    l = [f for f in fileList if t in f]
    return len(l)


def move_meta(file_pair):
    sortf = file_pair[0]
    rtdf = file_pair[1]
    n = file_pair[2]

    prog = "rootcp"
    subprocess.run([prog, sortf+":metadata", rtdf])
    if n%100 ==0:
        print("moved sort file!", n)


def run_rtdfriend(input_file):
    prog_path = '../qpixrtd/build'
    prog = find_prog(prog_path, 'rtdfriend')
    subprocess.run([prog, input_file])

def update_branches():
    input_path = '/media/argon/NVME2/Kevin/qpix/output/neutrinos/bad_rot/neutrinos_rtd/'
    prog_path = '../qpixrtd/build'
    found = find_prog(prog_path, 'rtdfriend')
    f = os.listdir(input_path)
    f = [i for i in f if ".root" in i]
    f = [os.path.join(input_path, i) for i in f]
    print("running on n files:" , len(f))
    input("enter to continue.")

    pool = mp.Pool()
    r = pool.map_async(run_rtdfriend, f)
    r.wait()

def main():

    rtd_dir = '/media/argon/NVME2/Kevin/qpix/output/neutrinos/neutrinos_rtd/'
    sort_dir = '/media/argon/NVME2/Kevin/qpix/output/neutrinos/neutrinos_sort/'

    f = os.listdir(sort_dir)
    rtdf = os.listdir(rtd_dir)
    print(f"found {len(f)} files.")
    f = [i for i in f if ".root" in i]
    rtdf = [i for i in rtdf if ".root" in i]
    total = len(f)
    rtd_total = len(rtdf)

    rtdf = sorted(rtdf)
    f = sorted(f)

    def checkTyp(typ, files, rtd_files):
        sum = 0
        for en in typ:
            counts = countIn(en, files)
            print(f"{en} has count:", counts)
            sum += counts
            rtd_counts = countIn(en, rtd_files)
            if rtd_counts != counts:
                print(f"{en} @ sort counts ({counts}) not equal to rtd_counts ({rtd_counts})")
        if sum != total:
            print(f"zpos ({sum}) does not have total ({total})")

    # test pdgs
    pdgs = ['pdg--14', 'pdg--12', 'pdg-12', 'pdg-14']
    checkTyp(pdgs, f, rtdf)

    # test energies
    energies = range(250,10001,250)
    energies = ['E-'+str(en)+'_' for en in energies]
    checkTyp(energies, f, rtdf)

    # test evts
    evts = range(100)
    evts = ['_evt-'+str(en)+'_' for en in evts]
    checkTyp(evts, f, rtdf)

    # test FHC type
    isFHC = ['FHC-1', 'FHC-0']
    checkTyp(isFHC, f, rtdf)

    # test z pos
    zpos = ['z-10', 'z-80', 'z-180', 'z-280', 'z-350']
    checkTyp(zpos, f, rtdf)

    # test angle
    angles = ['t-1_sorted', 't-2_sorted', 't-3_sorted', 't-4_sorted', 't-5_sorted']
    checkTyp(angles, f, rtdf)

    # combine the files
    # build the list we want to move
    # combFiles = []
    # for i, rtd in enumerate(rtdf):
    #     typs = rtd[:-9]
    #     for src in f:
    #         if typs == src[:-5]:
    #             srcf = src
    #             f.remove(src)
    #             break
    #     pair = [os.path.join(sort_dir, srcf), os.path.join(rtd_dir, rtd), i]
    #     combFiles.append(pair)
    #     if i%1000==0:
    #         print("finding pair:", i, "out of total:", len(rtdf))
    # 
    # print("making the scp move!")
    # pool = mp.Pool()
    # r = pool.map_async(move_meta, combFiles)
    # r.wait()
    # print("fin.")

if __name__ == "__main__":
    update_branches()
    # main()
