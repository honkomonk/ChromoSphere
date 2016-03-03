import argparse

from pythonosc import dispatcher, osc_server

import numpy
import pygame

class OSCDisplay:
  def __init__(self, ip="127.0.0.1", port=1234, pixelScale = 16):
    self.ip = ip
    self.port = port
    self.pixelScale = pixelScale
    self.currentResolution = (16,16)
    self.currentBitMode = 8

    pygame.init()
    self.screen = pygame.display.set_mode(
      (self.currentResolution[0]*self.pixelScale, self.currentResolution[1]*self.pixelScale), 0, self.currentBitMode)  # default window ... mit beliebigen werten
    print(self.screen.get_flags())

    self.msgDispatcher = dispatcher.Dispatcher()
    self.msgDispatcher.set_default_handler(print)
    self.msgDispatcher.map("/debug", print)
    self.msgDispatcher.map("/setMode", self.setDisplayMode, "Set display resolution and bitmode")
    self.msgDispatcher.map("/colormap", self.setColormap, "Set colormap")
    self.msgDispatcher.map("/pixmap", self.updatePixelbuffer, "Update pixelbuffer using colormap")


  def runServer(self):
    self.server = osc_server.ThreadingOSCUDPServer((self.ip, self.port), self.msgDispatcher)
    print("Serving on {}".format(self.server.server_address))
    self.server.serve_forever()


  def setDisplayMode(self, addr, args, resX, resY, bitMode):
    print( "Set resolution to ({0}, {1}), Mode: {2}".format(resX, resY, bitMode))
    resolution = numpy.array((resX, resY))
    self.screen = pygame.display.set_mode(resolution*self.pixelScale, 0, bitMode)
    self.currentResolution = resolution
    self.currentBitMode = bitMode


  def setColormap(self, addr, args, cmapString):
    print("Setting color palette ")
    cmap = numpy.fromstring(cmapString, dtype=numpy.uint8)
    cmap = cmap.reshape((256,3))
    self.screen.set_palette(cmap)


  def updatePixelbuffer(self, addr, args, bufferString):
    pixelBuffer = numpy.fromstring(bufferString, dtype=numpy.uint8)
    
    #print(pixelBuffer.shape)
    #print(self.currentBitMode, self.currentResolution)
    if self.currentBitMode == 8:
      pixelBuffer = pixelBuffer.reshape(self.currentResolution)
    elif self.currentBitMode == 32:
      pixelBuffer = pixelBuffer.reshape(self.currentResolution + (3,))

    scaledDisplay = pixelBuffer.repeat(self.pixelScale, axis=0).repeat(self.pixelScale, axis=1)

    pygame.surfarray.blit_array(self.screen, scaledDisplay)
    pygame.display.flip()




if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("--ip",
      default="127.0.0.1", help="The ip to listen on")
  parser.add_argument("--port",
      type=int, default=1234, help="The port to listen on")
  args = parser.parse_args()

  pyGameDisplaySever = OSCDisplay(ip=args.ip, port=args.port)
  pyGameDisplaySever.runServer()