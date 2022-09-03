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


cells = [np.hsplit(row,100) for row in np.vsplit(img,50)]

#img: 2000(L)x1000(h) i.e. un array de 1000 fois 2000 pixels (np.ndim(img): 2, len(img)=1000, len(img[0])=2000)
# np.vsplit(img,50) le divise en 50 bandes horizontales (row) qui sont des arrays de 20x2000.
#puis np.hsplit(row,100) divise chaque row en 100 digits

#print("np.ndim(cells):", np.ndim(cells)) #4 
#print("len(cells):", len(cells)) #50
#print("taille de cells[0]:", len(cells[0])) #100
#print("taille de cells[0][0]:", len(cells[0][0])) #20
#print("taille de cells[0][0][0]:", len(cells[0][0][0])) #20
#print("premier pixel:", cells[0][0][0][0]) #affiche un seul pixel, print(cells[0][0][0][0][0]) #Error
#print(cells[0][0]) #affiche un digit: 20x20 pixels


# First half is trainData, remaining is testData
#i[:50] --> "slicing"
#train_cells c'est la moitié gauche de digits.png, test_cells c'est la moitié droite
train_cells = [ i[:50] for i in cells ] #50 premiers (0->50)
test_cells = [ i[50:] for i in cells] #50 derniers (50->99)

#print("train_cells[0][0]:",train_cells[0][0])
#print("taille de train_cells:",len(train_cells)) #50
#print("taille de train_cells[0]:",len(train_cells[0])) #50
#donc train_cells contient 2500 cells
#print("train_cells[0][0]:",train_cells[0][0])
#print("test_cells[0][49]:",test_cells[0][48])

######     Now training      ########################

deskewed = [list(map(deskew,row)) for row in train_cells]
#print("taille de deskewed:",len(deskewed)) #50
#print(train_cells[0][0])
#print(deskewed[0][0])

hogdata = [list(map(hog,row)) for row in deskewed]
trainData = np.float32(hogdata).reshape(-1,64)
responses = np.repeat(np.arange(10),250)[:,np.newaxis]

svm = cv.ml.SVM_create() #ref: https://docs.opencv.org/4.x/d1/d2d/classcv_1_1ml_1_1SVM.html dans "Member Function Documentation"
svm.setKernel(cv.ml.SVM_LINEAR)
svm.setType(cv.ml.SVM_C_SVC)
svm.setC(2.67)
svm.setGamma(5.383)

#Inheritance diagram de cv::ml::SVM est on top de https://docs.opencv.org/4.x/d1/d2d/classcv_1_1ml_1_1SVM.html

#train(): cv::ml::SVM Class Reference --> Public member functions inherited from cv::ml::StatModel 
svm.train(trainData, cv.ml.ROW_SAMPLE, responses) #cv.ml.ROW_SAMPLE: enum cv::ml::SampleTypes
#save(): je suppose que c'est hérité de cv::Algorithm car il n'y a que là que je voie cette fonction. https://docs.opencv.org/4.x/d3/d46/classcv_1_1Algorithm.html
#Saves the algorithm to a file. In order to make this method work, the derived class must implement Algorithm::write(FileStorage& fs).
svm.save('svm_data.dat')
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
result = svm.predict(testData)[1]

#######   Check Accuracy   ########################
mask = result==responses
correct = np.count_nonzero(mask)
print(correct*100.0/result.size)
