//
//  pointLight.h
//  COMP371-A2
//
//  Created by Mathew Plouffe on 2018-05-17.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef pointLight_h
#define pointLight_h

#include "light.h"

class PointLight : public Light
{
	protected:
		glm::vec3 position;
		bool attentuated;
		float constant;
		float linear;
		float quadratic;
	public:
		PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, float c, float l, float q);
		PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position);
};


#endif /* pointLight_h */
