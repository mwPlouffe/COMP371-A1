//
//  heightMap.h
//  COMP371-A1
//
//  Created by Mathew Plouffe on 2018-05-12.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef heightMap_h
#define heightMap_h

#include "mesh.h"
#define MAP_SCALE 1

#define cimg_use_png
#define cimg_use_jpeg
#define cimg_use_tiff
#define cimg_display 0
#include "CImg.h"

using namespace cimg_library;

class HeightMap : public Mesh
{
	protected:
		virtual void index(void);
		virtual void indexList(void);
	private:
		int width;
		int height;
		void generateVertices(CImg<unsigned char> image);
	public:
		HeightMap(cimg_library::CImg<unsigned char> *image);
	
	
	
};
#endif /* heightMap_h */
