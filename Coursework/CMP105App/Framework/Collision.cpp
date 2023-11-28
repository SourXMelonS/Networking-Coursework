#include "Collision.h"

// Check AABB for collision. Returns true if collision occurs.
bool Collision::checkBoundingBox(GameObject* s1, GameObject* s2)
{
	if (s1->getCollisionBox().left + s1->getCollisionBox().width < s2->getCollisionBox().left)
		return false;
	if (s1->getCollisionBox().left > s2->getCollisionBox().left + s2->getCollisionBox().width)
		return false;
	if (s1->getCollisionBox().top + s1->getCollisionBox().height < s2->getCollisionBox().top)
		return false;
	if (s1->getCollisionBox().top > s2->getCollisionBox().top + s2->getCollisionBox().height)
		return false;

	return true;
}

// Check for bounding box collision with a point/vector2 rather than two boxes.
bool Collision::checkBoundingBox(GameObject* s1, sf::Vector2i s2)
{
	if (s1->getCollisionBox().left + s1->getCollisionBox().width < s2.x)
		return false;
	if (s1->getCollisionBox().left > s2.x)
		return false;
	if (s1->getCollisionBox().top + s1->getCollisionBox().height < s2.y)
		return false;
	if (s1->getCollisionBox().top > s2.y)
		return false;

	return true;
}

// Check bounding circle collision. Returns true if collision occurs.
bool Collision::checkBoundingCircle(GameObject* s1, GameObject* s2)
{
	// Get radius and centre of sprites.
	float radius1 = s1->getSize().x / 2;
	float radius2 = s2->getSize().x / 2;
	float xpos1 = s1->getPosition().x + radius1;
	float xpos2 = s2->getPosition().x + radius2;
	float ypos1 = s1->getPosition().y + radius1;
	float ypos2 = s2->getPosition().y + radius2;

	if (pow(xpos2 - xpos1, 2) + pow(ypos2 - ypos1, 2) < pow(radius1 + radius2, 2))
	{
		return true;
	}
	return false;
}

#define SIDES 4

//The following code is from Kieran Thorpe
bool Collision::TouchTopof(GameObject* s1, GameObject* s2)
{
	float s1Bottom = s1->getCollisionBox().top + s1->getCollisionBox().height;
	float s1Right = s1->getCollisionBox().left + s1->getCollisionBox().width;

	float s2Bottom = s2->getCollisionBox().top + s2->getCollisionBox().height;
	float s2Right = s2->getCollisionBox().left + s2->getCollisionBox().width;

	return
		(
			s1Bottom > s2->getCollisionBox().top && // checks the the bottom of the first object against the top of the second object
			s1Bottom < s2->getCollisionBox().top + (s2->getCollisionBox().height / 2) && // checks the bottom of the first object to the halfway point of the second object
			s1Right> s2->getCollisionBox().left + (s2->getCollisionBox().width / SIDES) && // checks the right hand side of the first object against the left hand side with a slight deadzone
			s1->getCollisionBox().left < s2Right - (s2->getCollisionBox().width / SIDES) // checks the left hand side of the first object against the right hand side of the second with a slight deadzone
			); //returns true if all of those conditions are met
}

bool Collision::TouchBottomof(GameObject* s1, GameObject* s2)
{
	float s1Bottom = s1->getCollisionBox().top + s1->getCollisionBox().height;
	float s1Right = s1->getCollisionBox().left + s1->getCollisionBox().width;

	float s2Bottom = s2->getCollisionBox().top + s2->getCollisionBox().height;
	float s2Right = s2->getCollisionBox().left + s2->getCollisionBox().width;

	return
		(
			s1->getCollisionBox().top < (s2Bottom) && // checks the the top of the first object against the bottom of the second object 
			s1->getCollisionBox().top >(s2Bottom) - (s2->getCollisionBox().height / 2) && // checks the top of the first object to the halfway point of the second object
			s1Right > s2->getCollisionBox().left + (s2->getCollisionBox().width / SIDES) &&// checks the right hand side of the first object against the left hand side with a slight deadzone
			s1->getCollisionBox().left < (s2Right)-(s2->getCollisionBox().width / SIDES)// checks the left hand side of the first object against the right hand side of the second with a slight deadzone
			); //returns true if all of those conditions are met
}

bool Collision::TouchLeftof(GameObject* s1, GameObject* s2)
{
	float s1Bottom = s1->getCollisionBox().top + s1->getCollisionBox().height;
	float s1Right = s1->getCollisionBox().left + s1->getCollisionBox().width;

	float s2Bottom = s2->getCollisionBox().top + s2->getCollisionBox().height;
	float s2Right = s2->getCollisionBox().left + s2->getCollisionBox().width;
	return
		(
			s1Right <= s2Right && // checks that the right hand side of the first object is less than the second object right hand side
			s1Right >= s2->getCollisionBox().left - SIDES && // checks that the right hand side of the is greater than the left hand side minus the sides
			s1->getCollisionBox().top <= s2Bottom - (s2->getCollisionBox().width / SIDES) && // checks the top of the first object against the bottom of the second minus the width divided by the deadzone
			s1Bottom >= (s2->getCollisionBox().top) + (s2->getCollisionBox().width / SIDES) // checks the bottom of the first object against the top of the second object minus the width divided by the deadzone
			); //returns true if all of those conditions are met
}

bool Collision::TouchRightof(GameObject* s1, GameObject* s2)
{
	float s1Bottom = s1->getCollisionBox().top + s1->getCollisionBox().height;
	float s1Right = s1->getCollisionBox().left + s1->getCollisionBox().width;

	float s2Bottom = s2->getCollisionBox().top + s2->getCollisionBox().height;
	float s2Right = s2->getCollisionBox().left + s2->getCollisionBox().width;

	return(
		s1->getCollisionBox().left >= s2->getCollisionBox().left &&  // checks that the left hand side of the first object is greater than the second object left hand side
		s1->getCollisionBox().left <= s2Right + SIDES && // checks that the left hand side of the is less than the right hand side plus the sides
		s1->getCollisionBox().top <= s2Bottom - (s2->getCollisionBox().width / SIDES) && // checks the top of the first object against the bottom of the second minus the width divided by the deadzone
		s1Bottom >= s2->getCollisionBox().top + (s2->getCollisionBox().width / SIDES) // checks the bottom of the first object against the top of the second object minus the width divided by the deadzone
		);
}

