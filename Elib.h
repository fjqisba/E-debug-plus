
#ifndef __LIB_H
#define __LIB_H

/*
    版权声明：
    本文件版权为易语言作者吴涛所有，仅授权给第三方用作开发易语言支持库，禁止用于其他任何场合。
*/

// 以下是由系统定义的基本数据类型，不可再更改。

#define		_SDT_NULL		0           // 空白数据类型
#define		_SDT_ALL		MAKELONG (MAKEWORD (0, 0), 0x8000)		// 通用型
	/*  仅用于支持库命令定义其参数或返回值的数据类型，当用于定义库命令参数时，
	_SDT_ALL可以匹配所有数据类型（数组类型必须符合要求）。*/
#define		SDT_BYTE		MAKELONG (MAKEWORD (1, 1), 0x8000)		// 字节
#define		SDT_SHORT		MAKELONG (MAKEWORD (1, 2), 0x8000)		// 短整数
#define		SDT_INT			MAKELONG (MAKEWORD (1, 3), 0x8000)		// 整数
#define		SDT_INT64		MAKELONG (MAKEWORD (1, 4), 0x8000)		// 长整数
#define		SDT_FLOAT		MAKELONG (MAKEWORD (1, 5), 0x8000)		// 小数
#define		SDT_DOUBLE		MAKELONG (MAKEWORD (1, 6), 0x8000)		// 双精度小数
#define		SDT_BOOL		MAKELONG (MAKEWORD (2, 0), 0x8000)		// 逻辑
#define		SDT_DATE_TIME	MAKELONG (MAKEWORD (3, 0), 0x8000)		// 日期时间
#define		SDT_TEXT		MAKELONG (MAKEWORD (4, 0), 0x8000)		// 文本
#define		SDT_BIN			MAKELONG (MAKEWORD (5, 0), 0x8000)		// 字节集
#define		SDT_SUB_PTR		MAKELONG (MAKEWORD (6, 0), 0x8000)		// 记录用户易语言子程序的代码地址

// 用作区分数据类型的类别。
#define	DTM_SYS_DATA_TYPE_MASK		0x80000000
#define	DTM_USER_DATA_TYPE_MASK		0x40000000
#define	DTM_LIB_DATA_TYPE_MASK		0x00000000

// 在数据类型中的数组标志，如果某数据类型值此位置1，则表示为此数据类型的数组。
// 本标志仅用作在运行时为具有AS_RECEIVE_VAR_OR_ARRAY或AS_RECEIVE_ALL_TYPE_DATA
// 标志的库命令参数说明其为是否为数组数据，其他场合均未使用。因此其他地方均
// 可以忽略本标志。
#define	DT_IS_ARY					0x20000000

typedef DWORD DATA_TYPE;
typedef DATA_TYPE* PDATA_TYPE;

typedef struct
{
	LPTSTR		m_szName;				// 参数名称
	LPTSTR		m_szExplain;			// 参数详细解释
	SHORT		m_shtBitmapIndex;		// 参见 CMD_INFO 中的同名成员
	SHORT		m_shtBitmapCount;		// 参见 CMD_INFO 中的同名成员

	DATA_TYPE	m_dtDataType;           // 参数的数据类型

	INT			m_nDefault;
		// 系统基本类型参数的默认指定值（在编译时编译器将自动处理）：
		//     1、数值型：直接为数值（如为小数，只能指定其整数部分，
		//		  如为长整数，只能指定不超过INT限值的部分）；
		//     2、逻辑型：1等于真，0等于假；
		//     3、文本型：本成员此时为LPTSTR指针，指向默认文本串；
		//     4、其它所有类型参数一律无默认指定值。

	#define		AS_HAS_DEFAULT_VALUE				(1 << 0)
			// 本参数有默认值，默认值在m_nDefault中说明，与下标志互斥。
	#define		AS_DEFAULT_VALUE_IS_EMPTY			(1 << 1)
			// 本参数有默认值，默认值为空，与上标志互斥。
    // 下面五个标志同时只能有一个置位。
	#define		AS_RECEIVE_VAR					    (1 << 2)
			// 为本参数提供数据时必须提供非数组型容器。
	#define		AS_RECEIVE_VAR_ARRAY				(1 << 3)
			// 为本参数提供数据时必须提供数组型容器。
	#define		AS_RECEIVE_VAR_OR_ARRAY			    (1 << 4)
			// 为本参数提供数据时必须提供数组或非数组型容器。
	#define		AS_RECEIVE_ARRAY_DATA			    (1 << 5)
			// 为本参数提供数据时必须提供数组型数据。
	#define		AS_RECEIVE_ALL_TYPE_DATA            (1 << 6)
			// 为本参数提供数据时可以提供非数组或数组数据。
	DWORD		m_dwState;
}
ARG_INFO, *PARG_INFO;

struct CMD_INFO
{
	LPTSTR		m_szName;			// 命令中文名称
	LPTSTR		m_szEGName;			// 命令英文名称，可以为空或NULL。

	LPTSTR		m_szExplain;		// 命令详细解释
	SHORT		m_shtCategory;		// 全局命令的所属类别，从1开始，减一后的值为指向LIB_INFO的
                                    // m_szzCategory成员所提供的某个类别字符串的索引。
                                    // 对象成员命令的此值为-1。

	#define		CT_IS_HIDED					(1 << 2)
		//   本命令是否为隐含命令（即不需要由用户直接输入的命令（如循环结束命令）或被废弃
	    // 但为了保持兼容性又要存在的命令）。
	#define		CT_IS_ERROR					(1 << 3)
		// 本命令在本库中不能使用，具有此标志一定隐含，主要用作在不同语言版本的相同库中使用，
		// 即：A命令在A语言版本库中可能需要实现并使用，但在B语言版本库中可能就不需要。如果
		// 程序中使用了具有此标志的命令，则只能支持该程序调入和编译，而不能支持运行。
		// 如具有此标志，可以不实现本命令的执行部分。
	#define		CT_DISABLED_IN_RELEASE_VER	(1 << 4)
		// 具有本标志的命令在易语言系统编译RELEASE版易程序时将被跳过，本类型命令必须无返回值。
	#define		CT_ALLOW_APPEND_NEW_ARG		(1 << 5)
		//   在本命令的参数表的末尾是否可以添加新的参数，新参数等同于参数表中的最后一个参数。
	#define		CT_RETURN_ARRAY_DATA		(1 << 6)
		// 用于说明m_dtRetType，说明是否为返回数组数据。
	WORD		m_wState;

