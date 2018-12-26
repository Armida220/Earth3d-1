#pragma once

#include <osgGA/FirstPersonManipulator>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include <OSGEarth/MapNode>

class WalkManipulator : public osgGA::FirstPersonManipulator
{
	typedef osgGA::FirstPersonManipulator inherited;

public:

	WalkManipulator(osgEarth::MapNode* mapNode);
protected:
	virtual bool handleKeyDown(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
	virtual bool performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy);
	virtual bool performMouseDeltaMovement(const float dx, const float dy);

	int _distance;
	osg::ref_ptr<osgEarth::MapNode> _mapNode;
};
