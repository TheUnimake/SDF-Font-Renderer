#pragma once
#include <math.h>
#include <queue>

class SDFRenderer {
public:

	SDFRenderer();
	~SDFRenderer();

	// Set texture width.
	// Param:
	//       unsigned int newWidth:
	//          New texture width.
	void SetWidth(unsigned int newWidth);
	
	// Set texture height.
	// Param:
	//       unsigned int newHeight:
	//          New texture height.
	void SetHeight(unsigned int newHeight);

	// Set maximal distance for clamping.
	// Param:
	//       float max:
	//          New maximal distance.
	void SetMaxDistance(float max);

	// Set minimal distance for claming.
	// Param:
	//       float min:
	//          New minimal distance.
	void SetMinDistance(float min);

	// Create and clamp signed distance field.
	// Param:
	//       bool* data: 
	//          An array which elements describes whether one element 
	//          is inside or outside of the object. This array must 
	//          have the dimension width * height.
	void CreateSDF(bool* data);

	// Get clamped signed distance field
	// Return (float*):
	//          Returns an array with dimensions width * height containing 
	//          the calculated signed distance field. Returned values 
	//          are clamped to MinDistance and MaxDistance and linear mapped 
    //          to the range 0.0 to 1.0.  
	//          Returns nullptr if no calculation was done.
	float* SDFRenderer::GetSDF() const;

private:
	struct DistInfo {
		unsigned int X;
		unsigned int Y;

		float nearestX;
		float nearestY;
	};

	void calculateDist(bool* data);
	void clampDist();

	float* distances;
	float maxDist;
	float minDist;
	unsigned int width;
	unsigned int height;
	unsigned int allocatedWidth;
	unsigned int allocatedHeight;

	std::queue<DistInfo> blocks;
};