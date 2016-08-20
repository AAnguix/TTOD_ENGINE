#include "AStar.h"
#include "XML\XMLTreeNode.h"
//#include "Render\DebugRender.h"
#include <algorithm>
#include <assert.h>

CAStar::CAStar() 
{
	//LoadMap();
}

CAStar::~CAStar() 
{
	DestroyMap();
}

CAStar::TNode::TNode()
{
	m_Position = v3fZERO;
	m_F = 0.0f;
	m_G = 0.0f;
	m_H = 0.0f;
	m_Parent = NULL;
	m_InOpenList = false;
	m_Closed = false;
}


CAStar::TNode::TNode(CXMLTreeNode &TreeNode)
{
	m_Position = TreeNode.GetVect3fProperty("position", v3fZERO);
	m_F = 0.0f;
	m_G = 0.0f;
	m_H = 0.0f;
	m_Parent = NULL;
	m_InOpenList = false;
	m_Closed = false;
}

CAStar::TNode* CAStar::AddNode(Vect3f Position)
{
	TNode *l_Node = new TNode();
	l_Node->m_Position = Position;
	m_map.push_back(l_Node);
	return l_Node;
}

void CAStar::AddNeighbours(TNode* Node, VNodes Neighbours)
{
	for(size_t i=0;i<Neighbours.size();++i)
	{
		Node->m_Neighbours.push_back(PNodeAndDistance(Neighbours[i], ((Node->m_Position - Neighbours[i]->m_Position).Length())));
	}	
}

bool CAStar::LoadNodes(const std::string &Filename)
{
	DestroyMap();
	m_Filename = Filename;
	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(Filename.c_str()))
	{
		CXMLTreeNode l_Nodes = l_XML["nodes"];
		if (l_Nodes.Exists())
		{
			for (int i = 0; i < l_Nodes.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Nodes(i);

				if (l_Element.GetName() == std::string("node"))
				{
					TNode* l_Node = new TNode(l_Element);
					m_map.push_back(l_Node);
				}
				else if (l_Element.GetName() == std::string("neighbours"))
				{
					unsigned int l_NodeID = l_Element.GetIntProperty("node");
					TNode* l_ParentNode = GetNode(l_NodeID);
					VNodes l_Neighbours;

					for (int i = 0; i < l_Element.GetNumChildren(); ++i)
					{
						CXMLTreeNode l_NeighBourNode = l_Element(i);
						if (l_NeighBourNode.GetName() == std::string("neighbour"))
						{
							TNode* l_Neighbour = GetNode(l_NeighBourNode.GetIntProperty("node"));
							assert(l_ParentNode->m_Position != l_Neighbour->m_Position);
							l_Neighbours.push_back(l_Neighbour);
						}
					}

					if (l_Neighbours.size()>0)
						AddNeighbours(l_ParentNode, l_Neighbours);
				}

			}
			return true;
		}
		else { assert(false); }
	}
	else { assert(false); }
	return false;
}

CAStar::TNode* CAStar::GetNode(unsigned int ID) const
{
	if (ID<=m_map.size())
	{
		return m_map[ID];
	}
	return nullptr;
}

