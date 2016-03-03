
from pythonosc import osc_message_builder
from pythonosc import udp_client

import numpy as np
from matplotlib import cm


def lutFromCmap(cmapName):
    cmap = cm.get_cmap(cmapName)
    lut = [cmap(i)[:-1] for i in range(256)]
    lutArray = (np.array(lut)*255).astype(np.uint8)
    return lutArray


class OscPixelPusher():
    def __init__(self, ip ="127.0.0.1", port=1234):
        self.client = udp_client.UDPClient(ip, port)


    def setLed(self, id, r, g, b):
        msgBuilder = osc_message_builder.OscMessageBuilder(address = "/pixmap")
        msgBuilder.add_arg(int(id), msgBuilder.ARG_TYPE_INT)
        msgBuilder.add_arg(int(r), msgBuilder.ARG_TYPE_INT)
        msgBuilder.add_arg(int(g), msgBuilder.ARG_TYPE_INT)
        msgBuilder.add_arg(int(b), msgBuilder.ARG_TYPE_INT)
        msg = msgBuilder.build()
        self.client.send(msg)
        pass

    def setDisplayMode(self, resolution, bitMode):
        msgBuilder = osc_message_builder.OscMessageBuilder(address = "/setMode")
        msgBuilder.add_arg(int(resolution[0]), msgBuilder.ARG_TYPE_INT)
        msgBuilder.add_arg(int(resolution[1]), msgBuilder.ARG_TYPE_INT)
        msgBuilder.add_arg(int(bitMode), msgBuilder.ARG_TYPE_INT)
        msg = msgBuilder.build()
        self.client.send(msg)

    def setColormap(self, colormap):
        msgBuilder = osc_message_builder.OscMessageBuilder(address = "/colormap")
        msgBuilder.add_arg(colormap.tostring(), msgBuilder.ARG_TYPE_BLOB)
        msg = msgBuilder.build()
        self.client.send(msg)

    def updatePixmap(self, pixmap):
        msgBuilder = osc_message_builder.OscMessageBuilder(address = "/pixmap")
        msgBuilder.add_arg(pixmap.tostring(), msgBuilder.ARG_TYPE_BLOB)
        msg = msgBuilder.build()
        self.client.send(msg)

    def debugMsg(self, message):
        msgBuilder = osc_message_builder.OscMessageBuilder(address = "/debug")
        msgBuilder.add_arg(message)
        msg = msgBuilder.build()
        self.client.send(msg)

    def toggleLed(self, on=True):
        pass
