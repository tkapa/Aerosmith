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

	m_moveTarget.set(m_rand() % (m_initialData.mapData.width - 2) + 1.5, m_rand() % (m_initialData.mapData.width - 2) + 1.5);
}

void Aerosmith::update(const BotInput &input, BotOutput27 &output)
{
	/*output.moveDirection = m_moveTarget - input.position;
	if (output.moveDirection.length() < 2)
	{
		m_moveTarget.set(m_rand() % (m_initialData.mapData.width - 2) + 1.5, m_rand() % (m_initialData.mapData.width - 2) + 1.5);
	}*/
	if (initPass || destinationReached) {
		m_map.clear();
		m_openList.push_back(findValidNode());

		initPass = false;
		destinationReached = false;
	}

	while (m_openList.size() >= 1 && !pathFound) {

		NodePos m_currentNode = m_openList.front();

		for (auto it = m_openList.begin();it <= m_openList.end();++it) {		//Find the smallest F on the list
			if (m_map.getNode(*it).f <= m_map.getNode(m_smallestFNode).f) {
				m_smallestFNode = *it;
			}
		}

		for (int oy = -1;oy < 2;++oy) {			//Offset Y
			for (int ox = -1; ox < 2;++ox) {	//Offset X
				NodePos adj = NodePos(m_currentNode.x + ox, m_currentNode.y + oy);	//Get the pos of the node
				int m_newG = m_g + m_map.getNode(adj).c;							//alter G

				if (ox == 0 && oy == 0)			//Skip this node
					continue;
				if (ox != 0 && oy != 0)			//Skip diagonal nodes
					continue;
				if (m_map.getNode(adj).wall)	//Skip walls
					continue;

				if (m_map.getNode(adj).state == Node::NodeState::StateClosed)
					continue;
				else if (m_map.getNode(adj).state == Node::NodeState::StateOpen && m_newG < m_map.getNode(adj).g) {
					m_map.getNode(adj).g = m_newG;
					m_map.getNode(adj).h = abs(adj.x - input.position.x) + abs(adj.y - input.position.y);
					m_map.getNode(adj).parent = m_currentNode;
					m_map.getNode(adj).f = m_map.getNode(adj).g + m_map.getNode(adj).h;
				}
				else if (m_map.getNode(adj).state == Node::NodeState::StateNone) {
					m_map.getNode(adj).g = m_newG;
					m_map.getNode(adj).h = abs(adj.x - input.position.x) + abs(adj.y - input.position.y);
					m_map.getNode(adj).parent = m_currentNode;
					m_map.getNode(adj).f = m_map.getNode(adj).g + m_map.getNode(adj).h;
					m_map.getNode(adj).state = Node::NodeState::StateOpen;
					m_openList.push_back(adj);
					++eraseIterator;
				}
			}
		}
		
		m_map.getNode(m_currentNode).state == Node::NodeState::StateClosed;
		m_currentNode = m_smallestFNode;
		//m_openList.erase(m_openList.begin() + eraseIterator);
	}

	if (pathFound) {
		m_moveTarget = kf::Vector2(m_map.getNode(m_openList[pathIter]).parent.x, m_map.getNode(m_openList[pathIter]).parent.y);

		output.moveDirection = m_moveTarget - input.position;

		if (output.moveDirection.length() <= 0.5f) {
			++pathIter;
		}


		// How to render text on the screen.
		output.text.clear();
		char buf[100];
		sprintf(buf, "%d", input.health);
		output.text.push_back(TextMsg(buf, input.position - kf::Vector2(0.0f, 1.0f), 0.0f, 0.7f, 1.0f, 80));
	}
	output.motor = 1.0f;
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
