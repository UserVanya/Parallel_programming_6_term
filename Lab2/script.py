import numpy as np
import pandas as pd
from scipy import stats 
import matplotlib.pyplot as plt
def confidenceInterval (serviceTimes, alpha = 0.01):
  n = len(serviceTimes)
  return np.sqrt(np.var(serviceTimes, ddof=1)) / np.sqrt(n) * stats.t.ppf(1-alpha/2, n-1)

df = pd.read_table("res.txt", sep='\t', index_col = None, header = 0)

#average over runs
df = df.groupby(['n'])['Time'].agg([np.mean, confidenceInterval]).reset_index()
plt.plot(df['n'], np.log10(df['mean']), marker = 'o', linestyle = '-', color = 'k', label = 'simulation', markerfacecolor='w')
plt.ylabel(r"$\log_{10}^{Time}$")
plt.xlabel(r"n")
plt.savefig("M=3200, K=3200.jpg")