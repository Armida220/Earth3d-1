/**
	@brief 由外部模型组成的Map
	@author Hao Luo
	@date 2015/01/14
*/

#ifndef OSGEARTHX_CORE_EXTERNALS_MAP_H
#define OSGEARTHX_CORE_EXTERNALS_MAP_H 1

#include <osgParticle/PrecipitationEffect>  
#include <osgEarthUtil/Shadowing>
#include <osgEarth/TerrainEngineNode>
#include <osgEarthUtil/Fog>
#include <osg/Fog>
#include <osgEarthUtil/Controls>

#include <osgEarthXCoreExport/Export.h>
#include <Map/BaseMap.h>

#include <Provider/ExternalProvider/SkyProvider.h>
#include <Provider/ExternalProvider/GraticuleProvider.h>

#include <osgEarth/VirtualProgram>
#include <osgEarth/TerrainEngineNode>

#include <osgEarthUtil/Controls>

#include <osgEarthUtil/ContourMap>
#include <osgEarthUtil/LODBlending>
#include <osgEarthUtil/NormalMap>
#include <osgEarthUtil/VerticalScale>

#include <osgEarthUtil/TerrainProfile>
#include <osgEarth/GeoMath>
#include <osgEarth/Registry>

struct App
{
	osgEarth::TerrainEngineNode*           engine;

	osg::ref_ptr<osgEarth::Util::ContourMap>     contourMap;
	osg::ref_ptr<osgEarth::Util::LODBlending>    lodBlending;
	osg::ref_ptr<osgEarth::Util::NormalMap>      normalMap;
	osg::ref_ptr<osgEarth::Util::VerticalScale>  verticalScale;

	App()
	{
		contourMap = new osgEarth::Util::ContourMap();
		lodBlending = new osgEarth::Util::LODBlending();
		normalMap = new osgEarth::Util::NormalMap();
		verticalScale = new osgEarth::Util::VerticalScale();
	}
};

namespace osgEarthXCore
{
	class SetSkyProviderOperation;
	class RemoveSkyProviderOperation;

	class SetGraticuleProviderOperation;
	class RemoveGraticuleProviderOperation;

	class SetCasterOperation;
	class ResetCasterOperation;
	class TerrainProfileGraph;

	class OSGEARTHX_CORE_EXPORT ExternalsMap:
		virtual public BaseMap
	{
	public:
		friend SetSkyProviderOperation;
		friend RemoveSkyProviderOperation;
		friend SetGraticuleProviderOperation;
		friend RemoveGraticuleProviderOperation;
		friend SetCasterOperation;
		friend ResetCasterOperation;
	protected:
		osg::ref_ptr< SkyProvider > m_refSkyProvider;
		osg::ref_ptr< GraticuleProvider > m_refGraticuleProvider;
		osg::ref_ptr<osgEarth::Util::ShadowCaster> m_refCaster;
		osg::ref_ptr<osgEarth::Util::FogEffect>  m_refFogEffect;
		osg::ref_ptr<osg::Fog> m_refFog;
		osg::ref_ptr<osgParticle::PrecipitationEffect> m_refRain;
		osg::ref_ptr<osgParticle::PrecipitationEffect> m_refSnow;

		osg::ref_ptr<osgEarth::Util::Controls::Grid> m_refGridTerrain;
		osg::ref_ptr<osg::Camera> m_refProfileCamera;
		osg::ref_ptr< osgEarth::Util::TerrainProfileCalculator > m_refCalculator;
		App m_Terrain;

	public:
		ExternalsMap();

		void setSkyProvider( SkyProvider* pSkyProvider );
		SkyProvider* getSkyProvider();
		void removeSkyProvider();

		void setGraticuleProvider( GraticuleProvider* pGraticuleProvider );
		GraticuleProvider* getGraticuleProvider();
		void removeGraticuleProvider();
		void setupViewShed(osg::Vec3d pos, osg::Vec3 dir);
		void UpdateShadowCaster(const osg::Vec3d& pos, double heading, double tilt, double range);
		void stopViewShed();
		void setFog(bool bFog);
		void setSnow(bool bSnow);
		void setRain(bool bRain);
		void setTerrainEffect(bool bEffect);
		void LineOfSight(double dlon, double dlat, double dAlt, double dlonEnd, double dlatEnd, double dAltEnd);
		void RadialLineOfSight(double dlon, double dlat, double dAlt, double dr);
		void TerrainProfile(double dlon, double dlat, double dlonEnd, double dlatEnd, bool bShow);
		void ClearCalResult();
		void ClearScene();
	protected:
		virtual ~ExternalsMap();
		virtual void initMapViewer( MapViewer* pMapViewer );

		void _removeSkyProvider();
		void _setSkyProvider( SkyProvider* pSkyProvider );

		void _removeGraticuleProvider();
		void _setGraticuleProvider( GraticuleProvider* pGraticuleProvider );

		void _setCaster();
		void _resetCaster();
	private:
	};

