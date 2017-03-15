#include "bot.h"
#include "time.h"

extern "C"
{
	BotInterface27 BOT_API *CreateBot27()
	{
		return new Aerosmith();
	}
}

Aerosmith::Aerosmith()
{
	m_rand(time(0)+(int)this);
}

Aerosmith::~Aerosmith()
{
}

void Aerosmith::init(const BotInitialData &initialData, BotAttributes &attrib)
{
	m_initialData = initialData;
	attrib.health=1.0;
	attrib.motor=1.0;
	attrib.weaponSpeed=1.0;
	attrib.weaponStrength=1.0;
	dir.set(1, 0);

	m_map.init(initialData.mapData.width, initialData.mapData.height);
	for (int y = 0;y<m_map.m_height;++y)
	{
		for (int x = 0;x<m_map.m_width;++x)
		{
			m_map.getNode(NodePos(x, y)).wall = m_initialData.mapData.data[x + y*m_map.m_width].wall;
		}
	}

	initPass = true;
	m_map.clear();
	dest = findValidNode();
}

void Aerosmith::update(const BotInput &input, BotOutput27 &output)
{	
	output.motor = 1.0f;
	findPath(dest, NodePos(std::abs(input.position.x), std::abs(input.position.y)), output);
	kf::Vector2 moveToPos = kf::convertVector2<kf::Vector2>(m_map.getNode(kf::convertVector2<NodePos>(input.position)).parent);
	moveToPos.x += 0.5f;
	moveToPos.y += 0.5f;
	kf::Vector2 distToDest = kf::Vector2(dest.x, dest.y) - input.position;

	if (distToDest.length() <= 2.0f) {
		m_map.clear();
		dest = findValidNode();
	}	
	output.moveDirection = moveToPos - input.position;
	output.lookDirection = output.moveDirection;
	output.action = BotOutput27::shoot;
}

void Aerosmith::result(bool won)
{
}

void Aerosmith::bulletResult(bool hit)
{

}

NodePos Aerosmith::findValidNode() {
	NodePos newPos = NodePos(m_rand() % (m_initialData.mapData.width - 2) + 1.5, m_rand() % (m_initialData.mapData.width - 2) + 1.5);
	Node thisNode = m_map.getNode(newPos);	

	while (thisNode.wall) {
		newPos = NodePos(m_rand() % (m_initialData.mapData.width - 2) + 1.5, m_rand() % (m_initialData.mapData.width - 2) + 1.5);
		thisNode = m_map.getNode(newPos);
	}

	return newPos;
}

bool Aerosmith::findPath(NodePos destinationNode, NodePos myPosition, BotOutput27 &output) {
	bool pathFound = false;
	m_openList.clear();
	m_openList.push_back(destinationNode);

	while (!m_openList.empty()) {

		auto currIt = m_openList.begin();
		for (auto it = m_openList.begin();it != m_openList.end();++it) {		//Find the smallest F on the list
			if (m_map.getNode(*it).f <= m_map.getNode(*currIt).f) {
				currIt = it;
			}
		}

		NodePos currentNodePos = *currIt;
		Node &m_currentNode = m_map.getNode(currentNodePos);

		m_currentNode.state = Node::StateClosed;
		m_openList.erase(currIt);

		for (int oy = -1;oy < 2;++oy) {			//Offset Y
			for (int ox = -1; ox < 2;++ox) {	//Offset X
				NodePos &adj = NodePos(currentNodePos.x + ox, currentNodePos.y + oy);	//Get the pos of the node
				int m_newG = m_g + m_map.getNode(adj).c;							//alter G

				if (ox == 0 && oy == 0)			//Skip this node
					continue;	
				if (m_map.getNode(adj).wall)	//Skip walls
				{
					output.motor = 0.5f;
					continue;
				}
				if (ox == -1 && oy == -1) {
					if (m_map.getNode(NodePos(adj.x + 1, adj.y)).wall || m_map.getNode(NodePos(adj.x, adj.y + 1)).wall) continue;
				}
				if (ox == 1 && oy == -1) {
					if (m_map.getNode(NodePos(adj.x - 1, adj.y)).wall || m_map.getNode(NodePos(adj.x, adj.y + 1)).wall) continue;
				}
				if (ox == 1 && oy == 1) {
					if (m_map.getNode(NodePos(adj.x -1, adj.y)).wall || m_map.getNode(NodePos(adj.x, adj.y -1)).wall) continue;
				}
				if (ox == -1 && oy == 1) {
					if (m_map.getNode(NodePos(adj.x +1 , adj.y)).wall || m_map.getNode(NodePos(adj.x, adj.y-1 )).wall) continue;
				}

				if (m_map.getNode(adj).state == Node::StateClosed)		//Skip closed nodes
					continue;
				else if (m_map.getNode(adj).state == Node::StateOpen && m_newG < m_map.getNode(adj).g) {
					m_map.getNode(adj).g = m_newG;
					m_map.getNode(adj).h = abs(adj.x - destinationNode.x) + abs(adj.y - destinationNode.y);
					m_map.getNode(adj).parent = currentNodePos;
					m_map.getNode(adj).f = m_map.getNode(adj).g + m_map.getNode(adj).h;
				}
				else if (m_map.getNode(adj).state == Node::StateNone) {
					m_map.getNode(adj).g = m_newG;
					m_map.getNode(adj).h = abs(adj.x - destinationNode.x) + abs(adj.y - destinationNode.y);
					m_map.getNode(adj).parent = currentNodePos;
					m_map.getNode(adj).f = m_map.getNode(adj).g + m_map.getNode(adj).h;
					m_map.getNode(adj).state = Node::StateOpen;
					m_openList.push_back(adj);

				}

				if (adj.x == myPosition.x && adj.y == myPosition.y)
					pathFound = true;
			}
		}
	}

	return pathFound;
}
