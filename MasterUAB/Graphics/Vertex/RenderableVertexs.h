#ifndef _RENDERABLEVERTEX_H
#define _RENDERABLEVERTEX_H

#include <D3D11.h>
#include <assert.h>
#include "Utils\Utils.h"
#include "Render\RenderManager.h"
#include "Effects\EffectTechnique.h"

class CRenderableVertexs
{

public:

	virtual bool Render(CRenderManager *RenderManager, CEffectTechnique *EffectTechnique, void *Parameters, int numVertexs = -1)
	{
		assert(!"This method mustn't be called");
		return false;
	}

	virtual bool RenderIndexed(CRenderManager *RenderManager, CEffectTechnique *EffectTechnique, void *Parameters, unsigned int IndexCount=-1, unsigned int StartIndexLocation=0, unsigned int BaseVertexLocation=0)
	{
		assert(!"This method mustn't be called");
		return false;
	}

	static void CalcTangentsAndBinormals(void *VtxsData, unsigned short *IdxsData, size_t VtxCount, size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t NormalStride, size_t TangentStride, size_t BiNormalStride, size_t TextureCoordsStride) 
	{  
		Vect3f *tan1 = new Vect3f[VtxCount * 2];  
		Vect3f *tan2 = tan1 + VtxCount;  
		ZeroMemory(tan1, VtxCount * sizeof(Vect3f) * 2);  
		unsigned char *l_VtxAddress=(unsigned char *)VtxsData;  
	
		for(size_t b=0;b<IdxCount;b+=3)  
		{   
			unsigned short i1=IdxsData[b];   
			unsigned short i2=IdxsData[b+1];   
			unsigned short i3=IdxsData[b+2];  
	
			Vect3f *v1=(Vect3f *)&l_VtxAddress[i1*VertexStride+GeometryStride];   
			Vect3f *v2=(Vect3f *)&l_VtxAddress[i2*VertexStride+GeometryStride];   
			Vect3f *v3=(Vect3f *)&l_VtxAddress[i3*VertexStride+GeometryStride];  
			Vect2f *w1=(Vect2f *)&l_VtxAddress[i1*VertexStride+TextureCoordsStride];   
			Vect2f *w2=(Vect2f *)&l_VtxAddress[i2*VertexStride+TextureCoordsStride];   
			Vect2f *w3=(Vect2f *)&l_VtxAddress[i3*VertexStride+TextureCoordsStride];  
	
			float x1=v2->x-v1->x;   
			float x2=v3->x-v1->x;   
			float y1=v2->y-v1->y; 
	
			float y2=v3->y-v1->y;   
			float z1=v2->z-v1->z;   
			float z2=v3->z-v1->z;  
			float s1=w2->x-w1->x;   
			float s2=w3->x-w1->x;   
			float t1=w2->y-w1->y;   
			float t2=w3->y-w1->y;  
			float r = 1.0F / (s1 * t2 - s2 * t1);  
	
			Vect3f sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,    (t2 * z1 - t1 * z2) * r);   
			Vect3f tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,    (s1 * z2 - s2 * z1) * r);  
	
			assert(i1<VtxCount);  
			assert(i2<VtxCount);   
			assert(i3<VtxCount);  

