#ifndef _CAMERAKEY_H
#define _CAMERAKEY_H

#include "Cinematics\CameraInfo.h"

class CCameraKey 
{ 
	public: 
		CCameraInfo m_CameraInfo; 
		float m_Time; 
		CCameraKey(CCameraInfo &CameraInfo, float Time); 
};

#endif