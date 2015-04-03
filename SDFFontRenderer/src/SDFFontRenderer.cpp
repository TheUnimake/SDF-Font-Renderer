#include <array>
#include <ft2build.h> 
#include FT_FREETYPE_H
#include "SDFFontRenderer.h"

struct Rect {
	float X;
	float Y;
	float Width;
	float Height;
};

struct FontGlyph {
	float BearingX;
	float BearingY;
	float Width;
	float Height;
	float Advance;
	Rect  TextureRect;
};

struct Font {
	std::array<FontGlyph, 256> Glyphs;
	float LineHeight;
	int TabSpaceCount;
};

SDFFontRenderer::SDFFontRenderer() {
	fontTexture = nullptr;
}

SDFFontRenderer::~SDFFontRenderer() {
	if (fontTexture != nullptr) {
		delete[] fontTexture;
	}
}

void SDFFontRenderer::SetWidth(unsigned int newWidth) {
	width = newWidth;
}

void SDFFontRenderer::SetHeight(unsigned int newHeight) {
	height = newHeight;
}

void SDFFontRenderer::SetFalloff(unsigned int newFalloff) {
	falloff = newFalloff;
}

void SDFFontRenderer::LoadFont(int fontSize, const char* fontFilename) {
	Font font;

	// font variables
	FT_Library   library;
	FT_Init_FreeType(&library);

	FT_Face    face;
	FT_UInt    glyph_index;
	FT_Bitmap  bitmap;

	int glyphI, left, right, top, bottom, penX, penY, i, currentLineMaxHeight;


	// allocate font texture 

	if (fontTexture != nullptr) {
		if (allocatedWidth != width || allocatedHeight != height) {
			delete[] fontTexture;
		}
	}

	fontTexture = new bool[width * height];
	allocatedWidth = width;
	allocatedHeight = height;

	// init freetype 
	FT_New_Face(library, fontFilename, 0, &face);
	FT_Set_Char_Size(face, fontSize * 64, 0, 96, 0);

	// apply line height
	font.LineHeight = fontSize * 96.f / 72.f;

	// begin values
	penX = falloff;
	penY = falloff;
	currentLineMaxHeight = fontSize * 96 / 72;

	// clear texture
	for (i = 0; i < width * height; i++) {
		fontTexture[i] = false;
	}

	// render each glyph bitmap to rectangle with the upper left corner (penX, penY) 
	for (unsigned char c = 0; c < 255; c++) {
		// load glyph and render to cache bitmap
		glyph_index = FT_Get_Char_Index(face, c);
		FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);
		bitmap = face->glyph->bitmap;

		// need next line ?
		if (width < penX + bitmap.width) {
			penY += currentLineMaxHeight + 2 * falloff;
			currentLineMaxHeight = 9 * 96 / 72;
			penX = falloff;
		}

		// is line higher than expected ?
		if (bitmap.rows > currentLineMaxHeight) {
			currentLineMaxHeight = bitmap.rows;
		}

		// glyph bounding box 
		left = penX;
		right = left + bitmap.width;
		top = penY;
		bottom = top + bitmap.rows;

		// exceptions
		if (right > width) {
			throw std::exception("width too small");
		}

		if (bottom > height) {
			throw std::exception("height too small");
		}

		// render bitmap to texture data
		glyphI = 0;

		for (int y = top; y < bottom; y++) {
			for (int x = left; x < right; x++) {
				i = y * width + x;
				fontTexture[i] = bitmap.buffer[glyphI] > 0;
				glyphI++;
			}
		}

		// calulcate glyph metrics in pixels and texture coordinates in opengl format
		font.Glyphs[c].Advance = (float)face->glyph->metrics.horiAdvance / 64.f;
		font.Glyphs[c].BearingX = (float)face->glyph->metrics.horiBearingX / 64.f;
		font.Glyphs[c].BearingY = (float)face->glyph->metrics.horiBearingY / 64.f;
		font.Glyphs[c].Width = (float)face->glyph->metrics.width / 64.f;
		font.Glyphs[c].Height = (float)face->glyph->metrics.height / 64.f;
		font.Glyphs[c].TextureRect.X = (float)left / (float)width;
		font.Glyphs[c].TextureRect.Y = (float)top / (float)height;
		font.Glyphs[c].TextureRect.Height = (float)bitmap.rows / (float)width;
		font.Glyphs[c].TextureRect.Width = (float)bitmap.width / (float)height;

		// move pen
		penX += bitmap.width + 2 * falloff;
	}
}


void SDFFontRenderer::CreateSDF() {
	if (fontTexture == nullptr) {
		return;
	}

	if (allocatedWidth != width) {
		return;
	}

	if (allocatedHeight != height) {
		return;
	}

	renderer.SetWidth(width);
	renderer.SetHeight(height);
	renderer.SetMinDistance(0.f);
	renderer.SetMaxDistance((float)falloff);
	renderer.CreateSDF(fontTexture);
}

float* SDFFontRenderer::GetSDF() const {
	return renderer.GetSDF();
}

bool* SDFFontRenderer::GetTexture() const {
	return fontTexture;
}