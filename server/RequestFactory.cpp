#include "RequestFactory.h"

RequestFactory::RequestFactory()
{
	m_flag = false;
}

RequestFactory::RequestFactory(RequestMsg * msg)
{
	m_request = msg;
	m_flag = true;
}

RequestFactory::~RequestFactory()
{
}

Codec * RequestFactory::createCodec()
{
	if (m_flag == true)
	{
		//	使用当前的msg生产请求码对象
		return new RequestCodec(m_request);
	}
	else
	{
		//	生成空msg的请求码对象
		return new RequestCodec();
	}
}
