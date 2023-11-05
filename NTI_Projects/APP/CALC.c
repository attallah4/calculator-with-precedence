

#include "StdTypes.h"
#include "DIO_Interface.h"
#include "LCD_Interface.h"
#include "Utils.h"
#include "KeyPad_Interface.h"
#include "KeyPad_Cfg.h"
#include "CALC.h"

/****************GVARS*******************************/
u8 k=NO_KEY;
s32 G_NUM;
/****************************************************/
u8 arr_presedence[4]={'*','/','+','-'};
u8 arr_operators[5];
s32 arr_num[20]={0};
u8 num_ArrIndex=0;
u8 arr_operatorsIndex=0;
u8 new_eqn_flag;
s32 res;
u8 cop;
u8 flag_error;
/*****************************************************/
void CALC_Init(void)
{
	DIO_Init();
	LCD_Init();
	KEYPAD_Init();

}

void CALC_Runnable(void)
{
	s32 num=0;
	k=KEYPAD_GetKey();
	if (k!=NO_KEY)
	{
		if (k>='0'&&k<='9')
		{
			
			if (new_eqn_flag==1)
			{
				LCD_Clear();
				new_eqn_flag=0;
			}
			LCD_WriteChar(k);
			num=GET_NUM(k);
			cop=0;
			
		}
		else
		{
			
			
			
			num=GET_NUM('0');
			num/=10;
			LCD_WriteChar(k);
			arr_num[num_ArrIndex]=num;
			num_ArrIndex++;
			G_NUM=0;
			
			
			if (k!='C'&&k!='=')
			{
				
				
				if (new_eqn_flag==1)
				{
					LCD_Clear();
					arr_num[0]=res;
					num_ArrIndex=1;
					LCD_WriteNumber(res);
					LCD_WriteChar(k);
					new_eqn_flag=0;
				}
				arr_operators[arr_operatorsIndex]=k;
				arr_operatorsIndex++;
				
			}
			else if (k=='=')
			{
				
				
				cop=0;

				for (u8 j=0;j<2;j++)
				{
					for (u8 i=0;i<arr_operatorsIndex;i++)
					{
						u8 flag_op_done=0;
						if (j==0&&(arr_operators[i]=='*'))
						{
							arr_num[i]=arr_num[i]*arr_num[i+1];
							
							flag_op_done=1;
						}
						else if (j==0&&arr_operators[i]=='/')
						{
							arr_num[i]=arr_num[i]/arr_num[i+1];
							flag_op_done=1;
						}
						else if (j==1&&arr_operators[i]=='+')
						{
							arr_num[i]=arr_num[i]+arr_num[i+1];
							
							flag_op_done=1;
						}
						else if (j==1&&arr_operators[i]=='-')
						{
							arr_num[i]=arr_num[i]-arr_num[i+1];
							flag_op_done=1;
						}
						
						if (flag_op_done)
						{
							
							for(u8 z=i+1;z<num_ArrIndex;z++)
							{
								arr_num[z]=arr_num[z+1];

								
							}
							for(u8 z=i;z<arr_operatorsIndex;z++)
							{
								arr_operators[z]=arr_operators[z+1];
								
								
							}
							num_ArrIndex--;
							arr_operatorsIndex--;
							i--;
						}
						
						
					}
					
					
				}
				
				for (u8 j=0;j<4;j++)
				{
					for (u8 i=0;i<arr_operatorsIndex;i++)
					{
						u8 flag_op_done=0;
						if (j==0&&(arr_operators[i]==arr_presedence[j]))
						{
							arr_num[i]=arr_num[i]*arr_num[i+1];
							
							flag_op_done=1;
						}
						else if (j==1&&arr_operators[i]==arr_presedence[j])
						{
							arr_num[i]=arr_num[i]/arr_num[i+1];
							flag_op_done=1;
						}
						else if (j==2&&arr_operators[i]==arr_presedence[j])
						{
							arr_num[i]=arr_num[i]+arr_num[i+1];
							
							flag_op_done=1;
						}
						else if (j==3&&arr_operators[i]==arr_presedence[j])
						{
							arr_num[i]=arr_num[i]-arr_num[i+1];
							flag_op_done=1;
						}
						
						if (flag_op_done)
						{
							
							for(u8 z=i+1;z<num_ArrIndex;z++)
							{
								arr_num[z]=arr_num[z+1];

								
							}
							for(u8 z=i;z<arr_operatorsIndex;z++)
							{
								arr_operators[z]=arr_operators[z+1];
								
								
							}
							num_ArrIndex--;
							arr_operatorsIndex--;
						}
						
						
						
					}
					
					
				}
				res=arr_num[0];
				LCD_GoTo(2,0);
				LCD_WriteNumber(arr_num[0]);
				new_eqn_flag=1;
				num_ArrIndex=0;
				arr_operatorsIndex=0;
				
				
			}
			else if (k=='C')
			{
				LCD_Clear();
				num_ArrIndex=0;
				arr_operatorsIndex=0;
			}
			
		}
	}
}



s32 GET_NUM(u8 dig)
{
	u8 L_NUM=dig-'0';
	G_NUM=(G_NUM*10)+L_NUM;
	return G_NUM;
}