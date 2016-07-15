#ifndef _ASTAR_H_
#define _ASTAR_H_

#include <vector>
#include "Math\Vector3.h"
#include <string>

class CXMLTreeNode;

typedef std::vector< Vect3f > VPoints3;

class CAStar {

public:
	
	CAStar();
	~CAStar();
		
private:
	std::string m_Filename;
	struct TNode;
	typedef std::pair< TNode*, float > PNodeAndDistance;
	typedef std::vector< PNodeAndDistance > VNodesAndDistances;
	TNode* GetNode(unsigned int ID) const;

	struct TNode 
	{
		Vect3f m_Position;
		VNodesAndDistances m_Neighbours;
		TNode *m_Parent;
		float m_G;
		float m_H;
		float m_F;
		bool m_InOpenList;
		bool m_Closed;
		TNode();
		TNode(CXMLTreeNode &TreeNode);
	};

	typedef std::vector< TNode* > VNodes;
	VNodes m_map;

	struct TCompareNodes 
	{
		bool operator ()( const TNode *nodeA, const TNode *nodeB );
	};
	VNodes m_openList;

	void AddToOpenList( TNode *node );
	TNode *PopFirstFromOpenList();

	void LoadMap();
	void DestroyMap();

	VNodes PSearchPath( TNode* nodeA, TNode *nodeB );
	bool VisitNextNode( TNode *destinationNode );

	TNode *GetNearestNode( const Vect3f &point );

public:
	//void Render( LPDIRECT3DDEVICE9 device );
	bool LoadNodes(const std::string &Filename);
	TNode* AddNode(Vect3f Position);
	void AddNeighbours(TNode* Node, VNodes Neighbours);
	std::vector<Vect3f> SearchPath(const Vect3f &pointA, const Vect3f &pointB);

};

#endif // INC_A_STAR_H_