			tan1[i1] += sdir;   tan1[i2] += sdir;   tan1[i3] += sdir;  
			tan2[i1] += tdir;   tan2[i2] += tdir;   tan2[i3] += tdir;  
		}  

		for (size_t b=0;b<VtxCount;++b)  
		{   
			Vect3f *l_NormalVtx=(Vect3f *)&l_VtxAddress[b*VertexStride+NormalStride];  
			Vect3f *l_TangentVtx=(Vect3f *)&l_VtxAddress[b*VertexStride+TangentStride];  
			Vect4f *l_TangentVtx4=(Vect4f *)&l_VtxAddress[b*VertexStride+TangentStride];   
			Vect3f *l_BiNormalVtx=(Vect3f *)&l_VtxAddress[b*VertexStride+BiNormalStride];   
			const Vect3f& t=tan1[b];  

			// Gram-Schmidt orthogonalize   
			Vect3f l_VAl=t-(*l_NormalVtx)*((*l_NormalVtx)*t);   l_VAl.Normalize();   *l_TangentVtx=l_VAl;      // Calculate handedness   
			Vect3f l_Cross;   l_Cross=(*l_NormalVtx)^(*l_TangentVtx);   
			l_TangentVtx4->w=(l_Cross*(tan2[b]))< 0.0f ? -1.0f : 1.0f;      
			*l_BiNormalVtx=(*l_NormalVtx)^(*l_TangentVtx);  
		}  

		delete[] tan1; 
	} 

	virtual bool UpdateVertexs(void* Vtxs, unsigned int VtxsCount)
	{
		assert(!"This method mustn't be called");
		return false;
	}

};

// ************************************************************************************************************************************************************************************



template<class T>
class CTemplatedRenderableVertexs : public CRenderableVertexs
{

private:

	ID3D11Buffer *m_VertexBuffer;
	D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveTopology;
	unsigned int m_VertexsCount;
	unsigned int m_PrimitiveCount;
	bool m_Dynamic;

public:

	CTemplatedRenderableVertexs(void *Vtxs, unsigned int VtxsCount, D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology, unsigned int PrimitiveCount, bool Dynamic)
		: m_VertexsCount(VtxsCount), m_PrimitiveTopology(PrimitiveTopology), m_PrimitiveCount(PrimitiveCount), m_Dynamic(Dynamic)
	{
		D3D11_BUFFER_DESC l_BufferDescription;
		ZeroMemory(&l_BufferDescription, sizeof(l_BufferDescription));
		l_BufferDescription.Usage = Dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT; //D3D11_USAGE_DEFAULT
		l_BufferDescription.ByteWidth = sizeof(T)*m_VertexsCount;
		l_BufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		l_BufferDescription.CPUAccessFlags = Dynamic ? D3D11_CPU_ACCESS_WRITE : 0; //0
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory( &InitData, sizeof(InitData) );
		InitData.pSysMem = Vtxs;
		ID3D11Device *l_Device=CEngine::GetSingleton().GetRenderManager()->GetContextManager()->GetDevice();
		HRESULT l_HR=l_Device->CreateBuffer(&l_BufferDescription, &InitData,
		&m_VertexBuffer);

		if(FAILED(l_HR))
		return;
	}

	virtual ~CTemplatedRenderableVertexs()
	{
		CHECKED_RELEASE(m_VertexBuffer);
	}

	bool Render(CRenderManager *RenderManager, CEffectTechnique *EffectTechnique, void *Parameters, int numVertexs)
	{
		CEffectVertexShader *l_EffectVertexShader=EffectTechnique->GetVertexShader();
		CEffectPixelShader *l_EffectPixelShader=EffectTechnique->GetPixelShader();
		CEffectGeometryShader *l_EffectGeometryShader = EffectTechnique->GetGeometryShader();
		
		if(l_EffectPixelShader==NULL || l_EffectVertexShader==NULL || l_EffectVertexShader->GetVertexShader()==NULL || l_EffectPixelShader->GetPixelShader()==NULL )
		{
			return false;
		}
		ID3D11DeviceContext *l_DeviceContext=RenderManager->GetContextManager()->GetDeviceContext();
		UINT stride=sizeof(T);
		UINT offset=0;
		l_DeviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride,&offset);
		l_DeviceContext->IASetPrimitiveTopology(m_PrimitiveTopology);
		l_DeviceContext->IASetInputLayout(l_EffectVertexShader->GetVertexLayout());

