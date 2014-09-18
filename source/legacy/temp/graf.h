#ifndef TEMP_GRAF_H
#define TEMP_GRAF_H

struct Texture
{
	Texture(): pBuffer(0), nWidth(0), nHeight(0), nMaxU(0), nMaxV(0)  {};
	~Texture() { if(pBuffer) delete[] pBuffer; };
	
	DWORD* pBuffer;
	int  nWidth;
	int  nHeight;
	GLfloat nMaxU;
	GLfloat nMaxV;

private:
	Texture(const Texture& src);
	Texture& operator=(const Texture& src);
};

extern HINSTANCE g_hInstance;

BOOL bSetupPixelFormat(HDC);
GLvoid resize(GLsizei, GLsizei); 
GLvoid initializeGL(GLsizei, GLsizei); 
GLvoid drawScene(GLvoid);
void cleanup();
void makeCheckImage();

void createObjects();
void loadTexture(UINT resourceID, Texture& texture);
GLuint createTextureObject(Texture& texture);

#endif //TEMP_GRAF_H
