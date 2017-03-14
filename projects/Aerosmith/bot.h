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

	kf::Xor128 m_rand;
	BotInitialData m_initialData;
	kf::Vector2 dir;
	kf::Vector2 m_moveTarget;

	Map m_map;

	//NodePos m_currentNode;				//Initialised as the beginning of the path (destination)
	NodePos m_smallestFNode;			//the node with the smallest F
	std::vector<NodePos> m_openList;	//list for the open nodes
	std::vector<NodePos> m_pathList;	//List of nodes to follow
	bool pathFound = false;				//has a path been found?
	bool initPass = true;				//is this the first path made?
	bool destinationReached = false;	//Have I reached my destination
	int m_g = 0;						//Total distance travelled
	int eraseIterator = 0;
	int pathIter = 0;
};


#endif