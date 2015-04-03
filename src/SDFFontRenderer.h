#pragma once
#include "SDFRenderer.h"

class SDFFontRenderer {
public:
	SDFFontRenderer();
	~SDFFontRenderer();

	// Sets texture width.
	// Param:
	//       unsigned int newWidth:
	//          New texture width.
	void SetWidth(unsigned int newWidth);

	// Sets texture height.
	// Param:
	//       unsigned int newFalloff:
	//          New texture height.
	void SetHeight(unsigned int newHeight);

	// Sets the maximal distance to the interior of a glyph
	// considered by the sdf algorithm.
	// Param:
	//       unsigned int newFalloff:
	//          New falloff radius.
	void SetFalloff(unsigned int newFalloff);

	// Loads a font with FreeType library.
	// Param:
	//       int fontSize: 
	//          Indicates the desired font size. See
	//          www.freetype.org/freetype2/docs/reference/ft2-base_interface.html#FT_Set_Char_Size
	//       const char* fontFilename: 
	//          File path to a font file with a format supported
	//          by the FreeType library.
	void LoadFont(int fontSize, const char* fontFilename);
	
	// Creates the signed distance field based on the loaded font atlas.
	void CreateSDF();

	// Returns rendered font sdf atlas
	// Return (float*):
	//          Returns an array with dimensions width * height containing 
	//          normalized values in the range -1.0 to 1.0 where negative values 
	//          are inside of a glyph and positive values are outside of a glyph.
	float* GetSDF() const;

	// Returns rendered font atlas
	// Return (bool*):
	//          Returns an array with dimensions width * height containing 
	//          booleans with the value true or false for a pixel corresponding to
	//          whether this pixel is inside or outside of a glyph.
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