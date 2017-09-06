/*
通信示例 类
谢飞龙
2017-5-9
*/
#ifndef CDevilist_INCLUDES_H_
#define CDevilist_INCLUDES_H_
#include <string>
#include <vector>

#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>
#include <stdlib.h>
#include <string>
#include <locale.h>
#include <sstream>
#include <algorithm>
#include <cmath>
using namespace std;
#define TCPCLIENT_MAX_NUM	10

enum  CONNSTATE { csWAIT, csINIT, csCLOSED, csOPEN };

class CDevilist;
typedef struct _TCPCLIENT_INFO
{
	_TCPCLIENT_INFO() :pManger(NULL), pTCPClient(NULL)
	{

	}
	int					nIdx;
	CONNSTATE			ConnState;
	char				HostIP[16];
	int					HostPort;
	char				AdptName[64];
	void*				pTCPClient;
	int					m_ExitFlag;
	CDevilist			*pManger;
}TCPCLIENT_INFO, *PTCPCLIENT_INFO;
enum TypeOfCOM
{
	TypeOfCOM_UDP_Client = 0,//UDP客户端
	TypeOfCOM_UDP_Server = 1,//UDP服务器
	TypeOfCOM_TCP_Client = 2,//TCP客户端
	TypeOfCOM_TCP_Server = 3,//TCP服务器
	TypeOfCOM_SERIAL_Client = 4,//串口作为客户端
	TypeOfCOM_SERIAL_Server = 5,//串口作为服务器
};
enum TypeOfMSG
{
	TypeOfMSG_Com_NormalMsg = 0,//普通的消息提醒
	TypeOfMSG_Com_SendERR,	//发送失败
	TypeOfMSG_Com_SendOK,//发送成功
	TypeOfMSG_Com_RecvERR,//收到的错误
	TypeOfMSG_Com_RecvOK,//收到的消息

	//以下为TypeOfCOM_TCP_Server 类型时产生的消息类型
	TypeOfMSG_Com_TCPServer_NewClientConnecting,//当作为TCP服务器的时候产生的消息，表示有新的客户端加入
	TypeOfMSG_Com_TCPServer_NewClientDisConnected,//当作为TCP服务器的时候产生的消息，表示有客户端退出

												 //以下为TypeOfCOM_TCP_Client 类型时产生的消息类型
	TypeOfMSG_Com_TCPClient_StatusChanged	//通信状态发送改变
};
//回调函数原型定义
typedef void(*functionTranslateProcessBackNotice2)(TypeOfCOM typeofcom,int Value, string notice, void* userParam);
//字符串分隔通用函数
void mms_tokenize(vector<string> &v, const string& src, string tok,
	bool trim = false, string null_subst = "");


class CDevilist
{
public:
	CDevilist(TypeOfCOM type =TypeOfCOM_UDP_Client);
	~CDevilist();
	
	//公用
	void SetCOMtype(TypeOfCOM type = TypeOfCOM_UDP_Client);
	void Start();						//开始运行
	void Stop();						//停止运行
	void SetIsrunning(bool v = true);//设置运行与否的标志
	bool GetIsrunning();			//是否正在运行
	void InstallBackFunction(functionTranslateProcessBackNotice2 fun, void*param);//回调函数及参数安装
	void SendMsgToBackFuntion(int type,const char*msg);//发送消息给回调函数去处理
	void SetPollTime(long polltime);			//设置轮询间隔时间
	long GetPollTime();							//获得轮询间隔时间
	TypeOfCOM GetCOMtype();//获得通信类型
	
	//模式------>TypeOfCOM_UDP_Server TypeOfCOM_UDP_Client
	void SetServerToClientMSG(string msg);//设置要广播的内容,例如1#继电保护测试仪，默认是"1#测试仪"
	string GetServerToClientMSGString();//获得要广播的内容，这个是在线程中获得发送的内容	
	
