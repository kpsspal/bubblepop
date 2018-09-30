#include "common.h"
#include "resourcemgr.h"

ResourceMgr ResourceMgr::m_inst;
char *ResourceMgr ::WorkOnResource(int resourcei,DWORD &res_size)
{
	HRSRC res = FindResource(NULL, MAKEINTRESOURCE(resourcei), RT_RCDATA);
    if (!res)return NULL;
    HGLOBAL res_handle = res_handle = LoadResource(NULL, res);
    if (!res_handle)return NULL;
	res_size = SizeofResource(NULL, res);
    return (char*)LockResource(res_handle);
}