	/*  !!!!! 千万注意：如果返回值类型定义为 _SDT_ALL ，绝对不能返回数组(即CT_RETURN_ARRAY_DATA
		置位)或复合数据类型的数据(即用户或库自定义数据类型但不包含窗口或菜单单元)，
		因为用户程序无法自动删除复合类型中所分配的额外空间(如文本型或者字节集型成员等)。 */
	DATA_TYPE	m_dtRetType;    // 返回值类型。

	WORD		m_wReserved;    // 保留，必须为0。

    // 级别宏，用作为用户提供学习难度说明。
    #define	LVL_SIMPLE			1		// 初级
    #define	LVL_SECONDARY		2		// 中级
    #define	LVL_HIGH			3		// 高级
	SHORT		m_shtUserLevel;	// 命令的用户级别，本成员的值为上面的级别宏。

	SHORT		m_shtBitmapIndex;	// 指定图像索引，从1开始，0表示无。减一后的值为指向支持库中名为
                                    // "LIB_BITMAP"的BITMAP资源中某一部分16X13位图的索引。
	SHORT		m_shtBitmapCount;	// 图像数目(用作为IDE提供动画图片).

	INT			m_nArgCount;		// 命令的参数数目
	PARG_INFO	m_pBeginArgInfo;    // 指向本命令的参数定义信息数组
};
typedef CMD_INFO* PCMD_INFO;

struct LIB_DATA_TYPE_ELEMENT
{
	DATA_TYPE m_dtDataType; // 本数据成员的数据类型。

	LPBYTE m_pArySpec;  // 如果本成员为数组，则本成员提供数组指定串，否则此值为NULL。
        // 数组指定串的格式为：首先为一个BYTE记录该数组的维数（如果为0表示不为数组，最大值为0x7f），
        // 然后为对应数目的INT值顺序记录对应维的元素数目。

	LPTSTR m_szName;    // 本数据成员的名称，如果本数据成员所属的数据类型只有这一个数据成员，此值应该为NULL。
	LPTSTR m_szEGName;  // 本数据成员的英文名称，可以为空或NULL。
	LPTSTR m_szExplain; // 本数据成员的详细说明。

	#define LES_HAS_DEFAULT_VALUE  (1 << 0)    // 本数据成员有默认值，默认值在m_nDefault中说明。
	DWORD m_dwState;

    // 仅当 m_dwState 包含 LES_HAS_DEFAULT_VALUE 标志且非数组时才有效。
	INT m_nDefault;
		// 本数据成员的默认指定值：
		//     1、数值型：直接为数值（如为小数，只能指定其整数部分，
		//		  如为长整数，只能指定不超过INT限值的部分）；
		//     2、逻辑型：1等于真，0等于假；
		//     3、文本型：本变量此时为LPTSTR指针，指向默认文本串；
		//     4、其它所有类型参数一律无默认指定值。
};
typedef LIB_DATA_TYPE_ELEMENT* PLIB_DATA_TYPE_ELEMENT;

// 固定属性的数目
#define	FIXED_WIN_UNIT_PROPERTY_COUNT	8

// 每个固定属性定义
#define	FIXED_WIN_UNIT_PROPERTY	\
	{	_WT("左边"), _WT("left"),	NULL,	UD_INT,	NULL,	NULL	},	\
	{	_WT("顶边"), _WT("top"),	NULL,	UD_INT,	NULL,	NULL	},	\
	{	_WT("宽度"), _WT("width"),	NULL,	UD_INT,	NULL,	NULL	},	\
	{	_WT("高度"), _WT("height"),	NULL,	UD_INT,	NULL,	NULL	},	\
	{	_WT("标记"), _WT("tag"),	NULL,	UD_TEXT,	NULL,	NULL	},	\
	{	_WT("可视"), _WT("visible"),	NULL,	UD_BOOL,	NULL,	NULL	},	\
	{	_WT("禁止"), _WT("disable"),	NULL,	UD_BOOL,	NULL,	NULL	},	\
	{	_WT("鼠标指针"), _WT("MousePointer"),	NULL,	UD_CURSOR,	NULL,	NULL	}

// 用作定义窗口单元属性。
struct UNIT_PROPERTY
{
	LPTSTR m_szName;    // 属性名称，注意为利于在属性表中同时设置多对象的属性，相同意义属性的名称最好一致。
	LPTSTR m_szEGName;  // 英文名称。
	LPTSTR m_szExplain; // 属性解释。

