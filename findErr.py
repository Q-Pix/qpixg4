import os
import sys

def countIn(t, fileList):
    l = [f for f in fileList if t in f]
    return len(l)

def main():

    f = os.listdir('/media/argon/NVME2/Kevin/qpix/output/neutrinos')
    print(f"found {len(f)} files.")
    f = [i for i in f if ".root" in i]
    total = len(f)

    # test pdgs
    pdgs = ['pdg--14', 'pdg--12', 'pdg-12', 'pdg-14']
    sum=0
    for pdg in pdgs:
        counts = countIn(pdg, f)
        print(f"{pdg} has count:", counts)
        sum+=counts
    if sum != total:
        print(f"pdgs ({sum}) does not have total ({total})")

    # test energies
    energies = range(250,10001,250)
    energies = ['E-'+str(en)+'_' for en in energies]
    sum = 0
    for en in energies:
        counts = countIn(en, f)
        print(f"{en} has count:", counts)
        sum += counts
    if sum != total:
        print(f"energies ({sum}) does not have total ({total})")

    # test evts
    evts = range(100)
    evts = ['_evt-'+str(en)+'_' for en in evts]
    sum=0
    for en in evts:
        counts = countIn(en, f)
        print(f"{en} has count:", counts)
        sum += counts
    if sum != total:
        print(f"evts ({sum}) does not have total ({total})")

    # test FHC type
    isFHC = ['FHC-1', 'FHC-0']
    sum = 0
    for en in isFHC:
        counts = countIn(en, f)
        print(f"{en} has count:", counts)
        sum += counts
    if sum != total:
        print(f"fhc ({sum}) does not have total ({total})")

    # test z pos
    zpos = ['z-10', 'z-80', 'z-180', 'z-280', 'z-350']
    sum=0
    for en in zpos:
        counts = countIn(en, f)
        print(f"{en} has count:", counts)
        sum+=counts
    if sum != total:
        print(f"zpos ({sum}) does not have total ({total})")

    # test angle
    angles = ['t-1.root', 't-2.root', 't-3.root', 't-4.root', 't-5.root']
    sum = 0
    for en in angles:
        counts = countIn(en, f)
        print(f"{en} has count:", counts)
        sum += counts
    if sum != total:
        print(f"zpos ({sum}) does not have total ({total})")


if __name__ == "__main__":
    main()
