#!/usr/bin/env python

#A partir des données de détection au format: 2 cols: timestamp [0 ou 1], arriver à "segmenter" les régions
#qui contiennent une séquence interessante (où l'objet recherché est fréquemment détecté).



import numpy as np
  
#Importer les donnees de detect.txt (générées par hog_pd_file) dans un array
#deux cols dans detect.txt, je veux seulement la deuxième donc 1
#https://www.geeksforgeeks.org/import-text-files-into-numpy-arrays/  
data = np.loadtxt("detect.txt", usecols=1, dtype=int)

#print(data)

print("np.shape(data): ", np.shape(data)) #np.shape(data): (1838,)
print("taille de data: ", data.size)


s_sub=30 #taille de chaque segment à analyser


for i in range(0, data.size-s_sub):
	sub = data[i: i+ s_sub]
	print(sub)
	print(np.nonzero(sub))


