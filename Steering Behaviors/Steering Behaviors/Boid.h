#pragma once

#include "Vector2.h"
#include "GameObject.h"
#include "Path.h"

#include <random>

class Boid : public GameObject
{
public:
	Vector2 m_direction;
	float m_velocity;
	float m_neighbourRadious;
	Path m_path;
	int currentNode;
	int pathDir;

	Vector2 Rotate(Vector2 vector, float angle)
	{
		Vector2 pivot;
		pivot.y = sin(RAD(angle))*vector.x + cos(RAD(angle))*vector.y;
		pivot.x = cos(RAD(angle))*vector.x - sin(RAD(angle))*vector.y;
		return pivot;
	}

	double Dot(Vector2 a, Vector2 b)
	{
		return((a.x * b.x) + (a.y*b.y));
	}

	double Projection(Vector2 a, Vector2 b)
	{
		double dot = Dot(a, b);
		return dot / (a.Lenght()*b.Lenght());
	}

	Vector2 Seek(GameObject obj, double m)
	{
		Vector2 dir = obj.m_position - this->m_position;
		dir.Normalize();
		return dir*m;
	}

	Vector2 Seek(Vector2 obj, double m)
	{
		Vector2 dir = obj - this->m_position;
		dir.Normalize();
		return dir*m;
	}

	Vector2 Flee(GameObject obj, double m, double r)
	{
		Vector2 dir = this->m_position - obj.m_position;
		if (dir.Lenght() < r)
		{
			dir.Normalize();
		}
		else
		{
			dir = dir * 0;
		}
		return dir*m;
	}

	Vector2 Flee(Vector2 obj, double m, double r)
	{
		Vector2 dir = this->m_position - obj;
		if (dir.Lenght() < r)
		{
			dir.Normalize();
		}
		else
		{
			dir = dir * 0;
		}
		return dir*m;
	}

	Vector2 Arrive(GameObject obj, double m, double r)
	{
		Vector2 dir = this->m_position - obj.m_position;
		double dist = dir.Lenght();
		if (dist < r)
		{
			dir.Normalize();
			dir = dir * (dist / r);
		}
		else
		{
			dir.Normalize();
		}
		return dir*m;
	}

