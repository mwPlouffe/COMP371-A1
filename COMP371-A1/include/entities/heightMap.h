//
//  heightMap.h
//  COMP371-A1
//
//  Created by Mathew Plouffe on 2018-05-12.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef heightMap_h
#define heightMap_h

#include "object.h"
#include <iostream>

class HeightMap : public Object
{
	int width, height;
	public:
	HeightMap(void);
	HeightMap(std::vector<GLfloat> vertices, int w, int h);
	HeightMap(std::vector<GLfloat> vertices, std::vector<GLfloat> colours, int w, int h);
	virtual std::vector<GLuint>triangleIndex(void);
	virtual std::vector<GLuint>pointIndex(void);
	virtual void init(void);
	
};
#endif /* heightMap_h */
