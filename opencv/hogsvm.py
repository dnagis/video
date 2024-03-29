#!/usr/bin/env python


#Training par HOG de reconnaissance de handwritten digits
#copie commentée de samples/python/tutorial_code/ml/py_svm_opencv/hogsvm.py
#---> pour essayer de ***comprendre*** (pas d'objectif bien défini)
#export OPENCV_SAMPLES_DATA_PATH=/initrd/mnt/dev_save/packages/opencv-4.6.0/
#/initrd/mnt/dev_save/packages/opencv-4.6.0/samples/data/digits.png #2000x1000 8bits grayscale

import cv2 as cv
import numpy as np

SZ=20
bin_n = 16 # Number of bins


affine_flags = cv.WARP_INVERSE_MAP|cv.INTER_LINEAR

## [deskew]
def deskew(img):
    m = cv.moments(img)
    if abs(m['mu02']) < 1e-2:
        return img.copy()
    skew = m['mu11']/m['mu02']
    M = np.float32([[1, skew, -0.5*SZ*skew], [0, 1, 0]])
    img = cv.warpAffine(img,M,(SZ, SZ),flags=affine_flags)
    return img
## [deskew]

## [hog]
def hog(img):
    gx = cv.Sobel(img, cv.CV_32F, 1, 0)
    gy = cv.Sobel(img, cv.CV_32F, 0, 1)
    mag, ang = cv.cartToPolar(gx, gy)
    bins = np.int32(bin_n*ang/(2*np.pi))    # quantizing binvalues in (0...16)
    bin_cells = bins[:10,:10], bins[10:,:10], bins[:10,10:], bins[10:,10:]
    mag_cells = mag[:10,:10], mag[10:,:10], mag[:10,10:], mag[10:,10:]
    hists = [np.bincount(b.ravel(), m.ravel(), bin_n) for b, m in zip(bin_cells, mag_cells)]
    hist = np.hstack(hists)     # hist is a 64 bit vector
    return hist
## [hog]

#digits.png: PNG image data, 2000 x 1000, 8-bit grayscale, non-interlaced
#digits.png affichage graphique montre 50 row de handwritten digits ([0-9]x5)
img = cv.imread(cv.samples.findFile('digits.png'),0)
if img is None:
    raise Exception("we need the digits.png image from samples/data here !")

#img: 2000(L)x1000(h) i.e. un array de 1000 fois 2000 pixels (np.ndim(img): 2, len(img)=1000, len(img[0])=2000)
#print("np.shape(img):", np.shape(img)) # --> (1000, 2000) --> shape(): n elements dans chaque dimension de la plus haute à la plus basse. Donc 1000 rows de 2000 px

cells = [np.hsplit(row,100) for row in np.vsplit(img,50)]

# np.vsplit(img,50) divise img en 50 bandes horizontales (rows) qui sont des arrays de 20x2000.
#puis np.hsplit(row,100) divise chaque row en 100 digits

#print("np.ndim(cells):", np.ndim(cells)) # --> 4 dimensions
#print("np.shape(cells):", np.shape(cells)) #shape() donne la taille de chaque dimension, ici: (50, 100, 20, 20)
#print("len(cells):", len(cells)) #50
#print("taille de cells[0]:", len(cells[0])) #100
#print("taille de cells[0][0]:", len(cells[0][0])) #20
#print("taille de cells[0][0][0]:", len(cells[0][0][0])) #20
#print("premier pixel:", cells[0][0][0][0]) #affiche un seul pixel, si j'essaie au delà: print(cells[0][0][0][0][0]) #Error
#print(cells[0][0]) #affiche un digit façon "ascii art": 20x20 pixels






#"First half is trainData, remaining is testData"
#i[:50] --> "slicing"
#train_cells c'est la moitié gauche de digits.png, test_cells c'est la moitié droite

train_cells = [ i[:50] for i in cells ] #50 premiers (0->50)
#print("np.shape(train_cells):", np.shape(train_cells)) #shape() donne la taille de chaque dimension, ici: (50, 50, 20, 20)
#il doit y avoir 50 iterations = iteration sur la première dimension (les rows). i[:50] retrieve les 50 premiers elements de chaque iteration

test_cells = [ i[50:] for i in cells] #50 derniers (50->99)

#print("train_cells[0][0]:",train_cells[0][0])
#print("taille de train_cells:",len(train_cells)) #50
#print("taille de train_cells[0]:",len(train_cells[0])) #50
#donc train_cells contient 2500 cells
#print("train_cells[0][0]:",train_cells[0][0])
#print("test_cells[0][0]:",test_cells[0][0])

######     Now training      ########################

