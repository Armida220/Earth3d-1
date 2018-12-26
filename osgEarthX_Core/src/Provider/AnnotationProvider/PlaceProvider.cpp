#include <Provider/AnnotationProvider/PlaceProvider.h>
//OSGX
#include <StaticOSGViewerAssistant/StaticOSGViewerAssistant.h>
#include <boost/algorithm/string.hpp>

using namespace osgEarthXCore;

#include <ConvertChar/ConvertChar.h>
#include <osgViewer/Viewer>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include "boost/algorithm/string/find.hpp"
#include <osg/Depth>
#include <osg/ImageStream>

PlaceProvider::PlaceProvider():m_bShowTip(false),m_bShowFlash(false),m_bCull(true)
{
	osgEarth::Symbology::TextSymbol* pTextSymbol = _style.getOrCreateSymbol<osgEarth::Symbology::TextSymbol>();
	pTextSymbol->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
	pTextSymbol->font() = "simsun.ttc";
	pTextSymbol->size() = 20;
	pTextSymbol->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_BOTTOM;
	m_fPriority = 0.0;
}

PlaceProvider::~PlaceProvider()
{
	if (m_refEditor.get())
		if (m_Editors->containsNode(m_refEditor.get()))
			m_Editors->removeChild(m_refEditor.get());
	if(m_refCanvas.get())
	    m_refCanvas->removeControl(m_refTip.get());
}

void PlaceProvider::setLocalText( const std::string& str )
{
	//setUnicodeText( osgX::ConvertChar::convertLocalStrToUTF8Str( str ) );
	setUnicodeText(str);
}

void osgEarthXCore::PlaceProvider::setLocalTextExt(const std::string & str, unsigned int sz, unsigned int col, const std::string & fnt)
{
	osgEarth::Symbology::TextSymbol* pTextSymbol = _style.getOrCreateSymbol<osgEarth::Symbology::TextSymbol>();
	pTextSymbol->font() = fnt;
	pTextSymbol->size() = sz;
	pTextSymbol->fill()->color() = osgEarth::Symbology::Color(col);
	pTextSymbol->halo()->color() = osgEarth::Symbology::Color(0xffffffff);
	pTextSymbol->halo()->width() = 8.0;
	pTextSymbol->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_BOTTOM;
	//setUnicodeText(osgX::ConvertChar::convertLocalStrToUTF8Str(str));
	setUnicodeText(str);
}

void PlaceProvider::setFlash(bool bShow)
{
	m_bShowFlash = bShow;
}

bool PlaceProvider::getFlash()
{
	return m_bShowFlash;
}

void osgEarthXCore::PlaceProvider::setCullingNodes(osg::Group * grp)
{
	m_refCullingNodes = grp;
}

void osgEarthXCore::PlaceProvider::setPriority(float fPriority)
{
	m_fPriority = fPriority;
}

float osgEarthXCore::PlaceProvider::getPriority()
{
	return m_fPriority;
}

