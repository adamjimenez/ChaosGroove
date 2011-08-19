
#ifndef	DEFKEYTABLE


#define		DEFKEYTABLE		0

enum		EKeyboardLayout	{
K_VK_LEFT=0  ,
K_VK_UP ,
K_VK_DOWN ,
K_VK_RIGHT	,
K_VK_SPACE , 
K_VK_L_SHIFT , 
K_VK_R_SHIFT ,
K_VK_RETURN ,
K_VK_R_CONTROL , 
K_VK_L_CONTROL ,
K_VK_F1 ,
K_VK_F2 ,
K_VK_F3 , K_VK_F4 , K_VK_F5 , K_VK_F6 , K_VK_F7 , K_VK_F8 , K_VK_F9 , K_VK_F10 ,K_VK_F11 , K_VK_F12 ,
K_VK_BACK , K_VK_TAB ,
K_VK_ESCAPE ,
K_VK_A ,
K_VK_B ,
K_VK_C ,
K_VK_D ,
K_VK_E ,
K_VK_F ,
K_VK_G ,
K_VK_H ,
K_VK_I ,
K_VK_J ,
K_VK_K ,
K_VK_L ,
K_VK_M ,
K_VK_N ,
K_VK_O ,
K_VK_P ,
K_VK_Q ,
K_VK_R ,
K_VK_S ,
K_VK_T ,
K_VK_U ,
K_VK_V ,
K_VK_W ,
K_VK_X ,
K_VK_Y ,
K_VK_Z ,
K_VK_0,
K_VK_1,
K_VK_2,
K_VK_3 ,
K_VK_4 ,
K_VK_5 ,
K_VK_6 ,
K_VK_7 ,
K_VK_8 ,
K_VK_9,
K_VK_NUM0 ,K_VK_NUM1 ,K_VK_NUM2 ,K_VK_NUM3 ,K_VK_NUM4 ,K_VK_NUM5 ,K_VK_NUM6 ,K_VK_NUM7 ,K_VK_NUM8 ,K_VK_NUM9  ,
K_VK_MULTIPLY ,
K_VK_ADD ,
K_VK_SUBTRACT ,
K_VK_DECIMAL ,
K_VK_DIVIDE ,
K_VK_CLEAR ,
K_VK_MENU ,
K_VK_LWIN ,
K_VK_RWIN , 
K_VK_NUMLOCK ,
K_VK_SCROLL ,
K_VK_OEM_1,K_VK_OEM_PLUS ,      
K_VK_OEM_COMMA ,  
K_VK_OEM_MINUS   ,  
K_VK_OEM_PERIOD , 
K_VK_OEM_2     ,     
K_VK_OEM_3     ,     
K_VK_OEM_4      ,    
K_VK_OEM_5     ,     
K_VK_OEM_6    ,      
K_VK_OEM_7        , 
K_VK_END , 
K_VK_HOME , 
K_VK_DEL , 
K_VK_INSERT , 
K_VK_SNAPSHOT , 
K_VK_PRIOR ,
K_VK_NEXT ,
K_VK_ERROR 
 };

struct	KeyStroke
{
	EKeyboardLayout	toucheID ;
	unsigned	short		KeyCode ;
};


extern	KeyStroke			KeyboardLayout[] ;




#endif
