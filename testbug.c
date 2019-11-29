/* progam to transfer serial as sw=0 parag and sw=1 jambhulkar use timer 1 in auto reload
  */
#include <reg51.h>
#include<absacc.h>
sbit settime= P3^3;
sbit c_gio= P3^4;
sbit c_phut=P3^5;
sbit L6 = P1^5;
sbit L5 = P1^4;
sbit L4 = P1^3;
sbit L3 = P1^2;
sbit L2 = P1^1;
sbit L1  = P1^0;
char so[10]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
char hr, min, sec;
char hr1, min1,hr0,min0,bmode;
char num[60]={0X00,0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X10,0X11,0X12,0X13,0X14,0X15,0X16,0X17,0X18,0X19,0X20,0X21,0X22,0X23,0X24,0X25,0X26,0X27,0X28,0X29,0X30,0X31,0X32,0X33,0X34,0X35,0X36,0X37,0X38,0X39,0X40,0X41,0X42,0X43,0X44,0X45,0X46,0X47,0X48,0X49,0X50,0X51,0X52,0X53,0X54,0X55,0X56,0X57,0X58,0X59};
char s0,s1,h0,h1,p0,p1;

void delay(unsigned x);
 bcdcon(unsigned char);
void bcdconv2(char mybyte,char *a,char *b);
void serial(unsigned char);
sbit sw = P1^7;
//void time1(unsigned char );
void displayall();
void displayP();
void displayH();
void sethrs();
void setmin();
void set_rtc_time();
void set_rtc_time1();
void set_time() interrupt 2
	{
		bmode++;
		delay(500);
		if (bmode ==3) bmode=0;
		set_rtc_time1();
		
	}
void main()
  	{
			unsigned char x,y,z;
					hr1=0;
					min1=0;
			 /* 	delay(200);
				XBYTE[10]= 0X20;
				XBYTE[11]= 0X83; // BCD, 24hrs, daylight saving
				XBYTE[0]= 0X55;
				XBYTE[2]= 0X58;
				XBYTE[4]= 0X16;
				XBYTE[7]= 0X19;
				XBYTE[8]= 0X10;
				XBYTE[9]= 0X04;
		//		XBYTE[6]= 0X00;
				XBYTE[11]= 0X03; // cpa nhat gio
					*/
					settime=1;
					c_gio=1;
					c_phut=1;
					set_rtc_time();	
					bmode=0;
					TMOD = 0x20;		 //timer 1 in mode 2
					TH1 = 0xFA;
					SCON = 0x50;
					TR1=1;
					IE=0x84;


	while(1)
		{
			if (bmode==0)
				{
						hr = XBYTE[4];
						x=bcdcon(hr);
						serial(':'); 
						delay(10);
						min = XBYTE[2];
						y= bcdcon(min);
						serial(':');   
						delay(10);
						sec = XBYTE[0];
						z= bcdcon(sec);
						delay(10);
						serial(0x0D);
						serial(0x0A);
			
					displayall();	
			}
			//chinh gio phut
			if (bmode==1)
				{
					sethrs();
					setmin();
					displayH();
					displayP();
				}
			
				
	 	
}
	 }
void set_rtc_time1()
	 {
			hr=num[hr0];
		 min=num[min0];
		  delay(200);
		XBYTE[10]= 0X20;
		XBYTE[11]= 0X83; // BCD, 24hrs, daylight saving
		XBYTE[0]= 0X00;
		XBYTE[2]= 0x50;
		XBYTE[4]= 0x21;
		XBYTE[7]= 0X19;
		XBYTE[8]= 0X10;
		XBYTE[9]= 0X04;
//		XBYTE[6]= 0X00;
		XBYTE[11]= 0X03; // cap nhat gio
		}
void set_rtc_time()
	 {
		 
		delay(200);
		XBYTE[10]= 0X20;
		XBYTE[11]= 0X83; // BCD, 24hrs, daylight saving
		XBYTE[0]= 0X55;
		XBYTE[2]= 0X58;
		XBYTE[4]= 0X15;
		XBYTE[7]= 0X19;
		XBYTE[8]= 0X11;
		XBYTE[9]= 0X26;
//		XBYTE[6]= 0X00;
		XBYTE[11]= 0X03; // cap nhat gio
		}
void delay(unsigned int itime)
	{
	unsigned int i,j;
	for(i=0;i<itime;i++)
	for(j=0;j<300;j++);
	}
 bcdcon(unsigned char mydata)
	{
	 unsigned char x,y,z;
	 z= mydata;
	 y = mydata & 0xF0;
	 y = y >> 4;
	 y = y | 0x30;
	 x = z & 0x0F;
	 x = x | 0x30;
	 serial(y);
	 serial(x);
	}
void serial(unsigned char x)
	{
			SBUF = x;
			while(TI==0);
			TI=0;
	}
void bcdconv2(char mybyte,char *a,char *b) // Function to convert data into BCD format.
{
    char x,y;
    x=mybyte&0x0F;
    x=x|0x30;
    y=mybyte&0xF0;
    y=y>>4;
    y=y|0x30;
	y=y-48;
	x=x-48;
	*b=so[y];
	*a=so[x];
}
void sethrs()
{
	if (c_gio==0)
	{
			hr1++;
			delay(100);
			if (hr1>11) hr1=0;
			hr0=hr1;
		
	}
}
void setmin()
{
	if (c_phut==0)
		{
			min1++;
			delay(100);
			if (min>59) min1=0;
			min0=min1;
		}
}
void displayP()
{
		//H
						bcdconv2(hr0,&h0,&h1);
					L2=1;
					P2=h0;
					delay(2);
				  P1=0x00;
					L1=1;
					P2=h1;
					delay(2);
					P1=0x00;
}
void displayH()
{
		//P 
				bcdconv2(min0,&p0,&p1);
				L4=1;
					P2=p0;
					delay(2);
				  P1=0x00;
					L3=1;
					P2=p1;
					delay(2);
					P1=0x00;
}
void displayall()
{
					 bcdconv2(sec,&s0,&s1);
				bcdconv2(min,&p0,&p1);
				bcdconv2(hr,&h0,&h1);
				//S
					L6=1;
					P2=s0;
					delay(2);
				  P1=0x00;
					L5=1;
					P2=s1;
					delay(2);
					P1=0x00;
				//P
				L4=1;
					P2=p0;
					delay(2);
				  P1=0x00;
					L3=1;
					P2=p1;
					delay(2);
					P1=0x00;
					//H
					L2=1;
					P2=h0;
					delay(2);
				  P1=0x00;
					L1=1;
					P2=h1;
					delay(2);
					P1=0x00;
				}