void PlaceProvider::createControl()
{
	m_fields.clear();
	//std::vector <std::string> fields;
	boost::split(m_fields, m_strTip, boost::is_any_of("&"));

	osgText::Font *fntKai = osgText::readFontFile("simkai.ttf");
	if (m_refContent.get() == NULL){
		m_refContent = new osgEarth::Util::Controls::Grid();
		m_refContent->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_CENTER);
		m_refContent->setBorderColor(1, 1, 0, 1);
	}
	else {
		m_refContent->clearControls();
	}

	for (int i = 0; i < m_fields.size(); i += 2)
	{
		osgEarth::Util::Controls::ControlVector vecCtrls;
		
		std::string ext = m_fields[i].size()>4 ? m_fields[i].substr(m_fields[i].size()-4,4) : "";
		if (ext == ".png" || ext == ".jpg" || ext == ".gif" || ext == "jpeg") {
			osg::ref_ptr<osg::Image> imgQzx = osgDB::readImageFile(m_fields[i]);
			osgEarth::Util::Controls::RoundedFrame* ctrl = new osgEarth::Util::Controls::RoundedFrame();
			ctrl->setImage(imgQzx.get());
			vecCtrls.push_back(ctrl);
		}
		else {
			//osgEarth::Util::Controls::LabelControl* content = new osgEarth::Util::Controls::LabelControl(osgX::ConvertChar::convertLocalStrToUTF8Str(m_fields[i]), osg::Vec4(1, 1, 1, 1), 22);
			osgEarth::Util::Controls::LabelControl* content = new osgEarth::Util::Controls::LabelControl(m_fields[i], osg::Vec4(1, 1, 1, 1), 22);
			content->setEncoding(osgText::String::ENCODING_UTF8);
			content->setFont(fntKai);
			vecCtrls.push_back(content);
		}

		ext = m_fields[i + 1].size()>4 ? m_fields[i+1].substr(m_fields[i+1].size() - 4, 4) : "";
		if (ext == ".png" || ext == ".jpg" || ext == ".gif" || ext == "jpeg") {
			osg::ref_ptr<osg::Image> imgQzx = osgDB::readImageFile(m_fields[i+1]);
			osgEarth::Util::Controls::RoundedFrame* ctrl = new osgEarth::Util::Controls::RoundedFrame();
			ctrl->setImage(imgQzx.get());
			vecCtrls.push_back(ctrl);
		}
		else {
			//osgEarth::Util::Controls::LabelControl* content = new osgEarth::Util::Controls::LabelControl(osgX::ConvertChar::convertLocalStrToUTF8Str(m_fields[i+1]), osg::Vec4(1, 1, 1, 1), 22);
			osgEarth::Util::Controls::LabelControl* content = new osgEarth::Util::Controls::LabelControl(m_fields[i + 1], osg::Vec4(1, 1, 1, 1), 22);
			content->setEncoding(osgText::String::ENCODING_UTF8);
			content->setFont(fntKai);
			vecCtrls.push_back(content);
		}
		m_refContent->addControls(vecCtrls);
	}
}

void PlaceProvider::setTipContent(std::string str, unsigned int tp)
{
	m_strTip = str;
	if (m_refTip.get() && m_bShowTip)
		createControl();
}

void osgEarthXCore::PlaceProvider::setTipButton(std::string str)
{
	m_strTipButton = str;
}

void osgEarthXCore::PlaceProvider::setTipHandler(TipClickHandler * hd)
{
	m_tipHandler = hd;

}

std::string osgEarthXCore::PlaceProvider::getTipField(std::string str)
{
	std::vector<std::string>::iterator result;
	result = std::find(m_fields.begin(), m_fields.end(), str);
	if (result != m_fields.end()) {
		result++;
		if(result != m_fields.end())
		    return *result;
	}
	return "";
}


void osgEarthXCore::PlaceProvider::setField(std::string strKey, std::string strVal)
{
	if (!strKey.empty()) {
		m_Attributes[strKey] = strVal;
	}
}


std::string osgEarthXCore::PlaceProvider::getField(std::string strKey)
{
	if (m_Attributes.find(strKey) != m_Attributes.end()) {
		return m_Attributes[strKey];
	}
	return "";
}


void osgEarthXCore::PlaceProvider::setEditor(osg::Group* con)
{
	m_Editors = con;
}

void osgEarthXCore::PlaceProvider::startEdit()
{
	if (m_refEditor.get() == NULL) {
		m_refEditor = new PlaceNodeEditor(this);
	}
	if(!m_Editors->containsNode(m_refEditor.get()))
		m_Editors->addChild(m_refEditor.get());
	setCullingActive(false);
}


void osgEarthXCore::PlaceProvider::stopEdit()
{
	setCullingActive(true);
	if (m_refEditor.get())
		if(m_Editors->containsNode(m_refEditor.get()))
			m_Editors->removeChild(m_refEditor.get());
}

