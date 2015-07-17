#include <iostream>
#include "LaserLine.hpp"

using namespace vizkit3d;

LaserLine::LaserLine() : showRange(false),
                        color(0, 255, 0, 255)
{
    vertices = new osg::Vec3Array();
    colors = new ::osg::Vec4Array();
    geometry = new osg::Geometry();
    primitiveSet = new osg::DrawArrays();
}

LaserLine::~LaserLine()
{
}

osg::ref_ptr<osg::Node> LaserLine::createMainNode()
{
    /**
     * set up the line style
     */
    geometry->getOrCreateStateSet()->setAttribute(new osg::LineWidth(3.5f), osg::StateAttribute::ON);
    geometry->getOrCreateStateSet()->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
    geometry->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
    geometry->getOrCreateStateSet()->setMode(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // Add a blend function.
    osg::ref_ptr<osg::BlendFunc> blend ( new osg::BlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
    geometry->getOrCreateStateSet()->setAttributeAndModes ( blend.get(), osg::StateAttribute::ON );

    // Set the hint.
    osg::ref_ptr<osg::Hint> hint ( new osg::Hint ( GL_LINE_SMOOTH_HINT, GL_NICEST ) );
    geometry->getOrCreateStateSet()->setAttributeAndModes ( hint.get(), osg::StateAttribute::ON );


    /**
     * add the primitive set to geometry
     */
    geometry->addPrimitiveSet(primitiveSet);

    /**
     * set line colors
     */
    colors->push_back(osg::Vec4(color.red()  / 255.0, color.green()  / 255.0, color.blue()  / 255.0, color.alpha()  / 255.0));

    osg::ref_ptr<osg::Geode> geode(new osg::Geode());
    geode->addDrawable(geometry.get());

    return geode.get();
}

void LaserLine::updateMainNode ( osg::Node* node )
{
    osg::Geode* geode = static_cast<osg::Geode*>(node);

    //laser scan depth values
    std::vector<uint32_t> ranges = data.ranges;

    double angle = data.start_angle; //start angle
    double angleStep = data.angular_resolution; //angle step
    double maxRange = data.maxRange / 1000.0; //convert millimeters to meters

    vertices->clear();

    if (showRange) {
        vertices->push_back(osg::Vec3f(0, 0, 0));
        for (int i = 0; i < ranges.size(); i++) {
            Eigen::Vector3d limit = Eigen::Quaterniond(Eigen::AngleAxisd(angle + (i * angleStep), Eigen::Vector3d::UnitZ())) * Eigen::Vector3d(maxRange, 0, 0.0);
            Eigen::Vector3d pt = Eigen::Quaterniond(Eigen::AngleAxisd(angle + (i * angleStep), Eigen::Vector3d::UnitZ())) * Eigen::Vector3d((ranges[i]) / 1000.0, 0, 0.0);
            vertices->push_back(osg::Vec3f(pt[0], pt[1], pt[2]));
        }

        //update line vertices
        primitiveSet->set(osg::PrimitiveSet::TRIANGLE_FAN, 0, vertices->size());
    }
    else {

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
                if (abs(pt0[0] - pt1[0]) < 0.01 && (pt0[0] <  limit[0] && pt1[0] < limit[0])) {
                    vertices->push_back(osg::Vec3f(pt0[0], pt0[1], pt0[2]));
                    vertices->push_back(osg::Vec3f(pt1[0], pt1[1], pt1[2]));
                }
            }
        }

        //update line vertices
        primitiveSet->set(osg::PrimitiveSet::LINES, 0, vertices->size());
    }
    geometry->setVertexArray(vertices.get());

    //update line colors
    geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
    geometry->setColorArray(colors.get());
}

void LaserLine::updateDataIntern(base::samples::LaserScan const& value)
{
    data = value;
}

bool LaserLine::isShowRange() const {
    return showRange;
}
void LaserLine::setShowRange(bool value) {
    showRange = value;
}

QColor LaserLine::getColor() {
    return color;
}

void LaserLine::setColor(QColor value) {
    color = value;
    colors->clear();
    colors->push_back(osg::Vec4(color.red() / 255.0, color.green()  / 255.0, color.blue()  / 255.0, color.alpha()  / 255.0));
}


//Macro that makes this plugin loadable in ruby, this is optional.
VizkitQtPlugin(LaserLine)


