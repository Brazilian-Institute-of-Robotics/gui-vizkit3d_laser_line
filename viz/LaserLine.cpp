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
    return new osg::Geode();
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