	#define		UD_INT			    1001	// 数据为INT值
	#define		UD_DOUBLE		    1002	// 数据为DOUBLE值
	#define		UD_BOOL			    1003	// 数据为BOOL值
	#define		UD_DATE_TIME	    1004	// 数据为DATE值
	#define		UD_TEXT			    1005	// 数据为字符串
	#define		UD_PICK_INT			1006	// 数据为INT值，用户只能选择，不能编辑。
	#define		UD_PICK_TEXT		1007	// 数据为待选字符串，用户只能选择，不能编辑。
	#define		UD_EDIT_PICK_TEXT	1008	// 数据为待选字符串，用户可以编辑。
	#define		UD_PIC			    1009	// 为图片文件数据
	#define		UD_ICON			    1010	// 为图标文件数据
	#define		UD_CURSOR		    1011
		//   第一个INT记录鼠标指针类型，具体值见 Windows API 的 LoadCursor 函数。
        // 如为-1，则为自定义鼠标指针，此时后跟相应长度的鼠标指针文件内容。
	#define		UD_MUSIC		    1012	// 为声音文件数据
	#define		UD_FONT			    1013    // 为一个LOGFONT数据结构，不能再改。
	#define		UD_COLOR		    1014    // 数据为COLORREF值。
	#define		UD_COLOR_TRANS	    1015
		//   数据为COLORREF值，允许透明颜色(用CLR_DEFAULT代表，CLR_DEFAULT在
        // VC++的COMMCTRL.H头文件中定义)。
	#define		UD_FILE_NAME	    1016
		//   数据为文件名字符串。此时m_szzPickStr中的数据为：
		// 对话框标题 + "\0" + 文件过滤器串 + "\0" + 默认后缀 + "\0" +
		// "1"（取保存文件名）或"0"（取读入文件名） + "\0"
	#define		UD_COLOR_BACK	    1017
		// 数据为COLORREF值，允许系统默认背景颜色(用CLR_DEFAULT代表)。
	#define		UD_ODBC_CONNECT_STR	1021
		// ODBC数据连接文本
	#define		UD_ODBC_SELECT_STR	1022
		// ODBC数据查询SQL文本
	#define		UD_IMAGE_LIST		1023
		// 图片组，数据结构为：
		#define	IMAGE_LIST_DATA_MARK	(MAKELONG ('IM', 'LT'))
		/*
		DWORD: 标志数据：为 IMAGE_LIST_DATA_MARK
		COLORREF: 透明颜色（可以为CLR_DEFAULT）
		后面为图片组数据，用CImageList::Read和CImageList::Write读写。
		*/
	#define		UD_CUSTOMIZE		1024    // 自定义类型属性

	SHORT m_shtType;	// 属性的数据类型（为上面的宏值）。

	#define	UW_HAS_INDENT		(1 << 0)	// 在属性表中显示时向外缩进一段，一般用于子属性。
	#define	UW_GROUP_LINE		(1 << 1)	// 在属性表中本属性下显示分组底封线。
	#define	UW_ONLY_READ		(1 << 2)    // 只读属性，设计时不可用，运行时不能写。
	#define	UW_CANNOT_INIT		(1 << 3)    // 设计时不可用，但运行时可以正常读写。与上标志互斥。
	WORD m_wState;

	LPTSTR m_szzPickStr;
		// 当m_nType为UP_PICK_INT、UP_PICK_TEXT、UD_EDIT_PICK_TEXT、UD_FILE_NAME时不能为NULL。
		// 顺序记录以"\0"分隔的所有备选文本（除开UD_FILE_NAME为所说明的特殊格式），最后以一个"\0"结束。
};
typedef UNIT_PROPERTY* PUNIT_PROPERTY;

typedef struct
{
	LPTSTR		m_szName;       // 参数名称
	LPTSTR		m_szExplain;    // 参数详细解释

	#define EAS_IS_BOOL_ARG     (1 << 0)	// 为逻辑型参数，如无此标志，则认为是整数型参数
	DWORD		m_dwState;
}
EVENT_ARG_INFO, *PEVENT_ARG_INFO;

struct EVENT_INFO
{
	LPTSTR		m_szName;			// 事件名称
	LPTSTR		m_szExplain;		// 事件详细解释

	#define		EV_IS_HIDED			(1 << 0)
    	// 本事件是否为隐含事件（即不能被一般用户所使用或被废弃但为了保持兼容性又要存在的事件）。
	#define		EV_RETURN_INT		(1 << 3)
        // 本事件的处理子程序需要返回一个整数。
	#define		EV_RETURN_BOOL		(1 << 4)
        // 本事件的处理子程序需要返回一个逻辑值，与上标志互斥。
	DWORD		m_dwState;

	INT				m_nArgCount;		// 事件的参数数目
	PEVENT_ARG_INFO	m_pEventArgInfo;	// 事件参数
};
typedef EVENT_INFO* PEVENT_INFO;

////////////////////////////////////

typedef DWORD  HUNIT;

// 通用接口指针。
typedef void (WINAPI *PFN_INTERFACE) ();

// 窗口单元对外接口ID。
#define	ITF_CREATE_UNIT					1	// 创建单元
// 下面两个接口仅在可视化设计窗口界面时使用。
#define	ITF_PROPERTY_UPDATE_UI			2	// 说明属性目前可否被修改
#define	ITF_DLG_INIT_CUSTOMIZE_DATA		3	// 使用对话框设置自定义数据
#define	ITF_NOTIFY_PROPERTY_CHANGED		4	// 通知某属性数据被修改
#define	ITF_GET_ALL_PROPERTY_DATA		5	// 取全部属性数据
#define	ITF_GET_PROPERTY_DATA			6	// 取某属性数据
#define	ITF_IS_NEED_THIS_KEY			8	//   询问单元是否需要指定的按键信息，用作窗口单元
                                        	// 截获处理默认为运行时环境处理的按键，如TAB、
                                            // SHIFT+TAB、UP、DOWN等。

typedef PFN_INTERFACE (WINAPI *PFN_GET_INTERFACE) (INT nInterfaceNO);

//////////////////////////////////// 接口：

// 创建单元，成功时返回HUNIT，失败返回NULL。
typedef HUNIT (WINAPI *PFN_CREATE_UNIT) (
        LPBYTE pAllPropertyData,            //   指向本窗口单元的已有属性数据，由本窗口单元的
                                            // ITF_GET_PROPERTY_DATA接口产生，如果没有数据则为NULL。
        INT nAllPropertyDataSize,           //   提供pAllPropertyData所指向数据的尺寸，如果没有则为0。
		DWORD dwStyle,                      //   预先设置的窗口风格。
        HWND hParentWnd,                    //   父窗口句柄。
        UINT uID,                           //   在父窗口中的ID。
        HMENU hMenu,                        //   未使用。
        INT x, INT y, INT cx, INT cy,       //   指定位置及尺寸。
		DWORD dwWinFormID, DWORD dwUnitID,  //   本窗口单元所在窗口及本身的ID，用作通知到系统。
		HWND hDesignWnd ,                //   如果blInDesignMode为真，则hDesignWnd提供所设计窗口的窗口句柄。
        BOOL blInDesignMode);       //   说明是否被易语言IDE调用以进行可视化设计，运行时为假。

