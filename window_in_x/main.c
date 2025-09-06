#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define WIDTH  600
#define HEIGHT WIDTH 
#define FPS    60

#define STEPS 10
#define BG_COLOR 0x1d2021

#define RED    0xFB4934 
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define BLACK  0x000000
#define WHITE  0xffffff

typedef uint32_t Color;

typedef struct {
	int x;
	int y;
}Position;


Color image[HEIGHT][WIDTH];

void draw_px(int x, int y, Color c) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        image[y][x] = c;
}

void clear_buffer(Color bg) {
    for (int y = 0; y < HEIGHT; ++y)
        for (int x = 0; x < WIDTH; ++x)
            image[y][x] = bg;
}

void draw_circlef(int cx, int cy, int radius, Color color) {
    for (int y = -radius; y <= radius; ++y) {
        for (int x = -radius; x <= radius; ++x) {
			int a = x*x + y*y ;
            if (a<=radius*radius)
                draw_px(cx + x, cy + y, color);
        }
    }
}
void draw_circle(int cx, int cy, int radius, Color color) {
    for (int y = -radius; y <= radius; ++y) {
        for (int x = -radius; x <= radius; ++x) {
			int a = x*x + y*y ;
            if (a<=radius*radius + radius && a>=radius*radius - radius)
                draw_px(cx + x, cy + y, color);
        }
    }
}


void draw_line( float x1,float y1, float x2,float y2, Color color ) 
{
	int ya = y1<y2? y1:y2;
	int yb = y2<y1? y1:y2;
	int xa = x1<x2? x1:x2;
	int xb = x2<x1? x1:x2;
	int dx =abs(x2-x1);
	int dy =abs(y2-y1);

	float a,b;


	if (x2-x1!=0){
		a = (y2-y1) / (x2-x1);
		b = y1 - a * x1;
	}
	

	if (x1==x2){
		for (int y = ya;y<=yb;++y)
			draw_px(x1, y, color);
		return;
	}
	if (dx >=dy){
		for (int i=xa;i<=xb;++i){
			int y = a*i + b;
			draw_px(i, y, color);
		}
	}else 
	if (dy > dx){
		for (int i=ya;i<=yb;++i){
			int x = (i - b) / a;
			draw_px(x, i, color);
		}
	}
}


void draw_trianglef(int x1,int y1,int x2,int y2,int x3,int y3, Color color) {

    if (y1 > y2) { int tmp=x1; x1=x2; x2=tmp; tmp=y1; y1=y2; y2=tmp; }
    if (y1 > y3) { int tmp=x1; x1=x3; x3=tmp; tmp=y1; y1=y3; y3=tmp; }
    if (y2 > y3) { int tmp=x2; x2=x3; x3=tmp; tmp=y2; y2=y3; y3=tmp; }

    int total_height = y3 - y1;
    for (int i=0; i<=total_height; i++) {
        int second_half = i > (y2 - y1) || y2 == y1;
        int segment_height = second_half ? (y3 - y2) : (y2 - y1);
        float alpha = (float)i / total_height;
        float beta  = (float)(i - (second_half ? y2 - y1 : 0)) / segment_height;

        int ax = x1 + (x3 - x1) * alpha;
        int bx = second_half ? x2 + (x3 - x2) * beta : x1 + (x2 - x1) * beta;

        if (ax > bx) { int tmp = ax; ax = bx; bx = tmp; }
        for (int j = ax; j <= bx; j++)
            draw_px(j, y1 + i, color);
    }
}
void draw_rectanglef(int x,int y,uint width,uint height,Color color)
{
	for(int i = x;i<=x+width;i=++i){
		for(int j = y;j<=y+height;++j){
			draw_px(i, j,color);
		}
	}

}
void draw_rectangle(int x,int y,uint width,uint height,Color color)
{
	draw_line(x,y,x + width,y ,color);
	draw_line(x, y, x, y+height,color);
	draw_line(x+width, y, x+width, y+height,color);
	draw_line(x ,y+ height,x+width ,y+ height,color);
}



void draw_triangle(int x1,int y1, int x2,int y2,int x3,int y3,Color color)
{
	draw_line(x1, y1, x2, y2, color);
	draw_line(x2, y2, x3, y3, color);
	draw_line(x3, y3, x1, y1, color);

}

void draw_ellipse(int x,int y, uint w,uint h, Color color ) {
	int a = w/2;
	int b = h/2;
	for (int i = -a; i<=a;++i){
		for (int j = -b; j<=b;++j){
			float eq = (float)(i*i)/(a*a) + (float)(j*j)/(b*b);
				if (eq <= 1.00 && eq >= 0.90)
					draw_px(x+i,y+j, color);
		}
	}
}


void draw_ellipsef(int x,int y, uint w,uint h, Color color ) {
	int a = w/2;
	int b = h/2;
	for (int i = -a; i<=a;++i){
		for (int j = -b; j<=b;++j){
			float eq = (float)(i*i)/(a*a) + (float)(j*j)/(b*b);
				if (eq <= 1.00 )
					draw_px(x+i,y+j, color);
		}
	}
}

void cxy(int *x,int *y)
{
	*x = WIDTH /2;
	*y = HEIGHT /2;
}

// USER Drawing fn's

void game_box(int x, int y, size_t size, size_t border, Color fg_color,Color bg_color){
	// draw_rectanglef(int x, int y, uint width, uint height, Color color)

	draw_rectanglef(x, y, size, size, bg_color);
	draw_rectanglef(x+border, y+border, size-10, size-10, fg_color);


}