		l_DeviceContext->VSSetShader(l_EffectVertexShader->GetVertexShader(),NULL, 0);
		ID3D11Buffer *l_ConstantBufferVS=l_EffectVertexShader->GetConstantBuffer(0);
		l_DeviceContext->UpdateSubresource(l_ConstantBufferVS, 0, NULL,Parameters, 0, 0 );
		l_DeviceContext->VSSetConstantBuffers( 0, 1, &l_ConstantBufferVS);
		
		l_DeviceContext->PSSetShader(l_EffectPixelShader->GetPixelShader(),NULL, 0);
		ID3D11Buffer *l_ConstantBufferPS=l_EffectPixelShader->GetConstantBuffer(0);
		l_DeviceContext->UpdateSubresource(l_ConstantBufferPS, 0, NULL,Parameters, 0, 0 );
		l_DeviceContext->PSSetConstantBuffers(0, 1, &l_ConstantBufferPS);

		if (l_EffectGeometryShader)
		{
			ID3D11Buffer *l_ConstantBufferGS = l_EffectGeometryShader->GetConstantBuffer(0);
			l_DeviceContext->UpdateSubresource(l_ConstantBufferGS, 0, NULL, Parameters, 0, 0);
			l_DeviceContext->GSSetConstantBuffers(0, 1, &l_ConstantBufferGS);
			l_DeviceContext->GSSetShader(l_EffectGeometryShader->GetGeometryShader(), NULL, 0);
		}
		else
		{
			l_DeviceContext->GSSetShader(NULL, NULL, 0);
		}
		
		l_DeviceContext->Draw(numVertexs >= 0 ? numVertexs : m_VertexsCount, 0);
		return true;
	}

	bool UpdateVertexs(void* Vtxs, unsigned int VtxsCount)
	{
		assert(m_Dynamic);

		D3D11_MAPPED_SUBRESOURCE l_MappedResource;
		ZeroMemory(&l_MappedResource, sizeof(l_MappedResource));

		ID3D11DeviceContext* l_DeviceContext = CEngine::GetSingleton().GetRenderManager()->GetContextManager()->GetDeviceContext();
		HRESULT l_HR = l_DeviceContext->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &l_MappedResource);
		if (FAILED(l_HR))
			return false;

		memcpy(l_MappedResource.pData, Vtxs, sizeof(T)*VtxsCount);

		l_DeviceContext->Unmap(m_VertexBuffer, 0);

		return true;
	}
};

