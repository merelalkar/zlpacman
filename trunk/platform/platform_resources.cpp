
#include "platform_windows.h"
using namespace Pegas;


/************************************************************************
   StringResource class implementation
*************************************************************************/

void StringResource::create(const IntResourceCode& code) 
{
	m_resourceCode = code;
	m_izInitialized = true;
}

void StringResource::load()
{
	if(!m_izInitialized) return;

	TCHAR buffer[k_bufferLength];
	
	memset(buffer, 0, sizeof(TCHAR) * k_bufferLength);
	LoadString(m_resourceCode.module, m_resourceCode.resourceID, buffer, k_bufferLength);

	m_loadedString = buffer;
}

void StringResource::destroy()
{
	m_izInitialized = false;
}

/**********************************************************************
		TextureResource class implementation
**********************************************************************/

int TextureResource::s_instanceCounter = 0;
HDC TextureResource::s_deviseContext = 0;
std::set<int>* TextureResource::s_pTextureValidSizes = 0;

TextureResource::TextureResource()
	:m_pPixels(0), m_pInvertedPixels(0), m_nImageWidth(0), m_nImageHeght(0),
	m_textureID(0), m_invertedTextureID(0), m_fMaxUCoord(0.0f), m_fMaxVCoord(0.0f)
{
	inreaseInstanceCounter();
}
	
TextureResource::~TextureResource()
{
	decreaseInstanceCounter();
}

void TextureResource::create(const IntResourceCode& code)
{
	m_resourceCode = code;
	m_izInitialized = true;
}

void TextureResource::load()
{
	if(!m_izInitialized)
		return;
	
	destroy();

	HDC memDC = getDeviceContext();
	HBITMAP hBitmap = LoadBitmap(m_resourceCode.module, MAKEINTRESOURCE(m_resourceCode.resourceID));
	
	int nPitch;
	BITMAPINFO* pbmInfo = createBitmapInfoStruct(hBitmap, nPitch);
	
	LPBYTE pBuffer = new BYTE[pbmInfo->bmiHeader.biSizeImage];
	GetDIBits(memDC, hBitmap, 0, pbmInfo->bmiHeader.biHeight, pBuffer, pbmInfo, DIB_RGB_COLORS);

	int innerWidth = pbmInfo->bmiHeader.biWidth;
	int innerHeight = pbmInfo->bmiHeader.biHeight;
	if(s_pTextureValidSizes->count(innerWidth) == 0)
	{
		std::set<int>::iterator find_it = std::find_if(s_pTextureValidSizes->begin(), 
			s_pTextureValidSizes->end(), std::bind2nd(std::greater<int>(), innerWidth));
		if(find_it != s_pTextureValidSizes->end())
			m_nImageWidth = *find_it;
	}else
	{
		m_nImageWidth = innerWidth;
	}

	if(s_pTextureValidSizes->count(innerHeight) == 0)
	{
		std::set<int>::iterator find_it = std::find_if(s_pTextureValidSizes->begin(), 
			s_pTextureValidSizes->end(), std::bind2nd(std::greater<int>(), innerHeight));
		if(find_it != s_pTextureValidSizes->end())
			m_nImageHeght = *find_it;
	}else
	{
		m_nImageHeght = innerHeight;
	}	

	m_fMaxUCoord = (innerWidth * 1.0) / m_nImageWidth;
	m_fMaxVCoord = (innerHeight * 1.0) / m_nImageHeght;
	m_pPixels = new DWORD[m_nImageWidth * m_nImageHeght];
	m_pInvertedPixels = new DWORD[m_nImageWidth * m_nImageHeght];
		
	BYTE* pIndexPointer = pBuffer;
	DWORD* pPixelsPointer = m_pPixels;
	DWORD* pInvertedPixelsPointer = m_pInvertedPixels;
	DWORD srcColor, dstColor, invColor;

	for(int y = 0; y < m_nImageHeght; y++)
	{
		DWORD* pCurrentBitString = (DWORD*)pIndexPointer;
		for(int x = 0; x < m_nImageWidth; x++)
		{
			dstColor = invColor = 0;
			if(x < innerWidth && y < innerHeight)
			{
				srcColor = pCurrentBitString[x];
				if(srcColor != k_colorKey)
				{
					BYTE blue = (BYTE)((srcColor & 0x00ff0000) >> 16);
					BYTE green = (BYTE)((srcColor & 0x0000ff00) >> 8);
					BYTE red = (BYTE)(srcColor & 0x000000ff);
					dstColor = (red << 16) | (green << 8) | blue | 0xff000000;
					invColor = ((255 - red) << 16) | ((255 - green) << 8) | (255 - blue) | 0xff000000;
				}
			}
			
			*pPixelsPointer = dstColor;
			*pInvertedPixelsPointer = invColor;
			
			pPixelsPointer++;
			pInvertedPixelsPointer++;
		}

		if(y < innerHeight)
		{
			pIndexPointer+= nPitch;
		}
	}

	delete[] pBuffer;
	LocalFree(pbmInfo);

	m_textureID = createTexture(m_pPixels, m_nImageWidth, m_nImageHeght);
	m_invertedTextureID = createTexture(m_pInvertedPixels, m_nImageWidth, m_nImageHeght);
}

