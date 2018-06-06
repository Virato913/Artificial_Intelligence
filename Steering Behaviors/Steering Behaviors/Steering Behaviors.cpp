// Steering Behaviors.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Boid.h";

int main()
{
	Boid test;
	std::vector<GameObject> testObstacle;
	GameObject newObstacle;
	newObstacle.m_position = Vector2(0, 3);
	newObstacle.radius = 1;
	testObstacle.push_back(newObstacle);
	test.m_direction = Vector2(0, 1);
	test.m_position = Vector2(0, 0);
	test.radius = 1.0f;
	Vector2 prueba(2, 2);
	//Vector2 objetivo(4, 4);
	//Vector2 force = test.Flee(prueba, objetivo, 5, 10);
	//Vector2 force = test.Wander(prueba, Vector2(1, 0), 5, 2, 1, 45);
	Vector2 prueba2 = prueba.Normalized();
	test.ObstacleAvoidance(testObstacle, 2.5f);
    return 0;
}