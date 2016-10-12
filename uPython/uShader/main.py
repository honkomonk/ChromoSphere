from machine import Timer, Pin
from utime import ticks_ms
from neopixel import NeoPixel
from func_tables import sin_table, cos_table, gamma_table
from color_tables import cm_inferno as cm


cycle_time = 100
freq = 1

# NeoPixel setup
np_ledcount = 16
np_pin = Pin(0, Pin.OUT)
np = NeoPixel(np_pin, np_ledcount)

np_ledpos_x = bytearray([  0,  15,  31,  47,  63,  79,  95, 111, 127, 143, 159, 175, 191, 207, 223, 239])
np_ledpos_y = bytearray([127, 143, 159, 175, 191, 207, 223, 239,   0,  15,  31,  47,  63,  79,  95, 111])

# apply gamma correction to the colormap
cmgc = bytearray([gamma_table[v] for v in cm])

# cache object
buf = np.buf
buf_write = np.write

# define refresh function for shader
# vr, vg, vb = 0, 0, 0
# def sin_shader(t): 
#     c = ticks_ms() // cycle_time
#     for n in range(np_ledcount):
#         vb = sin_table[((c + np_ledpos_x[n]) * freq) & 0xff]
#         vb = gamma_table[vb & 0xff]
#         vr = sin_table[((c + np_ledpos_y[n]) * freq) & 0xff]
#         vr = gamma_table[vr & 0xff]
#         # np[n] = (vr,0,vb)

#         buf[n * 3] = vg
#         buf[n * 3 + 1] = vr
#         buf[n * 3 + 2] = vb
#         buf_write()

v = 0

# @micropython.viper
def colormap_shader(t): 
    c = ticks_ms() // cycle_time
    for n in range(np_ledcount):
        v = sin_table[((c + np_ledpos_x[n]) * freq) & 0xff]
        # v = gamma_table[v & 0xff]

        buf[n * 3] = cmgc[v * 3 + 1]
        buf[n * 3 + 1] = cmgc[v * 3]
        buf[n * 3 + 2] = cmgc[v * 3 + 2]
        buf_write()

shader_update = colormap_shader

shader_valid = False
try:
    shader_update(0)
    shader_valid = True
except Exception as ex:
    print("Error in shader_update() routine!")
    raise ex


# start timer for shader update
s_timer = Timer(-1)
if shader_valid:
    s_timer.init(period=20, mode=Timer.PERIODIC, callback=shader_update)


# just a helper function to stop the timer
def stop():
    s_timer.deinit()