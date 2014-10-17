
#include "platform_resources.h"
using namespace Pegas;

#include "log.h"

namespace Pegas
{
	using namespace packt;
	/**********************************************************************
			TextureResource class implementation
	**********************************************************************/
	TextureResource::TextureResource()
	{
		Pegas_log_info("TextureResource constructor");

		m_isLoaded = false;
		m_nImageWidth = 0;
		m_nImageHeght = 0;
		m_nInnerImageWidth = 0;
		m_nInnerImageHeight = 0;
	}

	TextureResource::~TextureResource()
	{
		Pegas_log_info("TextureResource destructor");
	}

	void TextureResource::create(const PathResourceCode& code)
	{
		Pegas_log_info("TextureResource::create [path: %s]", code._path.c_str());

		m_pTexture = new packt::GraphicsTexture(code._app, code._path.c_str());
	}

	void TextureResource::load()
	{
		Pegas_log_info("TextureResource::load");

		if(m_isLoaded)
		{
			Pegas_log_warning("texture already load");
			return;
		}

		if(!m_pTexture.IsValid())
		{
			Pegas_log_error("texture object not create!");
			return;
		}

		status result = m_pTexture->load();
		if(result != STATUS_OK)
		{
			Pegas_log_error("texture not load!");
			return;
		}

		m_isLoaded = true;

		m_nImageWidth = m_pTexture->getWidth();
		m_nImageHeght = m_pTexture->getHeight();
		m_nInnerImageWidth = m_pTexture->getWidth();
		m_nInnerImageHeight = m_pTexture->getHeight();
	}

	void TextureResource::destroy()
	{
		Pegas_log_info("TextureResource::destroy");

		if(m_pTexture.IsValid())
		{
			m_pTexture->unload();
		}
	}

	void TextureResource::apply()
	{
		Pegas_log_info("TextureResource::apply");

		if(m_pTexture.IsValid())
		{
			m_pTexture->apply();
		}
	}

	/*******************************************************************************
			FontResource class implementation
	********************************************************************************/
}