// 如果指定属性目前可以被操作，返回真，否则返回假。
typedef BOOL (WINAPI *PFN_PROPERTY_UPDATE_UI) (
        HUNIT hUnit,            // 由PFN_CREATE_UNIT返回的已创建窗口单元的句柄，下同。
        INT nPropertyIndex);    // 所需要查询属性的索引值，下同。

// 用作设置类型为UD_CUSTOMIZE的单元属性。
// 如果需要重新创建该单元才能修改单元外形，请返回真。
typedef BOOL (WINAPI *PFN_DLG_INIT_CUSTOMIZE_DATA) (
        HUNIT hUnit,
        INT nPropertyIndex,
		BOOL* pblModified ,   //   如果pblModified不为NULL，请在其中返回是否
                                    // 被用户真正修改（便于易语言IDE建立UNDO记录）。
        LPVOID pReserved );   // 保留未用。

// 用作记录某属性的具体属性值。
union UNIT_PROPERTY_VALUE
{
	INT			m_int;			// 对应的属性类别：UD_INT、UD_PICK_INT，下同。
	DOUBLE		m_double;		// UD_DOUBLE
	BOOL		m_bool;			// UD_BOOL
	DATE		m_dtDateTime;	// UD_DATE_TIME
	COLORREF	m_clr;			// UD_COLOR、UD_COLOR_TRANS、UD_COLOR_BACK

	LPTSTR		m_szText;		//   UD_TEXT、UD_PICK_TEXT、UD_EDIT_PICK_TEXT、
			                    // UD_ODBC_CONNECT_STR、UD_ODBC_SELECT_STR

	LPTSTR		m_szFileName;	// UD_FILE_NAME

    // UD_PIC、UD_ICON、UD_CURSOR、UD_MUSIC、UD_FONT、UD_CUSTOMIZE、UD_IMAGE_LIST
	struct
	{
		LPBYTE  m_pData;
		INT     m_nDataSize;
	} m_data;

	UNIT_PROPERTY_VALUE ()
	{
		memset ((LPBYTE)this, 0, sizeof (UNIT_PROPERTY_VALUE));
	}
};
typedef UNIT_PROPERTY_VALUE* PUNIT_PROPERTY_VALUE;

//   通知某属性（非UD_CUSTOMIZE类别属性）数据被用户修改，需要根据该修改相应更改
// 内部数据及外形，如果确实需要重新创建才能修改单元外形，请返回真。
//   注意：必须进行所传入值的合法性校验。
typedef BOOL (WINAPI *PFN_NOTIFY_PROPERTY_CHANGED) (
        HUNIT hUnit,
        INT nPropertyIndex,
		PUNIT_PROPERTY_VALUE pPropertyValue,    // 用作修改的相应属性数据。
        LPTSTR* ppszTipText);    // 目前尚未使用。

// 取某属性数据到pPropertyValue中，成功返回真，否则返回假。
/*  注意：如果在设计时（由调用PFN_CREATE_UNIT时的blInDesignMode参数决定），
pPropertyValue必须返回所存储的值。如果在运行时（blInDesignMode为假），必须
返回实际的当前实时值。
    比如说，编辑框窗口单元的“内容”属性，设计时必须返回内部所保存的值，而
运行时就必须调用 GetWindowText 去实时获取。 */
typedef BOOL (WINAPI *PFN_GET_PROPERTY_DATA) (
        HUNIT hUnit,
        INT nPropertyIndex,
		PUNIT_PROPERTY_VALUE pPropertyValue);   // 用作接收欲读取属性的数据。

//   返回本窗口单元的全部属性数据，由该窗口单元的实现代码自行设计格式将
// 所有属性数据组合到一起。此窗口单元的PFN_CREATE_UNIT接口必须能够正确解
// 读此数据。
typedef HGLOBAL (WINAPI *PFN_GET_ALL_PROPERTY_DATA) (HUNIT hUnit);

// 询问单元是否需要指定的按键信息，如果需要，返回真，否则返回假。
typedef BOOL (WINAPI *PFN_IS_NEED_THIS_KEY) (
        HUNIT hUnit,
        WORD wKey);

////////////////////////////////////

#define	UNIT_BMP_SIZE			24		// 单元标志位图的宽度和高度。
#define	UNIT_BMP_BACK_COLOR		(RGB (192, 192, 192))	// 单元标志位图的背景颜色。

struct LIB_DATA_TYPE_INFO  // 库定义数据类型结构
{
	LPTSTR m_szName;    // 名称
	LPTSTR m_szEGName;  // 英文名称，可为空或NULL。
	LPTSTR m_szExplain; // 详细解释，如无则可为NULL。

	INT m_nCmdCount;        // 本数据类型成员方法的数目（可为0）。
	LPINT m_pnCmdsIndex;	// 顺序记录本类型中所有成员方法命令在支持库命令表中的索引值，可为NULL。

    //   本类型是否为隐含类型（即不能由用户直接用作定义的类型，如被废弃
    // 但为了保持兼容性又要存在的类型）。
	#define		LDT_IS_HIDED				(1 << 0)
    // 本类型在本库中不能使用，具有此标志一定隐含。
    // 即使具有此标志，本类型的类型数据也必须完整定义。
	#define		LDT_IS_ERROR				(1 << 1)
    // 是否为窗口单元，如此标志置位则m_nElementCount必为0，
	#define		LDT_WIN_UNIT				(1 << 6)
    // 是否为容器型窗口单元，如有此标志，LDT_WIN_UNIT必须置位。
	#define		LDT_IS_CONTAINER			(1 << 7)
    // 是否为仅用作提供功能的窗口单元（如时钟），如此标志置位则LDT_WIN_UNIT必置位。
    // 具有此标志的单元在运行时无可视外形。
	#define		LDT_IS_FUNCTION_PROVIDER	(1 << 15)
    // 仅用作窗口单元，如此标志置位则表示此单元不能接收输入焦点，不能TAB键停留。
	#define		LDT_CANNOT_GET_FOCUS		(1 << 16)
    // 仅用作窗口单元，如此标志置位则表示此单元可以接收输入焦点，但默认不停留TAB键，
    // 本标志与上标志互斥。
	#define		LDT_DEFAULT_NO_TABSTOP		(1 << 17)
    // 是否为需要自调整位置或尺寸的条状窗口单元（如工具条、状态条等），对于具有此标志的单元，
    // 所在窗口尺寸改变后易语言运行时环境会自动发送给WU_SIZE_CHANGED消息。
    // 注意：条状窗口单元如果需要自动顶部对齐必须具有 CCS_TOP 窗口风格，如果需要自动底部
    // 对齐必须具有 CCS_BOTTOM 窗口风格。
	#define		LDT_BAR_SHAPE				(1 << 20)
	DWORD   m_dwState;

