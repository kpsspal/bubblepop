class ResourceMgr
{
	static ResourceMgr m_inst;
public:
	char *WorkOnResource(int resourcei,DWORD &res_size);
	static ResourceMgr &get(){return m_inst;};
};