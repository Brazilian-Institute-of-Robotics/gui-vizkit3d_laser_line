#! /usr/bin/env ruby
require 'vizkit'
require 'orocos'

include Orocos

Orocos.initialize
Orocos.load_typekit 'base'

laserScanSample = Types::Base::Samples::LaserScan.new

startAngle = -Math::PI
angleStep = Math::PI / 50
amp = 5

laserScanSample.start_angle = -Math::PI
laserScanSample.angular_resolution = 0.001
laserScanSample.minRange = 1000
laserScanSample.maxRange = 8000

laserScanSample.ranges = []

numberOfSamples = 1000

num = 0
theta = -Math::PI

#
# generate a test line
#
while num <= numberOfSamples
    range = 250 * (Math.sin(theta) + 1) + 2000
    laserScanSample.ranges << range.round
    theta += angleStep
    num = num + 1
end

view3d = Vizkit.vizkit3d_widget
laser_line = Vizkit.default_loader.LaserLine
laser_line.show_all_lines = true

timer = Qt::Timer.new
timer.connect(SIGNAL('timeout()')) do
    laser_line.updateData(laserScanSample);
end


timer.start(10)

view3d.show
Vizkit.exec