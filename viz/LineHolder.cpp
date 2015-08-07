#include "LineHolder.hpp"

namespace vizkit3d_laser_line {

LineHolder::LineHolder()
{
    colors = new osg::Vec4Array();
    lineRangeVertices = new osg::Vec3Array();
    switchNode = new osg::Switch();
    showRange = false;
    lineWidth = 1.0f;

    setupNormalLine();
    setupLineRange();
}

LineHolder::~LineHolder()
{
}

void LineHolder::setupNormalLine()
{
    normalLineNode = new osg::Geode();

    normalLineDrawable = new osg::Geometry();
    normalLineDrawable->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES));

    normalLineNode->addDrawable(normalLineDrawable.get());

    setupStateSet(normalLineNode->getOrCreateStateSet());

    switchNode->addChild(normalLineNode.get(), true);
}

void LineHolder::setupLineRange()
{
    lineRangeNode = new osg::Geode();

    lineRangeDrawable = new osg::Geometry();
    lineRangeDrawable->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_FAN));

    lineRangeNode->addDrawable(lineRangeDrawable.get());

    setupStateSet(lineRangeNode->getOrCreateStateSet());

    switchNode->addChild(lineRangeNode, showRange);

}

void LineHolder::setupStateSet(osg::StateSet *stateset)
{
    stateset->setAttribute(new osg::LineWidth(lineWidth), osg::StateAttribute::ON);
    stateset->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
    stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
    stateset->setMode(GL_LINE_SMOOTH_HINT, GL_NICEST);
    stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    // Add a blend function.
    osg::ref_ptr<osg::BlendFunc> blend ( new osg::BlendFunc ( GL_ONE, GL_ONE) );
    stateset->setAttributeAndModes ( blend.get(), osg::StateAttribute::ON );

    // Set the hint.
    osg::ref_ptr<osg::Hint> hint ( new osg::Hint ( GL_LINE_SMOOTH_HINT, GL_NICEST ) );
    stateset->setAttributeAndModes ( hint.get(), osg::StateAttribute::ON );
}

osg::ref_ptr<osg::Node> LineHolder::getRootNode()
{
    return switchNode;
}

void LineHolder::update(osg::Vec3Array *vertices, osg::Vec4 color)
{

    colors->clear();
    colors->push_back(color);

    normalLineNode->getOrCreateStateSet()->setAttribute(new osg::LineWidth(lineWidth), osg::StateAttribute::ON);

    normalLineDrawable->setPrimitiveSet(0, new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, vertices->size()));
    normalLineDrawable->setVertexArray(vertices);

    normalLineDrawable->setColorBinding(osg::Geometry::BIND_OVERALL);
    normalLineDrawable->setColorArray(colors.get());

    if (showRange) {

        lineRangeVertices->clear();

        lineRangeVertices->push_back(osg::Vec3(0, 0, 0));

        for (int i = 0; i < vertices->size() / 2; i++) {
            lineRangeVertices->push_back((*vertices)[i * 2]);
        }

        lineRangeDrawable->setPrimitiveSet(0, new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_FAN, 0, lineRangeVertices->size()));
        lineRangeDrawable->setVertexArray(lineRangeVertices.get());

        lineRangeDrawable->setColorBinding(osg::Geometry::BIND_OVERALL);
        lineRangeDrawable->setColorArray(colors.get());
    }
}

void LineHolder::setShowRange(bool value)
{
    showRange = value;
    switchNode->setChildValue(lineRangeNode.get(), showRange);
}

bool LineHolder::isShowRange()
{
    return showRange;
}

void LineHolder::setLineWidth(float value)
{
    lineWidth = value;
}

float LineHolder::getLineWidth() {
    return lineWidth;
}

} /* namespace vizkit3d_laser_line */
