#include <iostream>
#include "LaserLine.hpp"

using namespace vizkit3d;
using namespace vizkit3d_laser_line;

LaserLine::LaserLine()
{
    color = osg::Vec4(0.0, 1.0, 0.0, 0.0);
    vertices = new osg::Vec3Array();
    lineHolder = new LineHolder();
    showAllLines = false;
}

LaserLine::~LaserLine()
{
}

osg::ref_ptr<osg::Node> LaserLine::createMainNode()
{
    return lineHolder->getRootNode();
}

void LaserLine::updateMainNode (osg::Node* node)
{
    lineHolder->update(vertices, color);
}

void LaserLine::updateDataIntern(base::samples::LaserScan const& value)
{
    loadLineVerticesFromLaserScan(value);
}

bool LaserLine::isShowRange() const
{
    return lineHolder->isShowRange();
}
void LaserLine::setShowRange(bool value)
{
    lineHolder->setShowRange(value);
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
    return (double)lineHolder->getLineWidth();
}

void LaserLine::setLineWidth(double lineWidth)
{
    lineHolder->setLineWidth((float)lineWidth);
}

bool LaserLine::getShowAllLines()
{
    return showAllLines;
}

void LaserLine::setShowAllLines(bool value)
{
    showAllLines = value;
}

bool LaserLine::loadLineVerticesFromLaserScan(base::samples::LaserScan const& laserScan)
{
    std::vector<uint32_t> ranges = laserScan.ranges;

    double angle = laserScan.start_angle; //start angle
    double angleStep = laserScan.angular_resolution; //angle step
    double maxRange = laserScan.maxRange / 1000.0; //convert millimeters to meters

    vertices->clear();

    if (!ranges.empty() && ranges.size() >= 2) {


        for (int i = 0; i < ranges.size()-1; i++) {

            //get the max range point
            Eigen::Vector3d limit = Eigen::Quaterniond(Eigen::AngleAxisd(angle + (i * angleStep), Eigen::Vector3d::UnitZ())) * Eigen::Vector3d(maxRange, 0, 0.0);

            /**
             * get the start and end line points
             * compute the rotation using the angle
             */
            Eigen::Vector3d pt0 = Eigen::Quaterniond(Eigen::AngleAxisd(angle + (i * angleStep), Eigen::Vector3d::UnitZ())) * Eigen::Vector3d((ranges[i]) / 1000.0, 0, 0.0);
            Eigen::Vector3d pt1 = Eigen::Quaterniond(Eigen::AngleAxisd(angle + ((i+1) * angleStep), Eigen::Vector3d::UnitZ())) * Eigen::Vector3d((ranges[i+1]) / 1000.0, 0, 0.0);

            /**
             * Filter occlusion points and points with values equal the max range
             */
            if ((showAllLines) || (abs(pt0[0] - pt1[0]) < 0.1 && (pt0[0] <  limit[0] && pt1[0] < limit[0]))) {
                vertices->push_back(osg::Vec3f(pt0[0], pt0[1], pt0[2]));
                vertices->push_back(osg::Vec3f(pt1[0], pt1[1], pt1[2]));
            }
        }

        return true;
    }

    return false;
}


//Macro that makes this plugin loadable in ruby, this is optional.
VizkitQtPlugin(LaserLine)


