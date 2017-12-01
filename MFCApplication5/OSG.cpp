#include "stdafx.h"
#include "OSG.h"
#include "MainFrm.h"
#include <math.h>

#ifdef DEBUG
	#pragma comment(lib,"OpenThreadsd.lib")
	#pragma comment(lib,"osgd.lib")
	#pragma comment(lib,"osgDBd.lib")
	#pragma comment(lib,"osgFXd.lib")
	#pragma comment(lib,"osgGAd.lib")
	#pragma comment(lib,"osgManipulatord.lib")
	#pragma comment(lib,"osgParticled.lib")
	#pragma comment(lib,"osgShadowd.lib")
	#pragma comment(lib,"osgSimd.lib")
	#pragma comment(lib,"osgTerraind.lib")
	#pragma comment(lib,"osgTextd.lib")
	#pragma comment(lib,"osgUtild.lib")
	#pragma comment(lib,"osgViewerd.lib")

	#pragma comment(lib,"osgEarthd.lib")
	#pragma comment(lib,"osgEarthUtild.lib")
	#pragma comment(lib,"osgEarthAnnotationd.lib") //地名标注
	#pragma comment(lib,"osgEarthSymbologyd.lib")//地名标注
#else
	#pragma comment(lib,"OpenThreads.lib")
	#pragma comment(lib,"osg.lib")
	#pragma comment(lib,"osgDB.lib")
//	#pragma comment(lib,"osgFX.lib")
	#pragma comment(lib,"osgGA.lib")
//	#pragma comment(lib,"osgManipulator.lib")
//	#pragma comment(lib,"osgParticle.lib")
//	#pragma comment(lib,"osgShadow.lib")
//	#pragma comment(lib,"osgSim.lib")
//	#pragma comment(lib,"osgTerrain.lib")
//	#pragma comment(lib,"osgText.lib")
	#pragma comment(lib,"osgUtil.lib")
	#pragma comment(lib,"osgViewer.lib")

	#pragma comment(lib,"osgEarth.lib")
	#pragma comment(lib,"osgEarthUtil.lib")
	#pragma comment(lib,"osgEarthAnnotation.lib")//地名标注
	#pragma comment(lib,"osgEarthSymbology.lib")//地名标注
#endif




COSG::COSG(HWND hwnd) : m_IsRender(FALSE) , m_pLabelEventHandler(NULL)
{
	m_hWnd = hwnd;

}


COSG::~COSG()
{

}


void COSG::InitOSG(RECT rect)
{
	m_RelativeRect = rect;
	m_IsRender = TRUE;

	InitScenceGraph();
	InitCamerarConfig();
	InitOsgEarth();
}

void COSG::InitScenceGraph()
{
	m_pRoot = new osg::Group;
	m_earth = osgDB::readNodeFile("../earthfile/globel.earth");
	m_pRoot->addChild(m_earth);
	m_mapNode = dynamic_cast<osgEarth::MapNode*>(m_earth.get());//osgEarth::MapNode::findMapNode(m_earth);;// 

	m_EarthLabel = new osg::Group;;
	m_pRoot->addChild(m_EarthLabel);

	m_wuti = new osg::Group;;
	m_pRoot->addChild(m_wuti);




}

void COSG::InitCamerarConfig()
{
	RECT rect;
	::GetWindowRect(m_hWnd, &rect);

	rect.left += m_RelativeRect.left;
	rect.right += m_RelativeRect.left;
	rect.top += m_RelativeRect.bottom - m_RelativeRect.top;
	rect.bottom += m_RelativeRect.bottom - m_RelativeRect.top;

	m_pViewr = new osgViewer::Viewer();
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData(m_hWnd);
	traits->x = rect.left;
	traits->y = rect.top;
	traits->width = rect.right - rect.left;
	traits->height = rect.bottom -rect.top;
	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
	traits->setInheritedWindowPixelFormat = true;  
	traits->inheritedWindowData = windata;
	osg::ref_ptr<osg::GraphicsContext> pGC = osg::GraphicsContext::createGraphicsContext(traits);

	traits->y = 0;
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setGraphicsContext(pGC.get());

	osg::ref_ptr<osg::Viewport> vieport = new osg::Viewport(traits->x, traits->y, traits->width, traits->height);
	camera->setViewport(vieport.get());
	camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0f, 1000.0f);

	m_pViewr->setCamera(camera.get());

	m_pViewr->setSceneData(m_pRoot.get());
	m_pViewr->realize();
	m_pViewr->getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_PRIMITIVES);

	m_pViewr->getCamera()->setNearFarRatio(0.000003f);

}


