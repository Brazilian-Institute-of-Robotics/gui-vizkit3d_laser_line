#include <iostream>
#include "LaserLine.hpp"

using namespace vizkit3d;

struct LaserLine::Data {
    // Copy of the value given to updateDataIntern.
    // Making a copy is required because of how OSG works
    base::samples::LaserScan data;
};


LaserLine::LaserLine()
    : p(new Data)
{
}

LaserLine::~LaserLine()
{
    delete p;
}

osg::ref_ptr<osg::Node> LaserLine::createMainNode()
{
    geometry = new osg::Geometry();

    vertices = new osg::Vec3Array();

    primitiveSet = new osg::DrawArrays();

    colors = new ::osg::Vec4Array();

    geometry->getOrCreateStateSet()->setAttribute(new osg::LineWidth(2.0f), osg::StateAttribute::ON);
    geometry->addPrimitiveSet(primitiveSet);

    osg::ref_ptr<osg::Geode> geode(new osg::Geode());
    geode->addDrawable(geometry.get());

    return geode.get();
}

void LaserLine::updateMainNode ( osg::Node* node )
{
    osg::Geode* geode = static_cast<osg::Geode*>(node);

    std::vector<Eigen::Vector3d> points;
    p->data.convertScanToPointCloud<Eigen::Vector3d>(points, Eigen::Affine3d::Identity(), false);

    vertices->clear();
    for (int i = 0; i < points.size(); i++) {
        vertices->push_back(osg::Vec3(points[i][0], points[i][1], points[i][2]));
    }

    for (int i = 0; i < vertices->size(); i++) colors->push_back(osg::Vec4(0.1, 0.9, 0.1, 1.0));

    primitiveSet->set(osg::PrimitiveSet::LINE_STRIP, 0, vertices->size());
    geometry->setVertexArray(vertices.get());

    geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    geometry->setColorArray(colors.get());
}

void LaserLine::updateDataIntern(base::samples::LaserScan const& value)
{
    p->data = value;
}

//Macro that makes this plugin loadable in ruby, this is optional.
VizkitQtPlugin(LaserLine)


