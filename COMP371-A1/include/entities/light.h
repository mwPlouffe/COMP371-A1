//
//  light.h
//  COMP371-A2
//
//  Created by Mathew Plouffe on 2018-05-17.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef Light_h
#define Light_h

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Light
{
	protected:
	glm::vec3 ambientLight;
	glm::vec3 diffuseLight;
	glm::vec3 specularLight;
	public:
		Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	
};

#endif /* Light_h */
