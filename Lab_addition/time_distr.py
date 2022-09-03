from cProfile import label
import numpy as np
import pandas as pd
from pyparsing import col
from scipy import stats 
import matplotlib.pyplot as plt
def confidenceInterval (serviceTimes, alpha = 0.01):
  n = len(serviceTimes)
  return np.sqrt(np.var(serviceTimes, ddof=1)) / np.sqrt(n) * stats.t.ppf(1-alpha/2, n-1)

df = pd.read_table("res.txt", sep='\t', index_col = None, header = 0)

#average over runs

df = df.groupby(['M', 'P', 'NP'])['Time'].agg([np.mean, confidenceInterval]).reset_index()
i = 1
colormap = plt.cm.gist_ncar
for m in df['M'].unique():
  for p in df['P'].unique():
    color = colormap(i / (df['P'].unique().size * df['M'].unique().size + 1))
    i+=1
    df_to_plot = df[df.P == p][df.M == m]
    #plt.errorbar(x=df_to_plot['NP'], y=df_to_plot['mean'], yerr=df_to_plot['confidenceInterval'], marker='x', linestyle='None', label=f"M={m}, P={p}", uplims=True, lolims=True, color=color)
    plt.plot(df_to_plot['NP'], df_to_plot["mean"], 'o-', color=color, label=f"M={m}, P={p}", markerfacecolor="w")
plt.legend(loc='best')
plt.xlabel("NP")
plt.ylabel("Time")
plt.ylim((0, 0.125))
plt.grid()
plt.savefig("time_distr1.jpg")
plt.show()