void TextureResource::destroy()
{
	if(m_pPixels) delete[] m_pPixels;
	if(m_pInvertedPixels) delete[] m_pInvertedPixels;

	m_izInitialized = false;
	m_pPixels = 0;
	m_pInvertedPixels = 0;
	m_nImageWidth = 0;
	m_nImageHeght = 0;

	m_textureID = 0;
	m_invertedTextureID = 0;
	m_fMaxUCoord = 0.0f;
	m_fMaxVCoord = 0.0f;
}

int TextureResource::inreaseInstanceCounter()
{
	if(s_instanceCounter == 0)
	{
		s_deviseContext = CreateCompatibleDC(NULL);
		s_pTextureValidSizes = new std::set<int>();
		s_pTextureValidSizes->insert(64);
		s_pTextureValidSizes->insert(128);
		s_pTextureValidSizes->insert(256);
		s_pTextureValidSizes->insert(512);
		s_pTextureValidSizes->insert(1024);
	}
	
	return (++s_instanceCounter);
}

int TextureResource::decreaseInstanceCounter()
{
	s_instanceCounter--;
	
	if(s_instanceCounter == 0)
	{
		DeleteDC(s_deviseContext);
		
		delete s_pTextureValidSizes;
		s_pTextureValidSizes = 0;
	}

	return s_instanceCounter;
}

PBITMAPINFO TextureResource::createBitmapInfoStruct(HBITMAP hBmp, int& nPitch)
{
	BITMAP bmp; 
    PBITMAPINFO pbmi; 
    WORD    cClrBits; 

    // Retrieve the bitmap color format, width, and height. 
    GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp); 
         

    // Convert the color format to a count of bits. 
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
    if (cClrBits == 1) 
        cClrBits = 1; 
    else if (cClrBits <= 4) 
        cClrBits = 4; 
    else if (cClrBits <= 8) 
        cClrBits = 8; 
    else if (cClrBits <= 16) 
        cClrBits = 16; 
    else if (cClrBits <= 24) 
        cClrBits = 24; 
    else cClrBits = 32; 

    // Allocate memory for the BITMAPINFO structure. (This structure 
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD 
    // data structures.) 

     if (cClrBits < 24) 
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
                    sizeof(BITMAPINFOHEADER) + 
                    sizeof(RGBQUAD) * (1<< cClrBits)); 

     // There is no RGBQUAD array for the 24-bit-per-pixel format. 

     else 
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR,	sizeof(BITMAPINFOHEADER)); 

    // Initialize the fields in the BITMAPINFO structure. 

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    pbmi->bmiHeader.biWidth = bmp.bmWidth; 
    pbmi->bmiHeader.biHeight = bmp.bmHeight; 
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
    if (cClrBits < 24) 
        pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

    // If the bitmap is not compressed, set the BI_RGB flag. 
    pbmi->bmiHeader.biCompression = BI_RGB; 

    // Compute the number of bytes in the array of color 
    // indices and store the result in biSizeImage. 
    // For Windows NT, the width must be DWORD aligned unless 
    // the bitmap is RLE compressed. This example shows this. 
    // For Windows 95/98/Me, the width must be WORD aligned unless the 
    // bitmap is RLE compressed.
	nPitch = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8;
    pbmi->bmiHeader.biSizeImage = nPitch * pbmi->bmiHeader.biHeight; 
    // Set biClrImportant to 0, indicating that all of the 
    // device colors are important. 
    pbmi->bmiHeader.biClrImportant = 0; 
    
	return pbmi;
}