void COSG::PreFrameUpdate()
{

}
void COSG::PostFrameUpdate()
{

}

osgViewer::Viewer* COSG::GetViewer()
{
	return m_pViewr;
}

void COSG::Render(void *ptr)
{
	COSG* pOSG = (COSG*)ptr;

	osgViewer::Viewer* pViewr = pOSG->GetViewer();
	pViewr->run();
/*
	while (!pViewr->done() && pOSG->m_IsRender)
	{
		pOSG->PreFrameUpdate();
		pViewr->frame();
		pOSG->PostFrameUpdate();
	}*/

	_endthread();

}

void COSG::InitOsgEarth()
{
	m_em = new osgEarth::Util::EarthManipulator;
	if (m_mapNode.valid())
	{
		m_em->setNode(m_mapNode.get());
	}
	//优化操作效果
	m_em->getSettings()->setArcViewpointTransitions(true);
	m_pViewr->setCameraManipulator(m_em.get());
	
	//开机动画
	m_em->setViewpoint(osgEarth::Viewpoint("test",108.75, 34.43, 444.02, -15.84, -53.01, 26305.0),5);
	

	//初始化天空
	osgEarth::Config skyconfig;
	double hours = skyconfig.value("hours", 12.0);
	osg::ref_ptr<osgEarth::Util::SkyNode> skynode = osgEarth::Util::SkyNode::create(m_mapNode);

	osgEarth::Util::DateTime datetime(2017, 7, 16, hours);
	
	skynode->setSunVisible(true);
	skynode->setDateTime(datetime);
	skynode->attach(m_pViewr, 1);
	m_pRoot->addChild(skynode.get());


	m_ChinaBound = m_mapNode->getMap()->getModelLayerByName("ChinaBounder");
	SetChinaBouderyOptacity(0.5);///图界线透明度

	//新增加一个地标
	osg::Image* china = osgDB::readImageFile("../earthfile/res/icon.png");
		
	const osgEarth::Util::SpatialReference* geoSRS = m_mapNode->getMapSRS()->getGeographicSRS();
	osgEarth::Annotation::Style pm;
	pm.getOrCreate<osgEarth::Annotation::IconSymbol>()->url()->setLiteral("../earthfile/res/icon.png");
	pm.getOrCreate<osgEarth::Annotation::IconSymbol>()->declutter() = true;
	pm.getOrCreate<osgEarth::Annotation::TextSymbol>()->halo() = osgEarth::Annotation::Color("#003500"); //边框
	pm.getOrCreate<osgEarth::Annotation::TextSymbol>()->fill()->color() = osgEarth::Annotation::Color("#ffffff"); //字体颜色
	pm.getOrCreate<osgEarth::Annotation::TextSymbol>()->size() = 20.0;
	//pm.getOrCreate<osgEarth::Annotation::TextSymbol>()->font() = "simsun.ttc";
	pm.getOrCreate<osgEarth::Annotation::TextSymbol>()->encoding() = osgEarth::Annotation::TextSymbol::ENCODING_UTF8; 

	osg::ref_ptr<osgEarth::Annotation::PlaceNode> pn = new osgEarth::Annotation::PlaceNode(m_mapNode, osgEarth::Annotation::GeoPoint(geoSRS, 108.76, 34.43),NULL,"aaaa", pm);
	osg::ref_ptr<osgEarth::Annotation::PlaceNode> pn2 = new osgEarth::Annotation::PlaceNode(m_mapNode, osgEarth::Annotation::GeoPoint(geoSRS, 108.7554, 34.441), NULL, "T2", pm);
	osg::ref_ptr<osgEarth::Annotation::PlaceNode> pn3 = new osgEarth::Annotation::PlaceNode(m_mapNode, osgEarth::Annotation::GeoPoint(geoSRS, 108.9425, 34.2714), NULL, "bbbb", pm);

	m_EarthLabel->addChild(pn);
	m_EarthLabel->addChild(pn2);
	m_EarthLabel->addChild(pn3);




	//增加视角位置显示控件
	AddViewPointLabel();

	AddAirPort();

	DoPreLine();
}

void COSG::ExitOSG()
{
	m_IsRender = FALSE;
}


void COSG::SetChinaBouderyOptacity(double opt)
{
	if (m_ChinaBound)
	{
		m_ChinaBound->setOpacity(opt);
	}

}


