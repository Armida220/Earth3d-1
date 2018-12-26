/**
	@brief osgEarth::Anntation::PlaceNode Ã·π©’ﬂ
	@author Hao Luo
	@date 2015/04/01
*/

#ifndef OSGEARTHX_CORE_PLACE_PROVIDER_H
#define OSGEARTHX_CORE_PLACE_PROVIDER_H 1

#include <osgEarthXCoreExport/Export.h>

#include <Provider/AnnotationProvider/ImageProvider.h>
//#include <Provider/AnnotationProvider/PlaceNodeEx>
#include <osgEarthUtil/Controls>
#include <osgViewer/Viewer>
#include <osgEarth/Random>
#include <osgEarthAnnotation/PlaceNode>
#include <Provider/PlaceNodeEditor.h>

namespace osgEarthXCore
{
	struct TipClickHandler;
	class OSGEARTHX_CORE_EXPORT PlaceProvider : 
		public osgEarth::Annotation::PlaceNode,
		//public PlaceNodeEx,
		public ImageProvider
	{
	public:
		PlaceProvider();

		void setLocalText( const std::string& str );
		void setLocalTextExt(const std::string& str, unsigned int sz, unsigned int col, const std::string& fnt);
		void setUnicodeText( const std::string& str );

		void setCulling(bool bCull);
		bool getCulling();
		void setShowText(bool bShow);
		void setControlCanvas(osgEarth::Util::Controls::ControlCanvas* canvas);
		void showTip(bool bShow);
		void UpdteTip(osgViewer::Viewer* vw);
		void setTipContent(std::string str, unsigned int tp);
		void setTipButton(std::string str);
		void setTipHandler(TipClickHandler* hd);
		std::string getTipField(std::string str);
		void setField(std::string strKey, std::string strVal);
		std::string getField(std::string strKey);

		void setEditor(osg::Group* con);
		void startEdit();
		void stopEdit();

		void clearAttached();
		void setFlash(bool bShow);
		bool getFlash();
		void setCullingNodes(osg::Group* grp);
		void setPriority(float fPriority);
		float getPriority();
	protected:
		virtual ~PlaceProvider();
		virtual void _setImage( osg::Image* pImage );
		virtual void _setImage2(osg::Image* pImage);
		void createControl();
		void createTipButton();
		void clearAttachedEditor();
		void clearAttachedTip();

		osg::ref_ptr< osgEarth::Util::Controls::ControlCanvas > m_refCanvas;
		osg::ref_ptr< osgEarth::Util::Controls::Control > m_refTip;
		osg::ref_ptr< osgEarth::Util::Controls::Grid > m_refContent;
		osg::ref_ptr< osgEarth::Util::Controls::HBox > m_refTipButton;

		osg::ref_ptr< TipClickHandler > m_tipHandler;
		osg::ref_ptr< osg::Group > m_Editors;
		osg::ref_ptr< PlaceNodeEditor > m_refEditor;
		osg::ref_ptr<osg::Group> m_refCullingNodes;
		
		std::string m_strTip;
		std::string m_strTipButton;
		bool m_bShowTip;
		bool m_bShowFlash;
		bool m_bCull;
		float m_fPriority;
		std::vector <std::string> m_fields;
		std::map<std::string, std::string> m_Attributes;
	private:
	};

	struct OSGEARTHX_CORE_EXPORT TipClickHandler : public osgEarth::Util::Controls::ControlEventHandler
	{
		TipClickHandler(PlaceProvider* pla) :_place(pla) {}
		void onClick(osgEarth::Util::Controls::Control* control, const osg::Vec2f& pos, int mouseButtonMask)
		{
			_place->showTip(false);
			osgEarth::Util::Controls::CheckBoxControl* ck = dynamic_cast<osgEarth::Util::Controls::CheckBoxControl*>(control);
			ck->setValue(true);
		}

		osg::ref_ptr< PlaceProvider> _place;
	};

	struct PlayClickHandler : public osgEarthXCore::TipClickHandler
	{
		struct PlayClickCallback : public osg::Referenced
		{
			// called when valid map coordinates are found under the mouse
			virtual void playClicked(PlaceProvider* obj, const std::string &strID) = 0;
			
			virtual ~PlayClickCallback() { }
		};

		PlayClickHandler::PlayClickHandler(PlaceProvider* pla) : osgEarthXCore::TipClickHandler(pla)
		{
		}
		void onClick(osgEarth::Util::Controls::Control* control, const osg::Vec2f& pos, int mouseButtonMask)
		{
			if (_place != NULL)
			{
				for (Callbacks::iterator i = _callbacks.begin(); i != _callbacks.end(); ++i)
					i->get()->playClicked(_place.get(),control->getName());
			}
		}

		void addCallback(PlayClickCallback* callback){ _callbacks.push_back(callback); }

		typedef std::vector< osg::ref_ptr<PlayClickCallback> > Callbacks;
		Callbacks _callbacks;
	};
}

#endif