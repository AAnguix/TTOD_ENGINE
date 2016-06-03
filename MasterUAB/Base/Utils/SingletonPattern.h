#ifndef _SINGLETON_H
#define _SINGLETON_H

#include <assert.h>

template <typename T> class CSingleton
{

protected:
		static T* ms_Singleton;		
public:
	CSingleton( void )
	{
		assert( !ms_Singleton );
		#if defined( _MSC_VER ) && _MSC_VER < 1200	 
			int offset = (int)(T*)1 - (int)(CSingleton <T>*)(T*)1;
			ms_Singleton = (T*)((int)this + offset);
		#else
			ms_Singleton = static_cast< T* >( this );
		#endif
	}

	virtual ~CSingleton(void)
	{  
		assert( ms_Singleton );  
		ms_Singleton = 0;  
	}

	static T& GetSingleton(void)
	{	
		assert(ms_Singleton);  
		return (*ms_Singleton); 
	}

	static T* GetSingletonPtr(void)
	{ 
		return ms_Singleton; 
	}
};

template<typename T> T* CSingleton<T>::ms_Singleton = 0;

#endif
