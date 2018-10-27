#pragma once
#include "stdafx.h"

using namespace std;
// CPage3 对话框
typedef struct WindowInfo    //窗体信息的内存空间分布
{
	DWORD WindowCount;            //窗口的个数
	vector<HTREEITEM> WindowId;   //窗口的ID,现用来存放窗口的根节点
	vector<DWORD> WindowAddr;     //窗口内存地址,在编译出来的时候这个地址已经失效了
	//vector<WindowPropery> Windows;
}*pWindowInfo;


/*typedef struct ButtonPropery
{
	DWORD ChildWindowId;  //子窗口ID
	BYTE ZeroByte1[20];
	string name;  //按钮名称
	BYTE ZeroByte2[5];
	DWORD left; //左边
	DWORD top;  //顶边
	DWORD width; //宽度
	DWORD height;//高度
	BYTE ZeroByte3[12];
	DWORD Propery;  //3,4??

	DWORD Unknow3;
	string tag;         //标记,如果为空,则占一个NULL字节
	DWORD Unknow4;
	DWORD propery;      //二进制第一位为可视,第二位为禁止,第三位为可停留焦点.1为真,0为假
	DWORD focusorder;   //停留顺序

	BYTE Unknow1[28];
	DWORD style;        //类型,0为通常,1为默认
	DWORD HorzAlign;    //横向对齐方式,0为左边,1为居中,2为右边
	DWORD VertAlign;    //纵向对齐方式,0为顶边,1为居中,2为底边

	DWORD picSize;  //图片资源大小   BYTE pic[picSize];

	DWORD fontSize; //字体资源大小  BYTE font[fontSize]; //字体,如果字体资源大小为0,则此值不存在

	DWORD captionSize;  //BYTE caption[captionSize];  //标题,如果标题长度为0,则此值不存在
};*/

typedef struct WindowPropery  //窗口属性
{
	DWORD WindowDataAddr;		  //窗口编译附加的无用数据?
	DWORD ControlDataAddr;		  //控件编译附加的无用数据?
	DWORD ZeroByte1;			  //0x00000000
	DWORD ZeroByte2;			  //0x00000000
	DWORD ControlCount;			  //控件的个数
	DWORD ControlSize;			  //所有组件占空间总大小
	vector<HTREEITEM> ControlID;   //控件的ID,现用来存放控件的根节点

	DWORD ZeroByte3;			  //0x00000000
	vector<DWORD> Controloffset;  //控件空间大小


	DWORD ChildWindowId;  //子窗口的ID,注:每个窗体的子窗口默认为0x10001 
	vector<string> ControlType;   //控件类型,为方便程序而增加的，原本不存在
	BYTE ZeroByte[26];    //空白字节

	DWORD left; //左边
	DWORD top;  //顶边
	DWORD width; //宽度
	DWORD height; //高度
	BYTE UnknowByte1[12]; //尚不清楚
	DWORD MousePointerSize; //自定义鼠标指针所占字节集的大小,如果是选项中的鼠标指针,则为4
	DWORD MousePointer;  //选项中的鼠标指针特殊值,如果为自定义鼠标指针,则为-1,默认型为0    //BYTE customMousePointer[MousePointerSize]; //自定义鼠标指针字节集,如果为选项中的鼠标指针,则此值不存在
	                     //标准箭头型 0x7F00,十字型0x7F03
						 //文本编辑型 0x7F01,沙漏型0x7F02
						 //箭头及问号型 0x7F8B,箭头及沙漏型0x7F8A
					     //禁止符型0x7F88,四向箭头型0x7F86
						 //北<->东箭头型0x7F83,北<->南箭头型0x7F85
						 //北<->西箭头型0x7F82,西<->东箭头型0x7F84
						 //向上箭头型0x7F04,手型0x7F89
	                     
	
	//string tag;   //标记,如果为空,则占一个NULL字节
	DWORD ZeroByte4;  //为0
	DWORD visible_disable;  //二进制第一位为可视,1为真,0为假;第二位为禁止,1为真,0为假
	BYTE UnknowByte2[40]; //尚不清楚
	DWORD border;   //0为无边框,1为普通可调边框,2为普通固定边框,3为窄标题可调边框,4为窄标题固定边框,5为镜框式可调边框,6为镜框式固定边框
	DWORD BackPicMode;  //0为图片居左上,1为图片平铺,2为图片居中,3为缩放图片
	DWORD BackColor; //0xFF000000代表默认底色,否则为R<<6||G<<4||B<<2||0;

	DWORD MaxBtn; //最大化按钮,1为真,0为假
	DWORD MinBtn; //最小化按钮,1为真,0为假
	DWORD CtrlBtn;  //控制按钮,1为真,0为假
	DWORD pos;   //位置,0为通常,1为居中,2为最小化,3为最大化

	DWORD CanMove;//可否移动,1为真,0为假
	DWORD PlayCount;  //播放次数,0为循环播放,1为仅播放1次,2为不播放
	DWORD EnterToNext; //回车下移焦点,1为真,0为假
	DWORD EscClose;  //Esc键关闭,1为真,0为假

	DWORD F1OpenHelp; //F1键打开帮助,1为真,0为假
	DWORD HelpContext; //帮助标志值
	DWORD ShowInTaskBar; //在任务条中显示,1为真,0为假
	DWORD HitMove; //随意移动,1为真,0为假

	DWORD shape; //外形,取值范围为0-29
	DWORD Topmost; //总在最前,1为真,0为假
	DWORD KeepTitleBarActive;//保持标题条激活,1为真,0为假
	DWORD ClassNameSize; //窗口类名长度                        //BYTE ClassName[ClassNameSize];  //窗口类名,若窗口类名长度为0,则此值不存在

	DWORD BackPicSize;   //底图大小,0表示无底图            //DWORD BackPic[BackPicSize];  底图图片,如果底图大小为0,则此值不存在
	DWORD iconSize;     //图标大小                         //BYTE icon[iconSize]; //图标,如果图标大小为0,则此值不存在
	DWORD BackMusicSize;  //背景音乐大小                   //BYTE BackMusic[BackMusicSize];  //背景音乐,如果背景音乐大小为0,则此值不存在
	DWORD captionSize;   //标题长度                        //BYTE caption[captionSize];  //标题,如果标题长度为0,则此值不存在
	DWORD HelpFileNameSize;  //帮助文件名长度              //BYTE HelpFileName[HelpFileNameSize]; //帮助文件名,如果帮助文件名长度为0,则此值不存在
}*pWindowPropery;



class CPage3 : public CDialog
{
	DECLARE_DYNAMIC(CPage3)

public:
	CPage3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage3();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTreeCtrl m_Tree;
	CImageList m_ICO;
	CStatic ControlType;
	vector<WindowPropery> Window;

	afx_msg void OnTvnItemexpandingTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangingTree1(NMHDR *pNMHDR, LRESULT *pResult);
};
