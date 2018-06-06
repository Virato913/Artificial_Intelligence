#pragma once

#include "Vector2.h"

#include <vector>

class Path {
public:
	std::vector<Vector2> m_nodes;

	void AddNode(Vector2 newNode)
	{
		m_nodes.push_back(newNode);
	}

	std::vector<Vector2> GetNodes()
	{
		return m_nodes;
	}
};