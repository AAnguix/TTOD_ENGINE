#include "AStar.h"
#include "Render\DebugRender.h"
#include <algorithm>

CAStar::CAStar() {
	LoadMap();
}

CAStar::~CAStar() {
	DestroyMap();
}

CAStar::TNode* CAStar::AddNode(Vect3f Position)
{
	TNode *l_Node = new TNode();
	l_Node->position = Position;
	m_map.push_back(l_Node);
	return l_Node;
}

void CAStar::AddNeighbours(TNode* Node, VNodes Neighbours)
{
	for(size_t i=0;i<Neighbours.size();++i)
	{
		Node->neighbours.push_back( PNodeAndDistance(Neighbours[i], ((Node->position - Neighbours[i]->position ).Length())) );
	}	
}

void CAStar::LoadMap() 
{
	TNode *node1 = new TNode();
	node1->position = Vect3f( 0.0f, 0.0f, 0.0f );

	TNode *node2 = new TNode();
	node2->position = Vect3f( 10.0f, 0.0f, 10.0f );

	TNode *node3 = new TNode();
	node3->position = Vect3f( 0.0f, 0.0f, -5.0f );

	TNode *node4 = new TNode();
	node4->position = Vect3f( 10.0f, 0.0f, -10.0f );

	TNode *node5 = new TNode();
	node5->position = Vect3f( -10.0f, 0.0f, 5.0f );

	// Vecinos
	node1->neighbours.push_back( PNodeAndDistance( node2, (( node1->position - node2->position ).Length() ) ) );
	node1->neighbours.push_back( PNodeAndDistance( node3, (( node1->position - node3->position ).Length() ) ) );
	node1->neighbours.push_back( PNodeAndDistance( node5, (( node1->position - node5->position ).Length() ) ) );
	node2->neighbours.push_back( PNodeAndDistance( node1, (( node2->position - node1->position ).Length() ) ) );
	node3->neighbours.push_back( PNodeAndDistance( node1, (( node3->position - node1->position ).Length() ) ) );
	node3->neighbours.push_back( PNodeAndDistance( node4, (( node3->position - node4->position ).Length() ) ) );
	node3->neighbours.push_back( PNodeAndDistance( node5, (( node3->position - node5->position ).Length() ) ) );
	node4->neighbours.push_back( PNodeAndDistance( node3, (( node4->position - node3->position ).Length() ) ) );
	node5->neighbours.push_back( PNodeAndDistance( node1, (( node5->position - node1->position ).Length() ) ) );
	node5->neighbours.push_back( PNodeAndDistance( node3, (( node5->position - node3->position ).Length() ) ) );

	// Inserciones
	m_map.push_back( node1 );
	m_map.push_back( node2 );
	m_map.push_back( node3 );
	m_map.push_back( node4 );
	m_map.push_back( node5 );
}

void CAStar::DestroyMap() {
	VNodes::const_iterator it;
	for( it = m_map.begin(); it != m_map.end(); ++it ) {
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

bool CAStar::TCompareNodes::operator()( const TNode *nodeA, const TNode *nodeB ) {
	return nodeA->f < nodeB->f;
}


CAStar::VNodes CAStar::PSearchPath( TNode* nodeA, TNode *nodeB ) {
	// Marcamos todos los nodos como no visitados
	VNodes::const_iterator it;
	for( it = m_map.begin(); it != m_map.end(); ++it ) {
		TNode *node = *it;
		node->f = 0.0f;
		node->g = 0.0f;
		node->h = 0.0f;
		node->parent = NULL;
		node->inOpenList = false;
		node->closed = false;
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
			currentNode = currentNode->parent;
		}
	}
	return result;
}

void CAStar::AddToOpenList( TNode *node ) {
	node->inOpenList = true;
	m_openList.push_back( node );
}

CAStar::TNode *CAStar::PopFirstFromOpenList() {
	TNode *firstNode = *( m_openList.begin() );
	firstNode->inOpenList = false;
	m_openList.erase( m_openList.begin() );
	return firstNode;
}

bool CAStar::VisitNextNode( TNode *destinationNode ) {
	// Mirar si quedan nodos en la lista de nodos abiertos
	if( m_openList.size() > 0 ) {
		// Coge el primer nodo y sacarlo de la lista abierta
		TNode *currentNode = PopFirstFromOpenList();

		// Marcarlo como cerrado
		currentNode->closed = true;

		// Mirar todos sus vecinos
		VNodesAndDistances::iterator it;
		for( it = currentNode->neighbours.begin(); it != currentNode->neighbours.end(); ++it ) {
			// Solo para los vecinos que no est�n cerrados
			TNode *currentNeighbour = it->first;
			float distanceToNode = it->second;
			if( !currentNeighbour->closed ) {
				// Calcular su g llegando a este nodo desde el currentNode
				float g = currentNode->g + distanceToNode;

				// Comprobar si ya estaba en la lista de abiertos
				if( !currentNeighbour->inOpenList ) {
					// Si no estaba en la lista

					// Hacer que su padre sea el actual
					currentNeighbour->parent = currentNode;

					// Si el vecino que comprobamos es el destino, ya tenemos el camino
					if( currentNeighbour == destinationNode ) {
						return true;

					} else {
						// Asignar los valores de g, h y f
						currentNeighbour->g = g;
						currentNeighbour->h = Vect3f( currentNeighbour->position - currentNode->position ).Length();
						currentNeighbour->f = currentNeighbour->g + currentNeighbour->h;

						// Meterlo en la lista de abiertos
						AddToOpenList( currentNeighbour );
					}

				} else {
					// Si ya estaba en la lista

					// Comprobamos que el camino nuevo, sea mejor que el camino ya conocido
					if( g < currentNeighbour->g ) {
						// Si el nuevo camino es mejor, nos quedamos con ese

						// Hacer que su padre sea el actual
						currentNeighbour->parent = currentNode;

						// Asignar la nueva g y recalcular f (h no cambia)
						currentNeighbour->g = g;
						currentNeighbour->f = currentNeighbour->g + currentNeighbour->h;
					}
				}
			}
		}

		// Ordenar la lista abierta despu�s de las posibles inserciones y cambios de valor de f
		std::sort( m_openList.begin(), m_openList.end(), TCompareNodes() );
	}
	return false;
}

CAStar::TNode *CAStar::GetNearestNode( const Vect3f &point ) {
	TNode *bestNode = NULL;
	float nearestSquaredDistance = FLT_MAX;

	VNodes::const_iterator it;
	for( it = m_map.begin(); it != m_map.end(); ++it ) {
		TNode *currentNode = *it;
		float currentSquaredDistance = Vect3f(( currentNode->position - point ) ).SquaredLength();
		if( currentSquaredDistance < nearestSquaredDistance ) {
			nearestSquaredDistance = currentSquaredDistance;
			bestNode = currentNode;
		}
	}
	return bestNode;
}

VPoints3 CAStar::SearchPath( const Vect3f &pointA, const Vect3f &pointB ) {
	TNode *nodeA = GetNearestNode( pointA );
	TNode *nodeB = GetNearestNode( pointB );
	VNodes nodes = PSearchPath( nodeA, nodeB );

	VPoints3 points;
	VNodes::const_iterator it;
	for( it = nodes.begin(); it != nodes.end(); ++it ) {
		TNode *currentNode = *it;
		points.push_back( currentNode->position );
	}
	return points;
}