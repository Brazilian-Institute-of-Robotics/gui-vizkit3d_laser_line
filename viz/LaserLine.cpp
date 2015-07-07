#include <iostream>
#include <osg/Array>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/LineWidth>
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
    /**
     * Used to draw a primitive (QUAD, TRIANGLE, LINE)
     */
    osg::ref_ptr<osg::Geometry> geometry(new osg::Geometry());

    /**
     * Store the vertices used to draw the primitives
     */
    osg::ref_ptr<osg::Vec3Array> vertices(new osg::Vec3Array());

    /**
     * Store the colors of the primitive
     */
    osg::ref_ptr<osg::Vec4Array> colors(new ::osg::Vec4Array());


    /**
     * left line vertices
     */
    vertices->push_back(osg::Vec3(0, 0, 0));
    vertices->push_back(osg::Vec3(1, 0, 0));

    /**
     * right line vertices
     */
    vertices->push_back(osg::Vec3(1, 1, 0));
    vertices->push_back(osg::Vec3(0, 1, 0));

    /**
     * back line vertices
     */
    vertices->push_back(osg::Vec3(0, 0, 0));
    vertices->push_back(osg::Vec3(0, 1, 0));

    /**
     * front line vertices
     */
    vertices->push_back(osg::Vec3(1, 0, 0));
    vertices->push_back(osg::Vec3(1, 1, 0));

    geometry->setVertexArray(vertices.get());

    /**
     * Define primite properties
     */
    geometry->getOrCreateStateSet()->setAttribute(new osg::LineWidth(2.0f), osg::StateAttribute::ON);
    geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, vertices->size()));

    /**
     * Fill the primitive colors
     */
    for (int i = 0; i < vertices->size(); i++) {
        colors->push_back(osg::Vec4(0.9, 0.1, 0.1, 1.0));
    }


    /**
     * set the colors
     */
    geometry->setColorArray(colors.get());
    geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

    osg::ref_ptr<osg::Geode> geode(new osg::Geode());
    geode->addDrawable(geometry.get());

    return geode.get();
}

void LaserLine::updateMainNode ( osg::Node* node )
{
    osg::Geode* geode = static_cast<osg::Geode*>(node);
    // Update the main node using the data in p->data
}

void LaserLine::updateDataIntern(base::samples::LaserScan const& value)
{
    p->data = value;
//    std::cout << "got new sample data" << std::endl;
}

//Macro that makes this plugin loadable in ruby, this is optional.
VizkitQtPlugin(LaserLine)

