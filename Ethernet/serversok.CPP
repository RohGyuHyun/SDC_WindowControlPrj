////////////////////////////////////////////////////////////////////
//SverSok.cpp
//����Ŭ���� �Լ� �ҽ�
#include "stdafx.h"
#include "serversok.h"
//������
CSeverSock::CSeverSock()
{
	CAsyncSocket::CAsyncSocket();
}
//�޼����� ������ ������ ����
void CSeverSock::SetWnd(HWND hwnd)
{
	m_pHwnd=hwnd;
}

//���ο� Ŭ���̾�Ʈ�� ����Ǿ�����
//����Ǵ� �Լ�
void CSeverSock::OnAccept( int nErrorCode )
{
	 TRACE("Errocode = %d",nErrorCode);
	 //Ŭ���̾�Ʈ Ŭ������ m_pChild�� ��������ش�.
	 Accept(m_pChild);
	 //���ο� Ŭ���̾�Ʈ�� ����Ǿ��ٴ�
	 //WM_ACCEPT_SOCKET �޼����� �����մϴ�.
	 SendMessage(m_pHwnd,WM_ACCEPT_SOCKET,0,0);
}
//OnAccept�Լ� ������ ���ο� Ŭ���̾�Ʈ�� �����
//m_pChild ������ �Ѱ��ݴϴ�.
CClientSock* CSeverSock::GetAcceptSocket()
{
	return &m_pChild;
}
/////////////////////////////////////////////////////////////////////////





