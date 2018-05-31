//
//  camera.cpp
//  COMP371-A1
//
//  Created by Mathew Plouffe on 2018-05-15.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "camera.h"

//the camera class provides the ability to move the camera around the model, as well as storing useful data on what the camera is looking at

Camera::Camera(void)
{
	//intentionally empty
}
Camera::Camera(glm::vec3 eye, glm::vec3 dir, glm::vec3 up)
{
	position = eye;
	direction = dir;
	orientation = up;

}
Camera::Camera(glm::vec3 eye, glm::vec3 dir)
{
	position = eye;
	direction = dir;
	orientation = glm::vec3(0.0f,1.0,0.0f);
}
glm::mat4 Camera::init(void)
{
	//returns a view matrix for the camera
	return glm::lookAt(position, direction, orientation);
}
glm::mat4 Camera::zoom(glm::vec3 scale)
{
	//TODO implement a zoom functionality
	return glm::mat4();
}
glm::mat4 Camera::strafe(glm::vec3 direction)
{
	//generate a translation matrix for the provided direction
	return glm::translate(glm::mat4(), direction);
}
glm::mat4 Camera::rotate(glm::vec3 orientation)
{
	//generate a rotation matrix for the provided direction of rotation
	return glm::rotate(glm::mat4(), glm::radians(1.0f), orientation);
}
glm::mat4 Camera::updateModel(bool *keys)
{
	//this function gathers all the model transforms into one matrix
	//based upon the states provided by the window
	//returns the transformation matrix for use by the renderer
	
	glm::mat4 transform;
	if (keys[0] == true)
	{
		//up
		transform = strafe( glm::vec3(0.0f,-1.0f,0.0f)) * transform;
	}
	if (keys[1] == true)
	{
		//down
		transform = strafe( glm::vec3(0.0f,1.0f,0.0f) ) * transform;
	}
	if (keys[2] == true)
	{
		//left
		transform = strafe(glm::vec3(-1.0f,0.0f,0.0f)) * transform;
	}
	if (keys[3] == true)
	{
		//right
		transform = strafe(glm::vec3(1.0f,0.0f,0.0f)) * transform;
	}
	if (keys[4] == true)
	{
		//zoom in
		transform = strafe(glm::vec3(0.0f,0.0f,1.0f)) * transform;
	}
	if (keys[5] == true)
	{
		//zoom out
		transform = strafe(glm::vec3(0.0f,0.0f,-1.0f)) * transform;
	}
	if (keys[6] == true)
	{
		//rotate down-up
		transform = rotate(glm::vec3(1.0f,0.0f,0.0f)) * transform;
	}
	if (keys[7] == true)
	{
		//rotate right-left
		transform = rotate(glm::vec3(0.0f,-1.0f,0.0f)) * transform;
	}
	if (keys[8] == true)
	{
		//rotate up-down
		transform = rotate(glm::vec3(-1.0f,0.0f,0.0f)) * transform;
	}
	if (keys[9] == true)
	{
		//rotate left-right
		transform = rotate(glm::vec3(0.0f,1.0f,0.0f)) * transform;
	}	return transform;
}
