#!/usr/bin/env python

"""flames.py - Realtime Fire Effect Demo
Pete Shinners, April 3, 2001

Ok, this is a pretty intense demonstation of using
the surfarray module and numeric. It uses an 8bit
surfaces with a colormap to represent the fire. We
then go crazy on a separate Numeric array to get
realtime fire. I'll try to explain my methods here...

This flame algorithm is very popular, and you can find
it for just about all graphic libraries. The fire effect
works by placing random values on the bottom row of the
image. Then doing a simplish blur that is weighted to
move the values upward. Then slightly darken the image
so the colors get darker as they move up. The secret is
that the blur routine is "recursive" so when you blur
the 2nd row, the values there are used when you blur
the 3rd row, and all the way up.

This fire algorithm works great, but the bottom rows
are usually a bit ugly. In this demo we just render
a fire surface that has 3 extra rows at the bottom we
just don't use.

Also, the fire is rendered at half the resolution of
the full image. We then simply double the size of the
fire data before applying to the surface.

Several of these techniques are covered in the pygame
surfarray tutorial. doubling an image, and the fancy
blur is just a modified version of what is in the tutorial.

This runs at about 40fps on my celeron-400
"""


import pygame
from pygame.surfarray import *
from pygame.locals import *

from numpy import *
from numpy.random import random_integers as randint

import time

from oscPixelPusher import OscPixelPusher


RES = array((18,10))
displayScale = 16

MAX = 246
RESIDUAL = 86
HSPREAD, VSPREAD = 26, 78
VARMIN, VARMAX = -2, 3

def main():
    "main function called when the script is run"
    #first we just init pygame and create some empty arrays to work with    

    opp = OscPixelPusher()
    time.sleep(1)
    opp.setDisplayMode(RES, 8)
    time.sleep(1)
    cmap = setpalette()
    opp.setColormap(cmap.astype(uint8))

    flame = zeros(RES + (0,3), dtype=integer)  # array um 3 Zeilen erweitern
    print(flame.shape)

    flame[3:5,8:10] = randint(0, MAX, 4).reshape((2,2))
    #the mainloop is pretty simple, the work is done in these funcs
    while True:  #not pygame.event.peek((QUIT,KEYDOWN,MOUSEBUTTONDOWN)):
        modifyflamebase(flame)
        #flame[3:5,8:10] += randint(VARMIN, VARMAX, 4).reshape((2,2))
        #flame[:] = where(flame > MAX, 0, flame)

        #flame[4,8] += 32

        processflame(flame)
        #blitdouble(screen, flame, doubleflame)
        
        opp.updatePixmap(flame[:,:-3].astype(uint8))

        flame = clip(flame, 0, 255)
        # set_plasma_buffer(rgb, cmap, flame)
        # rgb.send()

        time.sleep(0.05)


def setpalette():
    "here we create a numeric array for the colormap"
    gstep, bstep = 75, 150
    cmap = zeros((256, 3))
    cmap[:,0] = minimum(arange(256)*3, 255)
    cmap[gstep:,1] = cmap[:-gstep,0]
    cmap[bstep:,2] = cmap[:-bstep,0]
    return cmap


def randomflamebase(flame):
    "just set random values on the bottom row"
    flame[:,-1] = randint(0, MAX, flame.shape[0])
    #print flame[:,-1]


def modifyflamebase(flame):
    "slightly change the bottom row with random values"
    bottom = flame[:,-1]
    mod = randint(VARMIN, VARMAX, bottom.shape[0])
    add(bottom, mod, bottom)
    maximum(bottom, 0, bottom)
    #if values overflow, reset them to 0
    #bottom[:] = choose(greater(bottom,MAX), (bottom,0))
    bottom[:] = where(bottom > MAX, 0, bottom)


def processflame(flame):
    "this function does the real work, tough to follow"
    notbottom = flame[:,:-1]  

    #first we multiply by about 60%
    multiply(notbottom, 146, notbottom)
    right_shift(notbottom, 8, notbottom)

    #work with flipped image so math accumulates.. magic!
    flipped = flame[:,::-1]

    #all integer based blur, pulls image up too
    tmp = flipped * 20
    right_shift(tmp, 8, tmp)
    tmp2 = tmp >> 1
    add(flipped[1:,:], tmp2[:-1,:], flipped[1:,:]) # blur unten
    add(flipped[:-1,:], tmp2[1:,:], flipped[:-1,:]) # blur oben
    add(flipped[1:,1:], tmp[:-1,:-1], flipped[1:,1:]) # blur rechts unten
    add(flipped[:-1,1:], tmp[1:,:-1], flipped[:-1,1:]) # blur rechts oben

    tmp = flipped * 80
    right_shift(tmp, 8, tmp)
    add(flipped[:,1:], tmp[:,:-1]>>1, flipped[:,1:])
    add(flipped[:,2:], tmp[:,:-2], flipped[:,2:])

    #make sure no values got too hot
    minimum(notbottom, MAX, notbottom)
    #notbottom = where(notbottom > MAX, MAX, notbottom)

if __name__ == '__main__': main()

    