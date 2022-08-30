#!/usr/bin/python

#Matplotlib hello world
#pip install matplotlib: 7 ou 8 deps

#backend: Affichage nécéssite un GUI backend: qt5: aufs qt5; pip install pyqt5 (3 pkgs: pyqt5 PyQt5-Qt5 PyQt5-sip)
#liste des backends: You can pretend to put a wrong backend argument, then it will return you a ValueError with the list of valid matplotlib backends, like this:
#import matplotlib
#matplotlib.use('WRONG_ARG')
#Agg = Anti Grain Geometry c++ library
#FAQ: https://matplotlib.org/2.0.2/faq/usage_faq.html
import matplotlib; matplotlib.use('Qt5Agg')


import matplotlib.pyplot as plt
import numpy as np

plt.style.use('_mpl-gallery') #Pour pouvoir choisir le plot format


#detect.txt: chaque row est du type: "53 98" sans les "", et avec un nombre d'espaces indéfini
data = np.loadtxt('detect.txt')
x = data[:, 0]
y = data[:, 1]


#Format style line, par défaut je suppose
#https://matplotlib.org/stable/tutorials/introductory/pyplot.html#formatting-the-style-of-your-plot
#Arg 3 ('ro': red circle, 'bs': blue square) serait "LineSpec"
#https://www.mathworks.com/help/matlab/creating_plots/specify-line-and-marker-appearance-in-plots.html
#plt.plot(x, y,'bs')



#https://matplotlib.org/stable/plot_types/stats/eventplot.html#sphx-glr-plot-types-stats-eventplot-py
#https://matplotlib.org/stable/plot_types/basic/bar.html#sphx-glr-plot-types-basic-bar-py
fig, ax = plt.subplots()
ax.bar(x, y, width=1, edgecolor="black", linewidth=1)






plt.show() #pas indiqué dans le tuto 'hello world'





