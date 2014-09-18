
#include "stdafx.h"
#include "graf.h"
#include "resource.h"

GLsizei g_windowWidth = 0;
GLsizei g_windowHeight = 0;

GLuint g_texName = 0;
DWORD* g_pPixels;
int g_nWidth = 0;
int g_nHeight = 0;

HINSTANCE g_hInstance = 0;
HDC g_hDC = 0;
HGLRC g_hGLRC = 0;

#define checkImageWidth 64
#define checkImageHeight 64
 
GLubyte checkImage[checkImageHeight][checkImageWidth][4];

const int k_maxTextures = 5;
Texture g_vTextures[k_maxTextures];
GLuint   g_vTextureIDs[k_maxTextures];
int g_counter = 1;

PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp, int& nPitch);
 
void makeCheckImage(Texture& texture)
{
	int i,j,c;
	DWORD color;

	texture.pBuffer = new DWORD[64 * 64];
	texture.nWidth = 64;
	texture.nHeight = 64;
 
	DWORD* pPointer = texture.pBuffer;
	for (i=0; i<texture.nHeight; i++)
	{
		for (j=0; j<texture.nWidth; j++)
		{
			c=(((i&0x8)==0)^((j&0x8)==0))*255;
	        color = 0x99000000|(c << 16)|(c << 8)|c;
			*pPointer = color;
			pPointer++;
		}	
	}
}



BOOL bSetupPixelFormat(HDC hdc) 
{
	g_hDC = hdc;

    PIXELFORMATDESCRIPTOR pfd, *ppfd; 
    int pixelformat; 
 
    ppfd = &pfd; 
 
    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR); 
    ppfd->nVersion = 1; 
    ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |PFD_DOUBLEBUFFER; 
    ppfd->dwLayerMask = PFD_MAIN_PLANE; 
     
    pixelformat = ChoosePixelFormat(hdc, ppfd); 
 
    if ( (pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0 ) 
    { 
        MessageBox(NULL, TEXT("ChoosePixelFormat failed"), TEXT("Error"), MB_OK); 
        return FALSE; 
    } 
 
    if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE) 
    { 
        MessageBox(NULL, TEXT("SetPixelFormat failed"), TEXT("Error"), MB_OK); 
        return FALSE; 
    }

	g_hGLRC = wglCreateContext(g_hDC);
 
    return TRUE; 
}

GLvoid initializeGL(GLsizei width, GLsizei height) 
{
	wglMakeCurrent(g_hDC, g_hGLRC);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 

	resize(width, height);
} 


GLvoid resize( GLsizei width, GLsizei height ) 
{ 
    glViewport(0, 0, (g_windowWidth = width), (g_windowHeight = height));
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, (GLdouble)g_windowWidth, (GLdouble)g_windowHeight, 0); 
}

GLvoid drawScene(GLvoid) 
{ 
	wglMakeCurrent(g_hDC, g_hGLRC);

    glClear(GL_COLOR_BUFFER_BIT); 
 
    glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glColor4ub(255, 255, 255, 255);
		
	
	GLfloat maxU = (GLfloat) ((g_windowWidth * 1.0) / g_vTextures[0].nWidth);
	GLfloat maxV = (GLfloat) ((g_windowHeight * 1.0) / g_vTextures[0].nHeight);
	
	glBindTexture(GL_TEXTURE_2D, g_vTextureIDs[0]);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, maxV); glVertex2i(0, 0);
	glTexCoord2f(maxU, maxV); glVertex2i(g_windowWidth, 0);
	glTexCoord2f(maxU, 0.0f); glVertex2i(g_windowWidth, g_windowHeight);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(0, g_windowHeight);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, g_vTextureIDs[1]);
	
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, g_vTextures[1].nMaxV);	glVertex2i(100, 100);
	
	glTexCoord2f(g_vTextures[1].nMaxU, 
		g_vTextures[1].nMaxV);					glVertex2i(171, 100); 
	glTexCoord2f(g_vTextures[1].nMaxU, 0.0f); 	glVertex2i(171, 196);
	glTexCoord2f(0.0f, 0.0f); 					glVertex2i(100, 196);
	
	glEnd();
 	
	glDisable(GL_TEXTURE_2D);

	
	glBegin(GL_QUADS);
	glColor4ub(0, 200, 0, 255);
	glVertex2i(200, 100);
	glVertex2i(350, 100);
	glVertex2i(350, 200);
	glVertex2i(200, 200);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor4ub(0, 0, 0, 255);
	glVertex2i(200, 100);
	glVertex2i(350, 100);
	glVertex2i(350, 200);
	glVertex2i(200, 200);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, g_vTextureIDs[2]);

	glBegin(GL_QUADS);
	glColor4ub(255, 0, 0, 255);
	glTexCoord2f(0.0f, g_vTextures[2].nMaxV);	glVertex2i(10, 10);

	glTexCoord2f(g_vTextures[2].nMaxU, 
		g_vTextures[2].nMaxV);					glVertex2i(20, 10); 
	glTexCoord2f(g_vTextures[2].nMaxU, 0.0f); 	glVertex2i(20, 20);
	glTexCoord2f(0.0f, 0.0f);					glVertex2i(10, 20);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glFlush();

	
 
    SwapBuffers(g_hDC); 
}

