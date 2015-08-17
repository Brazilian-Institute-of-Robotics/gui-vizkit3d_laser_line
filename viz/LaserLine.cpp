#include <iostream>
#include "LaserLine.hpp"

using namespace vizkit3d;
using namespace vizkit3d_laser_line;


static inline double milimeters_to_meters(double x)  { return x / 1000.0; }

LaserLine::LaserLine()
    : color(osg::Vec4(0.0, 1.0, 0.0, 0.0))
    , vertices(new osg::Vec3Array())
    , showAllLines(false)
{
}

LaserLine::~LaserLine()
{
}

osg::ref_ptr<osg::Node> LaserLine::createMainNode()
{
    return lineHolder.getRootNode();
}

void LaserLine::updateMainNode (osg::Node* node)
{
    lineHolder.update(vertices, color);
}

void LaserLine::updateDataIntern(base::samples::LaserScan const& value)
{
    loadLineVerticesFromLaserScan(value);
}

bool LaserLine::isShowRange()
{
    return lineHolder.isShowRange();
}
void LaserLine::setShowRange(bool value)
{
    lineHolder.setShowRange(value);
}

QColor LaserLine::getColor()
{
    QColor qcolor;
    qcolor.setRgbF(color[0], color[1], color[2], color[3]);
    return qcolor;
}

void LaserLine::setColor(QColor value)
{
    if (value.isValid()) {
        color.set(value.redF(),
                  value.greenF(),
                  value.blueF(),
                  value.alphaF());
    }
}

double LaserLine::getLineWidth()
{
    return (double)lineHolder.getLineWidth();
}

void LaserLine::setLineWidth(double lineWidth)
{
    lineHolder.setLineWidth((float)lineWidth);
}

bool LaserLine::getShowAllLines()
{
    return showAllLines;
}

void LaserLine::setShowAllLines(bool value)
{
    showAllLines = value;
}

osg::ref_ptr<osg::Vec3Array> LaserLine::computePoints(base::samples::LaserScan const& laserScan, std::vector<bool>& validity)
{
    osg::Vec3Array *points = new osg::Vec3Array();

    std::vector<uint32_t> ranges = laserScan.ranges;

    double angle = laserScan.start_angle; //start angle
    double angleStep = laserScan.angular_resolution; //angle step

    for (int i = 0; i < ranges.size(); i++) {

        //compute the angle rotation
        Eigen::Quaterniond rot = Eigen::Quaterniond(Eigen::AngleAxisd(angle + (i * angleStep), Eigen::Vector3d::UnitZ()));
        Eigen::Vector3d pt =  rot * Eigen::Vector3d(milimeters_to_meters(ranges[i]), 0.0, 0.0);

        //laser line point
        points->push_back(osg::Vec3f(pt[0], pt[1], pt[2]));

        //check if range is valid
        validity.push_back(laserScan.isRangeValid(ranges[i]));
    }

    return points;
}

void LaserLine::computeLines(osg::Vec3Array& lineVertices, osg::Vec3Array const& points, std::vector<bool> const& validity) {

    const double maxDt = 2.5;

    lineVertices.clear();

    for (int i = 0; i < points.size()-1; i++) {

        if (!validity[i+0] || !validity[i+1]){
            continue;
        }

        //if the distance is bigger than max distance, then this line is dropped
        double dt = (points[i+0] - points[i+1]).length();

        if (dt > maxDt) {
            continue;
        }

        lineVertices.push_back(points[i+0]);
        lineVertices.push_back(points[i+1]);
    }

}

bool LaserLine::loadLineVerticesFromLaserScan(base::samples::LaserScan const& laserScan)
{
    if (laserScan.ranges.empty() || laserScan.ranges.size() < 2) {
        return false;
    }

    std::vector<bool> validity;
    osg::ref_ptr<osg::Vec3Array> points = computePoints(laserScan, validity);

    computeLines(*vertices, *points, validity);

    return true;
}


//Macro that makes this plugin loadable in ruby, this is optional.
VizkitQtPlugin(LaserLine)