void COSG::AddViewPointLabel()
{
	osgEarth::Util::Controls::ControlCanvas* cans = osgEarth::Util::Controls::ControlCanvas::getOrCreate(m_pViewr);
	
	osgEarth::Util::Controls::LabelControl * viewCoords = new osgEarth::Util::Controls::LabelControl("TestViewPort", osg::Vec4f(1.0, 1.0, 1.0, 1.0));
	viewCoords->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_LEFT);
	viewCoords->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_TOP);
	viewCoords->setBackColor(0, 0, 0, 0.5);
	viewCoords->setSize(100, 50);
	viewCoords->setMargin(10);
	viewCoords->setPadding(10);
	viewCoords->setFont(osgEarth::Registry::instance()->getDefaultFont());
	cans->addControl(viewCoords);
	
	osgEarth::Util::Controls::LabelControl * mouseCoords = new osgEarth::Util::Controls::LabelControl("MouseViewPort", osg::Vec4f(1.0, 1.0, 1.0, 1.0));
	mouseCoords->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_RIGHT);
	mouseCoords->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_BOTTOM);
	mouseCoords->setBackColor(0, 0, 0, 0.5);
	mouseCoords->setSize(100, 50);
	mouseCoords->setMargin(10);
	mouseCoords->setPadding(10);
	mouseCoords->setFont(osgEarth::Registry::instance()->getDefaultFont());
	cans->addControl(mouseCoords);

	if (m_pLabelEventHandler == NULL)
	{
		m_pLabelEventHandler = new CLabelEventHandler(viewCoords, mouseCoords, m_mapNode);
	}

	m_pViewr->addEventHandler(m_pLabelEventHandler);
	
}

void COSG::FlyGo(float jing, float wei, float height)
{
	m_em->setViewpoint(osgEarth::Viewpoint("test", jing, wei, 0, -15, -53, height), 5);

}


void COSG::AddAirPort()
{
	//太TMD的关键了  osgearth_los
	m_wuti->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	m_wuti->getOrCreateStateSet()->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false));


	m_csn = new osg::CoordinateSystemNode;
	m_csn->setEllipsoidModel(new osg::EllipsoidModel());

	m_csn->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	m_csn->getOrCreateStateSet()->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false));

	m_AirPort = osgDB::readNodeFile("../earthfile/ive/airport.ive");
//	m_mtAirPortSelf = new osg::MatrixTransform;
//	m_mtAirPortSelf->addChild(m_AirPort);
//	m_mtAirPortSelf->setMatrix(osg::Matrixd::scale(1, 1, 1)); //*osg::Matrixd::rotate(1.57 / 2, osg::Vec3(0, 0, 1))
//	m_mtAirPortSelf->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);

	m_mtAriPort = new osg::MatrixTransform;
	m_mtAriPort->addChild(m_AirPort);
	m_mtAriPort->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);

//	m_wuti->addChild(m_mtAriPort);
	

	osg::Matrixd mtTemp;
	m_csn->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(
		(osg::DegreesToRadians(34.43)),
		(osg::DegreesToRadians(108.75)),  //, 
		400, mtTemp);

	m_mtAriPort->setMatrix(mtTemp);



//
	//飞机

	m_fly = osgDB::readNodeFile("../earthfile/ive/fly.ive");
	m_mtFlySelf = new osg::MatrixTransform;
	m_mtFlySelf->addChild(m_fly);
	m_mtFlySelf->setMatrix(osg::Matrixd::scale(100, 100, 100)*osg::Matrixd::rotate(-1.57/ 2, osg::Vec3(0, 0, 1))); //
	m_mtFlySelf->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);


	m_mtFly = new osg::MatrixTransform;
	m_mtFly->addChild(m_mtFlySelf);
	m_wuti->addChild(m_mtFly);

	m_csn->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(
			(osg::DegreesToRadians(34.442937)),
			(osg::DegreesToRadians(108.781299)),  //, 
		500, mtTemp);

	m_mtFly->setMatrix(mtTemp);



}