void CAStar::LoadMap() 
{
	TNode *node1 = new TNode();
	node1->m_Position = Vect3f( 0.0f, 0.0f, 0.0f );

	TNode *node2 = new TNode();
	node2->m_Position = Vect3f(10.0f, 0.0f, 10.0f);

	TNode *node3 = new TNode();
	node3->m_Position = Vect3f(0.0f, 0.0f, -5.0f);

	TNode *node4 = new TNode();
	node4->m_Position = Vect3f(10.0f, 0.0f, -10.0f);

	TNode *node5 = new TNode();
	node5->m_Position = Vect3f(-10.0f, 0.0f, 5.0f);

	// Vecinos
	node1->m_Neighbours.push_back( PNodeAndDistance( node2, (( node1->m_Position - node2->m_Position ).Length() ) ) );
	node1->m_Neighbours.push_back( PNodeAndDistance( node3, (( node1->m_Position - node3->m_Position ).Length() ) ) );
	node1->m_Neighbours.push_back( PNodeAndDistance( node5, (( node1->m_Position - node5->m_Position ).Length() ) ) );
	node2->m_Neighbours.push_back( PNodeAndDistance( node1, (( node2->m_Position - node1->m_Position ).Length() ) ) );
	node3->m_Neighbours.push_back( PNodeAndDistance( node1, (( node3->m_Position - node1->m_Position ).Length() ) ) );
	node3->m_Neighbours.push_back( PNodeAndDistance( node4, (( node3->m_Position - node4->m_Position ).Length() ) ) );
	node3->m_Neighbours.push_back( PNodeAndDistance( node5, (( node3->m_Position - node5->m_Position ).Length() ) ) );
	node4->m_Neighbours.push_back( PNodeAndDistance( node3, (( node4->m_Position - node3->m_Position ).Length() ) ) );
	node5->m_Neighbours.push_back( PNodeAndDistance( node1, (( node5->m_Position - node1->m_Position ).Length() ) ) );
	node5->m_Neighbours.push_back( PNodeAndDistance( node3, (( node5->m_Position - node3->m_Position ).Length() ) ) );

	// Inserciones
	m_map.push_back( node1 );
	m_map.push_back( node2 );
	m_map.push_back( node3 );
	m_map.push_back( node4 );
	m_map.push_back( node5 );
}

void CAStar::DestroyMap() {
	VNodes::const_iterator it;
	for( it = m_map.begin(); it != m_map.end(); ++it ) 
	{
		delete *it;
	}
	m_map.clear();
}

//void CAStar::Render( LPDIRECT3DDEVICE9 device ) 
//{
//	D3DXMATRIX translation;
//	VNodes::const_iterator it;
//
//	for( it = m_map.begin(); it != m_map.end(); ++it ) 
//	{
//		const TNode *node = *it;
//		// Dibujar el nodo
//		D3DXMatrixTranslation( &translation, node->position.x, node->position.y, node->position.z );
//		device->SetTransform( D3DTS_WORLD, &translation );
//		CDebugRender::DrawSphere( device, 1.0, 0x00ff00, 10 );
//		
//		D3DXMatrixTranslation( &translation, 0.0f, 0.0f, 0.0f );
//		device->SetTransform( D3DTS_WORLD, &translation );
//
//		// Dibujar las relaciones con los vecinos
//		VNodesAndDistances::const_iterator nit;
//		for( nit = node->neighbours.begin(); nit != node->neighbours.end(); ++nit ) {
//			const TNode *neighbourNode = nit->first;
//			CDebugRender::DrawLine( device, node->position, neighbourNode->position, 0xff0000 );
//		}
//	}
//	D3DXMatrixTranslation( &translation, 0.0f, 0.0f, 0.0f );
//	device->SetTransform( D3DTS_WORLD, &translation );
//}

bool CAStar::TCompareNodes::operator()( const TNode *nodeA, const TNode *nodeB ) 
{
	return nodeA->m_F < nodeB->m_F;
}


CAStar::VNodes CAStar::PSearchPath( TNode* nodeA, TNode *nodeB ) 
{
	// Marcamos todos los nodos como no visitados
	VNodes::const_iterator it;
	for( it = m_map.begin(); it != m_map.end(); ++it ) 
	{
		TNode *node = *it;
		node->m_F = 0.0f;
		node->m_G = 0.0f;
		node->m_H = 0.0f;
		node->m_Parent = NULL;
		node->m_InOpenList = false;
		node->m_Closed = false;
	}

	// Inicializamos la lista abierta con el nodo inicial
	m_openList.clear();
	AddToOpenList( nodeA );

	// Ir visitando nodos hasta que se encuentre el destino o no queden nodos abiertos
	bool pathFound = false;
	while( !pathFound && m_openList.size() > 0 ) {
		pathFound = VisitNextNode( nodeB );
	}

	// Recorrer la lista de nodos desde el destino hasta el inicio, por los punteros que apuntan a los nodos padres
	VNodes result;
	if( pathFound ) {
		TNode *currentNode = nodeB;
		while( currentNode ) {
			result.insert( result.begin(), currentNode );
			currentNode = currentNode->m_Parent;
		}
	}
	return result;
}

