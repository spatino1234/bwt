import matplotlib.pyplot as plt
counts = [100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000]
timesradix = [0.000234, 0.000522, 0.001506, 0.003522, 0.011813, 0.039441, 0.096976, 0.241828, 0.995900]
timesqsort = [0.000361, 0.000850, 0.003057, 0.008378, 0.021482, 0.066831, 0.156613, 0.371227, 1.322083]
timesbase = [0.0020440610005607596, 0.004197367999950075, 0.01448775700009719, 0.03767959800097742, 0.1471289479995903, 0.6521419350010547, 3.7471751119992405, 15.748234406000847, 45.639420752999285]
timesaj = [0.00348519300132466, 0.007095871000274201, 0.023820414000510937, 0.05231612300121924, 0.20183125900075538, 0.7892549429998326, 3.9949833169994236, 15.274174332000257, 45.7122261029981]
timessais = [0.011136827000882477, 0.021461452000949066, 0.0462286390029476, 0.10498083199490793, 0.23132913900190033, 0.6952713670034427, 1.5717837830015924, 3.9834053489976213, 19.383366431000468]

CB_color_cycle = ['#377eb8', '#ff7f00', '#4daf4a',
                  '#f781bf', '#a65628', '#984ea3',
                  '#999999', '#e41a1c', '#dede00']
print("test")
plt.xscale("log")
plt.yscale("log")
plt.xlabel("DNA Length (bases)")
plt.ylabel("Time to complete (s)")
plt.plot(counts, timesbase, color = CB_color_cycle[0])
plt.plot(counts, timesaj, color = CB_color_cycle[1])
plt.plot(counts, timesradix, color = CB_color_cycle[2])
plt.plot(counts, timesqsort, color = CB_color_cycle[3])
plt.plot(counts, timessais, color = CB_color_cycle[4])
plt.title("Benchmarking for BWT variants, 20 repetitions of 50 runs each")
plt.legend(['base python bwt', 'numpy bwt', 'radix c bwt', 'qsort c bwt', 'sais bwt'])
plt.savefig("bwtbenchmarked.png")
plt.show()