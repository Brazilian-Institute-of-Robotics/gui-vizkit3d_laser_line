#ifndef vizkit3d_laser_line_LaserLine_H
#define vizkit3d_laser_line_LaserLine_H

#include <boost/noncopyable.hpp>
#include <vizkit3d/Vizkit3DPlugin.hpp>
#include <base/samples/LaserScan.hpp>
#include <osg/PositionAttitudeTransform>
#include <osg/Geode>
#include <osg/Geometry>

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
        osg::ref_ptr<osg::PositionAttitudeTransform> linePos;

        struct Data;
        Data* p;
    };
}
#endif