	Vector2 Wander(double m, double dist, double r, double angle)
	{
		Vector2 dir = this->m_direction;
		Vector2 circleCenter = dir.Normalized()*dist;
		Vector2 displacement = Vector2(r, 0);
		angle *= 0.5f;
		angle = RAD(angle);
		double circleAngle = circleCenter.GetAngle();
		double angleLow = circleAngle + angle;
		double angleHi = circleAngle - angle;
		displacement = Rotate(displacement, angleLow + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / angleHi - angleLow)));
		displacement += circleCenter;
		return Seek(displacement, m);
	}

	Vector2 Pursue(Vector2 pos, Vector2 obj, Vector2 objVel, double m)
	{
		Vector2 dir = obj - pos;
		double dist = dir.Lenght();
		double vel = dist / objVel.Lenght();
		Vector2 futurePos = obj + objVel * vel;
		return Seek(futurePos, m);
	}

	Vector2 Evade(Vector2 pos, Vector2 obj, Vector2 objVel, double m, double r)
	{
		Vector2 dir = pos - obj;
		double dist = dir.Lenght();
		double vel = dist / objVel.Lenght();
		Vector2 futurePos = obj + objVel * vel;
		return Flee(futurePos, m, r);
	}

	Vector2 Flocking(std::vector<Boid> AgentArray, double m)
	{
		Vector2 pivot = Alignment(AgentArray) + Cohesion(AgentArray) + Separation(AgentArray);
		pivot.Normalize();
		return pivot*m;
	}

	Vector2 Alignment(std::vector<Boid> AgentArray)
	{
		Vector2 pivot;
		int neighbourCount;
		for each (Boid agent in AgentArray)
		{
			if (agent.m_position != this->m_position)
			{
				if ((agent.m_position - this->m_position).Lenght() < m_neighbourRadious)
				{
					pivot += agent.m_direction;
					neighbourCount++;
				}
			}
		}
		if (neighbourCount == 0)
		{
			return pivot;
		}
		else
		{
			pivot /= neighbourCount;
			pivot.Normalize();
			return pivot;
		}
	}

	Vector2 Cohesion(std::vector<Boid> AgentArray)
	{
		Vector2 pivot;
		int neighbourCount;
		for each (Boid agent in AgentArray)
		{
			if (agent.m_position != this->m_position)
			{
				if ((agent.m_position - this->m_position).Lenght() < m_neighbourRadious)
				{
					pivot += agent.m_position;
					neighbourCount++;
				}
			}
		}
		if (neighbourCount == 0)
		{
			return pivot;
		}
		else
		{
			pivot /= neighbourCount;
			pivot = (pivot - this->m_position);
			pivot.Normalize();
			return pivot;
		}
	}

	Vector2 Separation(std::vector<Boid> AgentArray)
	{
		Vector2 pivot;
		int neighbourCount;
		for each (Boid agent in AgentArray)
		{
			if (agent.m_position != this->m_position)
			{
				if ((agent.m_position - this->m_position).Lenght() < m_neighbourRadious)
				{
					pivot += (agent.m_position - this->m_position);
					neighbourCount++;
				}
			}
		}
		if (neighbourCount == 0)
		{
			return pivot;
		}
		else
		{
			pivot /= neighbourCount;
			pivot *= -1;
			pivot.Normalize();
			return pivot;
		}
	}

	Vector2 ObstacleAvoidance(std::vector<GameObject> obstacleArray, float lookAhead)
	{
		Vector2 pivot, topLeft, topRight, bottomLeft, bottomRight, Avoid;
		pivot = m_direction;
		pivot.Normalize();
		pivot = Rotate(pivot, 90);
		topLeft = m_direction;
		topLeft.Normalize();
		topLeft *= lookAhead;
		topLeft += pivot;
		topRight = topLeft - (pivot * 2);
		bottomLeft = m_direction.Normalized()*-1 + pivot;
		bottomRight = bottomLeft - (pivot * 2);
		Vector2 BoxA, BoxB, BoxC, BoxD;
		BoxA = topRight - topLeft;
		BoxB = topLeft - bottomLeft;
		BoxC = bottomLeft - bottomRight;
		BoxD = bottomRight - topRight;
		for each (GameObject obstacle in obstacleArray)
		{
			if ((this->m_position - obstacle.m_position).Lenght() < (lookAhead * 2))
			{
				Vector2 a, b, c, d;
				a = obstacle.m_position - topLeft;
				b = obstacle.m_position - bottomLeft;
				c = obstacle.m_position - bottomRight;
				d = obstacle.m_position - topRight;
				Avoid += CheckBoundaries(obstacle, BoxA, topLeft, a);
				Avoid += CheckBoundaries(obstacle, BoxB, bottomLeft, b);
				Avoid += CheckBoundaries(obstacle, BoxC, bottomRight, c);
				Avoid += CheckBoundaries(obstacle, BoxD, topRight, d);
			}
		}
		//Infinity puede ser cualquier otra cosa, I guess.
		Avoid *= INFINITY;
		return Avoid;
	}

	Vector2 CheckBoundaries(GameObject obstacle, Vector2 BoxSide, Vector2 BoxCorner, Vector2 Vector)
	{
		Vector2 pivot;
		double proj = Projection(Vector, BoxSide);
		if (proj > 0 && proj < 1)
		{
			Vector2 ProjVector = BoxSide*proj;
			Vector2 Avoid = ProjVector - Vector;
			if (Avoid.Lenght() <= obstacle.radius)
			{
				Avoid.Normalize();
				pivot = Avoid;
			}
		}
		return pivot;
	}

	Vector2 PathFollowing(double distance, double m)
	{
		Vector2 follow;
		if (m_path.GetNodes().size() > 0)
		{
			std::vector<Vector2> path = m_path.GetNodes();
			follow = path[currentNode];
			if ((follow - this->m_position).Lenght() < distance)
			{
				currentNode++;
				if (currentNode >= path.size())
				{
					currentNode--;
				}
			}
		}
		return follow != Vector2(0, 0) ? Seek(follow, m) : Vector2(0, 0);
	}

	Vector2 Patrol(double distance, double m)
	{
		Vector2 follow;
		if (m_path.GetNodes().size() > 0)
		{
			std::vector<Vector2> path = m_path.GetNodes();
			follow = path[currentNode];
			if ((follow - this->m_position).Lenght() < distance)
			{
				currentNode+=pathDir;
				if (currentNode >= path.size() || currentNode<0)
				{
					pathDir *= -1;
					currentNode+=pathDir;
				}
			}
		}
		return follow != Vector2(0, 0) ? Seek(follow, m) : Vector2(0, 0);
	}
};