    ////////////////////////////////////////////
    // 以下成员只有在为窗口单元、菜单时才有效。

	DWORD m_dwUnitBmpID;		// 指定在支持库中的单元图像资源ID（注意不同于上面的图像索引），0为无。
                                // 尺寸必须为 24 X 24 ，背景颜色为 RGB (192, 192, 192) 。

	INT m_nEventCount;  // 本单元的事件数目。
	PEVENT_INFO m_pEventBegin;	// 定义本单元的所有事件。

	INT m_nPropertyCount;
	PUNIT_PROPERTY m_pPropertyBegin;

    // 用作提供本窗口单元的所有接口。
	PFN_GET_INTERFACE m_pfnGetInterface;

    ////////////////////////////////////////////
    // 以下成员只有在不为窗口单元、菜单时才有效。

	// 本数据类型中子成员的数目（可为0）。如为窗口、菜单单元，此变量值必为0。
	INT	m_nElementCount;
	PLIB_DATA_TYPE_ELEMENT m_pElementBegin;  // 指向子成员数组的首地址。
};
typedef LIB_DATA_TYPE_INFO* PLIB_DATA_TYPE_INFO;

/*////////////////////////////////////////////*/

struct LIB_CONST_INFO  // 库常量数据结构
{
	LPTSTR	m_szName;
	LPTSTR	m_szEGName;
	LPTSTR	m_szExplain;

	SHORT	m_shtReserved;  // 必须为 1 。

	#define	CT_NULL			0
	#define	CT_NUM			1	// sample: 3.1415926
	#define	CT_BOOL			2	// sample: 1
	#define	CT_TEXT			3	// sample: "abc"
	SHORT	m_shtType;

	LPTSTR	m_szText;		// CT_TEXT
	DOUBLE	m_dbValue;		// CT_NUM、CT_BOOL
};
typedef LIB_CONST_INFO* PLIB_CONST_INFO;

//////////////////////////////////////////// 常用数据结构。

typedef struct
{
    DWORD m_dwFormID;
    DWORD m_dwUnitID;
}
MUNIT, *PMUNIT;

#pragma pack (push, old_value)   // 保存VC++编译器结构对齐字节数。
#pragma pack (1)    // 设置为以一字节对齐。

struct MDATA_INF
{
    union
    {
        // 注意当对应参数具有AS_RECEIVE_VAR或AS_RECEIVE_VAR_ARRAY或AS_RECEIVE_VAR_OR_ARRAY
        // 标志定义时将使用下面的第二部分。

        // 第一部分。
	    BYTE	      m_byte;         // SDT_BYTE 数据类型的数据，下同。
	    SHORT	      m_short;        // SDT_SHORT
	    INT		      m_int;          // SDT_INT
	    INT64	      m_int64;        // SDT_INT64
	    FLOAT	      m_float;        // SDT_FLOAT
	    DOUBLE	      m_double;       // SDT_DOUBLE
        DATE          m_date;         // SDT_DATE_TIME
        BOOL          m_bool;         // SDT_BOOL

        char*         m_pText;        // SDT_TEXT，经过系统预处理，即使是空文本，此指针值也不会为NULL，以便于处理。
                                      // 指针所指向数据的格式见前面的描述。
                                      // !!!为了避免修改到常量段(m_pText有可能会指向易程序常量段区域)中
                                      // 的数据，只可读取而不可更改其中的内容，下同。

        LPBYTE        m_pBin;         // SDT_BIN，经过系统预处理，即使是空字节集，此指针值也不会为NULL，以便于处理。
                                      // 指针所指向数据的格式见前面的描述。
                                      // !!!只可读取而不可更改其中的内容。

        DWORD         m_dwSubCodeAdr; // SDT_SUB_PTR，为子程序代码地址指针。
        MUNIT         m_unit;         // 窗口单元、菜单数据类型的数据。

        void*         m_pCompoundData;// 复合数据类型数据指针，指针所指向数据的格式见前面的描述。
                                      // !!! 只可读取而不可更改其中的内容。

        void*         m_pAryData;     // 数组数据指针，指针所指向数据的格式见前面的描述。
                                      // 注意如果为文本或字节集数组，则成员数据指针可能为NULL。
                                      // !!! 只可读取而不可更改其中的内容。

        // 第二部分。
        // 为指向变量地址的指针，仅当参数具有AS_RECEIVE_VAR或AS_RECEIVE_VAR_ARRAY或
        // AS_RECEIVE_VAR_OR_ARRAY标志时才被使用。
        BYTE*	m_pByte;            // SDT_BYTE 数据类型变量的地址，下同。
	    SHORT*	m_pShort;           // SDT_SHORT
	    INT*	m_pInt;             // SDT_INT
	    INT64*	m_pInt64;           // SDT_INT64
	    FLOAT*	m_pFloat;           // SDT_FLOAT
	    DOUBLE*	m_pDouble;          // SDT_DOUBLE
        DATE*   m_pDate;            // SDT_DATE_TIME
        BOOL*   m_pBool;            // SDT_BOOL

