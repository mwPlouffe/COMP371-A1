//
//  spotLight.h
//  COMP371-A2
//
//  Created by Mathew Plouffe on 2018-05-17.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef spotLight_h
#define spotLight_h

#include "pointLight.h"

class SpotLight : public PointLight, public directionalLight
{
	float maxAngle;
	bool softEdges;
	public:
		PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 direction, float c, float l, float q, float angle, bool soft);
		PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 direction, bool soft);
};


#endif /* spotLight_h */
