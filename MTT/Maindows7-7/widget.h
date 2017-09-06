#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "Defpam.h"
#include "thread1.h"
#include <QPushButton>
#include <QListWidgetItem>
#include <QFileSystemWatcher>
#include <exportreport.h>
#include "Import/import.h"
#include "BroadCastingUpdClient.h"

#define conConnectPC			"SUCCECONNECT"		//	有上位机来联机，回复给上位机 联机
#define conSetTestParameter		"SETESTPARAER"		//	实验前接收主站数据
#define conGetLEDBinValue		"LEDBINVALUES"		//	获取开关量的状态
#define conDeviceConfig			"DevicConfige"          //      配置测试仪模式  4V3I
#define conTestStart			"TESTSTARTSTA"		//	开始测试
#define conTestEnd				"TESTENDSTATU"      //	测试结束
#define conTeCurrentSte			"CURRENTTSTEP"		//	当前测试在第几步
#define conGetResult			"SENDTERESULT"		//  上位机主动获取测试结果
#define conAcceptBinaryInfo		"ACCEPTBININF"
#define conOthersError			"OTHERSERRERR"		//  错误的标志
#define conSetDeviceTime		"SetDevicTime"
#define conGetDeviceTime		"GetDevicTime"
#define conDeviceStatus			"DeviceStatus"
#define conUpdateDeviceIP		"UpdateDeviIP"
#define conGetDeviceInfo		"GetDevicInfo"
#define conGPSorBMaStatus		"GPSorBStatus"
#define conXinTiaoBaoWen		"OTHERSXINTIA"		//  错误的标志


namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void init_par();
    //void updateUI();
    void readfunctionfile();  //读功能配置文件
    unsigned int TemplateCrcNum(char *Filename);
    int  CheckAuthrizeFile(char *templatefile);
    int  CheckFile(char *templatefile);
    bool CheckSerailNumber(char *templatefile);  //检查是序列是否匹配
    void alldeselect(int index);
    QString GetSerialNumber();
    int gpiofd;
    CDevilist m_TCPServer;
    void INcallBackExample(TypeOfCOM typeofcom,int Value, string notice);

protected slots:
    void sysinit();
    void moveEvent(QMoveEvent *);
    void sendDSPstate();
  //  void OnMouse();


private slots:

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidget_2_itemClicked(QListWidgetItem *item);
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void slotFileChanged(const QString& path);/*槽函数，处理下面fileWatcher的信号*/







private:
    Ui::Widget *ui;
    thread1 *uartthread;
    QFileSystemWatcher *fileWatcher;
    ExportReport *exportreport;
    Import *import;
    bool bMouse;




};

#endif // WIDGET_H