        char**  m_ppText;           // SDT_TEXT，注意*m_ppText可能为NULL（代表空文本）。
                                    // 写入新值之前必须释放前值，例句：NotifySys (NRS_MFREE, (DWORD)*m_ppText)。
                                    // !!!不可直接更改*m_ppText所指向的内容，只能释放原指针后设置入NULL（空文本）
                                    // 或使用NRS_MALLOC通知分配的新内存地址指针（下同）。

        LPBYTE* m_ppBin;            // SDT_BIN，注意*m_ppBin可能为NULL（代表空字节集）。
                                    // 写入新值之前必须释放前值，例句：NotifySys (NRS_MFREE, (DWORD)*m_ppBin)。
                                    // !!!不可直接更改*m_ppBin所指向的内容，只能释放原指针后设置入NULL（空字节集）
                                    // 或新指针。

        DWORD*  m_pdwSubCodeAdr;    // SDT_SUB_PTR，子程序代码地址变量地址。
        PMUNIT  m_pUnit;            // 窗口单元、菜单数据类型变量地址。

        void**  m_ppCompoundData;   // 复合数据类型变量地址。
                                    // !!!注意写入新值之前必须使用NRS_MFREE通知逐一释放所有成员（即：SDT_TEXT、
                                    // SDT_BIN及复合数据类型成员）及原地址指针。
                                    // !!!不可直接更改*m_ppCompoundData所指向的内容，只能释放原指针后设置入新指针。

        void**  m_ppAryData;        // 数组数据变量地址，注意：
                                    // 1、写入新值之前必须释放原值，例句：NotifySys (NRS_FREE_ARY,
                                    //    m_dtDataType, (DWORD)*m_ppAryData)，注意：此例句只适用于
                                    //    m_dtDataType为系统基本数据类型时的情况，如果为复合数据类型，
                                    //    必须根据其定义信息逐一释放。
                                    // 2、如果为文本或字节集数组，则其中成员的数据指针可能为NULL。
                                    // !!!不可直接更改*m_ppAryData所指向的内容，只能释放原指针后设置入新指针。
    };

    // 1、当用作传递参数数据时，如果该参数具有 AS_RECEIVE_VAR_OR_ARRAY 或
    //    AS_RECEIVE_ALL_TYPE_DATA 标志，且为数组数据，则包含标志 DT_IS_ARY ，
    //    这也是 DT_IS_ARY 标志的唯一使用场合。
    //    DT_IS_ARY 的定义为：
    //      #define	DT_IS_ARY   0x20000000
    // 2、当用作传递参数数据时，如果为空白数据，则为 _SDT_NULL 。
    DATA_TYPE m_dtDataType;
};
typedef MDATA_INF* PMDATA_INF;
// !!! ASSERT (sizeof (MDATA_INF) == sizeof (DWORD) * 3);
// 尺寸必须等于 sizeof (DWORD) * 3 。

#pragma pack (pop, old_value)    // 恢复VC++编译器结构对齐字节数。

//////////////////////////////////////////// 通知用数据结构。

/*/////////////*/
// 支持库可以通知易编辑环境(IDE)或易运行环境(RUNTIME)的码值：

struct MDATA
{
	MDATA ()
	{
		m_pData = NULL;
		m_nDataSize = 0;
	}

	LPBYTE	m_pData;
	INT		m_nDataSize;
};
typedef MDATA* PMDATA;

struct EVENT_NOTIFY
{
	// 记录事件的来源
	DWORD	m_dwFormID;		// 调用ITF_CREATE_UNIT接口所传递过来的所处窗口ID（dwWinFormID参数）
	DWORD	m_dwUnitID;		// 调用ITF_CREATE_UNIT接口所传递过来的窗口单元ID（dwUnitID参数）
	INT		m_nEventIndex;	//   事件索引（在窗口单元定义信息LIB_DATA_TYPE_INFO中m_pPropertyBegin
                            // 成员中的位置）

	INT		m_nArgCount;		// 本事件所传递的参数数目，最多 5 个。
	INT		m_nArgValue [5];	// 记录各参数值，SDT_BOOL 型参数值为 1 或 0。

    //!!! 注意下面两个成员在没有定义返回值的事件中无效，其值可能为任意值。
	BOOL	m_blHasReturnValue; // 用户事件处理子程序处理完毕事件后是否提供了返回值。
	INT		m_nReturnValue; // 用户事件处理子程序处理完毕事件后的返回值，逻辑值用数值 0（假） 和 1（真） 返回。

    /////////////////////////////////////

	EVENT_NOTIFY (DWORD dwFormID, DWORD dwUnitID, INT nEventIndex)
	{
		m_dwFormID = dwFormID;
		m_dwUnitID = dwUnitID;
		m_nEventIndex = nEventIndex;

		m_nArgCount = 0;
		m_blHasReturnValue = FALSE;
		m_nReturnValue = 0;
	}
};
typedef EVENT_NOTIFY* PEVENT_NOTIFY;

/*///////////////////////*/

// NES_ 宏为仅被易编辑环境(IDE)处理的通知。
#define NES_GET_MAIN_HWND			        1
	// 取易编辑环境主窗口的句柄，可以在支持库的AddIn函数中使用。
#define	NES_RUN_FUNC				        2
	// 通知易编辑环境运行指定的功能，返回一个BOOL值。
	// dwParam1为功能号。
	// dwParam2为一个双DWORD数组指针,分别提供功能参数1和2。

// NAS_ 宏为既被易编辑环境又被易运行环境处理的通知。
#define NAS_GET_LIB_DATA_TYPE_INFO          1002
    // 返回指定库定义数据类型的PLIB_DATA_TYPE_INFO定义信息指针。
    // dwParam1为欲获取信息的数据类型DATA_TYPE。
    // 如果该数据类型无效或者不为库定义数据类型，则返回NULL，否则返回PLIB_DATA_TYPE_INFO指针。

// NRS_ 宏为仅能被易运行环境处理的通知。
#define NRS_UNIT_DESTROIED			        2000
	// 通知系统指定的单元已经被销毁。
	// dwParam1为dwFormID
	// dwParam2为dwUnitID
