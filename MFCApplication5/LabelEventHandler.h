#pragma once

#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include <osgUtil/LineSegmentIntersector>
#include <osgEarth/MapNode>
#include <osgEarthUtil/Controls>
#include <osgEarth/TerrainEngineNode>
#include <osgEarth/GeoData>
#include<osgEarthUtil/EarthManipulator>

class CLabelEventHandler :
	public osgGA::GUIEventHandler
{
public:
	CLabelEventHandler(osgEarth::Util::Controls::LabelControl* viewLabel,
		osgEarth::Util::Controls::LabelControl* mouseLabel,
		osgEarth::MapNode* mm);
	~CLabelEventHandler();

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
private:
	osgEarth::Util::Controls::LabelControl* m_ViewLabel;
	osgEarth::Util::Controls::LabelControl* m_MouseLabel;
	osgEarth::MapNode* m_MapNode;
	osg::NodePath m_NodePath;
	
};

