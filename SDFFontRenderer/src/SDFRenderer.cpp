#include "SDFRenderer.h"
#include <iostream>

SDFRenderer::SDFRenderer() {

	// set default values
	width = 0;
	height = 0;

	minDist = 0.f;
	maxDist = 10.f;

	distances = nullptr;
}

SDFRenderer::~SDFRenderer() {

	// delete distance buffer
	if (distances != nullptr) {
		delete[] distances;
	}
}

void SDFRenderer::SetWidth(unsigned int newWidth) {
	width = newWidth;
}

void SDFRenderer::SetHeight(unsigned int newHeight) {
	height = newHeight;
}

void SDFRenderer::SetMaxDistance(float max) {
	maxDist = max;
}

void SDFRenderer::SetMinDistance(float min) {
	minDist = min;
}

void SDFRenderer::CreateSDF(bool* data) {

	// check if required values are set
	if (width == 0) {
		return;
	}

	if (height == 0) {
		return;
	}

	// allocate distance buffer
	if (distances == nullptr) {
		distances = new float[width * height];
	} else {
		// check dimensions of current buffer
		if (allocatedWidth != width || allocatedHeight != height) {
			delete[] distances;
			distances = new float[width * height];
		}
	}

	allocatedWidth = width;
	allocatedHeight = height;

	// do the magic
	calculateDist(data);
	clampDist();
}

void SDFRenderer::calculateDist(bool* data) {
	// first pass: find egdes
	unsigned int i, x, y;
	float maxDist = sqrtf(width * width + height * height), newDist, dx, dy;
	DistInfo info;

	// initialize distance values & processing queue
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			i = y * width + x;

			if (data[i]) {
				distances[i] = -maxDist;
			} else {
				distances[i] = maxDist;
			}

			// left
			if (x > 0) {
				if (data[i - 1] != data[i]) {
					info.X = x - 1;
					info.Y = y;

					info.nearestX = x - 0.5f;
					info.nearestY = y;

					blocks.push(info);
				}
			}

			// right
			if (x < width - 1) {
				if (data[i + 1] != data[i]) {
					info.X = x + 1;
					info.Y = y;

					info.nearestX = x + 0.5f;
					info.nearestY = y;

					blocks.push(info);
				}
			}
			// top  
			if (y > 0) {
				if (data[i - width] != data[i]) {
					info.X = x;
					info.Y = y - 1;

					info.nearestX = x;
					info.nearestY = y - 0.5f;

					blocks.push(info);
				}
			}

			// bottom  
			if (y < height - 1) {
				if (data[i + width] != data[i]) {
					info.X = x;
					info.Y = y + 1;

					info.nearestX = x;
					info.nearestY = y + 0.5f;

					blocks.push(info);
				}
			}
		}
	}

	unsigned long counter = 0;

	while (!blocks.empty()) {
		counter++;

		info = blocks.front();
		blocks.pop();

		i = info.Y * width + info.X;

		x = info.X;
		y = info.Y;

		dx = (info.X - info.nearestX);
		dy = (info.Y - info.nearestY);

		newDist = sqrtf(dx * dx + dy * dy);

		float dist = distances[i];

		// check if the suggested block is closer 

		bool closer = false;

		if (signbit(dist)) {
			closer = newDist < -dist;
		} else {
			closer = newDist < dist;
		}

		// if block is closer then inform all neighbors

		if (closer) {
			if (signbit(dist)) {
				distances[i] = -newDist;
			} else {
				distances[i] = newDist;
			}

			// left
			if (x > 0) {
				if (data[i - 1] == data[i]) {
					info.X = x - 1;
					info.Y = y;

					blocks.push(info);
				}
			}

			// right
			if (x < width - 1) {
				if (data[i + 1] == data[i]) {
					info.X = x + 1;
					info.Y = y;

					blocks.push(info);
				}
			}

			// top  
			if (y > 0) {
				if (data[i - width] == data[i]) {
					info.X = x;
					info.Y = y - 1;

					blocks.push(info);
				}
			}

			// top  
			if (y < height - 1) {
				if (data[i + width] == data[i]) {
					info.X = x;
					info.Y = y + 1;

					blocks.push(info);
				}
			}
		}
	}
}

void SDFRenderer::clampDist() {
	float dist;

	for (unsigned int i = 0; i < width * height; i++) {
		dist = abs(distances[i]);

		// clamp distances
		if (dist < 0.f) {
			if (dist < minDist) {
				distances[i] = 0.f;
			} else if (dist > maxDist) {
				distances[i] = 1.f;
			} else {
				distances[i] = -(distances[i] - minDist) / (maxDist - minDist);
			}
		} else {
			if (dist < minDist) {
				distances[i] = 0.f;
			} else if (dist > maxDist) {
				distances[i] = 1.f;
			} else {
				distances[i] = (distances[i] - minDist) / (maxDist - minDist);
			}
		}
	}
}

float* SDFRenderer::GetSDF() const {
	return distances;
}