deskewed = [list(map(deskew,row)) for row in train_cells]
#print("np.shape(deskewed):", np.shape(deskewed)) #(50, 50, 20, 20)
#print("taille de deskewed:",len(deskewed)) #50
#print(train_cells[0][0])
#print(deskewed[0][0])



hogdata = [list(map(hog,row)) for row in deskewed]
#print("np.shape(hogdata):", np.shape(hogdata)) #(50, 50, 64)


trainData = np.float32(hogdata).reshape(-1,64)
#print("np.shape(trainData):", np.shape(trainData)) #(2500, 64)
#.reshape(-1,64) --> "Flattening an array" (convertir un array de n dimensions en un array de moindre n dimensions). Ici on converti du (50, 50, 64) en (2500, 64)
#trainData est un array de 2500 x 64 (2500 digits de gauche. 50 rows de 50 digits). Chaque array de 64 est un Histogramme HOG de 64 bits d'un digit




responses = np.repeat(np.arange(10),250)[:,np.newaxis]
#print("np.shape(responses):", np.shape(responses)) #(2500, 1)
#np.arange(10) --> array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
#np.repeat(np.arange(10),3) --> array([0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9])
#[:,np.newaxis] --> transforme array([0, 0, 1, 1, 2, 2, 3, 3, ... en array([[0],[0],[1],[1],[2],[2],[3],[3],...
#donc responses est un array de 250 [0] suivi de 250 [1] etc ...





#Si je flippe responses a	vec np.flip j'ai en réponse le même taux de succès. Logique: il a été trained d'une certaine manière, il évalue de la même manière
#responses = np.repeat(np.flip(np.arange(10),0),250)[:,np.newaxis]



svm = cv.ml.SVM_create() #ref: https://docs.opencv.org/4.x/d1/d2d/classcv_1_1ml_1_1SVM.html dans "Member Function Documentation"
svm.setKernel(cv.ml.SVM_LINEAR)
svm.setType(cv.ml.SVM_C_SVC)
svm.setC(2.67)
svm.setGamma(5.383)

#Inheritance diagram de cv::ml::SVM est on top de https://docs.opencv.org/4.x/d1/d2d/classcv_1_1ml_1_1SVM.html

#train(): cv::ml::SVM Class Reference --> Public member functions inherited from cv::ml::StatModel 
#exemple de train(): https://docs.opencv.org/3.4/d1/d73/tutorial_introduction_to_svm.html
#--> svm->train(trainingDataMat, ROW_SAMPLE, labelsMat);
svm.train(trainData, cv.ml.ROW_SAMPLE, responses) #cv.ml.ROW_SAMPLE: enum cv::ml::SampleTypes




svm.save('svm_data.dat')
#save(): je suppose que c'est hérité de cv::Algorithm car il n'y a que là que je voie cette fonction. https://docs.opencv.org/4.x/d3/d46/classcv_1_1Algorithm.html
#Saves the algorithm to a file. In order to make this method work, the derived class must implement Algorithm::write(FileStorage& fs).
#cv::ml::SVM a une fonction load()
#Si je tente de grepper cette fonction dans les samples: greppe grep -r 'load(' .
#./digits_video.py:    model = cv.ml.SVM_load(classifier_fn)
#./letter_recog.py:        model.load(fn)
#./digits.py:        self.model = cv.ml.SVM_load(fn)
#./tutorial_code/core/mat_operations/mat_operations.py:def load():
#./tutorial_code/objectDetection/cascade_classifier/objectDetection.py:if not eyes_cascade.load(cv.samples.findFile(eyes_cascade_name)):
#Déjà ça a l'air de s'appeler un model


######     Now testing      ########################

deskewed = [list(map(deskew,row)) for row in test_cells]
hogdata = [list(map(hog,row)) for row in deskewed]
testData = np.float32(hogdata).reshape(-1,bin_n*4)

#print("np.shape(testData):", np.shape(testData)) # (2500, 64)

#j'essaie sur seulement quelques éléments de testData
#vvnxData = testData[1000:1002]
#print("np.shape(vvnxData):", np.shape(vvnxData))# (1, 64)




#Si je flip la data je passe d'un taux de succès de 93% à 17.56%
#flipedData = np.flip(testData, 1)
#result = svm.predict(flipedData)[1]

result = svm.predict(testData)[1]

#resultvvnx = svm.predict(vvnxData)[1]

#print("np.shape(resultvvnx):", np.shape(resultvvnx)) #(1, 1) 
#print("resultvvnx:", resultvvnx)






#######   Check Accuracy   ########################
mask = result==responses
correct = np.count_nonzero(mask)
print(correct*100.0/result.size)
