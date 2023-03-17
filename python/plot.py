import matplotlib.pyplot as plt
import numpy as np 

#import result.txt as array of complex numbers
data = np.loadtxt('result.txt', dtype=complex)

#plot real and imaginary part of the complex numbers
plt.plot(data.real, data.imag, 'o')
plt.show()