	//Operations
	class SetCasterOperation : public osg::Operation
	{
	public:
		SetCasterOperation(ExternalsMap* pExternalsMap)
		{
			m_pExternalsMap = pExternalsMap;

		}
		virtual void operator()(osg::Object* pObject);

	protected:
		virtual ~SetCasterOperation() {}

	private:
		ExternalsMap* m_pExternalsMap;
	};

	class ResetCasterOperation : public osg::Operation
	{
	public:
		ResetCasterOperation(ExternalsMap* pExternalsMap)
		{
			m_pExternalsMap = pExternalsMap;

		}
		virtual void operator()(osg::Object* pObject);

	protected:
		virtual ~ResetCasterOperation() {}

	private:
		ExternalsMap* m_pExternalsMap;
	};

	//Operations
	class SetSkyProviderOperation : public osg::Operation
	{
	public:
		SetSkyProviderOperation( ExternalsMap* pExternalsMap, SkyProvider* pSkyProvider )
		{
			m_pExternalsMap = pExternalsMap;
			m_pSkyProvider = pSkyProvider;
		}
		virtual void operator()( osg::Object* pObject );

	protected:
		virtual ~SetSkyProviderOperation(){}

	private:
		ExternalsMap* m_pExternalsMap;
		SkyProvider* m_pSkyProvider;
	};

	class RemoveSkyProviderOperation : public osg::Operation
	{
	public:
		RemoveSkyProviderOperation( ExternalsMap* pExternalsMap )
		{
			m_pExternalsMap = pExternalsMap;
		}
		virtual void operator()( osg::Object* pObject );

	protected:
		virtual ~RemoveSkyProviderOperation(){}

	private:
		ExternalsMap* m_pExternalsMap;
	};

	class SetGraticuleProviderOperation : public osg::Operation
	{
	public:
		SetGraticuleProviderOperation( ExternalsMap* pExternalsMap, GraticuleProvider* pGraticuleProvider )
		{
			m_pExternalsMap = pExternalsMap;
			m_pGraticuleProvider = pGraticuleProvider;

		}

		virtual void operator() ( osg::Object* pObject );

	protected:
		virtual ~SetGraticuleProviderOperation(){}

	private:
		ExternalsMap* m_pExternalsMap;
		GraticuleProvider* m_pGraticuleProvider;

	private:
	};

	class RemoveGraticuleProviderOperation : public osg::Operation
	{
	public:
		RemoveGraticuleProviderOperation( ExternalsMap* pExternalsMap )
		{
			m_pExternalsMap = pExternalsMap;
		}

		virtual void operator() ( osg::Object* pObject );

	protected:
		virtual ~RemoveGraticuleProviderOperation(){}

	private:
		ExternalsMap* m_pExternalsMap;

	};

	/**
	* Simple terrain profile display
	*/
	class TerrainProfileGraph : public osg::Group
	{
	public:
		/*
		* Callback that is fired when the TerrainProfile changes
		*/
		struct GraphChangedCallback : public osgEarth::Util::TerrainProfileCalculator::ChangedCallback
		{
			GraphChangedCallback(TerrainProfileGraph* graph) :
				_graph(graph)
			{
			}

			virtual void onChanged(const osgEarth::Util::TerrainProfileCalculator* sender)
			{
				_graph->setTerrainProfile(sender->getProfile());
			}

			TerrainProfileGraph* _graph;
		};

		TerrainProfileGraph(osgEarth::Util::TerrainProfileCalculator* profileCalculator, double graphWidth = 200, double graphHeight = 200) :
			_profileCalculator(profileCalculator),
			_graphWidth(graphWidth),
			_graphHeight(graphHeight),
			_color(1.0f, 1.0f, 0.0f, 1.0f),
			_backcolor(0.0f, 0.0f, 0.0f, 0.5f)
		{
			_graphChangedCallback = new GraphChangedCallback(this);
			_profileCalculator->addChangedCallback(_graphChangedCallback.get());

			float textSize = 8;
			osg::ref_ptr< osgText::Font> font = osgEarth::Registry::instance()->getDefaultFont();

			osg::Vec4 textColor = osg::Vec4f(1, 0, 0, 1);

			_distanceMinLabel = new osgText::Text();
			_distanceMinLabel->setCharacterSize(textSize);
			_distanceMinLabel->setFont(font.get());
			_distanceMinLabel->setAlignment(osgText::TextBase::LEFT_BOTTOM);
			_distanceMinLabel->setColor(textColor);

			_distanceMaxLabel = new osgText::Text();
			_distanceMaxLabel->setCharacterSize(textSize);
			_distanceMaxLabel->setFont(font.get());
			_distanceMaxLabel->setAlignment(osgText::TextBase::RIGHT_BOTTOM);
			_distanceMaxLabel->setColor(textColor);

			_elevationMinLabel = new osgText::Text();
			_elevationMinLabel->setCharacterSize(textSize);
			_elevationMinLabel->setFont(font.get());
			_elevationMinLabel->setAlignment(osgText::TextBase::RIGHT_BOTTOM);
			_elevationMinLabel->setColor(textColor);

			_elevationMaxLabel = new osgText::Text();
			_elevationMaxLabel->setCharacterSize(textSize);
			_elevationMaxLabel->setFont(font.get());
			_elevationMaxLabel->setAlignment(osgText::TextBase::RIGHT_TOP);
			_elevationMaxLabel->setColor(textColor);
		}