#define NRS_CONVERT_NUM_TO_INT              2001
	// 转换其它数值格式到整数。
	// dwParam1为 PMDATA_INF 指针，其 m_dtDataType 必须为数值型。
    // 返回转换后的整数值。
#define NRS_GET_CMD_LINE_STR			    2002
	// 取当前命令行文本
	// 返回命令行文本指针，有可能为空串。
#define NRS_GET_EXE_PATH_STR                2003
	// 取当前执行文件所处目录名称
	// 返回当前执行文件所处目录文本指针。
#define NRS_GET_EXE_NAME				    2004
	// 取当前执行文件名称
	// 返回当前执行文件名称文本指针。
#define NRS_GET_UNIT_PTR				    2006
	// 取单元对象指针
	// dwParam1为WinForm的ID
	// dwParam2为WinUnit的ID
	// 成功返回有效的单元对象CWnd*指针，失败返回NULL。
#define NRS_GET_AND_CHECK_UNIT_PTR			2007
	// 取单元对象指针
	// dwParam1为WinForm的ID
	// dwParam2为WinUnit的ID
	// 成功返回有效的单元对象CWnd*指针，失败自动报告运行时错误并立即退出程序。
#define NRS_EVENT_NOTIFY				    2008
	// 通知系统产生了事件。
	// dwParam1为PEVENT_NOTIFY指针。
	//   如果返回 0 ，表示此事件已被系统抛弃，否则表示系统已经成功传递此事件到用户
	// 事件处理子程序。
#define NRS_STOP_PROCESS_EVENT_NOTIFY	    2009
	// 通知系统暂停处理事件通知。
#define NRS_CONTINUE_PROCESS_EVENT_NOTIFY	2010
	// 通知系统继续处理事件通知。
#define	NRS_DO_EVENTS			            2018
	// 通知Windows系统处理所有已有事件。
#define NRS_GET_UNIT_DATA_TYPE              2022
	// dwParam1为WinForm的ID
	// dwParam2为WinUnit的ID
	// 成功返回有效的 DATA_TYPE ，失败返回 0 。
#define NRS_FREE_ARY                        2023
    // 释放指定数组数据。
    // dwParam1为该数据的DATA_TYPE，只能为系统基本数据类型。
    // dwParam2为指向该数组数据的指针。
#define NRS_MALLOC                          2024
    // 分配指定空间的内存，所有与易程序交互的内存都必须使用本通知分配。
    //   dwParam1为欲需求内存字节数。
    //   dwParam2如为0，则如果分配失败就自动报告运行时错并退出程序。
    // 如不为0，则如果分配失败就返回NULL。
    //   返回所分配内存的首地址。
#define NRS_MFREE                           2025
    // 释放已分配的指定内存。
    // dwParam1为欲释放内存的首地址。
#define NRS_MREALLOC                        2026
    // 重新分配内存。
    //   dwParam1为欲重新分配内存尺寸的首地址。
    //   dwParam2为欲重新分配的内存字节数。
    // 返回所重新分配内存的首地址，失败自动报告运行时错并退出程序。
#define	NRS_RUNTIME_ERR			            2027
	// 通知系统已经产生运行时错误。
	// dwParam1为char*指针，说明错误文本。
#define	NRS_EXIT_PROGRAM                    2028
	// 通知系统退出用户程序。
	// dwParam1为退出代码，该代码将被返回到操作系统。
#define NRS_GET_PRG_TYPE                    2030
    // 返回当前用户程序的类型，为2（调试版）或3（发布版）。

/*///////////////////////////////////////////////////////////////////*/
// 易编辑环境(IDE)或易运行环境(RUNTIME)可以通知支持库的码值：
#define	NL_SYS_NOTIFY_FUNCTION		1
	//   告知支持库通知系统用的函数指针，在装载支持库前通知，可能有多次，
	// 后通知的值应该覆盖前面所通知的值），忽略返回值。
	//   库可将此函数指针记录下来以便在需要时使用它通知信息到系统。
	//   dwParam1: (PFN_NOTIFY_SYS)


/*///////////////////////////////////////////////////////////////////*/

#define NR_OK		0
#define NR_ERR		-1
typedef INT (WINAPI *PFN_NOTIFY_LIB) (INT nMsg, DWORD dwParam1, DWORD dwParam2);
	// 此函数用作易编辑环境(IDE)或易运行环境(RUNTIME)通知支持库有关事件。
typedef INT (WINAPI *PFN_NOTIFY_SYS) (INT nMsg, DWORD dwParam1, DWORD dwParam2);
	// 此函数用作库通知易编辑环境(IDE)或易运行环境(RUNTIME)有关事件。

/* 所有命令和方法实现函数的原型。
   1、必须是 CDECL 调用方式；
   2、pRetData 用作返回数据；
   3、!!!如果指定库命令返回数据类型不为 _SDT_ALL ，可以
      不填充 pRetData->m_dtDataType，如果为 _SDT_ALL ，则必须填写；
   4、pArgInf 提供参数数据本身，所指向的 MDATA_INF 记录每个输入参数，数目等同于 nArgCount 。*/
