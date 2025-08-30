#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define WIDTH  800
#define HEIGHT 800
#define PADDING 40 
#define STEPS 20
#define PREFIX "output.ppm" // you should use .ppm extension  

#define RED    0xFB4934 
#define GREEN  0x00FF00
#define BLUE   0x0000FF

#define BG_COLOR 0x202020
#define FG_COLOR 0x928374

#define COLOR 0x2C074F



uint32_t image[HEIGHT][WIDTH];

typedef uint32_t Color ;


void cxy(int *x,int *y)
{
	*x = WIDTH /2;
	*y = HEIGHT /2;
}


void draw_px(int x,int y,Color color)
{
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
	image[y][x] = color;
}

void draw_pixels(Color bg)
{
    for(int y = 0;y < HEIGHT;++y){
        for(int x=0;x < WIDTH;++x){
			draw_px(x, y, bg);
        }
    }

}

void draw_graph(Color bg,Color fg)
{
    for(int y = 0;y < HEIGHT;++y){
        for(int x=0;x < WIDTH;++x){
			draw_px(x, y, bg);
        }
    }
	int cx, cy;
	cxy(&cx,&cy);

	int min = ( WIDTH < HEIGHT ) ? WIDTH : HEIGHT;
	for (int i =0;i<min/2 - PADDING ;++i) {
		draw_px(cx+i,  cy,fg);
		draw_px(cx,  cy+i,fg);
		draw_px(cx+(i * -1),  cy,fg);
		draw_px(cx,  cy+(i * -1),fg);
	}
	int c = min /2 - PADDING;

	for (int i=STEPS;i<c;i+=STEPS){
		for (int j=0;j<10;++j){

			draw_px(cx +i, (cy - 5) +j,RED);
			draw_px((-1 * i) +cx, (cy - 5) +j,RED);

			draw_px( (cx - 5) +j,cy +i ,RED);
			draw_px( (cx - 5) +j,(-1 * i) +cy, RED);
		}


	}

}

void draw_circle(int ix, int iy, int radius, Color color)
{
	for(int y = -radius;y<=radius;++y){
		for(int x = -radius;x<=radius;++x){
			int a = x*x + y*y ;
			if (a>=radius*radius - radius && a<=radius*radius +radius){
				draw_px(ix+x,iy+y,color);
			}
		}

	}

}

void draw_circlef(int ix, int iy, int radius, Color color)
{
	for (int i =0;i<=radius;i=i+1)
		draw_circle(ix,iy,i,color+i*color);

}

void draw_point(int x,int y)
{
	int cx,cy;
	cxy(&cx,&cy);

	draw_circlef(cx + x*STEPS,(cy -y*STEPS),1,FG_COLOR);
	
}



void draw_all()
{
    FILE *f= fopen(PREFIX, "wb");

    if (f == NULL){
        fprintf(stderr,"ERROR: Could not open file!!\n");
    }
    fprintf(f,"P6\n%d\n%d\n255\n",WIDTH,HEIGHT);


    for(int y = 0;y < HEIGHT;++y){
        for(int x=0;x < WIDTH;++x){

            Color color = image[y][x];
            uint8_t bytes[3] = {
                (color & 0xFF0000) >> 8*2,
                (color & 0x00FF00) >> 8*1,
                (color & 0x0000FF) >> 8*0,
            };

            fwrite(bytes,sizeof(bytes), 1,f);
        }
    };
	fclose(f);
}



int main(void) {
    draw_pixels(BG_COLOR);

	int x,y;
	cxy(&x,&y);

	// potato


	draw_graph(BG_COLOR,FG_COLOR);
	draw_circlef(x,y,x/2,GREEN);

	draw_all();
    return 0;
}
