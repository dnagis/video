#!/usr/bin/python

#Matplotlib hello world
#Install: Il faut un GUI backend: aufs qt5; pip install pyqt5


import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np

#data.txt
data = np.loadtxt('data.txt')
x = data[:, 0]
y = data[:, 1]
plt.plot(x, y,'ro')


plt.show() #pas indiqué dans le tuto





