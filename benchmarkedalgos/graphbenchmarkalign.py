import matplotlib.pyplot as plt
counts = [1, 2, 5, 10, 20, 50, 100, 200]
timesnaive = [1.2100397350004641, 1.058204458999171, 1.0597088719996464, 1.043174330000511, 1.0345601589997386, 1.031816109999454, 1.0227966849997756, 1.029259431000355]
timesbwt = [0.00011369299954822054, 0.00017939699955604738, 0.00047782699948584195, 0.0009211289998347638, 0.0010113380003531347, 0.001508610999735538, 0.002292801999828953, 0.003738245000022289]
timesboyer = [1.1175581219995365, 0.8316338920003545, 0.5178201329999865, 0.42443765700045333, 0.4088106319995859, 0.39546262900057627, 0.4059070530001918, 0.4119763420003437]

CB_color_cycle = ['#e69f00', '#56b4e9', '#f0e442',
                  '#d55e00']
print("test")
plt.xscale("log")
plt.yscale("log")
plt.xlabel("Query Length (bases)")
plt.ylabel("Time to complete (s)")
plt.plot(counts, timesnaive, color = CB_color_cycle[0])
plt.plot(counts, timesboyer, color = CB_color_cycle[1])
plt.plot(counts, timesbwt, color = CB_color_cycle[2])
# plt.plot(counts, timesnaive, color = CB_color_cycle[2])
plt.title("Benchmarking for Boyer-Moore and BWT Alignment")
plt.legend(['naive alignment', 'boyer-moore alignment', 'bwt alignment'])
plt.savefig("alignmentbenchmarked1.png")
plt.show()