	void SetServerToClientCMD(string msg);		//发送命令,例如我要发送一个退出"exit"
	string GetServerToClientCMDString();		//取得要发送的命令,这个是在内部线程中调用，发送完毕会自动清空命令
	
	
	

	//模式------>TypeOfCOM_SERIAL_Client = 4 和 TypeOfCOM_SERIAL_Server = 5,
		void	SetCom(int com,int speed=115200,int flow_ctrl=0,int databits=8,int stopbits=1,char parity='N');
		int		ClosePort();//这个在线程中调用，要关闭只需要执行Stop()函数
		int		WritePort(const char *Buf, int len);//写串口	
	
	//模式------>TypeOfCOM_TCP_Server （3）时的函数
		// 获得当前连接的客户端列表,Ip地址，以;分隔,最后以分号结尾，例如:192.168.1.101;192.168.1.103;192.168.1.105;
		//没有客户端，返回""
		string GetClientList();		
		// 删除一个客户端对象连接 释放资源,这个是线程自动调用，不用调用
		int RemoveClientSocketObject(void* lparam);


   

	//模式----------->TypeOfCOM_TCP_Client（2）
		const char * GetServerIp();//如果返回NULL，没有设置服务器IP地址
								   //添加TCP客户端连接对象，输入参数为TCP服务器的IP和端口
		int AddTCPClientObject(const char* pHostIP, int nHostPort);
		int AddTCPClientObject(const char* pHostIPOld, const char * newip,int newHostPort);
		//删除所有TCP客户端连接对象
		int DeleteAllTCPClient();

		//设置TCP客户端连接对象为csINIT状态
		int OpenOneTcpClient(int Idx);
		//获取TCP客户端连接状态
		int	GetTCPClientState(int Idx);


		//模式----------->TypeOfCOM_TCP_Client（2）TypeOfCOM_TCP_Server （3）
		//设置本地端口号,ServerIp在作为客户端的时候有效
		void SetPort(int port = 6806, const char*Serverip = NULL);//端口默认是UDP 6023,TCP Server:6806 TCP Client:6807
																  // 向指定的客户端发送数据
		int SendData(const char*address, const char * buf, int len);
		void SendDataAll(const char * buf, int len);
		int  GetPort();


	//内部线程用
		int * GetComPointer();						//取得串口或套接字指针
public:
	TCPCLIENT_INFO	        m_TCPClientInfo[TCPCLIENT_MAX_NUM];
	pthread_t				m_thread[TCPCLIENT_MAX_NUM];
	int						m_nTCPClientNum;
private:
	// 按照指定的串口参数打开串口，并创建串口接收线程
	int  OpenPort(int PortNo, int baudrate, char databits, char stopbits, char parity);
	int										m_comfd;//串口通信类
	bool 									m_Isrunning;
	pthread_t 								a_thread;
	pthread_t 								b_thread;
	pthread_t 								c_thread;
	string 									m_ServerToClientMSG;//消息
	string 									m_ServerToClientCMD;//命令
	long									m_PollTime;//微秒
	//回调函数,构造函数中，自动初始化为NULL
	functionTranslateProcessBackNotice2		m_fun;	
	//回调函数的参数,构造函数中，自动初始化为NULL
	void									*m_param;
	TypeOfCOM 								m_type;
	bool _startFinding();
	
	
	int										m_port;
	int										m_LocalPort;
	string									m_ServerIp;
	int m_speed;
	int m_flow_ctrl;
	int m_databits;
	int m_stopbits;
	char m_parity;
	
	
};
#endif
//class polygon {
//protected:
//	double side_length_;
//public:
//	polygon();
//	virtual ~polygon();
//	void set_side_length(double side_length);
//	virtual double area() const;
//};
// the types of the class factories
//typedef CDevilist* create_t(TypeOfCOM type);
//typedef void destroy_t(CDevilist*);

//
//extern "C" CDevilist* create(TypeOfCOM type = TypeOfCOM_UDP_Client);
//extern "C" void destroy(CDevilist* p);