void cleanup()
{
	wglDeleteContext(g_hGLRC);	
}


void createObjects()
{
	loadTexture(IDB_FELT, g_vTextures[0]);
	loadTexture(IDB_CARDBACK, g_vTextures[1]);
	loadTexture(IDB_CIRCLE, g_vTextures[2]);
	

	g_vTextureIDs[0] = createTextureObject(g_vTextures[0]);
	g_vTextureIDs[1] = createTextureObject(g_vTextures[1]);
	g_vTextureIDs[2] = createTextureObject(g_vTextures[2]);
}

void loadTexture(UINT resourceID, Texture& texture)
{
	std::set<int> validSizes;
	validSizes.insert(64);
	validSizes.insert(128);
	validSizes.insert(256);
	validSizes.insert(512);

	HDC memDC = CreateCompatibleDC(NULL);
	HBITMAP hBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(resourceID));
	
	int nPitch;
	BITMAPINFO* pbmInfo = CreateBitmapInfoStruct(hBitmap, nPitch);
	
	LPBYTE pBuffer = new BYTE[pbmInfo->bmiHeader.biSizeImage];
	GetDIBits(memDC, hBitmap, 0, pbmInfo->bmiHeader.biHeight, pBuffer, pbmInfo, DIB_RGB_COLORS);

	int innerWidth = pbmInfo->bmiHeader.biWidth;
	int innerHeight = pbmInfo->bmiHeader.biHeight;
	if(validSizes.count(innerWidth) == 0)
	{
		std::set<int>::iterator find_it = std::find_if(validSizes.begin(), validSizes.end(), 
			std::bind2nd(std::greater<int>(), innerWidth));
		if(find_it != validSizes.end())
			texture.nWidth = *find_it;
	}else
	{
		texture.nWidth = innerWidth; 		
	}

	if(validSizes.count(innerHeight) == 0)
	{
		std::set<int>::iterator find_it = std::find_if(validSizes.begin(), validSizes.end(), 
			std::bind2nd(std::greater<int>(), innerHeight));
		if(find_it != validSizes.end())
			texture.nHeight = *find_it;
	}else
	{
		texture.nHeight = innerHeight; 		
	}

	texture.nMaxU = (innerWidth * 1.0) / texture.nWidth;
	texture.nMaxV = (innerHeight * 1.0) / texture.nHeight;
	texture.pBuffer = new DWORD[texture.nWidth * texture.nHeight];
		
	BYTE* pIndexPointer = pBuffer;
	DWORD* pDstPointer = texture.pBuffer;
	DWORD color;
	for(int y = 0; y < texture.nHeight; y++)
	{
		DWORD* pCurrentBitString = (DWORD*)pIndexPointer;
		for(int x = 0; x < texture.nWidth; x++)
		{
			if(x < innerWidth && y < innerHeight)
			{
				color = pCurrentBitString[x];
				if(color != 0x000000ff)
				{
					BYTE blue = (BYTE)((color & 0x00ff0000) >> 16);
					BYTE green = (BYTE)((color & 0x0000ff00) >> 8);
					BYTE red = (BYTE)(color & 0x000000ff);
					color = (red << 16) | (green << 8) | blue | 0xff000000;
				}else
				{
					color = 0x00000000;
				}
			}else
			{
				color = 0x00000000;
			}

			(*pDstPointer) = color;
			pDstPointer++;
		}

		if(y < innerHeight)
			pIndexPointer+= nPitch;
	}

	delete[] pBuffer;
	
    LocalFree(pbmInfo); 
	DeleteDC(memDC);		
}

GLuint createTextureObject(Texture& texture)
{
	GLuint result;
		
	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D, result);
 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, texture.nWidth, texture.nHeight,
		0,GL_RGBA,GL_UNSIGNED_BYTE,	texture.pBuffer);

	return result;
}

PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp, int& nPitch)
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
