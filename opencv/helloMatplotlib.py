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


#Format style
#https://matplotlib.org/stable/tutorials/introductory/pyplot.html#formatting-the-style-of-your-plot
#Arg 3 ('ro': red circle, 'bs': blue square) serait "LineSpec"
#https://www.mathworks.com/help/matlab/creating_plots/specify-line-and-marker-appearance-in-plots.html
plt.plot(x, y,'bs')


plt.show() #pas indiqué dans le tuto





