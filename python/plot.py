import numpy as np
import sigmf
import scipy
import matplotlib.pyplot as plt
from py3gpp import *

# read in data
data_re = np.loadtxt('result_re', dtype=float)
data_im = np.loadtxt('result_im', dtype=float)
result = data_re + 1j*data_im
result = np.abs(result)

fs = 30.72e6
t_corr = np.arange(result.shape[0])/fs*1e3
plt.plot(t_corr, np.abs(result))
plt.show()






