#include "pathfind.h"
#include "kf/kf_log.h"

Node::Node()
{
	clear();
}

void Node::clear()
{
	c = 1;
	f = 0;
	g = 0;
	h = 0;
	parent.set(-1, -1);
	state = NodeState::StateNone;
}

Map::Map()
{
	m_width = 0;
	m_height = 0;
}

Map::~Map()
{

}

void Map::clear()
{
	for (int y = 0;y < m_height; ++y)
	{
		for (int x = 0;x < m_width; ++x)
		{
			getNode(NodePos(x, y)).clear();
		}
	}
}

void Map::init(int width, int height)
{
	m_width = width;
	m_height = height;
	m_nodes.resize(width*height);
	clear();
}
	
Node &Map::getNode(const NodePos &np)
{
	if (np.x < 0 || np.y < 0 || np.x >= m_width || np.y >= m_height)
	{
		kf_log("Invalid node coordinate: " << np.x << "," << np.y);
	}
	return m_nodes[np.x + np.y*m_width];
}

Node &Map::getNode(const kf::Vector2 &np)
{
	return getNode(NodePos(np.x, np.y));
}

Node &Map::operator[](const NodePos &np)
{
	return getNode(np);
}

Node &Map::operator[](const kf::Vector2 &np)
{
	return getNode(np);
}

