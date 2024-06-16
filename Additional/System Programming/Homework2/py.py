import matplotlib.pyplot as plt 
import numpy as np

df1 = ['1', '2', '4', '8', '16']
df2 = [512, 946, 1314, 1508, 1110]

plt.bar(df1, df2, color ='forestgreen', 
        width = 0.4)
 
plt.xlabel("Number of threads")
plt.ylabel("Average generations per second")
plt.title("Benchmark scalability")
plt.show()