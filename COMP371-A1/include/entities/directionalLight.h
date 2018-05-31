//
//  directionalLight.h
//  COMP371-A2
//
//  Created by Mathew Plouffe on 2018-05-17.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef directionalLight_h
#define directionalLight_h

#include "light.h"

class DirectionalLight : public Light
{
	protected:
		glm::vec3 direction;
	public:
		DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 dir);
	
};

#endif /* directionalLight_h */
