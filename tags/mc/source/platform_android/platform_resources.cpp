
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
	 : m_isLoaded(false)
	{
		Pegas_log_info("TextureResource constructor");
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
	}

	void TextureResource::destroy()
	{
		Pegas_log_info("TextureResource::destroy");

		if(m_pTexture.IsValid())
		{
			m_pTexture->unload();
			m_isLoaded = false;
		}
	}

	void TextureResource::apply()
	{
		Pegas_log_info("TextureResource::apply");

		if(!m_isLoaded)
		{
			Pegas_log_warning("texture has not load");
			return;
		}

		if(m_pTexture.IsValid())
		{
			m_pTexture->apply();
		}
	}

	/*******************************************************************************
			FontResource class implementation
	********************************************************************************/
}



