import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("data.csv")
figure, ax1 = plt.subplots()

ax1.plot(df[df.columns[6]])
ax1.plot(df[df.columns[8]])

#plt.tight_layout()
plt.show()