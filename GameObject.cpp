#include "StdAfx.h"
#include "GameObject.h"


GameObject::GameObject(void)
	:rotation(0)
{
}


GameObject::~GameObject(void)
{
}


void GameObject::Update(const float time_step)
{
	position += velocity * time_step;
}


Vector3& GameObject::GetPosition(void )
{
	return position;
}
void GameObject::SetPosition(Vector3& pos)
{
	position = pos;
}


Vector3& GameObject::GetVelocity(void )
{
	return velocity;
}
void GameObject::SetVelocity(Vector3& vel)
{
	velocity = vel;
}


float GameObject::GetRotation(void )
{
	return rotation;
}
void GameObject::SetRotation(float rot)
{
	rotation = rot;
}
