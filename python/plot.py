import numpy as np
import sigmf
import scipy
import matplotlib.pyplot as plt
from py3gpp import *

data_re = np.loadtxt('result_re', dtype=float)
data_im = np.loadtxt('result_im', dtype=float)
data_re1 = np.loadtxt('result_re1', dtype=float)
data_im1 = np.loadtxt('result_im1', dtype=float)
result = data_re + 1j*data_im
result = np.abs(result)
result1 = data_re1 + 1j*data_im1
result1 = np.abs(result1)

fs = 30.72e6
t_corr = np.arange(result.shape[0])/fs*1e3
t_corr1 = np.arange(result1.shape[0])/fs*1e3
#subplot t_corr and t_corr1
plt.subplot(211)    
plt.plot(t_corr, np.abs(result))
plt.subplot(212)
plt.plot(t_corr1, np.abs(result1))
plt.show()
#plt.plot(t_corr, np.abs(result))
#plt.show()
#plt.plot(t_corr1, np.abs(result1))
#plt.show()






