#ifndef GUI_VIZKIT3D_LASER_LINE_VIZ_LINEHOLDER_HPP_
#define GUI_VIZKIT3D_LASER_LINE_VIZ_LINEHOLDER_HPP_

#include <osg/Array>
#include <osg/BlendFunc>
#include <osg/Hint>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osg/Switch>
#include <osg/PrimitiveSet>

namespace vizkit3d_laser_line {

/**
 * This class is used to manage the creation and configuration of lines
 *
 * It can be used in the future to add more types of lines, such as, a glowing line or volumetric line
 *
 */
class LineHolder {

public:

    LineHolder();
    virtual ~LineHolder();

    /**
     * returns the line root node
     * @return Line root node
     */
    osg::ref_ptr<osg::Node> getRootNode();

    /**
     * Enable/disable the showing line range
     * @param[in]   value Set true to show line range
     */
    void setShowRange(bool value);

    /**
     * @return true if is line range is showing
     */
    bool isShowRange();

    /**
     * set the line width
     *
     * @param[in]   lineWidth The line width
     */
    void setLineWidth(float lineWidth);

    /**
     * @return The line width
     */
    float getLineWidth();

    /**
     * Update line data values
     *
     * @param[in]   vertices The lines vertices
     * @param[in]   color The lines color
     */
    void update(osg::Vec3Array *vertices, osg::Vec4 color);


private:

    /**
     * configure the normal line
     */
    void setupNormalLine();

    /**
     * configure the line range
     */
    void setupLineRange();

    /**
     * configure Node and Object's mode and attributes
     */
    void setupStateSet(osg::StateSet *stateset);

    /**
     * Switch node used to show and hide the line range node
     */
    osg::ref_ptr<osg::Switch> switchNode;

    /**
     * Node used to perform the line geometric operations
     */
    osg::ref_ptr<osg::Geode> normalLineNode;

    /**
     * Used to draw the line
     */
    osg::ref_ptr<osg::Geometry> normalLineDrawable;

    /**
     * Node used to perform the line range geometric operations
     */
    osg::ref_ptr<osg::Geode> lineRangeNode;

    /**
     * Used to draw the line range
     */
    osg::ref_ptr<osg::Geometry> lineRangeDrawable;

    /**
     * Used to store the range vertices
     */
    osg::ref_ptr<osg::Vec3Array> lineRangeVertices;

    /**
     * Array used to store the line color
     */
    osg::ref_ptr<osg::Vec4Array> colors;

    /**
     * Enable / Disable the line range showing
     */
    bool showRange;

    /**
     * The line width
     */
    float lineWidth;
};

} /* namespace vizkit3d_laser_line */

#endif /* GUI_VIZKIT3D_LASER_LINE_VIZ_LINEHOLDER_HPP_ */
