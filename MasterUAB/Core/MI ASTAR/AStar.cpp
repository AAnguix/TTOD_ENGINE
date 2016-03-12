#include "AStar.h"
#include <algorithm>
#include <math.h>
#include "Utils\Utils.h"

CAStar::CAStar(Vect3f StartPosition, Vect3f EndPosition)
{
	m_StartPosition=StartPosition;
	m_EndPosition=EndPosition;
	m_Finished=false;
}

CAStar::~CAStar()
{
	for(size_t i=0;i<m_OpenNodes.size();++i)
	{
		CHECKED_DELETE(m_OpenNodes[i]);
	}

	for(size_t i=0;i<m_ClosedNodes.size();++i)
	{
		CHECKED_DELETE(m_ClosedNodes[i]);
	}

	for(size_t i=0;i<m_Nodes.size();++i)
	{
		CHECKED_DELETE(m_Nodes[i]);
	}

	m_ClosedNodes.clear();
	m_OpenNodes.clear();
	m_Nodes.clear();
}

void CAStar::Initialize()
{
	CNode* A = new CNode("A",Vect3f(1.0f,0.0f,3.0f),1.0f,0.0f);
	CNode* B = new CNode("B",Vect3f(2.0f,0.0f,3.0f),1.0f,0.0f);
	CNode* C = new CNode("C",Vect3f(1.5f,1.3f,3.0f),1.0f,0.0f);
	CNode* D = new CNode("D",Vect3f(6.5f,1.3f,3.0f),1.0f,0.0f);
	CNode* E = new CNode("E",Vect3f(8.5f,1.3f,3.0f),1.0f,0.0f);

	m_Nodes.push_back(A);
	m_Nodes.push_back(B);
	m_Nodes.push_back(C);
	m_Nodes.push_back(D);
	m_Nodes.push_back(E);

	A->AddNeighbour(B);
	A->AddNeighbour(C);
	C->AddNeighbour(D);
	C->AddNeighbour(E);
}

std::vector<Vect3f> CAStar::Execute()
{
	//CNode* n_FirstNode = new CNode("Init",Vect3f(0.0f,0.0f,0.0f),0.0f,0.0f);
	m_CurrentNode = m_Nodes[0];
	m_CurrentNode->SetPriority(m_CurrentNode->GetLevel()+Heuristic(m_CurrentNode,m_EndPosition)); //TODO Calculate Priority
	m_OpenNodes.push_back(m_CurrentNode);

	while(!IsFinalNode(m_CurrentNode))
	{
		if(!AllNodesVisited())
		{
			VisitNextNode(m_CurrentNode);
		}
	}

	while(m_CurrentNode->GetParent()!=NULL)
	{
		m_Result.push_back(m_CurrentNode->GetPosition());
		m_CurrentNode=m_CurrentNode->GetParent();
	}

	m_Result.push_back(m_CurrentNode->GetPosition());

	return m_Result;
}

void CAStar::VisitNextNode(CNode* Node)
{
	size_t l_BestNode = ExtractBestFNode();
	m_CurrentNode=m_OpenNodes[l_BestNode];
	CloseNode(l_BestNode);
	ProcesarVecinos(m_CurrentNode);
}

size_t CAStar::ExtractBestFNode()
{
	size_t l_BestNode = 0;
	for(size_t i=0;i<m_OpenNodes.size();++i)
	{
		if(m_OpenNodes[i]->GetPriority()<m_OpenNodes[l_BestNode]->GetPriority())
		{
			l_BestNode=i;
		}
	}

	return l_BestNode;
}

void CAStar::CloseNode(size_t i)
{
	m_OpenNodes[i]->SetClosed(true);
	m_ClosedNodes.push_back(m_OpenNodes[i]);
	m_OpenNodes.erase(m_OpenNodes.begin()+i);
}

void CAStar::ProcesarVecinos(CNode* Node)
{
	/*¿Cómo obtener vecinos del NODO?*/
	for(size_t i=0;i<Node->m_Neighbours.size();++i)
	{
		if(!Node->m_Neighbours[i]->GetClosed())
		{
			CalculateG(Node->m_Neighbours[i]);

			if(std::find(m_OpenNodes.begin(), m_OpenNodes.end(), Node->m_Neighbours[i]) != m_OpenNodes.end()) 
			{	//Esta en la lista de nodos abiertos
				float l_NewG = Node->GetLevel() + Node->m_Neighbours[i]->GetLevel();

				if(l_NewG < Node->m_Neighbours[i]->GetLevel()) 
				{
					Node->m_Neighbours[i]->SetParent(Node);
					Node->m_Neighbours[i]->SetLevel(l_NewG);
					Node->m_Neighbours[i]->SetPriority(Node->m_Neighbours[i]->GetLevel()+Heuristic(Node->m_Neighbours[i],m_EndPosition));
				}

			} else //No está en la lista de nodos abiertos
			{
				Node->m_Neighbours[i]->SetParent(Node);
				
				if(IsFinalNode(Node))
				{
					m_Finished=true;
				}
				else
				{
					//save g,h
					Node->m_Neighbours[i]->SetPriority(Node->m_Neighbours[i]->GetLevel()+Heuristic(Node->m_Neighbours[i],m_EndPosition));
					m_OpenNodes.push_back(Node->m_Neighbours[i]);
				}
			}
		}
	}
}

void CAStar::CalculateG(CNode* Node)
{
	float l_Level = Node->GetLevel();
	float l_ParentLevel=0.0f;
	if(Node->GetParent()!=NULL)
		l_ParentLevel=Node->GetParent()->GetLevel();
	Node->SetLevel(l_Level+l_ParentLevel);
}

bool CAStar::IsFinalNode(CNode* Node)
{
	if(Node->GetPosition()==m_EndPosition)
		return true;

	return false;
}

bool CAStar::AllNodesVisited() /*All nodes closed*/
{
	for(size_t i=0;i<m_Nodes.size();++i)
	{
		if(!m_Nodes[i]->GetClosed())
			return false;
	}

	return true;
}

const float CAStar::Heuristic(CNode* CurrentNode, const Vect3f Destiny) const
{
	Vect3f m_Dist;
	float l_Distance;

	m_Dist = Destiny - CurrentNode->GetPosition();    

    // Euclidian Distance
    l_Distance=static_cast<float>(sqrt(m_Dist.x*m_Dist.x+m_Dist.y*m_Dist.y+m_Dist.z*m_Dist.z));

    // Manhattan distance
    //l_Distance=abs(xd)+abs(yd);
            
    // Chebyshev distance
    //d=max(abs(xd), abs(yd));

    return(l_Distance);
}



