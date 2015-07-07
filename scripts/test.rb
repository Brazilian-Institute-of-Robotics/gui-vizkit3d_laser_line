#! /usr/bin/env ruby
require 'vizkit'
require 'orocos'

Orocos.initialize

view3d = Vizkit.vizkit3d_widget
view3d.setGrid(false)
view3d.setTransformer(false)
view3d.setAxes(false)
laser_line = Vizkit.default_loader.LaserLine
view3d.show
Vizkit.exec