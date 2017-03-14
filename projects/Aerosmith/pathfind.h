#ifndef PATHFIND_H
#define PATHFIND_H
#include "kf/kf_vector2.h"
#include <vector>

typedef kf::Vector2T<int> NodePos;

class Node
{
public:
	enum NodeState
	{
		StateOpen,
		StateClosed,
		StateNone
	};
	int c;
	int f;
	int g;
	int h;
	bool wall;
	NodePos parent;
	NodeState state;

	Node();
	void clear();
};

class Map
{
public:
	Map();
	~Map();

	std::vector<Node> m_nodes;
	int m_width;
	int m_height;
	
	void clear();
	void init(int width, int height);
	Node &getNode(const NodePos &np);
	Node &getNode(const kf::Vector2 &np);
	Node &operator[](const NodePos &np);
	Node &operator[](const kf::Vector2 &np);
};


#endif