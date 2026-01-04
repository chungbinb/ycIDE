#include "stdafx.h"
#include "efs.h"

ETAG::ETAG()
{
	ID = 0;
	Type1 = 0;
	Type2 = (ETYPE)0;
}

ETAG::ETAG(UINT uint)
{
	*this = *(ETAG*)&uint;
}

bool ETAG::operator==(ETAG tag)
{
	return *this == (UINT)tag;
}

bool ETAG::operator!=(ETAG tag)
{
	return *this != (UINT)tag;
}

bool ETAG::operator==(UINT uint)
{
	return *(UINT*)this == uint;
}

bool ETAG::operator!=(UINT uint)
{
	return *(UINT*)this != uint;
}

bool ETAG::operator==(int uint)
{
	return *(UINT*)this == uint;
}

bool ETAG::operator!=(int uint)
{
	return *(UINT*)this != uint;
}


ETAG::operator UINT()
{
	return *(UINT*)this;
}

LIBTAG::LIBTAG()
{
	ID = 0;
	LibID = 0;
}

LIBTAG::LIBTAG(UINT uint)
{
	*this = *(LIBTAG*)&uint;
}

LIBTAG::operator UINT()
{
	return *(UINT*)this;
}

LIBCONST::LIBCONST()
{
	LibID = 0;
	ID = 0;
}

LIBCONST::LIBCONST(UINT uint)
{
	*this = *(LIBCONST*)&uint;
}

LIBCONST::operator UINT()
{
	return *(UINT*)this;
}

LIBCONSTSTRUCT::LIBCONSTSTRUCT()
{
	Index = 0;
}

LIBCONSTSTRUCT::LIBCONSTSTRUCT(UINT64 uint)
{
	*this = *(LIBCONSTSTRUCT*)&uint;
}

LIBCONSTSTRUCT::operator UINT64()
{
	return *(UINT64*)this;
}

EKeyValPair::EKeyValPair()
{
	this->Key = NULL;
	this->Value = NULL;
}

EKeyValPair::EKeyValPair(UINT64 uint64)
{
	*this = *(EKeyValPair*)&uint64;
}

EKeyValPair::operator UINT64()
{
	return *(UINT64*)this;
}

ESection_Library::ESection_Library()
{
	Major = 0;
	Minor = 0;
}

ESection_Library::ESection_Library(nullptr_t)
{
	new (this)ESection_Library();
}

bool ESection_Library::operator==(nullptr_t)
{
	return this->Guid.empty();
}

bool ESection_Library::operator!=(nullptr_t)
{
	return !this->Guid.empty();
}

bool EBase::operator==(nullptr_t)
{
	return this->Tag == NULL;
}

bool EBase::operator!=(nullptr_t)
{
	return this->Tag != NULL;
}

ESection_Variable::ESection_Variable()
{
	this->Tag = NULL;
	Attributes = V_None;
	DataType = EDT_VOID;
}

ESection_Variable::ESection_Variable(nullptr_t)
{
	new (this)ESection_Variable();
}

ESection_Program_Method::ESection_Program_Method()
{
	this->Tag = NULL;
	Attributes = M_None;
	ReturnType = EDT_VOID;
}

ESection_Program_Method::ESection_Program_Method(nullptr_t)
{
	new (this)ESection_Program_Method();
}

ESection_Program_Assembly::ESection_Program_Assembly()
{
	this->Tag = NULL;
	Status = C_None;
}

ESection_Program_Assembly::ESection_Program_Assembly(nullptr_t)
{
	new (this)ESection_Program_Assembly();
}

ESection_TagStatus::ESection_TagStatus()
{
	this->Tag = NULL;
	Status = C_None;
}

ESection_Resources_FormElement::ESection_Resources_FormElement()
{
	this->Tag = NULL;
	Left = Top = Width = Height = 0;
	Status = E_None;
}

ESection_Resources_FormElement::ESection_Resources_FormElement(nullptr_t)
{
	new (this)ESection_Resources_FormElement();
}

ESection_TagStatus::ESection_TagStatus(nullptr_t)
{
	new (this)ESection_TagStatus();
}

ESection_Resources_Form::ESection_Resources_Form()
{
	this->Tag = NULL;
}

ESection_Resources_Form::ESection_Resources_Form(nullptr_t)
{
	new (this)ESection_Resources_Form();
}

ESection_Resources_Const::ESection_Resources_Const()
{
	this->Tag = NULL;
	Type = (EConstType)0;
}

ESection_Resources_Const::ESection_Resources_Const(nullptr_t)
{
	new (this)ESection_Resources_Const();
}

ESection_Resources_Data::ESection_Resources_Data()
{
	this->Tag = NULL;
}

ESection_Resources_Data::ESection_Resources_Data(nullptr_t)
{
	new (this)ESection_Resources_Data();
}

ESection_Variable* ESection_Program_Assembly::FindField(ETAG tag)
{
	return FindInfo(this->Variables, tag);
}