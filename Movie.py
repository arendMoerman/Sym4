from Plotter import readData
import os

def Moviemaker(path, N):
    cluster = readData(path, N, plotmode=True)
    
def Make(path):
    os.system("cd /home/arend/Projects/Y4/movie/frames/")
    os.system("ffmpeg -framerate 60 -i {}/frames/%d.png movie.avi".format(path))

if __name__ == "__main__":
    path = "movie/"
    N = int(input("Give N: "))
    Moviemaker(path, N)
    Make(path)
