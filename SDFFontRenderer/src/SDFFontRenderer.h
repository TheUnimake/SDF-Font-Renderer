#pragma once
#include "SDFRenderer.h"

class SDFFontRenderer {
public:
	SDFFontRenderer();
	~SDFFontRenderer();

	void SetWidth(unsigned int newWidth);
	void SetHeight(unsigned int newHeight);
	void SetFalloff(unsigned int newFalloff);

	void LoadFont(int fontSize, const char* fontFilename);
	void CreateSDF();

	float* GetSDF() const;
	bool*  GetTexture() const;

private:
	unsigned int width;
	unsigned int height;
	unsigned int allocatedWidth;
	unsigned int allocatedHeight;
	unsigned int falloff;
	SDFRenderer  renderer;

	bool*        fontTexture;
};