void osgEarthXCore::PlaceProvider::clearAttached()
{
	clearAttachedEditor();
	clearAttachedTip();

	//if (m_Editors.get() && m_refEditor.get())
	//	if (m_Editors->containsNode(m_refEditor.get()))
	//		m_Editors->removeChild(m_refEditor.get());
	//if (m_refCanvas.get() && m_refTip.get())
	//	m_refCanvas->removeControl(m_refTip.get());
}

void osgEarthXCore::PlaceProvider::clearAttachedEditor()
{
	if (m_Editors.get() && m_refEditor.get())
		if (m_Editors->containsNode(m_refEditor.get()))
			m_Editors->removeChild(m_refEditor.get());
}

void osgEarthXCore::PlaceProvider::clearAttachedTip()
{
	if (m_refCanvas.get() && m_refTip.get())
		m_refCanvas->removeControl(m_refTip.get());
}

void osgEarthXCore::PlaceProvider::createTipButton()
{
	std::vector<std::string> strFields;
	//std::vector <std::string> fields;
	boost::split(strFields, m_strTipButton, boost::is_any_of("&"));

	osgText::Font *fntKai = osgText::readFontFile("simkai.ttf");
	if (m_refTipButton.get() == NULL) {
		m_refTipButton = new osgEarth::Util::Controls::HBox();
		m_refTipButton->setChildSpacing(10);
		//m_refTipButton->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_CENTER);
	}
	else {
		m_refTipButton->clearControls();
	}

	for (int i = 0; i < strFields.size(); i += 2)
	{
		//osgEarth::Util::Controls::ButtonControl *btn = new osgEarth::Util::Controls::ButtonControl(osgX::ConvertChar::convertLocalStrToUTF8Str(strFields[i+1]),
		//	22.0f, osg::Vec4f(1, 1, 1, 1), osg::Vec4f(0.5, 0.5, 0.5, 1), osg::Vec4f(1, .3, .3, 1));
		osgEarth::Util::Controls::ButtonControl *btn = new osgEarth::Util::Controls::ButtonControl(strFields[i + 1],
			22.0f, osg::Vec4f(1, 1, 1, 1), osg::Vec4f(0.5, 0.5, 0.5, 1), osg::Vec4f(1, .3, .3, 1));
		btn->setEncoding(osgText::String::ENCODING_UTF8);
		btn->setFont(fntKai);
		//btn->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_RIGHT);
		btn->addEventHandler(m_tipHandler.get());
		btn->setName(strFields[i]);
		m_refTipButton->addControl(btn);
	}
}

void PlaceProvider::UpdteTip(osgViewer::Viewer* vw)
{
	if (vw && m_refTip.get()) {
		osgEarth::GeoPoint pnt = getPosition();
		osg::Vec3d vWorld;
		pnt.toWorld(vWorld);
		osg::Camera* cam = vw->getCamera();
		osg::Matrix VPW = cam->getViewMatrix() *
			cam->getProjectionMatrix() *
			cam->getViewport()->computeWindowMatrix();
		osg::Vec3d vWin = vWorld * VPW;
		float y = cam->getViewport()->height() - vWin.y();
		osgEarth::Util::Controls::ControlContext cx;
		osg::Vec2f vh;
		m_refTip->calcSize(cx, vh);
		m_refTip->setPosition(vWin.x()-8-4, y-4-vh.y()-24);
	}
}

