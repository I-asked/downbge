from __future__ import absolute_import
import bpy
camera = bpy.context.edit_movieclip.tracking.camera

camera.sensor_width = 23.76
camera.units = 'MILLIMETERS'
camera.pixel_aspect = 1
camera.k1 = 0.0
camera.k2 = 0.0
camera.k3 = 0.0

