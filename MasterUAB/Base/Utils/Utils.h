#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>

#define UAB_GET_STANDARD_PROPERTY(Type, Variable) \
public: \
	Type Get##Variable##() const {return m_##Variable##;}

#define UAB_GET_SET_STANDARD_PROPERTY(Type, Variable) \
public: \
	void Set##Variable##(Type Variable) {m_##Variable##=Variable;} \
	Type Get##Variable##() const {return m_##Variable##;}

#define UAB_GET_PROPERTY_REFERENCE(Type, Variable) \
public: \
	const Type & Get##Variable##() const {return m_##Variable##;}

#define UAB_GET_SET_PROPERTY_REFERENCE(Type, Variable) \
public: \
	void Set##Variable##(const Type &Variable) {m_##Variable##=Variable;} \
	const Type & Get##Variable##() const {return m_##Variable##;}

#define CHECKED_RELEASE(x) if(x) {x->Release(); x = nullptr;}

#define CHECKED_DELETE_ARRAY(x) if(x) {delete[] x; x = nullptr;}

#define DEG2RAD(x) (x * (float)(3.14159265358979323846 / 180.0))

#define CHECKED_DELETE(x) if(x!=NULL) delete(x); x=NULL;

#define UAB_GET_PROPERTY_POINTER(Type, Variable)  \
public: \
	Type* Get##Variable##() const {return m_##Variable##;}

#define UAB_GET_VAR_ADDRESS(Variable) \
	public: \
	CEmptyPointerClass * Get##Variable##Address() const {return (CEmptyPointerClass *)&m_##Variable##;}

#endif