//根据控帛点输入路径（经度，纬度，高度，速度）
osg::ref_ptr<osg::AnimationPath>  COSG::CreateFlyLinePath(osg::Vec4Array* ctrl)
{
	osg::ref_ptr<osg::AnimationPath> animation = new osg::AnimationPath;
	animation->setLoopMode(osg::AnimationPath::NO_LOOPING);

	double shuiPingAngle;
	double chuiZhiAngle;

	osg::Matrix matrix;
	osg::Quat rotation;

	double time = 0;

	osg::Vec3 positionCurl;
	osg::Vec3 positionNext;

	FILE* fp = fopen("c:\\1.txt", "wt");

	for (osg::Vec4Array::iterator iter = ctrl->begin(); iter != ctrl->end(); iter++)
	{
		osg::Vec4Array::iterator iter2 = iter + 1;

		if (iter2 == ctrl->end())
		{
			break;
		}

		

		double x, y, z;
		double x2, y2, z2;
		m_csn->getEllipsoidModel()->convertLatLongHeightToXYZ(osg::DegreesToRadians(iter->y()), osg::DegreesToRadians(iter->x()), iter->z(), x, y, z);
		positionCurl = osg::Vec3(x, y, z);

		m_csn->getEllipsoidModel()->convertLatLongHeightToXYZ(osg::DegreesToRadians(iter2->y()), osg::DegreesToRadians(iter2->x()), iter2->z(), x2, y2, z2);
		positionNext = osg::Vec3(x2, y2, z2);

		//求出水平夹角 x=经度 y=纬度  与水线的夹角
		if (iter->x() == iter2->x())
		{
			shuiPingAngle = osg::PI_2; //90度
		}
		else
		{
			shuiPingAngle = atan(iter2->y() - iter->y()) / (iter2->x() - iter->x());
			if (iter2->x() > iter->x())
			{
				shuiPingAngle += osg::PI;
			}
		}

		//垂直夹角
		if (iter->z() == iter2->z())
		{
			chuiZhiAngle = 0;
		}
		else
		{
			if (0 == sqrt(pow(GetDis(positionCurl, positionNext), 2) - pow((iter2->z() - iter->z()), 2)))
			{
				chuiZhiAngle = osg::PI_2;
			}
			else
			{
				chuiZhiAngle = atan((iter2->z() - iter->z()) / sqrt(pow(GetDis(positionCurl, positionNext), 2) - pow((iter2->z() - iter->z()), 2)));
			}

			if (chuiZhiAngle >= osg::PI_2)
			{
				chuiZhiAngle = osg::PI_2;
			}

			if (chuiZhiAngle <= -osg::PI_2)
			{
				chuiZhiAngle = - osg::PI_2;
			}
		}

		//求飞机的变换矩阵
		m_csn->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(iter->y()), osg::DegreesToRadians(iter->x()), iter->z(), matrix);

		rotation.makeRotate(0, osg::Vec3(1.0, 0, 0), chuiZhiAngle, osg::Vec3(0, 1, 0), shuiPingAngle, osg::Vec3(0, 0, 1));
		matrix.preMultRotate(rotation);

		animation->insert(time, osg::AnimationPath::ControlPoint(positionCurl, matrix.getRotate()));
		

		m_csn->getEllipsoidModel()->convertLatLongHeightToXYZ(osg::DegreesToRadians(iter->y()), osg::DegreesToRadians(iter->x()), iter->z(), x, y, z);

		{
			double jing, wei, height;
			m_csn->getEllipsoidModel()->convertXYZToLatLongHeight(positionCurl.x(), positionCurl.y(), positionCurl.z(), jing, wei, height);
			fprintf(fp, "%f (%f %f %f) (%f %f %f)\n", time, positionCurl.x(), positionCurl.y(), positionCurl.z(), jing, wei, height);
		}

		time += GetRunTime(positionCurl, positionNext, iter->w());
	}

	animation->insert(time, osg::AnimationPath::ControlPoint(positionNext, matrix.getRotate()));

	{
		double jing, wei, height;
		m_csn->getEllipsoidModel()->convertXYZToLatLongHeight(positionCurl.x(), positionCurl.y(), positionCurl.z(), jing, wei, height);
		fprintf(fp, "%f (%f %f %f) (%f %f %f)\n", time, positionCurl.x(), positionCurl.y(), positionCurl.z(), jing, wei, height);
	}

	fclose(fp);
	return animation.release();
}

double COSG::GetDis(osg::Vec3 from, osg::Vec3 to)
{
	double x2 = (to.x() - from.x()) *(to.x() - from.x());
	double y2 = (to.y() - from.y()) *(to.y() - from.y());
	double z2 = (to.z() - from.z()) *(to.z() - from.z());
	return sqrt(x2 + y2 + z2);
}