GLuint TextureResource::createTexture(DWORD* pBuffer, int width, int height)
{
	GLuint result;
		
	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D, result);
 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, width, height,
		0,GL_RGBA,GL_UNSIGNED_BYTE,	pBuffer);

	return result;
}

/*******************************************************************************
		FontResource class implementation
********************************************************************************/

int FontResource::s_instanceCounter = 0;
HDC FontResource::s_deviseContext = 0;

FontResource::FontResource()
	:m_hFont(0), m_dlBaseOffset(0)
{
	inreaseInstanceCounter();	
}

FontResource::~FontResource()
{
	decreaseInstanceCounter();
}

void FontResource::create(const FontResourceCode& code)
{
	m_resourceCode = code;
	m_izInitialized = true;
}

void FontResource::load()
{
	if(!m_izInitialized)
		return;

	destroy();

	m_dlBaseOffset = glGenLists(k_numCharsInSet);

	char fontName[256];
	wcstombs(fontName, m_resourceCode.fontName.c_str(), 256);

	m_hFont = CreateFontA(-m_resourceCode.fontSize,	// Height Of Font
						0,							// Width Of Font
						0,							// Angle Of Escapement
						0,							// Orientation Angle
						FW_BOLD,					// Font Weight
						FALSE,						// Italic
						FALSE,						// Underline
						FALSE,						// Strikeout
						DEFAULT_CHARSET,			// Character Set Identifier
						OUT_TT_PRECIS,				// Output Precision
						CLIP_DEFAULT_PRECIS,		// Clipping Precision
						ANTIALIASED_QUALITY,		// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,	// Family And Pitch
						fontName);

	SelectObject(getDeviceContext(), m_hFont);
	wglUseFontBitmapsA(getDeviceContext(), 0, k_numCharsInSet, m_dlBaseOffset);
}

void FontResource::destroy()
{
	if(m_hFont)
	{
		DeleteObject(m_hFont);
	}

	m_hFont = 0;
	m_dlBaseOffset = 0;
}

GLubyte* FontResource::prepareString(const String& text, int& bufferLength)
{
#ifdef _UNICODE
	size_t size;
	wcstombs_s(&size, (char*)m_buffer, k_maxBufferLength, text.c_str(), _TRUNCATE);
	bufferLength = size - 1;
	
#else
	
	bufferLength = text.size();
	memcpy(m_buffer, text.c_str(), bufferLength);
	m_buffer[bufferLength] = 0;
	
#endif

	return m_buffer;
}

void FontResource::getTextExtent(const String& text, CURCOORD& width, CURCOORD& height)
{
	SIZE extent;

	SelectObject(getDeviceContext(), m_hFont);
	GetTextExtentPoint(getDeviceContext(), text.c_str(), text.size(), &extent);

	width = extent.cx;
	height = extent.cy;
}

int FontResource::inreaseInstanceCounter()
{
	if(s_instanceCounter == 0)
	{
		s_deviseContext = CreateCompatibleDC(0);
		_tsetlocale(LC_ALL, _text("Russian"));
	}

	return (++s_instanceCounter);
}

int FontResource::decreaseInstanceCounter()
{
	s_instanceCounter--;
	
	if(s_instanceCounter == 0)
	{
		DeleteDC(s_deviseContext);
	}

	return s_instanceCounter;
}

