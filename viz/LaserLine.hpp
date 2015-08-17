#ifndef vizkit3d_laser_line_LaserLine_H
#define vizkit3d_laser_line_LaserLine_H

#include <boost/noncopyable.hpp>
#include <vizkit3d/Vizkit3DPlugin.hpp>
#include <base/samples/LaserScan.hpp>
#include <osg/Array>
#include <osg/Geode>
#include <QStringList>
#include "LineHolder.hpp"

namespace vizkit3d {

class LaserLine
    : public vizkit3d::Vizkit3DPlugin<base::samples::LaserScan>
    , boost::noncopyable
{
    Q_OBJECT

public:
    Q_INVOKABLE
    void updateData(base::samples::LaserScan const &sample) {
        vizkit3d::Vizkit3DPlugin<base::samples::LaserScan>::updateData(sample);
    }

private:
    Q_PROPERTY( QColor color READ getColor WRITE setColor )
    Q_PROPERTY( bool show_range READ isShowRange WRITE setShowRange )
    Q_PROPERTY( double line_width READ getLineWidth WRITE setLineWidth )
    Q_PROPERTY( bool show_all_lines READ getShowAllLines WRITE setShowAllLines )

public:
    LaserLine();
    ~LaserLine();

    bool isShowRange();
    void setShowRange(bool showRange);

    QColor getColor();
    void setColor(QColor color);

    double getLineWidth();
    void setLineWidth(double lineWidth);

    bool getShowAllLines();
    void setShowAllLines(bool value);

protected:
    virtual osg::ref_ptr<osg::Node> createMainNode();
    virtual void updateMainNode(osg::Node* node);
    virtual void updateDataIntern(base::samples::LaserScan const& plan);

private:

    bool loadLineVerticesFromLaserScan(base::samples::LaserScan const& laserScan);

    /**
     * compute the points and set its validity
     */
    osg::ref_ptr<osg::Vec3Array> computePoints(base::samples::LaserScan const& laserScan, std::vector<bool>& validity);

    /**
     * compute the lines
     */
    void computeLines(osg::Vec3Array& lineVertices, osg::Vec3Array const& points, std::vector<bool> const& validity);

    /**
     * Store the vertices used to draw the primitives
     */
    osg::ref_ptr<osg::Vec3Array> vertices;

    /**
     * Used to manage the line setup and creation
     */
    vizkit3d_laser_line::LineHolder lineHolder;

    /**
     * Store the line color
     */
    osg::Vec4 color;

    /**
     * if true, show all lines
     */
    bool showAllLines;
};

}
#endif