typedef void (*PFN_EXECUTE_CMD) (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
// 运行支持库中ADDIN功能的函数
typedef INT (WINAPI *PFN_RUN_ADDIN_FN) (INT nAddInFnIndex);
// 创建库中提供的超级模板程序的函数
typedef INT (WINAPI *PFN_SUPER_TEMPLATE) (INT nTemplateIndex);

////////////////////////////////////////////////////

#define		LIB_FORMAT_VER		20000101	// 库格式号

/*	!!! 注意：支持库文件名的后缀必须固定为.FNx，其中x为一类型字母，目前有意义的后缀有：
	1、“.fne”：	带编辑信息、有运行支持代码的支持库；
	2、“.fnl”：	带编辑信息、无运行支持代码的支持库；
	3、“.fnr”：	不带编辑和声明信息、仅有运行支持代码的支持库；
*/

typedef struct
{
	DWORD				m_dwLibFormatVer;
		// 库格式号，应该等于LIB_FORMAT_VER。

	LPTSTR				m_szGuid;
		// 对应于本库的唯一GUID串，不能为NULL或空，相同库的所有后续版本此串都应相同。
        // 注意此 GUID 文本必须使用专用工具软件(如随本文档附带的guidgen.exe)生成，以防止出现重复。

	INT					m_nMajorVersion;	// 本库的主版本号，必须大于0。
	INT					m_nMinorVersion;	// 本库的次版本号。

	INT					m_nBuildNumber;
		// 构建版本号。
		//   本版本号仅用作区分相同正式版本号的系统软件（譬如仅仅修改了几个 BUG，
		// 不值得升级正式版本的系统软件）。任何公布过给用户使用的版本其构建版本
		// 号都应该不一样。
		//   赋值时应该顺序递增。

	INT					m_nRqSysMajorVer;		// 所需要易语言系统的主版本号，目前应该为 3 。
	INT					m_nRqSysMinorVer;		// 所需要易语言系统的次版本号，目前应该为 0 。
	INT					m_nRqSysKrnlLibMajorVer;	// 所需要的系统核心支持库的主版本号，目前应该为 3 。
	INT					m_nRqSysKrnlLibMinorVer;	// 所需要的系统核心支持库的次版本号，目前应该为 0 。

	LPTSTR				m_szName;		// 库名，不能为NULL或空。
    // 语言类别宏
    #define LT_CHINESE  1
    #define LT_ENGLISH  2
	INT					m_nLanguage;	// 本库所支持的语言，目前应该为 LT_CHINESE 。
	LPTSTR				m_szExplain;	// 有关本库的详细解释

	#define		LBS_FUNC_NO_RUN_CODE		(1 << 2)
		// 本库仅为声明库，没有对应功能的支持代码，因此不能运行。
	#define		LBS_NO_EDIT_INFO			(1 << 3)
		// 本库内无供编辑用的信息（编辑信息主要为：各种名称、解释字符串等），无法被易语言IDE加载。
	DWORD				m_dwState;

//////////////////  有关本库作者的信息。
	LPTSTR				m_szAuthor;
	LPTSTR				m_szZipCode;
	LPTSTR				m_szAddress;
	LPTSTR				m_szPhone;
	LPTSTR				m_szFax;
	LPTSTR				m_szEmail;
	LPTSTR				m_szHomePage;
	LPTSTR				m_szOther;

//////////////////
	INT                 m_nDataTypeCount;	// 本库中自定义数据类型的数目，必须等于m_pDataType所指向数组成员的数目。
	PLIB_DATA_TYPE_INFO m_pDataType;		// 本库中所有自定义数据类型的定义信息。

	INT					m_nCategoryCount;	// 全局命令类别数目，必须等同于下面m_szzCategory成员所实际提供的数目。
	LPTSTR				m_szzCategory;		// 全局命令类别说明表，每项为一字符串，前四位数字表示图象索引号（从1开始，0表示无）。
                                            // 减一后的值为指向支持库中名为"LIB_BITMAP"的BITMAP资源中某一部分16X13位图的索引。

	INT					m_nCmdCount;        // 本库中提供的所有命令(全局命令及对象方法)的数目(如无则为0)。
	PCMD_INFO			m_pBeginCmdInfo;	// 指向所有命令及方法的定义信息数组(如m_nCmdCount为0,则为NULL)。
	PFN_EXECUTE_CMD*    m_pCmdsFunc;		// 指向每个命令的实现代码首地址，(如m_nCmdCount为0,则为NULL)。

	PFN_RUN_ADDIN_FN	m_pfnRunAddInFn;	// 可为NULL，用作为易语言IDE提供附加功能。
	//     有关AddIn功能的说明，两个字符串说明一个功能。第一个为功能名称
	// （限20字符），第二个为功能详细介绍（限60字符），最后由两个空串结束。
	LPTSTR				m_szzAddInFnInfo;

	PFN_NOTIFY_LIB		m_pfnNotify;		// 不能为NULL，提供接收来自易语言IDE或运行环境通知信息的函数。

    // 超级模板暂时保留不用。
	PFN_SUPER_TEMPLATE	m_pfnSuperTemplate;	// 为NULL
	LPTSTR m_szzSuperTemplateInfo;	// 为NULL

	// 本库定义的所有常量。
	INT	m_nLibConstCount;   // 常量数目。
	PLIB_CONST_INFO m_pLibConst;    // 指向常量定义数组。

	LPTSTR m_szzDependFiles;	// 可为NULL
	    // 本库正常运行所需要依赖的其他文件，在制作安装软件时将会自动带上这些文件。
}
LIB_INFO, *PLIB_INFO;

/*////////////////////////////////////////////*/

#define	FUNCNAME_GET_LIB_INFO		"GetNewInf"     // 取本支持库的PLIB_INFO指针的输出函数名称
typedef PLIB_INFO (WINAPI *PFN_GET_LIB_INFO) ();	// GetNewInf的函数原型
typedef INT (*PFN_ADD_IN_FUNC) ();  // m_pfnRunAddInFn的函数原型

/*////////////////////////////////////////////*/

#define LIB_BMP_RESOURCE	"LIB_BITMAP"	    // 支持库中提供的图像资源的名称
#define LIB_BMP_CX			16					// 每一图像资源的宽度
#define LIB_BMP_CY			13					// 每一图像资源的高度
#define LIB_BMP_BKCOLOR		RGB(255, 255, 255)	// 图像资源的底色

//////////////////////////// 运行时所使用的宏

#define	WU_GET_WND_PTR			(WM_APP + 2)	// 用作支持窗口单元事件反馈。
#define	WU_SIZE_CHANGED			(WM_APP + 3)	// 用作在窗口尺寸改变后通知所有条状窗口单元。
#define	WU_PARENT_RELAYOUT_BAR	(WM_APP + 4)	//   用作通知顶层窗口重新布局所有的 bar 单元，
                                                // 通常在 bar 单元改变了自身尺寸后使用。

#endif