		~TerrainProfileGraph()
		{
			_profileCalculator->removeChangedCallback(_graphChangedCallback.get());
		}

		void setTerrainProfile(const osgEarth::Util::TerrainProfile& profile)
		{
			_profile = profile;
			redraw();
		}

		//Redraws the graph
		void redraw()
		{
			removeChildren(0, getNumChildren());

			addChild(createBackground(_graphWidth, _graphHeight, _backcolor));

			osg::Geometry* geom = new osg::Geometry;
			geom->setUseVertexBufferObjects(true);

			osg::Vec3Array* verts = new osg::Vec3Array();
			verts->reserve(_profile.getNumElevations());
			geom->setVertexArray(verts);
			if (verts->getVertexBufferObject())
				verts->getVertexBufferObject()->setUsage(GL_STATIC_DRAW_ARB);

			osg::Vec4Array* colors = new osg::Vec4Array();
			colors->push_back(_color);
			geom->setColorArray(colors);
			geom->setColorBinding(osg::Geometry::BIND_OVERALL);

			double minElevation, maxElevation;
			_profile.getElevationRanges(minElevation, maxElevation);
			double elevationRange = maxElevation - minElevation;

			double totalDistance = _profile.getTotalDistance();

			for (unsigned int i = 0; i < _profile.getNumElevations(); i++)
			{
				double distance = _profile.getDistance(i);
				double elevation = _profile.getElevation(i);

				double x = (distance / totalDistance) * _graphWidth;
				double y = ((elevation - minElevation) / elevationRange) * _graphHeight;
				verts->push_back(osg::Vec3(x, y, 0));
			}

			geom->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, verts->size()));
			osg::Geode* graphGeode = new osg::Geode;
			graphGeode->addDrawable(geom);
			addChild(graphGeode);

			osg::Geode* labelGeode = new osg::Geode;
			labelGeode->addDrawable(_distanceMinLabel.get());
			labelGeode->addDrawable(_distanceMaxLabel.get());
			labelGeode->addDrawable(_elevationMinLabel.get());
			labelGeode->addDrawable(_elevationMaxLabel.get());

			_distanceMinLabel->setPosition(osg::Vec3(0, 0, 0));
			_distanceMaxLabel->setPosition(osg::Vec3(_graphWidth - 15, 0, 0));
			_elevationMinLabel->setPosition(osg::Vec3(_graphWidth - 5, 10, 0));
			_elevationMaxLabel->setPosition(osg::Vec3(_graphWidth - 5, _graphHeight, 0));

			_distanceMinLabel->setText("0m");
			_distanceMaxLabel->setText(osgEarth::toString<int>((int)totalDistance) + std::string("m"));

			_elevationMinLabel->setText(osgEarth::toString<int>((int)minElevation) + std::string("m"));
			_elevationMaxLabel->setText(osgEarth::toString<int>((int)maxElevation) + std::string("m"));

			addChild(labelGeode);

		}

		osg::Node* createBackground(double width, double height, const osg::Vec4f& backgroundColor)
		{
			//Create a background quad
			osg::Geometry* geometry = new osg::Geometry();
			geometry->setUseVertexBufferObjects(true);

			osg::Vec3Array* verts = new osg::Vec3Array();
			verts->reserve(4);
			verts->push_back(osg::Vec3(0, 0, 0));
			verts->push_back(osg::Vec3(width, 0, 0));
			verts->push_back(osg::Vec3(width, height, 0));
			verts->push_back(osg::Vec3(0, height, 0));
			geometry->setVertexArray(verts);
			if (verts->getVertexBufferObject())
				verts->getVertexBufferObject()->setUsage(GL_STATIC_DRAW_ARB);

			osg::Vec4Array* colors = new osg::Vec4Array();
			colors->push_back(backgroundColor);
			geometry->setColorArray(colors);
			geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

			geometry->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

			osg::Geode* geode = new osg::Geode;
			geode->addDrawable(geometry);

			return geode;

		}

		osg::ref_ptr<osgText::Text> _distanceMinLabel, _distanceMaxLabel, _elevationMinLabel, _elevationMaxLabel;

		osg::Vec4f _backcolor;
		osg::Vec4f _color;
		osgEarth::Util::TerrainProfile _profile;
		osg::ref_ptr< osgEarth::Util::TerrainProfileCalculator > _profileCalculator;
		double _graphWidth, _graphHeight;
		osg::ref_ptr< GraphChangedCallback > _graphChangedCallback;
	};
}

#endif