void game_player(int x, int y, size_t size,size_t border,Color fg_color,Color bg_color)
{
	int e = (size-border) / 4;
	draw_rectanglef(x, y, size, size, bg_color);
	draw_rectanglef(x+border, y+border, size-border*2, size-border*2, fg_color);
	draw_rectanglef(x+size-e, y+e, 3,7,WHITE);
	draw_rectanglef(x+e, y+e, 3,7,WHITE);
}

int main(int argc,char *argv[]) {

	char *t = argv[1];
	if (t==NULL){
		printf("./main game\n./main cube\n./main circle\n");
		exit(1);
	}

    Display *d = XOpenDisplay(NULL);
    if (!d) return 1;

    Window w = XCreateSimpleWindow(d, DefaultRootWindow(d),
                                   1920/2 - WIDTH/2, 1080/2 - HEIGHT/2,
                                   WIDTH, HEIGHT, 0, 0, 0);
    XMapWindow(d, w);
    XSelectInput(d, w, ExposureMask | KeyPressMask);

	XClassHint *h = XAllocClassHint();
	h->res_class="w_test";
	XSetClassHint(d, w,h);


	GC gc = XCreateGC(d, w, 0, NULL);
    XSetForeground(d, gc, BG_COLOR);


	int boxs = WIDTH / STEPS;
	Position mv;
	mv.x = 0;
	mv.y = 0;
	int px = boxs+mv.x;
	int py = boxs+mv.y;
	Color pc = GREEN;

    while (1) {
		int cx,cy;
		cxy(&cx,&cy);
			clear_buffer(BG_COLOR);

		while (XPending(d)) {
			XEvent e;
			XNextEvent(d, &e);
			if (e.type == KeyPress && XLookupKeysym(&e.xkey, 0) == XK_q)
				exit(0);
		}
		if (strcmp(t,"game") ==0){

			XEvent e;
			XNextEvent(d, &e);
			for(int i = 0;i<=WIDTH;i=i+boxs){
				game_box(i, 0, boxs, 5,RED,WHITE);
				game_box(0, i, boxs, 5,RED,WHITE);
				game_box(i, HEIGHT-boxs, boxs, 5,RED,WHITE);
				game_box(HEIGHT-boxs,i, boxs, 5,RED,WHITE);
			}

			if (e.type == KeyPress ){
				int key = XLookupKeysym(&e.xkey,0);
				if (key == XK_Left){
					if (px - boxs >=boxs){
						px = px -boxs;
					}
				}else if (key == XK_Right){
					if  (px + boxs <WIDTH - boxs){
						px = px +boxs;
					}
				}else if (key == XK_Down){
					if  (py + boxs <HEIGHT - boxs){
						py = py +boxs;
					}
				}else if (key == XK_v){
					if (pc == RED)
						pc = GREEN;
					else
						pc = RED;
				}else if (key == XK_Up){
					if  (py  > boxs){
						py = py -boxs;
					}
				}else if (key == XK_q){
					exit(0);
				}
			}
			
			game_player(px, py, boxs, 3, BLACK, pc);

		}else if(strcmp(t,"cube") == 0){
			static float ang = 0.0f;   
			const float size = 150.0f; 
			const float dist = 400.0f; 
			const float scale = 250.0f;

			float v[8][3] = {
				{-size, -size, -size},
				{ size, -size, -size},
				{ size,  size, -size},
				{-size,  size, -size},
				{-size, -size,  size},
				{ size, -size,  size},
				{ size,  size,  size},
				{-size,  size,  size},
			};

			float sx[8], sy[8];
			float cosy = cosf(ang), siny = sinf(ang);
			float angx = ang * 0.6f;
			float cosx = cosf(angx), sinx = sinf(angx);

			for (int i = 0; i < 8; ++i) {
				float x = v[i][0] * cosy + v[i][2] * siny;
				float z = -v[i][0] * siny + v[i][2] * cosy;
				float y = v[i][1] * cosx - z * sinx;
				z = v[i][1] * sinx + z * cosx;
				float proj = scale / (z + dist);
				sx[i] = cx + x * proj;
				sy[i] = cy + y * proj;
			}

			int edges[12][2] = {
				{0,1},{1,2},{2,3},{3,0},
				{4,5},{5,6},{6,7},{7,4},
				{0,4},{1,5},{2,6},{3,7} 
			};

			Color edge_color = 0xFFFFFF;
			for (int e = 0; e < 12; ++e) {
				int a = edges[e][0], b = edges[e][1];
				draw_line(sx[a], sy[a], sx[b], sy[b], edge_color);
			}

			for (int i = 0; i < 8; ++i)
				draw_circlef((int)sx[i], (int)sy[i], 4, edge_color);

			ang += 0.06f;
		}else if(strcmp(t, "circle" )== 0){
			static float t = 0.0f;
			for (int r = 150; r > 0; r--) {
				int red   = (int)(128 + 127 * sin(t + r * 0.1));
				int green = (int)(128 + 127 * sin(t + r * 0.2));
				int blue  = (int)(128 + 127 * sin(t + r * 0.3));
				draw_circlef(cx, cy, r, (red << 16) | (green << 8) | blue);
			}
			t += 0.08f;
		}else{
			printf("./main game\n./main cube\n./main circle\n");
			exit(1);
		}


			XImage *img = XCreateImage(d, DefaultVisual(d, 0), 24, ZPixmap, 0,
									   (char *)image, WIDTH, HEIGHT, 32, 0);
			XPutImage(d, w, gc, img, 0, 0, 0, 0, WIDTH, HEIGHT);
			img->data = NULL;
			XDestroyImage(img);

			usleep(1.0/FPS * 1000*1000);
		}

    XCloseDisplay(d);
    return 0;
}

