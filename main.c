#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "usart3.h" 
#include "led.h" 		 	 
#include "lcd.h"
#include "beep.h"
#include "key.h" 	 
#include "string.h"	
u8 isIDsame(char *a,char *b){
	u8 i =0;
	for(i;i<8;i++){
		if(a[i] != b[i]){
			return 0;
		}
	}
	return 1;
}
void isNetworkOK(u8 yes){ 
	if(yes >0 ){
		POINT_COLOR=GRAYBLUE;
		LCD_ShowString(628,36,144,24,24,"            ");
		LCD_ShowString(628,36,108,24,24,"Connected");
		POINT_COLOR=BLUE;
	}else{
		POINT_COLOR=RED;
		LCD_ShowString(628,36,144,24,24,"            ");
		LCD_ShowString(628,36,144,24,24,"Disconnected");
		POINT_COLOR=BLUE;
	}
}
void get_received_cardid(u8 *USART3_RX_BUF,u8 *received_cardid){
	received_cardid[6] = USART3_RX_BUF[0];
	received_cardid[7] = USART3_RX_BUF[1];
	received_cardid[4] = USART3_RX_BUF[2];
	received_cardid[5] = USART3_RX_BUF[3];
	received_cardid[2] = USART3_RX_BUF[4];
	received_cardid[3] = USART3_RX_BUF[5];
	received_cardid[0] = USART3_RX_BUF[6];
	received_cardid[1] = USART3_RX_BUF[7];
}
void send_packet(u8 *received_cardid,u8 req,u8 *packet,u8 *temp){
	u8 i = 0;
	for(i;i<19;i++){
		temp[i] = packet[i];
	}	
	i = 0;
	for(i;i<8;i++){
		temp[9+i] = received_cardid[i];
	}
	if(req > 0){
		temp[18] = '1';
	}else{
		temp[18] = '0';
	}
	printf("%s",temp);
}
void get_cardid_from_packet(u8 *USART_RX_BUF, u8 *cardid){
	u8 i = 0;
	for(i;i<8;i++){
		cardid[i] = USART_RX_BUF[i+9];
	}
}
void beep_ms(u16 ms){
	BEEP = 1;
	delay_ms(ms);
	BEEP = 0;
}
void id_init(u8 *id){
	u8 i = 0;
	for(i;i<8;i++){
		id[i] = '0';
	}
}
u8 is_admin(char *a){
	u8 admin_cardid1[8] = "35044912";
	u8 admin_cardid2[8] = "3504B863";
	u8 admin_cardid3[8] = "75F3F2D7";
	u8 admin_cardid4[8] = "75F1B83E";
	u8 admin_cardid5[8] = "75F2AE8E";
	u8 admin_cardid6[8] = "A5EFEEC3";
	u8 admin_cardid7[8] = "75F0C8C4";
	u8 admin_cardid8[8] = "75F10292";
	u8 admin_cardid9[8] = "75F39ED0";
	u8 admin_cardid10[8] = "75F0706A";
	u8 admin_cardid11[8] = "75F51149";
	u8 admin_cardid12[8] = "75F485C7";
	u8 admin_cardid13[8] = "75EEDAA1";
	u8 admin_cardid14[8] = "75F3BC9A";
	u8 admin_cardid15[8] = "75EE7164";
	u8 admin_cardid16[8] = "C5FA41B4";
	u8 admin_cardid17[8] = "A5F6E665";
	u8 admin_cardid18[8] = "75EAABBE";
	u8 admin_cardid19[8] = "75F02E3A";
	u8 admin_cardid20[8] = "75EFA6EA";
	u8 admin_cardid21[8] = "75E3A135";
	u8 admin_cardid22[8] = "2B003DEC";
	if(isIDsame(a,admin_cardid1)) return 1;
	if(isIDsame(a,admin_cardid2)) return 1;
	if(isIDsame(a,admin_cardid3)) return 1;
	if(isIDsame(a,admin_cardid4)) return 1;
	if(isIDsame(a,admin_cardid5)) return 1;
	if(isIDsame(a,admin_cardid6)) return 1;
	if(isIDsame(a,admin_cardid7)) return 1;
	if(isIDsame(a,admin_cardid8)) return 1;
	if(isIDsame(a,admin_cardid9)) return 1;
	if(isIDsame(a,admin_cardid10)) return 1;
	if(isIDsame(a,admin_cardid11)) return 1;
	if(isIDsame(a,admin_cardid12)) return 1;
	if(isIDsame(a,admin_cardid13)) return 1;
	if(isIDsame(a,admin_cardid14)) return 1;
	if(isIDsame(a,admin_cardid15)) return 1;
	if(isIDsame(a,admin_cardid16)) return 1;
	if(isIDsame(a,admin_cardid17)) return 1;
	if(isIDsame(a,admin_cardid18)) return 1;
	if(isIDsame(a,admin_cardid19)) return 1;
	if(isIDsame(a,admin_cardid20)) return 1;
	if(isIDsame(a,admin_cardid21)) return 1;
	if(isIDsame(a,admin_cardid22)) return 1;
	return 0;
}
void show_off(){
	LCD_Fill(482,98,800,480,0xEA05);
}
void show_on(){
	LCD_Fill(482,98,800,480,0x8EE5);
}
int main(void){	
	u8 net_restart = 0;
	u8 reclen=0;
    u8 device_id[8]="00010002";	
    u8 req=0;
	u8 heart[10]="00010002,,";
	u32 times=0;
	u8 received_cardid[8] = "00000000";
	u32 run_time;
	u8 packet[19];
	u8 temp[19];
	u8 try_to_close = 0;
	u8 using_cardid[8] = "00000000";
    u32 i=0;
	u8 networkok = 0;
	u8 networkbad = 0;
	Stm32_Clock_Init(9);	
	uart_init(72,115200);	
	delay_init(72);	   	 	
	LED_Init();		  		
	BEEP_Init();         	
	KEY_Init();				
 	LCD_Init();				
 	LCD_Display_Dir(1);		
	POINT_COLOR=BLUE;
	LCD_DrawLine(1,96,800,96);
	LCD_DrawLine(1,97,800,97);
	LCD_DrawLine(480,96,480,480);
	LCD_DrawLine(481,96,481,480);
	LCD_ShowString(520,36,84,24,24,"Network");
	isNetworkOK(0);
	LCD_ShowString(100,36,240,24,24,"Device_id : 00010002");
	LCD_ShowString(100,200,96,24,24,"Card-ID:");
	show_off();
	usart3_init(36,9600);	
	POINT_COLOR=BLUE;
	delay_ms(100);
	USART3_RX_STA=0;
	LED0=!LED0;
	for(i;i<8;i++){
		packet[i] = device_id[i];
	}
	packet[8] = ',';
	packet[17] = ',';
	packet[18] = '1';
	LCD_ShowString(100,276,288,24,24,"                        ");
	LCD_ShowString(100,276,288,24,24,"Welcome! Please log in!");
 	while(1) 
	{		
		delay_ms(10);	   
		if(USART3_RX_STA&0X8000)
		{
			times++;
 			reclen=USART3_RX_STA&0X7FFF;	
		  	USART3_RX_BUF[reclen]=0;	 	
			get_received_cardid(USART3_RX_BUF,received_cardid);
			LCD_ShowString(197,200,96,24,24,received_cardid);
			if(is_admin(received_cardid) == 0 && isIDsame(received_cardid,using_cardid) == 0 && try_to_close == 0 && networkbad == 0){
			}
			if(isIDsame(received_cardid,using_cardid) && try_to_close == 1 && networkbad == 0){
				LCD_ShowString(100,276,288,24,24,"                        ");
				POINT_COLOR=RED;
				LCD_ShowString(100,276,288,24,24,"Are you sure to log out?");
				POINT_COLOR=BLUE;
				try_to_close = 2;
				USART3_RX_STA=0;
				beep_ms(75);
				continue;
			}
			if(isIDsame(received_cardid,using_cardid) && try_to_close == 2 && networkbad == 0){
				//
				req = 0;
				send_packet(received_cardid,req,packet,temp);
				try_to_close = 0;
				USART3_RX_STA=0;
				continue;
			}
			if(is_admin(received_cardid) && try_to_close == 1 && networkbad == 0){
				req = 0;
				send_packet(using_cardid,req,packet,temp);
				try_to_close = 0;
				LED0 = !LED0;
				if(LED0 == 1){
					show_on();
				}else{
					show_off();
				}
				USART3_RX_STA=0;
				continue;
			}
			if( net_restart == 1 && networkbad == 0){
				req = 0;
				send_packet(using_cardid,req,packet,temp);
				try_to_close = 0;
				net_restart = 0;
				show_off();
				LED0 = 0;
				USART3_RX_STA=0;
				continue;
			}
			if(is_admin(received_cardid) && networkbad == 1){
				beep_ms(75);
				LED0 = !LED0;
				if(LED0 == 1){
					show_on();
				}else{
					show_off();
				}
				USART3_RX_STA=0;
				net_restart = 1;
				continue;
			}
			USART3_RX_STA=0;
		}
		if(USART_RX_STA&0X8000){    
			reclen=USART_RX_STA&0X7FFF;
			USART_RX_BUF[reclen]=0;	 	
			if(reclen == 10  ){
				isNetworkOK(1);
				networkok = 0;
				networkbad = 0;
			}else if(reclen > 10){
				if(req == 1 && USART_RX_BUF[18] == '1'){
					show_on();
					get_cardid_from_packet(USART_RX_BUF, using_cardid);
					LCD_ShowString(100,276,288,24,24,"                        ");
					LCD_ShowString(100,276,288,24,24,"    Logged in.");
					LCD_ShowString(100,353,204,24,24,"                 ");
					LED0 = 1;
					try_to_close = 1;
					beep_ms(75);
				}
				if(req == 1 && USART_RX_BUF[18] == '0' && USART_RX_BUF[20] =='i' && USART_RX_BUF[21] =='n'){
					show_on();
					get_cardid_from_packet(USART_RX_BUF, using_cardid);
					LCD_ShowString(100,276,288,24,24,"                        ");
					LCD_ShowString(100,276,288,24,24,"    Logged in.");
					LCD_ShowString(100,353,204,24,24,"Tips:Be In Using.");
					LED0 = 1;
					try_to_close = 1;
					beep_ms(75);
				}
				if(req == 1 && USART_RX_BUF[18] == '0'){
					LCD_ShowString(100,353,204,24,24,"Tips:ErrorCardID.");
				}
				if(req == 0 && USART_RX_BUF[18] == '1'){
					show_off();
					id_init(using_cardid);
					LCD_ShowString(100,276,288,24,24,"                        ");
					LCD_ShowString(100,276,288,24,24,"Welcome! Please log in!");
					LCD_ShowString(100,353,204,24,24,"                 ");
					LED0 = 0;
					beep_ms(75);
				}
			}
			USART_RX_STA=0;
		}
		if(run_time%300 == 0){
			run_time = 0;
			printf("%s",heart);
			if(networkok>0){
				isNetworkOK(0);
				networkbad = 1;
			}
			if(try_to_close == 2){
				LCD_ShowString(100,276,288,24,24,"                        ");
				LCD_ShowString(100,276,288,24,24,"    Logged in.");
				try_to_close = 1;
			}
			networkok++;
		}
		run_time++;	
	}
}
