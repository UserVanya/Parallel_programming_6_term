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
plt.errorbar(df['n'][:16], df['mean'][:16], df['confidenceInterval'][:16], marker = 'x', linestyle = 'None', color = 'k', label = 'simulation')
plt.savefig("res.jpg")