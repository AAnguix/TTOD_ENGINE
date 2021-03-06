#ifndef _SCENERENDERERCOMMANDMANAGER_H
#define _SCENERENDERERCOMMANDMANAGER_H
#include "Utils\TemplatedVectorMapManager.h"
#include "SceneRendererCommand.h"

class CSceneRendererCommandManager : public CTemplatedVectorMapManager<CSceneRendererCommand> 
{ 
	private:  
		std::string	m_Filename; 
	public:  
		CSceneRendererCommandManager();  
		virtual ~CSceneRendererCommandManager(); 
		
		std::string GetNextName();  
		bool Load(const std::string &Filename);  
		bool Reload();  
		bool Execute(CRenderManager &RenderManager) const; 
		void ActivateCommand(unsigned int Index, bool Activate);
		const std::vector<CSceneRendererCommand *> & GetLUASceneRendererCommands();
};  

#endif