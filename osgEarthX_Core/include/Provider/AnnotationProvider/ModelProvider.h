/**
	@brief ModelNode Provider
	@author Hao Luo
	@date 2015/10/21
*/

#ifndef OSGEARTHX_CORE_MODEL_PROVIDER_H
#define OSGEARTHX_CORE_MODEL_PROVIDER_H 1

#include <osgEarthXCoreExport/Export.h>

#include <osgEarthAnnotation/ModelNode>
#include <Provider/PlaceNodeEditor.h>
#include <osgEarthSymbology\Geometry>

#include <osgCal/CoreModel>
#include <osgCal/Model>

namespace osgEarthXCore
{
	class Bimgrow : public osg::NodeCallback
	{
	public:
		Bimgrow(osg::Group* pgrp) :mGrp(pgrp), mStart(false), bFirst(true) {}
		/** Callback method called by the NodeVisitor when visiting a node.*/
		void start()
		{
			mStart = true;
			mt = osg::Timer::instance()->tick();
		}
		virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
		{
			if (mStart)
			{
				osg::Group* grp = dynamic_cast<osg::Group*>(node);

				if (grp)
				{
					if (bFirst)
					{
						bFirst = false;
						grp->removeChildren(0, grp->getNumChildren());
					}
					osg::Timer_t t = osg::Timer::instance()->tick();
					double ds = osg::Timer::instance()->delta_m(mt, t);
					if (ds>100)
					{
						mt = t;
						int s1 = grp->getNumChildren();
						int s2 = mGrp->getNumChildren();
						if (s2>s1)
						{
							grp->addChild(mGrp->getChild(s1));
						}
						else {
							mStart = false;
							bFirst = true;
						}
					}
				}
			}
		}

	protected:
		osg::ref_ptr<osg::Group> mGrp;
		bool mStart;
		osg::Timer_t mt;
		bool bFirst;
	};

	class OSGEARTHX_CORE_EXPORT ModelProvider : 
		virtual public osgEarth::Annotation::ModelNode,
		virtual public osgEarth::Symbology::ModelSymbol
	{
	public:
		ModelProvider();
		ModelProvider( const ModelProvider& modelProvider );
		ModelProvider( const osgEarth::Annotation::ModelNode& modelNode );
		ModelProvider( const osgEarth::Symbology::ModelSymbol& modelSymbol );

		virtual void initMapNode( osgEarth::MapNode* pMapNode );
		void setModelPath( const std::string& modelPath );
		const std::string& getModelPath();

		void setEditor(osg::Group* con);
		void startEdit();
		void stopEdit();
		void clearAttachedEditor();
		void splitShow();
		void setPlay(bool bPlay);
		bool getPlay();
		void setSpeed(double dSpeed);
		double getSpeed();
		void setPath(osgEarth::Symbology::LineString* lin);
		osgEarth::Symbology::LineString* getPath();
		void setAction(int iAct);
		int getAction();
	protected:
		virtual ~ModelProvider();

		std::string m_modelPath;
		osg::ref_ptr< osg::Group > m_Editors;
		osg::ref_ptr< PlaceNodeEditor > m_refEditor;
		osg::ref_ptr< osg::Group > m_Components;
		osg::ref_ptr<Bimgrow> m_Bimgrw;
		bool m_bPlay;
		double m_dSpeed; //Ã×Ã¿Ãë
		osg::ref_ptr< osgEarth::Symbology::LineString > m_pathLine;
		osg::ref_ptr<osgCal::Model> m_ndPeople;
	private:
		int m_iCurAction;
	};
}

#endif