void CAStar::AddToOpenList( TNode *node ) 
{
	node->m_InOpenList = true;
	m_openList.push_back( node );
}

CAStar::TNode *CAStar::PopFirstFromOpenList() 
{
	TNode *firstNode = *( m_openList.begin() );
	firstNode->m_InOpenList = false;
	m_openList.erase( m_openList.begin() );
	return firstNode;
}

bool CAStar::VisitNextNode( TNode *destinationNode ) 
{
	// Mirar si quedan nodos en la lista de nodos abiertos
	if( m_openList.size() > 0 ) {
		// Coge el primer nodo y sacarlo de la lista abierta
		TNode *currentNode = PopFirstFromOpenList();

		// Marcarlo como cerrado
		currentNode->m_Closed = true;

		// Mirar todos sus vecinos
		VNodesAndDistances::iterator it;
		for (it = currentNode->m_Neighbours.begin(); it != currentNode->m_Neighbours.end(); ++it) {
			// Solo para los vecinos que no estén cerrados
			TNode *currentNeighbour = it->first;
			float distanceToNode = it->second;
			if( !currentNeighbour->m_Closed ) {
				// Calcular su g llegando a este nodo desde el currentNode
				float g = currentNode->m_G + distanceToNode;

				// Comprobar si ya estaba en la lista de abiertos
				if( !currentNeighbour->m_InOpenList ) {
					// Si no estaba en la lista

					// Hacer que su padre sea el actual
					currentNeighbour->m_Parent = currentNode;

					// Si el vecino que comprobamos es el destino, ya tenemos el camino
					if( currentNeighbour == destinationNode ) {
						return true;

					} else {
						// Asignar los valores de g, h y f
						currentNeighbour->m_G = g;
						currentNeighbour->m_H = Vect3f( currentNeighbour->m_Position - currentNode->m_Position ).Length();
						currentNeighbour->m_F = currentNeighbour->m_G + currentNeighbour->m_H;

						// Meterlo en la lista de abiertos
						AddToOpenList( currentNeighbour );
					}

				} else {
					// Si ya estaba en la lista

					// Comprobamos que el camino nuevo, sea mejor que el camino ya conocido
					if( g < currentNeighbour->m_G ) {
						// Si el nuevo camino es mejor, nos quedamos con ese

						// Hacer que su padre sea el actual
						currentNeighbour->m_Parent = currentNode;

						// Asignar la nueva g y recalcular f (h no cambia)
						currentNeighbour->m_G = g;
						currentNeighbour->m_F = currentNeighbour->m_G + currentNeighbour->m_H;
					}
				}
			}
		}

		// Ordenar la lista abierta después de las posibles inserciones y cambios de valor de f
		std::sort( m_openList.begin(), m_openList.end(), TCompareNodes() );
	}
	return false;
}

CAStar::TNode *CAStar::GetNearestNode( const Vect3f &point ) 
{
	TNode *bestNode = NULL;
	float nearestSquaredDistance = FLT_MAX;

	VNodes::const_iterator it;
	for( it = m_map.begin(); it != m_map.end(); ++it ) {
		TNode *currentNode = *it;
		float currentSquaredDistance = Vect3f(( currentNode->m_Position - point ) ).SquaredLength();
		if( currentSquaredDistance < nearestSquaredDistance ) {
			nearestSquaredDistance = currentSquaredDistance;
			bestNode = currentNode;
		}
	}
	return bestNode;
}

std::vector<Vect3f> CAStar::SearchPath(const Vect3f &pointA, const Vect3f &pointB)
{
	TNode *nodeA = GetNearestNode( pointA );
	TNode *nodeB = GetNearestNode( pointB );
	VNodes nodes = PSearchPath( nodeA, nodeB );

	VPoints3 points;
	VNodes::const_iterator it;
	for( it = nodes.begin(); it != nodes.end(); ++it ) 
	{
		TNode *currentNode = *it;
		points.push_back( currentNode->m_Position );
	}
	return points;
}
