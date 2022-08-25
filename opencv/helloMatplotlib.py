#!/usr/bin/python

#Matplotlib hello world
#Install: Il faut un GUI backend: aufs qt5; pip install pyqt5 (3 pkgs: pyqt5 PyQt5-Qt5 PyQt5-sip)

#matplotlib: 7 ou 8 deps

import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np

#data.txt: chaque row est du type: "53 98" sans les "", et avec un nombre d'espaces indéfini
data = np.loadtxt('data.txt')
x = data[:, 0]
y = data[:, 1]
plt.plot(x, y,'ro')


plt.show() #pas indiqué dans le tuto





