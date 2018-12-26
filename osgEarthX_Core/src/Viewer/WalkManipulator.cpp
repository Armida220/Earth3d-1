#include "WalkManipulator.h"

WalkManipulator::WalkManipulator(osgEarth::MapNode* mapNode):_mapNode(mapNode),_distance(2)
{}

bool WalkManipulator::handleKeyDown(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{

	if (inherited::handleKeyDown(ea, us))
		return true;

	if (ea.getKey() == 'w')
	{
		osg::Matrixd rotation_matrix;
		rotation_matrix.makeRotate(_rotation);

		osg::Vec3d sideVector = getSideVector(rotation_matrix);

		osg::Vec3d mapPos;
		osg::CoordinateFrame out_frame;
		_mapNode->getMapSRS()->transformFromWorld(_eye, mapPos);
		_mapNode->getMapSRS()->createLocalToWorld(mapPos, out_frame);
		osg::Vec3d localUp = getUpVector(out_frame);

		osg::Vec3d forwardVector = localUp^sideVector;
		forwardVector.normalize();
		osg::Vec3d dv = forwardVector * _distance;
		_eye += dv;
		//moveForward(_distance);
		return true;

	}
	else if (ea.getKey() == 'a')
	{
		moveRight(-_distance);
		return true;
	}

	else if (ea.getKey() == 's')
	{
		osg::Matrixd rotation_matrix;
		rotation_matrix.makeRotate(_rotation);

		osg::Vec3d sideVector = getSideVector(rotation_matrix);

		osg::Vec3d mapPos;
		osg::CoordinateFrame out_frame;
		_mapNode->getMapSRS()->transformFromWorld(_eye, mapPos);
		_mapNode->getMapSRS()->createLocalToWorld(mapPos, out_frame);
		osg::Vec3d localUp = getUpVector(out_frame);

		osg::Vec3d forwardVector = localUp^sideVector;
		forwardVector.normalize();
		osg::Vec3d dv = forwardVector * _distance;
		_eye -= dv;
		//moveForward(-_distance);
		return true;
	}

	else if (ea.getKey() == 'd')
	{
		moveRight(_distance);
		return true;
	}
	else if (ea.getKey() == 'q' || ea.getKey() == osgGA::GUIEventAdapter::KEY_Page_Up)
	{
		osg::Vec3d mapPos;
		osg::CoordinateFrame out_frame;
		_mapNode->getMapSRS()->transformFromWorld(_eye, mapPos);
		_mapNode->getMapSRS()->createLocalToWorld(mapPos, out_frame);
		osg::Vec3d localUp = getUpVector(out_frame);

		osg::Vec3d dv = localUp * _distance;
		_eye += dv;
		//moveUp(_distance);
		return true;
	}

	else if (ea.getKey() == 'e' || ea.getKey() == osgGA::GUIEventAdapter::KEY_Page_Down)
	{
		osg::Vec3d mapPos;
		osg::CoordinateFrame out_frame;
		_mapNode->getMapSRS()->transformFromWorld(_eye, mapPos);
		_mapNode->getMapSRS()->createLocalToWorld(mapPos, out_frame);
		osg::Vec3d localUp = getUpVector(out_frame);

		osg::Vec3d dv = localUp * _distance;
		_eye -= dv;
		//moveUp(-_distance);
		return true;
	}

	else if (ea.getKey() == 'z')
	{
		_distance++;
		return true;
	}

	else if (ea.getKey() == 'x')
	{
		_distance--;
		if (_distance < 1) _distance = 1;
		return true;
	}

	else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Left)
	{
		osg::CoordinateFrame coordinateFrame = getCoordinateFrame(_eye);
		osg::Vec3d localUp = getUpVector(coordinateFrame);
		float ang = _distance;
		rotateYawPitch(_rotation, osg::DegreesToRadians(-ang), 0, localUp);
	}

	else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Right)
	{
		osg::CoordinateFrame coordinateFrame = getCoordinateFrame(_eye);
		osg::Vec3d localUp = getUpVector(coordinateFrame);
		float ang = _distance;
		rotateYawPitch(_rotation, osg::DegreesToRadians(ang), 0, localUp);
	}

	else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Up)
	{
		osg::CoordinateFrame coordinateFrame = getCoordinateFrame(_eye);
		osg::Vec3d localUp = getUpVector(coordinateFrame);
		float ang = _distance;
		rotateYawPitch(_rotation, 0, osg::DegreesToRadians(ang),localUp);
	}

	else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Down)
	{
		osg::CoordinateFrame coordinateFrame = getCoordinateFrame(_eye);
		osg::Vec3d localUp = getUpVector(coordinateFrame);
		float ang = _distance;
		rotateYawPitch(_rotation, 0, osg::DegreesToRadians(-ang), localUp);
	}

	return false;
}

bool WalkManipulator::performMovementLeftMouseButton(const double /*eventTimeDelta*/, const double dx, const double dy)
{
	// world up vector
	osg::CoordinateFrame coordinateFrame = getCoordinateFrame(_eye);
	osg::Vec3d localUp = getUpVector(coordinateFrame);

	rotateYawPitch(_rotation, -dx, dy, localUp);

	return true;
}


bool WalkManipulator::performMouseDeltaMovement(const float dx, const float dy)
{
	// rotate camera
	if (getVerticalAxisFixed()) {

		// world up vector
		osg::CoordinateFrame coordinateFrame = getCoordinateFrame(_eye);
		osg::Vec3d localUp = getUpVector(coordinateFrame);

		rotateYawPitch(_rotation, -dx, dy, localUp);

	}
	else

		rotateYawPitch(_rotation, -dx, dy);

	return true;
}
