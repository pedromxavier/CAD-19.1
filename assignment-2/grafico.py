import matplotlib.pyplot as plt
import numpy as np
import sys, os

argc = len(sys.argv)

colors = iter(['r', 'b', 'g', 'c', 'm', 'y', 'k'])

bars = 8;

modes = iter(map(lambda x: x[:-4], sys.argv[1:]))

def formattime(t, e):
    t = np.array(t, dtype=np.float64)
    if e >= 0: # s
        s = "s"
    elif 0 > e >= -3: # ms
        t *= 1e+3
        s = "ms"
    elif -3 > e >= -6: # us
        t *= 1e+6
        s = "µs"
    elif -6 > e >= -9: # ns
        t *= 1e+9
        s = "ns"
    elif -9 > e: # ps
        t *= 1e+12
        s = "ps"
    return t, s

def get_line(line):
    parts = line.split("|")
    n, v = int(parts[0]), int(parts[2])
    t, dt = tuple(map(float,parts[1].split("$")))
    return n, t, dt, v

def read_data(fname):
    with open(fname) as file:
        N = []
        T = []
        DT = []
        for line in file:
            n, t, dt, v = get_line(line)
            N.append(n)
            T.append(t)
            DT.append(dt)
    return N,T,DT


def polyformat(a,b,c):
    a,b,c = tuple(map(lambda x: "{:+.6f}".format(x), (a,b,c)))
    return r'${}N^2 {}N {}$'.format(a,b,c)


def main():
    if "-s" in sys.argv:
        i = sys.argv.index('-s')
        SAVE_FIG = True
        FIG_NAME = sys.argv[i+1]
        FIG_NAME = os.path.join(os.getcwd(), FIG_NAME)
        del sys.argv[i:i+2]
    else:
        SAVE_FIG = False

    if len(sys.argv) == 0:
        print('faltou nome do arquivo de resultados')
        return
    else:
        title = ''

    N, T, DT = [],[],[]
    header = []
    for fname in sys.argv[1:]:
        n, t, dt = read_data(fname)
        N.append(n)
        T.append(t)
        DT.append(dt)
    max_t = max(sum(T, []))
    max_l = max(map(len, N))

    step = int(max_l / bars + 1);

    e = np.log10(max_t)
    _, s = formattime(0, e)

    N = list(map(np.array, N))
    T  = list(map(lambda t: (formattime(t,e)[0]), T))
    DT  = list(map(lambda t: (formattime(t,e)[0]), DT))

    for n, t, dt in zip(N,T,DT):
        color = next(colors)
        a, b, c = np.polyfit(n,t,2)
        y = a*n**2 + b*n + c
        print(a,b,c)
        plt.scatter(n, t, s = 1, c=color)
        plt.errorbar(n, y, yerr=dt, color=color, linewidth=0, elinewidth=1, errorevery=step)
        header += plt.plot(n, y, color=color, label=polyformat(a,b,c) + r' [{}]'.format(next(modes)))
    plt.ylim(bottom=0)
    plt.xlim(left=0)
    plt.xlabel(r"$n$ (dimensão da matriz)")
    plt.ylabel(r"$t$ (%s)" % s)
    plt.legend()
    plt.grid(True)
    plt.title(title)
    if SAVE_FIG:
        plt.savefig(FIG_NAME)
    else:
        plt.show()

if __name__ == '__main__':
    main()
