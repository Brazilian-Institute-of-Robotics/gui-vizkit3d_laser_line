#ifndef vizkit3d_laser_line_LaserLine_H
#define vizkit3d_laser_line_LaserLine_H

#include <boost/noncopyable.hpp>
#include <vizkit3d/Vizkit3DPlugin.hpp>
#include <base/samples/LaserScan.hpp>
#include <osg/Array>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>

namespace vizkit3d
{
    class LaserLine
        : public vizkit3d::Vizkit3DPlugin<base::samples::LaserScan>
        , boost::noncopyable
    {
    Q_OBJECT
    public:
        LaserLine();
        ~LaserLine();

    Q_INVOKABLE void updateData(base::samples::LaserScan const &sample)
    {vizkit3d::Vizkit3DPlugin<base::samples::LaserScan>::updateData(sample);}

    protected:
        virtual osg::ref_ptr<osg::Node> createMainNode();
        virtual void updateMainNode(osg::Node* node);
        virtual void updateDataIntern(base::samples::LaserScan const& plan);
        
    private:

        struct Data;
        Data* p;

        /**
         * Store the vertices used to draw the primitives
         */
        osg::ref_ptr<osg::Vec3Array> vertices;

        /**
         * Store the colors of the primitive
         */
        osg::ref_ptr<osg::Vec4Array> colors;

        /**
         * Used to draw a primitive (QUAD, TRIANGLE, LINE)
         */
        osg::ref_ptr<osg::Geometry> geometry;

        osg::ref_ptr<osg::DrawArrays> primitiveSet;

    };
}
#endif
