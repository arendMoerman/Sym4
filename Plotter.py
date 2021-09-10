import matplotlib.pyplot as pt
import math

import matplotlib
matplotlib.rcParams.update({'font.size': 20})
matplotlib.rcParams['mathtext.fontset'] = 'stix'
matplotlib.rcParams['font.family'] = 'STIXGeneral'
pt.rcParams['xtick.top'] = True
pt.rcParams['ytick.right'] = True

def Convert(string):
    li = string.split(" ")
    li.pop()
    
    lim = []
    for lli in li:
        lim.append(float(lli))
    return lim

def readData(path, N, plotmode=False):
    i = 0 
    tot = 0
    cluster = [[] for _ in range(3*N)]
    mscale = []
    if plotmode == True:
        fig, ax = pt.subplots(1,1, figsize=(5,5))
    
    with open(path + "data.out", "r") as inp:
        for line in inp:
            line = Convert(line)
            mscale.append(line[0])
            cluster[i].append(line[1])
            cluster[i+1].append(line[2])
            cluster[i+2].append(line[3])
               
            i += 3
               
            if i == 3*N:

                if plotmode == True:
                    ax.set_xlabel(r"$x$")
                    ax.set_ylabel(r"$y$")
                    ax.set_xlim(-3,3)
                    ax.set_ylim(-3,3)
                    ax.set_aspect("equal")
                    ax.tick_params(which="both", direction="in")
                    pt.minorticks_on()
                    for i in range(N):
                        j = i * 3
                        ax.scatter(cluster[j][-1], cluster[j+1][-1], s=mscale[i]*2*N, edgecolors='none', color='black')
                        pt.savefig(fname="{}/frames/{}.png".format(path, tot),bbox_inches='tight', dpi=300)
                    pt.cla()
                    print(tot)
                    
                    mscale = []
                i = 0
                tot += 1
    return cluster

def readEnergy(path):
    ener = []
    time = []
    with open(path + "energies.out", "r") as energy:
        for line in energy:
            line = Convert(line)
            time.append(line[0])
            ener.append(line[-1])
    return ener, time

def Plotter(path, N):
     cluster = readData(path, N)
     fig, ax = pt.subplots(1,2, figsize=(10,5), gridspec_kw={'wspace':0.35})
     for i in range(N):
          j = i * 3
          ax[0].scatter(cluster[j], cluster[j+1], s=0.1)
     ax[0].set_box_aspect(1)
     ener, time = readEnergy(path)
     
     ax[1].plot(time, [abs((x-ener[0])/ener[0]) for x in ener])
     ax[1].set_box_aspect(1)
     ax[1].set_yscale("log")
     
     ax[0].set_xlabel(r"$x$")
     ax[0].set_ylabel(r"$y$")
     ax[0].tick_params(which="both", direction="in")
     
     ax[1].set_xlabel(r"$t$")
     ax[1].set_ylabel(r"log( |(E-E0)/E0| )")
     ax[1].tick_params(which="both", direction="in")
     
     pt.savefig(fname="imageOut.png",bbox_inches='tight', dpi=300)
     pt.show()

if __name__ == "__main__":
     path = ""
     N = int(input("Give N: "))
     Plotter(path, N)
               
