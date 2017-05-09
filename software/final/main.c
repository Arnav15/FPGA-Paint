/*---------------------------------------------------------------------------
  --      main.c                                                    	   --
  --      Christine Chen                                                   --
  --      Ref. DE2-115 Demonstrations by Terasic Technologies Inc.         --
  --      Fall 2014                                                        --
  --                                                                       --
  --      For use with ECE 298 Experiment 7                                --
  --      UIUC ECE Department                                              --
  ---------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>

#include "system.h"
#include "alt_types.h"
#include <unistd.h>  // usleep
#include "sys/alt_irq.h"

#include "Data.h"
#include "cy7c67200.h"
//#include "ImgData.h"

#define pencil 0
#define line 1
#define rect 2
#define circle 3
#define polygon 4
#define eraser 5
#define spray 6
#define fill 7
#define pick 8


//----------------------------------------------------------------------------------------//
//
//                                Main function
//
//----------------------------------------------------------------------------------------//
int maxmin(int no, int max, int min)
{
 if(no>max) {no=max;}
 if(no<min) {no=min;}

 return(no);
}

void delay()
{
    int i,max;
    max = 1000;
    for(i=0;i<max;i++);
}

void initialPutPixel(int x,int y, int color)
{
    int command = 1;
    IOWR(CURRX_BASE,0,x);
    IOWR(CURRY_BASE,0,y);
    IOWR(COLOR_FROM_BASE,0,color);
    IOWR(COMMAND_BASE,0,command);
    IOWR(COMMAND_BASE,0,0);
    //delay();
}

int getPixel(int x, int y)
{
	int command = 0;
	IOWR(CURRX_BASE,0,x);
	IOWR(CURRY_BASE,0,y);
	IOWR(COMMAND_BASE,0,command);
	IOWR(OTG_HPI_W_BASE, 0, 1);
	int val = IORD(COLOR_TO_BASE,0);
	delay();
	usleep(10000);
	IOWR(OTG_HPI_W_BASE, 0, 0);
	printf("%d", val);
	return val;
}
void initialScreen()
{
    unsigned int x,y,i,b,c,j;
    i=0;
    j=0;
    for(y=0;y<480;y++)
    {
        for(x=0;x<640;x++)
        {
           if(background_image[j][1] == 0)
        	   j++;

        	 b= background_image[j][0] << 8;
        	 background_image[j][1] -=1;
        	 if(background_image[j][1] == 0)
        	         	   j++;

        	 b = b | background_image[j][0];

            initialPutPixel(x,y,b);//0xe0e0 + (y<<8 | y));
            i=i+1;
           // printf()
        }
        printf("%d\n", y);
        usleep(10);
    }
}
int linehelper(int x, int y, int color)
{
	int i =0;
		for(i=0; i<1000;i++);
		int but = 0;
		int flag = 0;
		int end_x,end_y;
		while(1)
		{
			but = IORD(BUTTON_BASE, 0);
			if(but == 0)
				flag = 1;
			if(flag == 1 && but == 1)
			{
				end_x = IORD(PX_BASE, 0);
			    end_y = IORD(PY_BASE, 0);

				printf("making rect 1");
				//flag_square = 1;

				Bresenham(x,y,end_x,end_y,color);
				while(IORD(BUTTON_BASE,0));
				return 0;
			}

		}
}
void Bresenham(int x1, int y1, int x2, int y2,int color)
{
            int slope;
            int dx, dy, incE, incNE, d, x, y;
            int flag_dydx = 0, temp1,steep;
            int abs_dx , abs_dy;
            // Reverse lines where x1 > x2

            abs_dx = abs(x2 - x1);
            abs_dy = abs(y2 - y1);
            if(abs_dy > abs_dx)
            {
                steep = 1;
            }
            else
            {
                steep = 0;
            }

            if(steep ==1)
            {

               //swap(x1,y1) and swap(x2,y1)
                temp1 = x1;
                x1 = y1;
                y1 = temp1;

                temp1 = x2;
                x2 = y2;
                y2 = temp1;


            }
             if (x1 > x2)
            {
                //Bresenham(x2, y2, x1, y1);
                temp1 = x1;
                x1 = x2;
                x2 = temp1;

                temp1 = y1;
                y1 = y2;
                y2 = temp1;
                //return;
            }
            dx = x2 - x1;
            dy = y2 - y1;
            // Adjust y-increment for negatively sloped lines
            if (dy < 0)
            {
                slope = -1;
                dy = -dy;
            }
            else
            {
                slope = 1;
            }
            // Bresenham constants
            incE = 2 * dy;
            incNE = 2 * dy - 2 * dx;
            d = 2 * dy - dx;
            y = y1;
            // Blit
            for (x = x1; x <= x2; x++)
            {
               if(steep == 1)
               {
                 initialPutPixel(y,x,color);
               }
               else
               {
            	   initialPutPixel(x,y,color);
               }
                if (d <= 0)
                {
                    d += incE;
                }
                else
                {
                    d += incNE;
                    y += slope;

                }
            }
}

int circlehelper(int x,int y, int color)
{
	int i =0;
		for(i=0; i<1000;i++);
		int but = 0;
		int flag = 0;
		int end_x,end_y,center_x,center_y, rad;
		while(1)
		{
			but = IORD(BUTTON_BASE, 0);
			if(but == 0)
				flag = 1;
			if(flag == 1 && but == 1)
			{
				end_x = IORD(PX_BASE, 0);
			    end_y = IORD(PY_BASE, 0);
			     center_x = (x + end_x)/2;
			     center_y = (y + end_y)/2;
			     rad = sqrt(pow((x-end_x),2)+pow((y-end_y),2))/2;
//			     if (end_x >= x)
//				 {
//					 rad = (int)(end_x - x)/2;
//				 }
//				 else
//				 {
//					 rad = (int)(x - end_x)/2;
//				 }
			     printf("making rect 1");
				//flag_square = 1;

				DrawCircle(center_x,center_y,rad,color);
				while(IORD(BUTTON_BASE,0));
				return 0;
			}

		}
}
void DrawCircle( int x0, int y0, int radius, int color)
{
	int x_plus, y_plus;
	   int f = 1 - radius;
	   int ddF_x = 0;
	   int ddF_y = -2 * radius;
	   int x = 0;
	   int y = radius;

	   printf("\n x0 and y0 : %d , %d", x0,y0);
	   printf("\n radius : %d \n", radius);

	   y_plus = y0 +radius;
	   initialPutPixel(x0, y_plus, color);
	   delay();
	   delay();

	   y_plus = y0 - radius;
	   initialPutPixel(x0, y_plus, color);
	   delay();
	   delay();

	   x_plus = x0 + radius;
	   initialPutPixel(x_plus, y0, color);
	   delay();
	   delay();

	   x_plus = x0 - radius;
	   initialPutPixel(x_plus, y0, color);
	   delay();
	   delay();

	   while(x < y)
	   {
	     if(f >= 0)
	     {
	       y--;
	       ddF_y += 2;
	       f += ddF_y;
	     }
	     x++;
	     ddF_x += 2;
	     f += ddF_x + 1;

	     x_plus = x0 + x;
	     y_plus = y0 + y;
	     initialPutPixel(x_plus, y_plus, color);
	     delay();
	     delay();

	     x_plus = x0 - x;
	     y_plus = y0 + y;
	     initialPutPixel(x_plus, y_plus, color);
	     delay();
	     delay();

	     x_plus = x0 + x;
	     y_plus = y0 - y;
	     initialPutPixel(x_plus, y_plus, color);
	     delay();
	     delay();

	     x_plus = x0 - x;
	     y_plus = y0 - y;
	     initialPutPixel(x_plus, y_plus, color);
	     delay();
	     delay();

	     x_plus = x0 + y;
	     y_plus = y0 + x;
	     initialPutPixel(x_plus, y_plus, color);
	     delay();
	     delay();


	     x_plus = x0 - y;
	     y_plus = y0 + x;
	     initialPutPixel(x_plus, y_plus, color);
	     delay();
	     delay();

	     x_plus = x0 + y;
	     y_plus = y0 - x;
	     initialPutPixel(x_plus, y_plus, color);
	     delay();
	     delay();

	     x_plus = x0 - y;
	     y_plus = y0 - x;
	     initialPutPixel(x_plus, y_plus, color);
	     delay();
	     delay();

	   }
}
int squarehelper ( int x1, int x2, int color)
{
	int i =0;
	for(i=0; i<1000;i++);
	int but = 0;
	int flag = 0;
	int end_x,end_y;
	while(1)
	{
		but = IORD(BUTTON_BASE, 0);
		if(but == 0)
			flag = 1;
		if(flag == 1 && but == 1)
		{
			end_x = IORD(PX_BASE, 0);
		    end_y = IORD(PY_BASE, 0);

			printf("making rect 1");
			//flag_square = 1;

			DrawSquare(x1,x2,end_x,end_y,color);
			while(IORD(BUTTON_BASE,0));
			return 0;
		}

	}


}
void DrawSquare(int x1, int y1, int x2, int y2,int color)
{
            Bresenham(x1,y1,x1,y2,color);
            delay();
            Bresenham(x1,y2,x2,y2,color);
            delay();
            Bresenham(x2,y2,x2,y1,color);
            delay();
            Bresenham(x2,y1,x1,y1,color);
            delay();
}
int polygonhelper(int start_x, int start_y, int color)
{
	int i =0;
		for(i=0; i<1000;i++);
		int but = 0;
		int flag = 0;
		int end_x,end_y;
		int initial_x = start_x;
		int initial_y = start_y;
		while(1)
		{
			but = IORD(BUTTON_BASE, 0);
			if(but == 0)
				flag = 1;
			if(flag == 1 && but == 1)
			{
				end_x = IORD(PX_BASE, 0);
			    end_y = IORD(PY_BASE, 0);

				printf("making rect 1");
				//flag_square = 1;

				Bresenham(initial_x,initial_y,end_x,end_y,color);
				initial_x = end_x;
				initial_x = end_x;

				if((end_x == start_x && end_y == start_y)     ||
                        (end_x == start_x && end_y == start_y+1)   ||
                        (end_x == start_x && end_y == start_y-1)   ||
                        (end_x == start_x+1 && end_y == start_y)   ||
                        (end_x == start_x+1 && end_y == start_y+1) ||
                        (end_x == start_x+1 && end_y == start_y-1) ||
                        (end_x == start_x-1 && end_y == start_y)   ||
                        (end_x == start_x-1 && end_y == start_y+1) ||
                        (end_x == start_x-1 && end_y == start_y-1)
                      )
				{
					Bresenham(start_x, start_y,initial_x,initial_y,color);
					while(IORD(BUTTON_BASE,0));
					return 0;
				}
			}

		}


}
int main(void)
{
	//IO_init();
	IOWR(OTG_HPI_W_BASE, 0 , 0);
	initialScreen();
	int a,b;
	int flag_line = 0, flag_square = 0;
	//pencil
	for(a = 13; a < 35; a++)
								for(b = 30; b < 54; b++)
								{
									initialPutPixel(13, b, 0x0000);
									initialPutPixel(35, b, 0x0000);
									initialPutPixel(a, 30, 0x0000);
									initialPutPixel(a, 54, 0x0000);
								}
	//line
	for(a = 13; a < 35; a++)
								for(b = 58; b < 82; b++)
								{
									initialPutPixel(13, b, 0x0000);
									initialPutPixel(35, b, 0x0000);
									initialPutPixel(a, 58, 0x0000);
									initialPutPixel(a, 82, 0x0000);
								}
	//rect
	for(a = 13; a < 35; a++)
								for(b = 86; b < 110; b++)
								{
									initialPutPixel(13, b, 0x0000);
									initialPutPixel(35, b, 0x0000);
									initialPutPixel(a, 86, 0x0000);
									initialPutPixel(a, 110, 0x0000);
								}
	//circle
	for(a = 13; a < 35; a++)
								for(b = 114; b < 138; b++)
								{
									initialPutPixel(13, b, 0x0000);
									initialPutPixel(35, b, 0x0000);
									initialPutPixel(a, 114, 0x0000);
									initialPutPixel(a, 138, 0x0000);
								}
	//polygon
	for(a = 13; a < 35; a++)
								for(b = 142; b < 166; b++)
								{
									initialPutPixel(13, b, 0x0000);
									initialPutPixel(35, b, 0x0000);
									initialPutPixel(a, 142, 0x0000);
									initialPutPixel(a, 166, 0x0000);
								}
	//eraser
	for(a = 13; a < 35; a++)
								for(b = 170; b < 194; b++)
								{
									initialPutPixel(13, b, 0x0000);
									initialPutPixel(35, b, 0x0000);
									initialPutPixel(a, 170, 0x0000);
									initialPutPixel(a, 194, 0x0000);
								}
	//spray
	for(a = 13; a < 35; a++)
								for(b = 198; b < 222; b++)
								{
									initialPutPixel(13, b, 0x0000);
									initialPutPixel(35, b, 0x0000);
									initialPutPixel(a, 198, 0x0000);
									initialPutPixel(a, 222, 0x0000);
								}
	//fill
	for(a = 13; a < 35; a++)
								for(b = 226; b < 250; b++)
								{
									initialPutPixel(a, b, 0xb6b6);

								}
	//pick
	for(a = 13; a < 35; a++)
								for(b = 250; b < 280; b++)
								{
									initialPutPixel(a, b, 0xb6b6);

								}
	//clear
	for(a = 13; a < 35; a++)
								for(b = 282; b < 306; b++)
								{
									initialPutPixel(13, b, 0x0000);
									initialPutPixel(35, b, 0x0000);
									initialPutPixel(a, 282, 0x0000);
									initialPutPixel(a, 306, 0x0000);
								}


//	while(1)
//	{
//		IO_write(HPI_MAILBOX, 0x000a);
//		printf("[ERROR]:routine mailbox data is %x\n",IO_read(HPI_MAILBOX));
//		UsbWrite(0xc008,0x000f);
//		printf("%x\n",UsbRead(0xc008));
//		usleep(10*10000);
//	}
//	    alt_u16 intStat;
//		alt_u16 usb_ctl_val;
//		static alt_u16 ctl_reg = 0;
//		static alt_u16 no_device = 0;
//		alt_u16 fs_device = 0;
//		int keycode = 0;
//		alt_u8 toggle = 0;
//		alt_u8 data_size;
//		alt_u8 hot_plug_count;
//		alt_u16 code;
//		int button_value;
		alt_u16 px = 320;
		alt_u16 py = 240;
//		signed char  dx = 0;
//		signed char  dy = 0;
		alt_u8  pbutton = 0;
		int cursor_type,ink;
		int start_x, start_y, end_x, end_y;
		cursor_type= 0;
		ink = 0x0000;
		for(a = 12; a < 29; a++)
					for(b = 447; b<463; b++)
					{
						initialPutPixel(a,b,ink);
					}
				for(a = 13; a < 35; a++)
					for(b = 30; b < 54; b++)
					{
						initialPutPixel(13, b, 0xe0e0);
						initialPutPixel(35, b, 0xe0e0);
						initialPutPixel(a, 30, 0xe0e0);
						initialPutPixel(a, 54, 0xe0e0);
					}
	while(1)
	{
		px = IORD(PX_BASE, 0);
		py = IORD(PY_BASE, 0);
		IOWR(OTG_HPI_W_BASE, 0 , 0);
		pbutton = IORD(BUTTON_BASE, 0);


	//	printf("%d\n", flag_square);
		if(pbutton == 1)
		{
//			initialPutPixel(px-1, py-1, 0xffff);
//
//			initialPutPixel(px+1, py-1, 0xffff);
//
//			initialPutPixel(px, py-1, 0xffff);
//
//			initialPutPixel(px-1, py+1, 0xffff);
//
//			initialPutPixel(px+1, py+1, 0xffff);
//
//			initialPutPixel(px, py+1, 0xffff);

			 int i, j1, j2, k;

			// drawing area
			if(px > 47 && px <633 && py > 31 && py < 425)
			{
				//Functionality
				switch(cursor_type)
				{
				case pencil: initialPutPixel(px, py, ink);
							 break;

				case line:// if(flag_line == 1)
						  {
								//printf("FLAG 1");
								start_x = px;
								start_y = py;
								initialPutPixel(px,py,ink);
								//flag_line = 2;
								usleep(5000);
								linehelper(start_x, start_y, ink);
								continue;
						  }
//						  if(flag_line == 2)
//						  {
//							   // printf("FLAG 2");
//							  	end_x = px;
//								end_y = py;
//								flag_line = 1;
//								Bresenham(start_x, start_y, end_x, end_y, ink);
//								//usleep(5000);
//
//						  }
						  break;
				case rect:
						 //if (flag_square == 1)
						 {
							start_x = px;
							start_y = py;
							initialPutPixel(px,py,ink);
							printf("making rect 2");
							flag_square = 2;
							usleep(5000);
							for(a=0;a<1000;a++);
							flag_square = squarehelper(start_x, start_y, ink);

							continue;
						 }
//						 else if(flag_square == 2)
//						 {
//							end_x = px;
//							end_y = py;
//							printf("making rect 1");
//							flag_square = 1;
//
//							DrawSquare(start_x,start_y,end_x,end_y,ink);
//
//						  }
						 break;
				case circle:
							start_x = px;
							start_y = py;
							initialPutPixel(px,py,ink);
							circlehelper(start_x,start_y, ink);

							break;
				case polygon:
//							start_x = px;
//							start_y =py;
//							initialPutPixel(px,py,ink);
//							polygonhelper(start_x, start_y, ink);
					 if(flag_line == 1)
											  {
													//printf("FLAG 1");
													start_x = px;
													start_y = py;
													initialPutPixel(px,py,ink);
													flag_line = 2;
													usleep(5000);
													//flag_line = linehelper(start_x, start_y, ink);
													continue;
											  }
											  if(flag_line == 2)
											  {
												   // printf("FLAG 2");
												  	end_x = px;
													end_y = py;
													//flag_line = 1;
													Bresenham(start_x, start_y, end_x, end_y, ink);
													start_x = end_x;
													start_y = end_y;
													//usleep(5000);

											  }
							break;
				case eraser: initialPutPixel(px,py, 0xffff);
							 break;
				case spray :

							 j1 = py;
							 j2 = py;
							 for(k = 4; k>=0; k--)
							 {
								 for(i = px - k; i<= px + k; i++)
								 {
									 if(rand()/(int)(((unsigned)RAND_MAX +1)/2))
									 {
										 int randx = rand() % 5 ;
										 int randy = rand() % 5 ;
										 initialPutPixel(i + randx, j1+randy, ink);
									 }
									 if(rand()/(int)(((unsigned)RAND_MAX +1)/2))
									 {
										 int randx = rand() % 5 ;
										 int randy = rand() % 5 ;
										 initialPutPixel(i+randx, j2+randy, ink);

									 }

								 }
								 j1--;
								 j2++;
							 }
							 break;
				case fill:
//							if(flag_fillcolor == 1)
//							{
//								start_x = pX;
//								start_y = pY;
//								flag_fillcolor =2;
//								flag_color =1;
//								continue;
//							}
//							else if (flag_fillcolor == 2)
//							{
//								end_x = pX;
//								end_y = pY;
//
//								for(y_limit=start_y; y_limit<end_y; y_limit++)
//								{
//									for(x_limit=start_x; x_limit<end_x; x_limit=x_limit+2)
//									{
//										currentColor = GetPixel(x_limit,y_limit);
//										nextColor = GetPixel(x_limit+2,y_limit);
//										if(currentColor != nextColor)
//										{
//											if (flag_color == 1)
//											{
//												initial_x_point = x_limit+2;
//												flag_color = 2;
//												continue;
//											}
//											else if (flag_color == 2)
//											{
//												final_x_point = x_limit+2;
//												for(m=initial_x_point;m<=final_x_point;m++)
//												{
//													PutPixel(m,y_limit,color);
//												}
//												flag_color = 1;
//												break;
//											}
//										}
//									}
//								}
							break;

				case pick:
							break;


				}
			}

			else
			{
				if((px >= 45 && px <= 59)  && (py >= 457 && py <= 469))
				        {
				            ink = 0xFFFF;
				            for(a = 12; a < 29; a++)
				            			for(b = 447; b<463; b++)
				            			{
				            				initialPutPixel(a,b,ink);
				            			}

				        }
				else if((px >= 62 && px <= 75)  && (py >= 457 && py <= 469))
				{
					ink = 0xB6B6;
					for(a = 12; a < 29; a++)
								for(b = 447; b<463; b++)
								{
									initialPutPixel(a,b,ink);
								}

				}
				else if((px >= 78 && px <= 90)  && (py >= 457 && py <= 469))
				{
					ink = 0xE0E0;
					for(a = 12; a < 29; a++)
								for(b = 447; b<463; b++)
								{
									initialPutPixel(a,b,ink);
								}

				}
				else if((px >= 94 && px <= 107)  && (py >= 457 && py <= 469))
				{
					ink = 0xFCFC;
					for(a = 12; a < 29; a++)
								for(b = 447; b<463; b++)
								{
									initialPutPixel(a,b,ink);
								}

				}
				else if((px >= 109 && px <= 122)  && (py >= 457 && py <= 469))
				{
					ink = 0x1C1C;
					for(a = 12; a < 29; a++)
								for(b = 447; b<463; b++)
								{
									initialPutPixel(a,b,ink);
								}

				}
				else if((px >= 126 && px <= 139)  && (py >= 457 && py <= 469))
				{
					ink = 0x1F1F;
					for(a = 12; a < 29; a++)
								for(b = 447; b<463; b++)
								{
									initialPutPixel(a,b,ink);
								}


				}
				else if((px >= 142 && px <= 155)  && (py >= 457 && py <= 469))
				{
					ink = 0x0303;
					for(a = 12; a < 29; a++)
								for(b = 447; b<463; b++)
								{
									initialPutPixel(a,b,ink);
								}


				}
				else if((px >= 158 && px <= 170)  && (py >= 457 && py <= 469))
				{
					ink = 0xE3E3;
					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}


				}
				else if((px >= 173 && px <= 187)  && (py >= 457 && py <= 469))
				{
					ink = 0xFEFE;
					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}


				}
				else if((px >= 190 && px <= 203)  && (py >= 457 && py <= 469))
				{
					ink = 0x1E1E;
					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}


				}
				else if((px >= 206 && px <= 219)  && (py >= 457 && py <= 469))
				{
					ink = 0x9F9F;

					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}

				}
				else if((px >= 222 && px <= 235)  && (py >= 457 && py <= 469))
				{
					ink = 0x9393;
					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}


				}
				else if((px >= 238 && px <= 251)  && (py >= 457 && py <= 469))
				{
					ink = 0xE2E2;
					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}


				}
				else if((px >= 254 && px <= 267)  && (py >= 457 && py <= 469))
				{
					ink = 0xF1F1;
					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}


				}

				//Top row
				else if((px >= 45 && px <= 59)  && (py >= 441 && py <= 452))
				{
					ink = 0x0000;
					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}


				}
				else if((px >= 62 && px <= 74)  && (py >= 441 && py <= 452))
				{
					ink = 0x9292;
					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}


				}
				else if((px >= 78 && px <= 90)  && (py >= 441 && py <= 452))
				{
					ink = 0x8080;
					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}


				}
				else if((px >= 94 && px <= 107)  && (py >= 441 && py <= 452))
				{
					ink = 0x9090;
					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}


				}
				else if((px >= 109 && px <= 122)  && (py >= 441 && py <= 452))
				{
					ink = 0x1010;
					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}


				}
				else if((px >= 126 && px <= 139)  && (py >= 441 && py <= 452))
				{
					ink = 0x1212;
					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}

				}
				else if((px >= 142 && px <= 155)  && (py >= 441 && py <= 452))
				{
					ink = 0x0202;
					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}


				}
				else if((px >= 158 && px <= 170)  && (py >= 441 && py <= 452))
				{
					ink = 0x8282;
					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}


				}
				else if((px >= 173 && px <= 187)  && (py >= 441 && py <= 452))
				{
					ink = 0x9191;
					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}


				}
				else if((px >= 190 && px <= 203)  && (py >= 441 && py <= 452))
				{
					ink = 0x0909;

					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}

				}
				else if((px >= 206 && px <= 219)  && (py >= 441 && py <= 452))
				{
					ink = 0x1313;
					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}


				}
				else if((px >= 222 && px <= 235)  && (py >= 441 && py <= 452))
				{
					ink = 0x0A0A;
					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}


				}
				else if((px >= 238 && px <= 251)  && (py >= 441 && py <= 452))
				{
					ink = 0x8383;
					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}


				}
				else if((px >= 254 && px <= 267)  && (py >= 441 && py <= 452))
				{
					ink = 0x8888;
					for(a = 12; a < 29; a++)
													for(b = 447; b<463; b++)
													{
														initialPutPixel(a,b,ink);
													}


				}
				//Deselection of tool
				else
				switch(cursor_type)
				{
				case pencil: for(a = 13; a < 35; a++)
							for(b = 30; b < 54; b++)
							{
								initialPutPixel(13, b, 0x0000);
								initialPutPixel(35, b, 0x0000);
								initialPutPixel(a, 30, 0x0000);
								initialPutPixel(a, 54, 0x0000);
							}
						break;
				case line:
							for(a = 13; a < 35; a++)
							for(b = 58; b < 82; b++)
							{
								initialPutPixel(13, b, 0x0000);
								initialPutPixel(35, b, 0x0000);
								initialPutPixel(a, 58, 0x0000);
								initialPutPixel(a, 82, 0x0000);
							}
						break;
				case circle:
							for(a = 13; a < 35; a++)
							for(b = 114; b < 138; b++)
							{
								initialPutPixel(13, b, 0x0000);
								initialPutPixel(35, b, 0x0000);
								initialPutPixel(a, 114, 0x0000);
								initialPutPixel(a, 138, 0x0000);
							}
							break;
				case polygon:
							for(a = 13; a < 35; a++)
							for(b = 142; b < 166; b++)
							{
								initialPutPixel(13, b, 0x0000);
								initialPutPixel(35, b, 0x0000);
								initialPutPixel(a, 142, 0x0000);
								initialPutPixel(a, 166, 0x0000);
							}
							break;
				case eraser: for(a = 13; a < 35; a++)
							for(b = 170; b < 194; b++)
							{
								initialPutPixel(13, b, 0x0000);
								initialPutPixel(35, b, 0x0000);
								initialPutPixel(a, 170, 0x0000);
								initialPutPixel(a, 194, 0x0000);
							}
						break;
				case spray: for(a = 13; a < 35; a++)
							for(b = 198; b < 222; b++)
							{
								initialPutPixel(13, b, 0x0000);
								initialPutPixel(35, b, 0x0000);
								initialPutPixel(a, 198, 0x0000);
								initialPutPixel(a, 222, 0x0000);
							}
						break;
//				case fill:	for(a = 13; a < 35; a++)
//							for(b = 226; b < 250; b++)
//							{
//								initialPutPixel(13, b, 0xe0e0);
//								initialPutPixel(35, b, 0xe0e0);
//								initialPutPixel(a, 226, 0xe0e0);
//								initialPutPixel(a, 250, 0xe0e0);
//							}
//						break;
//				case pick: for(a = 13; a < 35; a++)
//							for(b = 254; b < 278; b++)
//							{
//								initialPutPixel(13, b, 0x0000);
//								initialPutPixel(35, b, 0x0000);
//								initialPutPixel(a, 254, 0x0000);
//								initialPutPixel(a, 278, 0x0000);
//							}
//						break;

				case rect: for(a = 13; a < 35; a++)
								for(b = 86; b < 110; b++)
								{
									initialPutPixel(13, b, 0x0000);
									initialPutPixel(35, b, 0x0000);
									initialPutPixel(a, 86, 0x0000);
									initialPutPixel(a, 110, 0x0000);
								}
							break;

				};



// SELECTION OF TOOL
				//pencil
				if(px > 13 && px <35 && py > 30 && py < 54)

						{

							//ink = 0x0000;
							cursor_type = pencil;
							int a,b;
							for(a = 13; a < 35; a++)
								for(b = 30; b < 54; b++)
								{
									initialPutPixel(13, b, 0xe0e0);
									initialPutPixel(35, b, 0xe0e0);
									initialPutPixel(a, 30, 0xe0e0);
									initialPutPixel(a, 54, 0xe0e0);
								}
						}
				//line
				if(px > 13 && px <35 && py > 58 && py < 82)
				{
					cursor_type = line;
					flag_line = 1;
					int a,b;
							for(a = 13; a < 35; a++)
								for(b = 58; b < 82; b++)
								{
									initialPutPixel(13, b, 0xe0e0);
									initialPutPixel(35, b, 0xe0e0);
									initialPutPixel(a, 58, 0xe0e0);
									initialPutPixel(a, 82, 0xe0e0);
								}
				}
				//rect
				if(px > 13 && px <35 && py > 86 && py < 110)
				{
					cursor_type = rect;
					printf("Selecting rect");
					flag_square = 1;
					int a,b;
							for(a = 13; a < 35; a++)
								for(b = 86; b < 110; b++)
								{
									initialPutPixel(13, b, 0xe0e0);
									initialPutPixel(35, b, 0xe0e0);
									initialPutPixel(a, 86 , 0xe0e0);
									initialPutPixel(a, 110, 0xe0e0);
								}
				}
				//circle
				if(px > 13 && px <35 && py > 114 && py < 138)
				{
					cursor_type = circle;
					for(a = 13; a < 35; a++)
								for(b = 114; b < 138; b++)
								{
									initialPutPixel(13, b, 0xe0e0);
									initialPutPixel(35, b, 0xe0e0);
									initialPutPixel(a, 114, 0xe0e0);
									initialPutPixel(a, 138, 0xe0e0);
								}
				}
				//polygon
				if(px > 13 && px <35 && py > 142 && py < 166)
				{
					cursor_type = polygon;
					flag_line = 1;
					for(a = 13; a < 35; a++)
								for(b = 142; b < 166; b++)
								{
									initialPutPixel(13, b, 0xe0e0);
									initialPutPixel(35, b, 0xe0e0);
									initialPutPixel(a, 142, 0xe0e0);
									initialPutPixel(a, 166, 0xe0e0);
								}
				}
				//eraser
				if(px > 13 && px <35 && py > 170 && py < 194)
				{
					//ink = 0xffff;
					cursor_type = eraser;
					int a,b;
					for(a = 13; a < 35; a++)
						for(b = 170; b < 194; b++)
						{
							initialPutPixel(13, b, 0xe0e0);
							initialPutPixel(35, b, 0xe0e0);
							initialPutPixel(a, 170, 0xe0e0);
							initialPutPixel(a, 194, 0xe0e0);
						}
				}

				//spray
				if(px > 13 && px <35 && py > 198 && py < 222)
				{
					cursor_type = spray;
					int a,b;

					for(a = 13; a < 35; a++)
						for(b = 198; b < 222; b++)
						{
							initialPutPixel(13, b, 0xe0e0);
							initialPutPixel(35, b, 0xe0e0);
							initialPutPixel(a, 198, 0xe0e0);
							initialPutPixel(a, 222, 0xe0e0);
						}
				}
				//fill

//			if(px > 13 && px <35 && py > 226 && py < 250)
//			{
//					for(a = 13; a < 35; a++)
//						for(b = 226; b < 250; b++)
//						{
//							initialPutPixel(13, b, 0xe0e0);
//							initialPutPixel(35, b, 0xe0e0);
//							initialPutPixel(a, 226, 0xe0e0);
//							initialPutPixel(a, 250, 0xe0e0);
//						}
//			}
//				//pick
//				if(px > 13 && px <35 && py > 254 && py < 278)
//				{
//					//ink = 0xffff;
//					//cursor_type = pick;
//					//ink = IORD(COLOR_TO_BASE,0);
//					int a,b;
//					for(a = 13; a < 35; a++)
//						for(b = 254; b < 278; b++)
//						{
//							initialPutPixel(13, b, 0xe0e0);
//							initialPutPixel(35, b, 0xe0e0);
//							initialPutPixel(a, 254, 0xe0e0);
//							initialPutPixel(a, 278, 0xe0e0);
//						}
//					ink = getPixel(px, py);
//					for(a = 12; a < 29; a++)
//						for(b = 447; b<463; b++)
//						{
//							initialPutPixel(a,b,ink);
//						}
//
//				}
				//clear
				if(px > 13 && px <35 && py > 282 && py < 306)
				{

					for(a = 47; a< 633; a++)
						for(b = 31; b<425; b++)
						{
						    initialPutPixel(a,b, 0xffff);
						}
				}
	//
	//			initialPutPixel(px-1, py, 0xffff);
	//
	//			initialPutPixel(px+1, py, 0xffff);
				}
			}
		printf("%d  %d\n", px , py);
		//usleep(20);
	}



	// printf("USB keyboard setup...\n\n");

	// //----------------------------------------SIE1 initial---------------------------------------------------//
	// USB_HOT_PLUG:
	// UsbSoftReset();

	// // STEP 1a:
	// UsbWrite (HPI_SIE1_MSG_ADR, 0);
	// UsbWrite (HOST1_STAT_REG, 0xFFFF);

	// /* Set HUSB_pEOT time */
	// UsbWrite(HUSB_pEOT, 600); // adjust the according to your USB device speed

	// usb_ctl_val = SOFEOP1_TO_CPU_EN | RESUME1_TO_HPI_EN;// | SOFEOP1_TO_HPI_EN;
	// UsbWrite(HPI_IRQ_ROUTING_REG, usb_ctl_val);

	// intStat = A_CHG_IRQ_EN | SOF_EOP_IRQ_EN ;
	// UsbWrite(HOST1_IRQ_EN_REG, intStat);
	// // STEP 1a end

	// // STEP 1b begin
	// UsbWrite(COMM_R0,0x0000);//reset time
	// UsbWrite(COMM_R1,0x0000);  //port number
	// UsbWrite(COMM_R2,0x0000);  //r1
	// UsbWrite(COMM_R3,0x0000);  //r1
	// UsbWrite(COMM_R4,0x0000);  //r1
	// UsbWrite(COMM_R5,0x0000);  //r1
	// UsbWrite(COMM_R6,0x0000);  //r1
	// UsbWrite(COMM_R7,0x0000);  //r1
	// UsbWrite(COMM_R8,0x0000);  //r1
	// UsbWrite(COMM_R9,0x0000);  //r1
	// UsbWrite(COMM_R10,0x0000);  //r1
	// UsbWrite(COMM_R11,0x0000);  //r1
	// UsbWrite(COMM_R12,0x0000);  //r1
	// UsbWrite(COMM_R13,0x0000);  //r1
	// UsbWrite(COMM_INT_NUM,HUSB_SIE1_INIT_INT); //HUSB_SIE1_INIT_INT
	// IO_write(HPI_MAILBOX,COMM_EXEC_INT);

	// while (!(IO_read(HPI_STATUS) & 0xFFFF) )  //read sie1 msg register
	// {
	// }
	// while (IO_read(HPI_MAILBOX) != COMM_ACK)
	// {
	// 	printf("[ERROR]:routine mailbox data is %x\n",IO_read(HPI_MAILBOX));
	// 	goto USB_HOT_PLUG;
	// }
	// // STEP 1b end

	// printf("STEP 1 Complete");
	// // STEP 2 begin
	// UsbWrite(COMM_INT_NUM,HUSB_RESET_INT); //husb reset
	// UsbWrite(COMM_R0,0x003c);//reset time
	// UsbWrite(COMM_R1,0x0000);  //port number
	// UsbWrite(COMM_R2,0x0000);  //r1
	// UsbWrite(COMM_R3,0x0000);  //r1
	// UsbWrite(COMM_R4,0x0000);  //r1
	// UsbWrite(COMM_R5,0x0000);  //r1
	// UsbWrite(COMM_R6,0x0000);  //r1
	// UsbWrite(COMM_R7,0x0000);  //r1
	// UsbWrite(COMM_R8,0x0000);  //r1
	// UsbWrite(COMM_R9,0x0000);  //r1
	// UsbWrite(COMM_R10,0x0000);  //r1
	// UsbWrite(COMM_R11,0x0000);  //r1
	// UsbWrite(COMM_R12,0x0000);  //r1
	// UsbWrite(COMM_R13,0x0000);  //r1

	// IO_write(HPI_MAILBOX,COMM_EXEC_INT);

	// while (IO_read(HPI_MAILBOX) != COMM_ACK)
	// {
	// 	printf("[ERROR]:routine mailbox data is %x\n",IO_read(HPI_MAILBOX));
	// 	goto USB_HOT_PLUG;
	// }
	// // STEP 2 end

	// ctl_reg = USB1_CTL_REG;
	// no_device = (A_DP_STAT | A_DM_STAT);
	// fs_device = A_DP_STAT;
	// usb_ctl_val = UsbRead(ctl_reg);

	// if (!(usb_ctl_val & no_device))
	// {
	// 	for(hot_plug_count = 0 ; hot_plug_count < 5 ; hot_plug_count++)
	// 	{
	// 		usleep(5*1000);
	// 		usb_ctl_val = UsbRead(ctl_reg);
	// 		if(usb_ctl_val & no_device) break;
	// 	}
	// 	if(!(usb_ctl_val & no_device))
	// 	{
	// 		printf("\n[INFO]: no device is present in SIE1!\n");
	// 		printf("[INFO]: please insert a USB keyboard in SIE1!\n");
	// 		while (!(usb_ctl_val & no_device))
	// 		{
	// 			usb_ctl_val = UsbRead(ctl_reg);
	// 			if(usb_ctl_val & no_device)
	// 				goto USB_HOT_PLUG;

	// 			usleep(2000);
	// 		}
	// 	}
	// }
	// else
	// {
	// 	/* check for low speed or full speed by reading D+ and D- lines */
	// 	if (usb_ctl_val & fs_device)
	// 	{
	// 		printf("[INFO]: full speed device\n");
	// 	}
	// 	else
	// 	{
	// 		printf("[INFO]: low speed device\n");
	// 	}
	// }



	// // STEP 3 begin
	// //------------------------------------------------------set address -----------------------------------------------------------------
	// UsbSetAddress();

	// while (!(IO_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG) )  //read sie1 msg register
	// {
	// 	UsbSetAddress();
	// 	usleep(10*1000);
	// }

	// UsbWaitTDListDone();

	// IO_write(HPI_ADDR,0x0506); // i
	// printf("[ENUM PROCESS]:step 3 TD Status Byte is %x\n",IO_read(HPI_DATA));

	// IO_write(HPI_ADDR,0x0508); // n
	// usb_ctl_val = IO_read(HPI_DATA);
	// printf("[ENUM PROCESS]:step 3 TD Control Byte is %x\n",usb_ctl_val);
	// while (usb_ctl_val != 0x03) // retries occurred
	// {
	// 	usb_ctl_val = UsbGetRetryCnt();

	// 	goto USB_HOT_PLUG;
	// }

	// printf("------------[ENUM PROCESS]:set address done!---------------\n");

	// // STEP 4 begin
	// //-------------------------------get device descriptor-1 -----------------------------------//
	// // TASK: Call the appropriate function for this step.
	// UsbGetDeviceDesc1(); 	// Get Device Descriptor -1

	// //usleep(10*1000);
	// while (!(IO_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG) )  //read sie1 msg register
	// {
	// 	// TASK: Call the appropriate function again if it wasn't processed successfully.
	// 	UsbGetDeviceDesc1();
	// 	usleep(10*1000);
	// }

	// UsbWaitTDListDone();

	// IO_write(HPI_ADDR,0x0506);
	// printf("[ENUM PROCESS]:step 4 TD Status Byte is %x\n",IO_read(HPI_DATA));

	// IO_write(HPI_ADDR,0x0508);
	// usb_ctl_val = IO_read(HPI_DATA);
	// printf("[ENUM PROCESS]:step 4 TD Control Byte is %x\n",usb_ctl_val);
	// while (usb_ctl_val != 0x03)
	// {
	// 	usb_ctl_val = UsbGetRetryCnt();
	// }

	// printf("---------------[ENUM PROCESS]:get device descriptor-1 done!-----------------\n");


	// //--------------------------------get device descriptor-2---------------------------------------------//
	// //get device descriptor
	// // TASK: Call the appropriate function for this step.
	// UsbGetDeviceDesc2(); 	// Get Device Descriptor -2

	// //if no message
	// while (!(IO_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG) )  //read sie1 msg register
	// {
	// 	//resend the get device descriptor
	// 	//get device descriptor
	// 	// TASK: Call the appropriate function again if it wasn't processed successfully.
	// 	UsbGetDeviceDesc2();
	// 	usleep(10*1000);
	// }

	// UsbWaitTDListDone();

	// IO_write(HPI_ADDR,0x0506);
	// printf("[ENUM PROCESS]:step 4 TD Status Byte is %x\n",IO_read(HPI_DATA));

	// IO_write(HPI_ADDR,0x0508);
	// usb_ctl_val = IO_read(HPI_DATA);
	// printf("[ENUM PROCESS]:step 4 TD Control Byte is %x\n",usb_ctl_val);
	// while (usb_ctl_val != 0x03)
	// {
	// 	usb_ctl_val = UsbGetRetryCnt();
	// }

	// printf("------------[ENUM PROCESS]:get device descriptor-2 done!--------------\n");


	// // STEP 5 begin
	// //-----------------------------------get configuration descriptor -1 ----------------------------------//
	// // TASK: Call the appropriate function for this step.
	// UsbGetConfigDesc1(); 	// Get Configuration Descriptor -1

	// //if no message
	// while (!(IO_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG) )  //read sie1 msg register
	// {
	// 	//resend the get device descriptor
	// 	//get device descriptor

	// 	// TASK: Call the appropriate function again if it wasn't processed successfully.
	// 	UsbGetConfigDesc1();
	// 	usleep(10*1000);
	// }

	// UsbWaitTDListDone();

	// IO_write(HPI_ADDR,0x0506);
	// printf("[ENUM PROCESS]:step 5 TD Status Byte is %x\n",IO_read(HPI_DATA));

	// IO_write(HPI_ADDR,0x0508);
	// usb_ctl_val = IO_read(HPI_DATA);
	// printf("[ENUM PROCESS]:step 5 TD Control Byte is %x\n",usb_ctl_val);
	// while (usb_ctl_val != 0x03)
	// {
	// 	usb_ctl_val = UsbGetRetryCnt();
	// }
	// printf("------------[ENUM PROCESS]:get configuration descriptor-1 pass------------\n");

	// // STEP 6 begin
	// //-----------------------------------get configuration descriptor-2------------------------------------//
	// //get device descriptor
	// // TASK: Call the appropriate function for this step.
	// UsbGetConfigDesc2(); 	// Get Configuration Descriptor -2

	// usleep(100*1000);
	// //if no message
	// while (!(IO_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG) )  //read sie1 msg register
	// {
	// 	// TASK: Call the appropriate function again if it wasn't processed successfully.
	// 	UsbGetConfigDesc2();
	// 	usleep(10*1000);
	// }

	// UsbWaitTDListDone();

	// IO_write(HPI_ADDR,0x0506);
	// printf("[ENUM PROCESS]:step 6 TD Status Byte is %x\n",IO_read(HPI_DATA));

	// IO_write(HPI_ADDR,0x0508);
	// usb_ctl_val = IO_read(HPI_DATA);
	// printf("[ENUM PROCESS]:step 6 TD Control Byte is %x\n",usb_ctl_val);
	// while (usb_ctl_val != 0x03)
	// {
	// 	usb_ctl_val = UsbGetRetryCnt();
	// }


	// printf("-----------[ENUM PROCESS]:get configuration descriptor-2 done!------------\n");


	// // ---------------------------------get device info---------------------------------------------//

	// // // TASK: Write the address to read from the memory for byte 7 of the interface descriptor to HPI_ADDR.
	// // IO_write(HPI_ADDR,0x056c);
	// // code = IO_read(HPI_DATA);
	// // code = code & 0x003;
	// // printf("\ncode = %x\n", code);

	// // if (code == 0x01)
	// // {
	// // 	printf("\n[INFO]:check TD rec data7 \n[INFO]:Keyboard Detected!!!\n\n");
	// // }
	// // else
	// // {
	// // 	printf("\n[INFO]:Keyboard Not Detected!!! \n\n");
	// // }

	// // // TASK: Write the address to read from the memory for the endpoint descriptor to HPI_ADDR.

	// // IO_write(HPI_ADDR,0x0576);
	// // IO_write(HPI_DATA,0x073F);
	// // IO_write(HPI_DATA,0x8105);
	// // IO_write(HPI_DATA,0x0003);
	// // IO_write(HPI_DATA,0x0008);
	// // IO_write(HPI_DATA,0xAC0A);
	// // UsbWrite(HUSB_SIE1_pCurrentTDPtr,0x0576); //HUSB_SIE1_pCurrentTDPtr

	// // //data_size = (IO_read(HPI_DATA)>>8)&0x0ff;
	// // //data_size = 0x08;//(IO_read(HPI_DATA))&0x0ff;
	// // //UsbPrintMem();
	// // IO_write(HPI_ADDR,0x057c);
	// // data_size = (IO_read(HPI_DATA))&0x0ff;
	// // printf("[ENUM PROCESS]:data packet size is %d\n",data_size);

	//  IO_write(HPI_ADDR,0x056c); //the start address

 //    if(  ( IO_read(HPI_DATA) & 0x0ff)  ==0x02)
 //    {
 //         printf("\n[INFO]:check TD rec data7 \n[INFO]:Mouse Detected!!!\n\n");
 //    }
 //    else
 //    {
 //        printf("\n[INFO]:Mouse Not Detected!!! \n\n");
 //    }

 //    IO_write(HPI_ADDR,0x057a); //the start address

 //    data_size = (IO_read(HPI_DATA)>>8)&0x0ff;
 //    printf("[ENUM PROCESS]:data packet size is %d\n",data_size);

	// // STEP 7 begin
	// //------------------------------------set configuration -----------------------------------------//
	// // TASK: Call the appropriate function for this step.
	// UsbSetConfig();		// Set Configuration

	// while (!(IO_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG) )  //read sie1 msg register
	// {
	// 	// TASK: Call the appropriate function again if it wasn't processed successfully.
	// 	UsbSetConfig();		// Set Configuration
	// 	usleep(10*1000);
	// }

	// UsbWaitTDListDone();

	// IO_write(HPI_ADDR,0x0506);
	// printf("[ENUM PROCESS]:step 7 TD Status Byte is %x\n",IO_read(HPI_DATA));

	// IO_write(HPI_ADDR,0x0508);
	// usb_ctl_val = IO_read(HPI_DATA);
	// printf("[ENUM PROCESS]:step 7 TD Control Byte is %x\n",usb_ctl_val);
	// while (usb_ctl_val != 0x03)
	// {
	// 	usb_ctl_val = UsbGetRetryCnt();
	// }

	// printf("------------[ENUM PROCESS]:set configuration done!-------------------\n");

	// //----------------------------------------------class request out ------------------------------------------//
	// // TASK: Call the appropriate function for this step.
	// UsbClassRequest();

	// while (!(IO_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG) )  //read sie1 msg register
	// {
	// 	// TASK: Call the appropriate function again if it wasn't processed successfully.
	// 	UsbClassRequest();
	// 	usleep(10*1000);
	// }

	// UsbWaitTDListDone();

	// IO_write(HPI_ADDR,0x0506);
	// printf("[ENUM PROCESS]:step 8 TD Status Byte is %x\n",IO_read(HPI_DATA));

	// IO_write(HPI_ADDR,0x0508);
	// usb_ctl_val = IO_read(HPI_DATA);
	// printf("[ENUM PROCESS]:step 8 TD Control Byte is %x\n",usb_ctl_val);
	// while (usb_ctl_val != 0x03)
	// {
	// 	usb_ctl_val = UsbGetRetryCnt();
	// }


	// printf("------------[ENUM PROCESS]:class request out done!-------------------\n");

	// // STEP 8 begin
	// //----------------------------------get descriptor(class 0x21 = HID) request out --------------------------------//
	// // TASK: Call the appropriate function for this step.
	// UsbGetHidDesc();

	// while (!(IO_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG) )  //read sie1 msg register
	// {
	// 	// TASK: Call the appropriate function again if it wasn't processed successfully.
	// 	UsbGetHidDesc();
	// 	usleep(10*1000);
	// }

	// UsbWaitTDListDone();

	// IO_write(HPI_ADDR,0x0506);
	// printf("[ENUM PROCESS]:step 8 TD Status Byte is %x\n",IO_read(HPI_DATA));

	// IO_write(HPI_ADDR,0x0508);
	// usb_ctl_val = IO_read(HPI_DATA);
	// printf("[ENUM PROCESS]:step 8 TD Control Byte is %x\n",usb_ctl_val);
	// while (usb_ctl_val != 0x03)
	// {
	// 	usb_ctl_val = UsbGetRetryCnt();
	// }

	// printf("------------[ENUM PROCESS]:get descriptor (class 0x21) done!-------------------\n");

	// // STEP 9 begin
	// //-------------------------------get descriptor (class 0x22 = report)-------------------------------------------//
	// // TASK: Call the appropriate function for this step.
	// UsbGetReportDesc();
	// //if no message
	// while (!(IO_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG) )  //read sie1 msg register
	// {
	// 	// TASK: Call the appropriate function again if it wasn't processed successfully.
	// 	UsbGetReportDesc();
	// 	usleep(10*1000);
	// }

	// UsbWaitTDListDone();

	// IO_write(HPI_ADDR,0x0506);
	// printf("[ENUM PROCESS]: step 9 TD Status Byte is %x\n",IO_read(HPI_DATA));

	// IO_write(HPI_ADDR,0x0508);
	// usb_ctl_val = IO_read(HPI_DATA);
	// printf("[ENUM PROCESS]: step 9 TD Control Byte is %x\n",usb_ctl_val);
	// while (usb_ctl_val != 0x03)
	// {
	// 	usb_ctl_val = UsbGetRetryCnt();
	// }

	// printf("---------------[ENUM PROCESS]:get descriptor (class 0x22) done!----------------\n");



	// //-----------------------------------get keycode value------------------------------------------------//
	// usleep(10000);
	//   while(1)
	//     {
	//       toggle++;
	//       IO_write(HPI_ADDR,0x0500); //the start address
	//       //data phase IN-1
	//       IO_write(HPI_DATA,0x051c); //500
	//       if(data_size ==8)
	//       {
	//           IO_write(HPI_DATA,0x0006);//2 data length
	//       }
	//       else
	//       {
	//           IO_write(HPI_DATA,0x000f & data_size);//2 data length
	//       }
	//       IO_write(HPI_DATA,0x0291);//4 //endpoint 1
	//       if(toggle%2)
	//       {
	//           IO_write(HPI_DATA,0x0001);//6 //data 1
	//       }
	//       else
	//       {
	//           IO_write(HPI_DATA,0x0041);//6 //data 1
	//       }
	//       IO_write(HPI_DATA,0x0013);//8
	//       IO_write(HPI_DATA,0x0000);//a
	//       UsbWrite(HUSB_SIE1_pCurrentTDPtr,0x0500); //HUSB_SIE1_pCurrentTDPtr
	//       //usleep(10*1000);
	//       while (!(IO_read(HPI_STATUS) & HPI_STATUS_SIE1msg_FLAG) )  //read sie1 msg register
	//       {
	//           IO_write(HPI_ADDR,0x0500); //the start address
	//           //data phase IN-1
	//           IO_write(HPI_DATA,0x051c); //500
	//           if(data_size ==8)
	//           {
	//              IO_write(HPI_DATA,0x0006);//2 data length
	//           }
	//           else
	//           {
	//              IO_write(HPI_DATA,0x000f & data_size);//2 data length
	//           }
	//           IO_write(HPI_DATA,0x0291);//4 //endpoint 1
	//           if(toggle%2)
	//           {
	//             IO_write(HPI_DATA,0x0001);//6 //data 1
	//           }
	//           else
	//           {
	//             IO_write(HPI_DATA,0x0041);//6 //data 1
	//           }
	//           IO_write(HPI_DATA,0x0013);//8
	//           IO_write(HPI_DATA,0x0000);//a
	//           UsbWrite(HUSB_SIE1_pCurrentTDPtr,0x0500); //HUSB_SIE1_pCurrentTDPtr
	//           usleep(10*1000);
	//       }//end while
	//       usb_ctl_val = UsbRead(HPI_SIE1_MSG_ADR);
	//       UsbWrite(HPI_SIE1_MSG_ADR, 0);
	//       while (usb_ctl_val != 0x1000)  //read sie1 msg register
	//       {
	//          if(usb_ctl_val == 0x000)
	//          {}
	//          else
	//          {
	//             printf("[SIE1 MSG]:SIE1 msg reg is %x\n",usb_ctl_val);
	//          }
	//          usb_ctl_val = UsbRead(HPI_SIE1_MSG_ADR);
	//          UsbWrite(HPI_SIE1_MSG_ADR, 0);
	//       }
	//       IO_write(HPI_ADDR,0x0508);
	//       usb_ctl_val = IO_read(HPI_DATA);

	//       printf("%d \n", usb_ctl_val);

	//       if (usb_ctl_val != 0x03)
	//       {
	//          pbutton = 0;
	//          dx = 0;
	//          dy = 0;
	//          button_value = 0;
	//       }

	//       else
	//      {
	//         if(data_size != 8 )
	//         {
	//           IO_write(HPI_ADDR,0x051c); //the start address
	//           button_value = IO_read(HPI_DATA);
	//           if((button_value & 0x00ff) ==0x0001)
	//           {
	//            //printf("left button\n");
	//             pbutton = 1;
	//           }
	//           else
	//           if((button_value& 0x00ff) ==0x0002)
	//           {
	//            //printf("right button\n");
	//            pbutton = 2;
	//           }
	//           else
	//           {
	//            pbutton = 0;
	//           }
	//           if((signed char)((button_value>>8)&0x00ff) != 0)
	//           {

	//              if((signed char)((button_value>>8)&0x00ff )==dx)
	//              {
	//                 dx = 0;
	//              }
	//               else
	//              {
	//                 dx = (signed char)((button_value>>8)&0x00ff);
	//              }

	//           }

	//        button_value = IO_read(HPI_DATA);

	//        if(button_value&0xff)
	//        {
	//           //printf("dy value is %d\n",button_value&0xff);
	//           if((signed char)(button_value&0x00ff) == dy)
	//           {
	//              dy = 0;
	//           }
	//           else
	//           {
	//              dy = (signed char)(button_value&0xff);
	//           }

	//         }
	//       }
	//       else //>8
	//       {
	//         IO_write(HPI_ADDR,0x051c); //the start address
	//         button_value = IO_read(HPI_DATA);
	//          if(((button_value>>8) & 0x00ff) ==0x0001)
	//         {
	//            //printf("left button\n");
	//            pbutton = 1;
	//         }
	//         else
	//         if(((button_value>>8) & 0x00ff) ==0x0002)
	//         {
	//            //printf("right button\n");
	//            pbutton = 2;
	//         }
	//         else
	//         {
	//            pbutton = 0;
	//         }
	//         button_value = IO_read(HPI_DATA);
	//         if((signed char)((button_value>>8)&0x00ff) != 0)
	//         {
	//            if((signed char)((button_value>>8)&0x00ff )==dy)
	//            {
	//               dy = 0;
	//            }
	//            else
	//            {
	//               dy = (signed char)((button_value>>8)&0x00ff);
	//            }

	//         }

	//        if(button_value&0x0ff)
	//        {
	//           //printf("dy value is %d\n",button_value&0xff);
	//           if((signed char)(button_value&0x00ff) == dx)
	//           {
	//              dx = 0;
	//           }
	//           else
	//           {
	//              dx = (signed char)(button_value&0xff);
	//           }

	//         }
	//       }

	//     }

	//     px = px + dx;
	//     py = py + dy;

	//     px = maxmin(px,639,0);
	//     py = maxmin(py,479,0);

	//     IOWR(PX_BASE, 0, px);
	//     IOWR(PY_BASE, 0, py);
	//     IOWR(BUTTON_BASE, 0, pbutton);
	//     if(pbutton == 1)
	//     {
	//     	initialPutPixel(px-1, py-1, 0xffff);
	//     	initialPutPixel(px+1, py-1, 0xffff);
	//     	initialPutPixel(px, py-1, 0xffff);
	//     	initialPutPixel(px-1, py+1, 0xffff);
	//     	initialPutPixel(px+1, py+1, 0xffff);
	//     	initialPutPixel(px, py+1, 0xffff);
	//     	initialPutPixel(px, py, 0xffff);
	//     	initialPutPixel(px-1, py, 0xffff);
	//     	initialPutPixel(px+1, py, 0xffff);
	//     	//initialPutPixel(300, 110, 0xffff);
	//     //initialPutPixel(500, 110, 0xffff);
	//     //initialPutPixel(10, 110, 0xffff);
	//     }
	//     printf("\n[MOUSE XY]:%d, %d", px, py);
	//     usleep(5*1000);
	//     //USB hot plug routine
	//     usb_ctl_val = UsbRead(ctl_reg);
	//     usleep(5*1000);
	//     usb_ctl_val = UsbRead(ctl_reg);
	//     usleep(5*1000);
	//     usb_ctl_val = UsbRead(ctl_reg);
	//     usleep(5*1000);
	//     usb_ctl_val = UsbRead(ctl_reg);
	//     usleep(5*1000);
	//     usb_ctl_val = UsbRead(ctl_reg);
	//     usleep(5*1000);
	//     usb_ctl_val = UsbRead(ctl_reg);
	//     usleep(5*1000);

	//     if(!(usb_ctl_val & no_device))
	//     {
	//        printf("\n[INFO]: the mouse has been removed!!! \n");
	//        printf("[INFO]: please insert again!!! \n");
	//     };
	//     usleep(5000);
	//     usb_ctl_val = UsbRead(ctl_reg);

	//     while (!(usb_ctl_val & no_device))
	//     {

	//        usb_ctl_val = UsbRead(ctl_reg);
	//        usleep(5*1000);
	//        usb_ctl_val = UsbRead(ctl_reg);
	//        usleep(5*1000);
	//        usb_ctl_val = UsbRead(ctl_reg);
	//        usleep(5*1000);

	//        if(usb_ctl_val & no_device)
	//          goto USB_HOT_PLUG;

	//        usleep(200);
	//     }

	// }
	return 0;
}

