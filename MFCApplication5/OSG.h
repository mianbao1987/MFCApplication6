#pragma once

#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osgViewer/api/win32/GraphicsWindowWin32>
#include <osgGA/TrackballManipulator>
#include <osgEarth/MapNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/Sky>
#include <osgEarth/ModelLayer>
#include<osgEarthUtil/EarthManipulator>
#include <osgEarthAnnotation/PlaceNode>

#include<osgEarthUtil/Controls>
#include <osgEarth/Registry>

#include <osg/Depth>
#include <osg/AnimationPath>
#include <osgEarth/MapNode>

#include "LabelEventHandler.h"

class COSG
{
public:
	COSG(HWND hWnd);
	~COSG();


	void InitOSG(RECT relative);
	void InitScenceGraph();
	void InitCamerarConfig();
	void PreFrameUpdate();
	void PostFrameUpdate();
	osgViewer::Viewer* GetViewer();
	void InitOsgEarth();
	static void Render(void *ptr);
	void ExitOSG();

	osgEarth::ModelLayer* m_ChinaBound;
private:
	HWND m_hWnd;
	RECT m_RelativeRect;
	osg::ref_ptr<osgViewer::Viewer> m_pViewr;
	osg::ref_ptr<osg::Group> m_pRoot;
	osg::ref_ptr<osgEarth::MapNode> m_mapNode;
	osg::ref_ptr<osgEarth::Util::EarthManipulator> m_em;
	BOOL m_IsRender;

	osgEarth::Annotation::PlaceNode* m_pPlaceNode;
	osg::ref_ptr<osg::Group> m_EarthLabel;

	CLabelEventHandler* m_pLabelEventHandler;
public:
	void SetChinaBouderyOptacity(double opt);
	void AddViewPointLabel();
	void FlyGo(float jing, float wei, float height);

	osg::ref_ptr<osg::Node> m_earth;

public:
	osg::ref_ptr<osg::CoordinateSystemNode> m_csn;
	osg::ref_ptr<osg::Node> m_AirPort;
	osg::ref_ptr<osg::MatrixTransform> m_mtAirPortSelf;
	osg::ref_ptr<osg::MatrixTransform> m_mtAriPort;
public:
	void AddAirPort();

	osg::ref_ptr<osg::Group> m_wuti;
	osg::ref_ptr<osg::CoordinateSystemNode> m_csn2;
	osg::ref_ptr<osg::Node> m_fly;
	osg::ref_ptr<osg::MatrixTransform> m_mtFlySelf;
	osg::ref_ptr<osg::MatrixTransform> m_mtFly;


	//预设路径飞行
public:
	osg::ref_ptr<osg::AnimationPath>  CreateFlyLinePath(osg::Vec4Array* ctrl);

	//两点间的距离公式
	double GetDis(osg::Vec3 from, osg::Vec3 to);

	//计算2点之间时间

	double GetRunTime(osg::Vec3 from, osg::Vec3 to,double speed);


	//预设路径

	void DoPreLine();

	//生成的预设路径
	osg::ref_ptr<osg::AnimationPath> m_apc;


	void DoPreLineNow();

	//TEST
	osg::Node* createPlane(osg::Node* node, const osgEarth::GeoPoint& pos, const osgEarth::SpatialReference* mapSRS, double radius, double time);
	osg::AnimationPath*  createAnimationPath(const osgEarth::GeoPoint& pos, const osgEarth::SpatialReference* mapSRS, float radius, double looptime);
};