#define CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(ClassName, TopologyType) \
template<class T> \
class ClassName : public CTemplatedRenderableVertexs<T> \
{ \
public: \
ClassName(void *Vtxs, unsigned int VtxsCount, unsigned int PrimitiveCount, bool Dynamic) \
: CTemplatedRenderableVertexs(Vtxs, VtxsCount, TopologyType, PrimitiveCount, Dynamic) \
{ \
} \
};
CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(CLinesListRenderableVertexs,D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(CLinesStripRenderableVertexs,D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(CTrianglesListRenderableVertexs,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(CTrianglesStripRenderableVertexs,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
CRENDERABLE_VERTEX_CLASS_TYPE_CREATOR(CPointListRenderableVertexs, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

//Clase templatizada por cada tipo de vértice que derive
//de CRenderableVertexs para renderizar utilizando vertex buffer e index buffer. La
//clase la vamos a definir de la siguiente manera.




template<class T>
class CTemplatedRenderableIndexedVertexs : public CRenderableVertexs
{

private:
	ID3D11Buffer *m_VertexBuffer;
	ID3D11Buffer *m_IndexBuffer;
	D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveTopology;
	DXGI_FORMAT m_IndexType;
	unsigned int m_VertexsCount;
	unsigned int m_IndexsCount;
	bool m_Dynamic;

public:
	CTemplatedRenderableIndexedVertexs(void *Vtxs, unsigned int VtxsCount, void *Indices, unsigned int IndexsCount, D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology,DXGI_FORMAT IndexType, bool Dynamic)
		: m_VertexsCount(VtxsCount), m_IndexsCount(IndexsCount), m_PrimitiveTopology(PrimitiveTopology), m_VertexBuffer(0), m_IndexBuffer(0), m_IndexType(IndexType), m_Dynamic(Dynamic)
{
	D3D11_BUFFER_DESC l_VertexBufferDesc;
	ZeroMemory(&l_VertexBufferDesc, sizeof(l_VertexBufferDesc));
	l_VertexBufferDesc.Usage = Dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT; //D3D11_USAGE_DEFAULT;

	l_VertexBufferDesc.ByteWidth=sizeof(T)*m_VertexsCount;
	l_VertexBufferDesc.BindFlags=D3D11_BIND_VERTEX_BUFFER;
	l_VertexBufferDesc.CPUAccessFlags = Dynamic ? D3D11_CPU_ACCESS_WRITE : 0; //0
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem=Vtxs;
	ID3D11Device *l_Device=CEngine::GetSingleton().GetRenderManager()->GetContextManager()->GetDevice();
	HRESULT hr=l_Device->CreateBuffer(&l_VertexBufferDesc, &InitData,
	&m_VertexBuffer);
	if(FAILED(hr))
	return;

	D3D11_BUFFER_DESC l_IndexBuffer;
	ZeroMemory(&l_IndexBuffer, sizeof(l_IndexBuffer));
	l_IndexBuffer.Usage = Dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT; //D3D11_USAGE_DEFAULT;
	l_IndexBuffer.ByteWidth=(m_IndexType==DXGI_FORMAT_R16_UINT ?
	sizeof(WORD) : sizeof(unsigned int))*m_IndexsCount;
	l_IndexBuffer.BindFlags=D3D11_BIND_INDEX_BUFFER;
	l_IndexBuffer.CPUAccessFlags = Dynamic ? D3D11_CPU_ACCESS_WRITE : 0; //0
	InitData.pSysMem=Indices;
	hr=l_Device->CreateBuffer(&l_IndexBuffer, &InitData, &m_IndexBuffer);
	if(FAILED(hr))
	return;
}
	virtual ~CTemplatedRenderableIndexedVertexs()
	{
	CHECKED_RELEASE(m_VertexBuffer);
	CHECKED_RELEASE(m_IndexBuffer);
	}
	
	bool RenderIndexed(CRenderManager *RenderManager, CEffectTechnique *EffectTechnique, void *Parameters, unsigned int IndexCount=-1, unsigned int StartIndexLocation=0, unsigned int BaseVertexLocation=0)
	{
		if(EffectTechnique==NULL)
		return false;

		CEffectVertexShader *l_EffectVertexShader=EffectTechnique->GetVertexShader();
		CEffectPixelShader *l_EffectPixelShader=EffectTechnique->GetPixelShader();
		CEffectGeometryShader *l_EffectGeometryShader = EffectTechnique->GetGeometryShader();
		ID3D11Buffer *l_ConstantBufferVS=l_EffectVertexShader->GetConstantBuffer(0);
		
		if (l_EffectVertexShader == NULL || l_ConstantBufferVS == NULL) //l_EffectPixelShader == NULL || 
		{
			assert(false);
			return false;
		}
		ID3D11DeviceContext *l_DeviceContext=RenderManager->GetContextManager()->GetDeviceContext();
		UINT stride=sizeof(T);
		UINT offset=0;
		l_DeviceContext->IASetIndexBuffer(m_IndexBuffer, m_IndexType, 0);
		l_DeviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride,&offset);
		l_DeviceContext->IASetPrimitiveTopology(m_PrimitiveTopology);
		l_DeviceContext->IASetInputLayout(l_EffectVertexShader->GetVertexLayout());
		
		l_DeviceContext->VSSetShader(l_EffectVertexShader->GetVertexShader(),NULL, 0);
		l_DeviceContext->UpdateSubresource(l_ConstantBufferVS, 0, NULL,Parameters, 0, 0 );
		l_DeviceContext->VSSetConstantBuffers(0, 1, &l_ConstantBufferVS);
		
		ID3D11PixelShader* l_PixelShader = NULL;
		ID3D11Buffer *l_ConstantBufferPS = NULL;
		if (l_EffectPixelShader)
		{
			l_PixelShader = l_EffectPixelShader->GetPixelShader();
			l_ConstantBufferPS = l_EffectPixelShader->GetConstantBuffer(0);
		}
		l_DeviceContext->PSSetShader(l_PixelShader, NULL, 0);
		if (l_ConstantBufferPS)
		{
			l_DeviceContext->UpdateSubresource(l_ConstantBufferPS, 0, NULL, Parameters, 0, 0);
			l_DeviceContext->PSSetConstantBuffers(0, 1, &l_ConstantBufferPS);
		}
		if (l_EffectGeometryShader)
		{
			ID3D11Buffer *l_ConstantBufferGS = l_EffectGeometryShader->GetConstantBuffer(0);
			l_DeviceContext->UpdateSubresource(l_ConstantBufferGS, 0, NULL, Parameters, 0, 0);
			l_DeviceContext->GSSetConstantBuffers(0, 1, &l_ConstantBufferGS);
			l_DeviceContext->GSSetShader(l_EffectGeometryShader->GetGeometryShader(), NULL, 0);
		}
		else
		{
			l_DeviceContext->GSSetShader(NULL, NULL, 0);
		}

		l_DeviceContext->DrawIndexed(IndexCount==-1 ? m_IndexsCount : IndexCount, StartIndexLocation, BaseVertexLocation);
		return true;
}

	bool UpdateVertexs(void* Vtxs, unsigned int VtxsCount)
	{
		assert(m_Dynamic);

		D3D11_MAPPED_SUBRESOURCE l_MappedResource;
		ZeroMemory(&l_MappedResource, sizeof(l_MappedResource));

		ID3D11DeviceContext* l_DeviceContext = CEngine::GetSingleton().GetRenderManager()->GetContextManager()->GetDeviceContext();
		HRESULT l_HR = l_DeviceContext->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &l_MappedResource);
		if (FAILED(l_HR))
			return false;

		memcpy(l_MappedResource.pData, Vtxs, sizeof(T)*VtxsCount);

		l_DeviceContext->Unmap(m_VertexBuffer, 0);

		return true;
	}
};

#define CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(ClassName, TopologyType, IndexType) \
template<class T> \
class ClassName : public CTemplatedRenderableIndexedVertexs<T> \
{ \
public: \
ClassName(void *Vtxs, unsigned int VtxsCount, void *Indices, unsigned int IndexsCount, bool Dynamic) \
: CTemplatedRenderableIndexedVertexs(Vtxs, VtxsCount, Indices, IndexsCount, TopologyType, IndexType, Dynamic) \
{ \
} \
};

CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CLinesListRenderableIndexed16Vertexs, D3D11_PRIMITIVE_TOPOLOGY_LINELIST, DXGI_FORMAT_R16_UINT);
CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CLinesListRenderableIndexed32Vertexs, D3D11_PRIMITIVE_TOPOLOGY_LINELIST, DXGI_FORMAT_R16_UINT);
CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CTriangleListRenderableIndexed16Vertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, DXGI_FORMAT_R16_UINT);
CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CTriangleListRenderableIndexed32Vertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, DXGI_FORMAT_R32_UINT);
CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CTriangleStripRenderableIndexed16Vertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, DXGI_FORMAT_R16_UINT);
CRENDERABLE_INDEXED_VERTEX_CLASS_TYPE_CREATOR(CTriangleStripRenderableIndexed32Vertexs, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, DXGI_FORMAT_R32_UINT);

#endif