double COSG::GetRunTime(osg::Vec3 from, osg::Vec3 to, double speed)
{
	double distance = GetDis(from, to);
	if (speed == 0)
	{
		return 100000000;
	}

	return distance / speed;
}

void COSG::DoPreLine()
{
	osg::ref_ptr<osg::Vec4Array> varTemp = new osg::Vec4Array;


	varTemp->push_back(osg::Vec4(34.442937, 108.781299, 500, 100));
	/*varTemp->push_back(osg::Vec4(34.442937, 108.781299, 500, 100));
	varTemp->push_back(osg::Vec4(34.442937, 108.781299, 500, 100));
	varTemp->push_back(osg::Vec4(34.442937, 108.781299, 500, 100));
	varTemp->push_back(osg::Vec4(34.442937, 108.781299, 500, 100));
*/

	/*varTemp->push_back(osg::Vec4(34.443537, 108.782299, 550, 200));
	varTemp->push_back(osg::Vec4(34.452937, 108.791299, 550, 300));
	varTemp->push_back(osg::Vec4(34.552937, 108.821299, 600, 100));
	varTemp->push_back(osg::Vec4(34.552937, 108.821299, 1000, 300));
	varTemp->push_back(osg::Vec4(34.442937, 108.781299, 500, 200));*/

	m_apc = CreateFlyLinePath(varTemp);

}

void COSG::DoPreLineNow()
{
//	m_mtFly->setUpdateCallback(new osg::AnimationPathCallback(m_apc, 0.0, 1.0));

	const osgEarth::SpatialReference* mapSRS = m_mapNode->getMapSRS();
	const osgEarth::SpatialReference* geoSRS = mapSRS->getGeographicSRS();

	osg::Node* plane2 = createPlane(m_mtFlySelf, osgEarth::GeoPoint(geoSRS, 108.781299,34.442937, 5000, osgEarth::ALTMODE_ABSOLUTE), mapSRS, 3000, 20);
	m_wuti->addChild(plane2);
//	m_em->setTetherNode(m_fly)
}

osg::Node*  COSG::createPlane(osg::Node* node, const osgEarth::GeoPoint& pos, const osgEarth::SpatialReference* mapSRS, double radius, double time)
{
	osg::MatrixTransform* positioner = new osg::MatrixTransform;
	positioner->addChild(node);
	osg::AnimationPath* animationPath = createAnimationPath(pos, mapSRS, radius, time);
	positioner->setUpdateCallback(new osg::AnimationPathCallback(animationPath, 0.0, 1.0));
//	positioner->setUpdateCallback(new osg::AnimationPathCallback(m_apc, 0.0, 1.0));
	return positioner;
}

osg::AnimationPath*  COSG::createAnimationPath(const osgEarth::GeoPoint& pos, const osgEarth::SpatialReference* mapSRS, float radius, double looptime)
{
	// set up the animation path 
	osg::AnimationPath* animationPath = new osg::AnimationPath;
	animationPath->setLoopMode(osg::AnimationPath::LOOP);

	int numSamples = 40;

	double delta = osg::PI * 2.0 / (double)numSamples;

	//Get the center point in geocentric
	osgEarth::GeoPoint mapPos = pos.transform(mapSRS);
	osg::Vec3d centerWorld;
	mapPos.toWorld(centerWorld);

	bool isProjected = mapSRS->isProjected();

	osg::Vec3d up = isProjected ? osg::Vec3d(0, 0, 1) : centerWorld;
	up.normalize();

	//Get the "side" vector
	osg::Vec3d side = isProjected ? osg::Vec3d(1, 0, 0) : up ^ osg::Vec3d(0, 0, 1);


	double time = 0.0f;
	double time_delta = looptime / (double)numSamples;

	osg::Vec3d firstPosition;
	osg::Quat firstRotation;

	for (unsigned int i = 0; i < (unsigned int)numSamples; i++)
	{
		double angle = delta * (double)i;
		osg::Quat quat(angle, up);
		osg::Vec3d spoke = quat * (side * radius);
		osg::Vec3d end = centerWorld + spoke;

		osg::Quat makeUp;
		makeUp.makeRotate(osg::Vec3d(0, 0, 1), up);

		osg::Quat rot = makeUp;
		animationPath->insert(time, osg::AnimationPath::ControlPoint(end, rot));
		if (i == 0)
		{
			firstPosition = end;
			firstRotation = rot;
		}
		time += time_delta;
	}

	animationPath->insert(time, osg::AnimationPath::ControlPoint(firstPosition, firstRotation));

	return animationPath;
}