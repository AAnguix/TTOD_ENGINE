#ifndef _ASTAR_H
#define _ASTAR_H

#include <vector>
#include "Math\Vector3.h"

class CAStar
{

private:

	class CNode
	{
		private:
			std::string m_Name;
			Vect3f m_Position;
			float m_Level;
			float m_Priority;
			bool m_Closed;
			CNode* m_Parent;


		public:
			std::vector<CNode*> m_Neighbours;

			Vect3f GetPosition() const {return m_Position;};

			float GetLevel() const {return m_Level;};
			void SetLevel(float Level){m_Level=Level;};

			float GetPriority() const {return m_Priority;};
			void SetPriority(float Priority){m_Priority=Priority;};

			bool GetClosed() const {return m_Closed;};
			void SetClosed(bool Closed){m_Closed=Closed;};

			CNode* GetParent() const{return m_Parent;};
			void SetParent(CNode* Parent){m_Parent=Parent;};

			void AddNeighbour(CNode* Node){m_Neighbours.push_back(Node); Node->m_Neighbours.push_back(this);}; //Node->SetParent(this); Si lo tienen varios, que padre tiene?

			CNode(const std::string Name, Vect3f Position, float Level, float Priority)
			{
				m_Name=Name;
				m_Position=Position;
				m_Level=Level;
				m_Priority=Priority;
				m_Closed=false;
				m_Parent=NULL;
			}
			virtual ~CNode(){};
			
	};

	/*bool operator<(const CNode & a, const CNode & b)
	{
		return a.GetPriority() > b.GetPriority();
	}*/

private:  
		std::vector<CNode*> m_Nodes;
		std::vector<CNode*> m_OpenNodes;
		std::vector<CNode*> m_ClosedNodes;
		bool m_Finished;
		CNode* m_CurrentNode;
		Vect3f m_StartPosition;
		Vect3f m_EndPosition;
		std::vector<Vect3f> m_Result;

		void CloseNode(size_t i);
		size_t ExtractBestFNode();
		void VisitNextNode(CNode* Node);
		void ProcesarVecinos(CNode* Node);
		void CalculateG(CNode* Node);
		
		bool IsFinalNode(CNode* Node);
		bool AllNodesVisited();

		const float Heuristic(CNode* Node, const Vect3f Destiny) const;

public:  
	CAStar(Vect3f StartPosition, Vect3f EndPosition);
	virtual ~CAStar();
	std::vector<Vect3f> Execute();
	void Initialize();
	
};

#endif