void PlaceProvider::showTip(bool bShow)
{
	//±à¼­×´Ì¬£¬²»Òªµ¯³ötip¿ò
	if (m_Editors.get() && m_refEditor.get())
		if (m_Editors->containsNode(m_refEditor.get()))
			return;

	if (m_bShowTip != bShow) {
		m_bShowTip = bShow;
		if (bShow) {
			if (m_refTip.get() == NULL)
			{
				m_refTip = new osgEarth::Util::Controls::VBox();
				m_refTip->setPosition(20, 20);
				//m_refTip->setPadding(10);

				osgEarth::Util::Controls::VBox* pBox = ((osgEarth::Util::Controls::VBox*)m_refTip.get());

				osgEarth::Util::Controls::VBox *vbox = new osgEarth::Util::Controls::VBox();
				vbox->setBorderColor(1, 1, 1, 1);
				vbox->setBackColor(.6, .5, .4, 0.8);

				osgEarth::Util::Controls::CheckBoxControl *ckClose = new osgEarth::Util::Controls::CheckBoxControl(true);
				ckClose->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_RIGHT);
				ckClose->addEventHandler(new TipClickHandler(this));
				vbox->addControl(ckClose);

				createControl();
				vbox->addControl(m_refContent.get());
				pBox->addControl(vbox);

				//osgText::Font *fntKai = osgText::readFontFile("simkai.ttf");
				//osgEarth::Util::Controls::ButtonControl *btn = new osgEarth::Util::Controls::ButtonControl(osgX::ConvertChar::convertLocalStrToUTF8Str("²¥·Å"),
				//	22.0f, osg::Vec4f(1, 1, 1, 1), osg::Vec4f(0.5, 0.5, 0.5, 1), osg::Vec4f(1, .3, .3, 1));
				//btn->setEncoding(osgText::String::ENCODING_UTF8);
				//btn->setFont(fntKai);
				//btn->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_RIGHT);
				//btn->addEventHandler(m_tipHandler.get());
				createTipButton();
				pBox->addControl(m_refTipButton.get());

				osg::ref_ptr<osg::Image> image = osgDB::readImageFile(osgDB::findLibraryFile("resource/arrowd.png"));
				if (image.valid())
				{
					osgEarth::Util::Controls::ImageControl *imgArrow = new osgEarth::Util::Controls::ImageControl(image.get());
					imgArrow->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_LEFT);
					imgArrow->setFixSizeForRotation(true);
					pBox->addControl(imgArrow);
				}
			}
			m_refCanvas->addControl(m_refTip.get());
		}
		else {
			m_refCanvas->removeControl(m_refTip.get());
		}
	}
}

void PlaceProvider::setControlCanvas(osgEarth::Util::Controls::ControlCanvas* canvas)
{
	m_refCanvas = canvas;
}

void PlaceProvider::setUnicodeText( const std::string& str )
{
	PlaceProvider::setText( str );
}

void PlaceProvider::_setImage( osg::Image* pImage )
{
	osg::ref_ptr<osg::ImageStream> imageStream = dynamic_cast<osg::ImageStream*>(pImage);
	if (imageStream.valid())
	{
		//imageStream->scaleImage(32, 32, 1);
		imageStream->play();
		PlaceNode::setIconImage(imageStream);
	}else
	    PlaceNode::setIconImage( pImage );
}

void PlaceProvider::_setImage2(osg::Image* pImage)
{
	PlaceNode::setIconImage2(pImage);
}

class ShowTxtOperation : public osg::Operation
{
public:
	ShowTxtOperation(PlaceProvider* pla, bool bShow): _pla(pla),_bShow(bShow){}

	virtual void operator()(osg::Object*)
	{
		_pla->showText(_bShow);
	}

protected:
	virtual ~ShowTxtOperation() {}

protected:
	PlaceProvider* _pla;
	bool _bShow;

private:
};

void PlaceProvider::setShowText(bool bShow)
{
	//showText(bShow);
	osgX::StaticOSGViewerAssistant::dealWithUpdateOperation(new ShowTxtOperation(this, bShow));
}

void PlaceProvider::setCulling(bool bCull)
{
	if (m_bCull == bCull) return;
	m_bCull = bCull;
	if (m_bCull){
		setOcclusionCulling(true, m_refCullingNodes);
	}
	else {
		setOcclusionCulling(false, NULL);
	}
}

bool PlaceProvider::getCulling()
{
	return m_bCull;
}