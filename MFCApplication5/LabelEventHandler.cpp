#include "stdafx.h"
#include "LabelEventHandler.h"
#include <osgEarth/DPLineSegmentIntersector>
using namespace osgEarth;
CLabelEventHandler::CLabelEventHandler(osgEarth::Util::Controls::LabelControl* viewLabel,
	osgEarth::Util::Controls::LabelControl* mouseLabel,
	osgEarth::MapNode* mm)
{
	m_ViewLabel = viewLabel;
	m_MouseLabel = mouseLabel;
	m_MapNode = mm;

	//Sample osgearth_elevation
	m_NodePath.push_back(m_MapNode->getTerrainEngine()); //Í·ÎÄ¼þ
}


CLabelEventHandler::~CLabelEventHandler()
{
}




bool CLabelEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	osgViewer::View* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
	osgUtil::LineSegmentIntersector::Intersections intersections;
	osg::Vec3d out_coords;
	if (viewer)
	{
		if (ea.getEventType() == ea.MOVE || ea.getEventType() == ea.DRAG)
		{
			{
				float local_x, local_y = 0.0;
				const osg::Camera* camera = viewer->getCameraContainingPosition(ea.getX(), ea.getY(), local_x, local_y);
				if (!camera) camera = viewer->getCamera();

				osg::Matrixd matrix;
				if (m_NodePath.size() > 1)
				{
					osg::NodePath prunedNodePath(m_NodePath.begin(), m_NodePath.end() - 1);
					matrix = osg::computeLocalToWorld(prunedNodePath);
				}

				matrix.postMult(camera->getViewMatrix());
				matrix.postMult(camera->getProjectionMatrix());

				double zNear = -1.0;
				double zFar = 1.0;
				if (camera->getViewport())
				{
					matrix.postMult(camera->getViewport()->computeWindowMatrix());
					zNear = 0.0;
					zFar = 1.0;
				}

				osg::Matrixd inverse;
				inverse.invert(matrix);

				osg::Vec3d startVertex = osg::Vec3d(local_x, local_y, zNear) * inverse;
				osg::Vec3d endVertex = osg::Vec3d(local_x, local_y, zFar) * inverse;

				// Use a double precision line segment intersector
				//osg::ref_ptr< osgUtil::LineSegmentIntersector > picker = new osgUtil::LineSegmentIntersector(osgUtil::Intersector::MODEL, startVertex, endVertex);
				osg::ref_ptr< DPLineSegmentIntersector > picker = new DPLineSegmentIntersector(osgUtil::Intersector::MODEL, startVertex, endVertex);

				// Limit it to one intersection, we only care about the first
				picker->setIntersectionLimit(osgUtil::Intersector::LIMIT_NEAREST);

				osgUtil::IntersectionVisitor iv(picker.get());
				m_NodePath.back()->accept(iv);

				if (picker->containsIntersections())
				{
					intersections = picker->getIntersections();
					// find the first hit under the mouse:
					osgUtil::LineSegmentIntersector::Intersection first = *(intersections.begin());
					out_coords = first.getWorldIntersectPoint();
					osgEarth::GeoPoint mapPoint;
					mapPoint.fromWorld(m_MapNode->getMapSRS()->getGeodeticSRS(), out_coords);
					char buff[512] = { 0 };
					sprintf_s(buff, sizeof(buff), "mouse:jing:%.6f wei:%.6f heigh:%.2f", mapPoint.x(), mapPoint.y(), mapPoint.z());
					m_MouseLabel->setText(buff); 
				}

			}
			osgEarth::Util::EarthManipulator* em = dynamic_cast<osgEarth::Util::EarthManipulator*>(viewer->getCameraManipulator());
			if (em)
			{	
				osgEarth::Distance rg = em->getViewpoint().range().get();
				osgEarth::GeoPoint vp = em->getViewpoint().focalPoint().get();
				rg.getValue();
				char buff[512] = { 0 };
				sprintf_s(buff, sizeof(buff), "View Lon:%.2f Lat:%.2f Hei:%.2f", vp.x(), vp.y(), rg.getValue());
				m_ViewLabel->setText(buff);
				
			}
			return ea.getEventType() != ea.DRAG;
		}
		
	}
	return false;
}

