#ifndef BOT_H
#define BOT_H
#include "bot_interface.h"
#include "kf/kf_random.h"
#include "pathfind.h"

#ifdef BOT_EXPORTS
#define BOT_API __declspec(dllexport)
#else
#define BOT_API __declspec(dllimport)
#endif

class Aerosmith:public BotInterface27
{
public:
	Aerosmith();
	virtual ~Aerosmith();
	virtual void init(const BotInitialData &initialData, BotAttributes &attrib);
	virtual void update(const BotInput &input, BotOutput27 &output);
	virtual void result(bool won);
	virtual void bulletResult(bool hit);
	NodePos findValidNode();
	bool findPath(NodePos startNode, NodePos endNode, BotOutput27 &output);
	void alterMotor(BotInput &input, BotOutput27 &output);

	kf::Xor128 m_rand;
	BotInitialData m_initialData;
	kf::Vector2 dir;
	kf::Vector2 m_moveTarget;

	Map m_map;

	//Pathfinding Variables
	NodePos m_smallestFNode;			//the node with the smallest F
	NodePos dest;						//Destination node
	std::vector<NodePos> m_openList;	//list for the open nodes
	bool pathFound = false;				//has a path been found?
	bool destinationReached = false;	//Have I reached my destination
	int m_g = 0;						//Total distance travelled

	//Combat Variables
	kf::Vector2 m_enemyInitPos;			//Initial enemy position
	kf::Vector2 m_enemyCurrPos;			//Current enemy pos
	kf::Vector2 m_estEnemyPos;			//Where I think the enemy will be
	int m_updateCount = 0;				//counts the number of updates
	int m_enemyUpdateCount = 0;			//notes the last update I'd seen the enemy
	int m_burstCount = 0;				//no of times I want to burst fire
	float m_lookAngle;					//Angle I wanna look at
	bool m_enemySeen = false;			//Have I seen the enemy?
};


#endif