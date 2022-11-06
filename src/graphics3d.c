#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"
#include "graphics3d.h"
#include "teapot_data.h"
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>

vec3f lightdir={0.577f,-0.577f,0.577f};
float rmx[3][3];
vec2f position={0,0};
vec3f ambiant_colour={20,20,20};//{0.2,0.2,0.2};
vec3f material_colour={20,220,40};
vec3f light_colour={255,255,255};//{1.0,1.0,1.0};
const float specularstrength=0.5f;
typedef  struct {uint16_t p[8]; uint16_t col; int16_t z;} quadtype;
#define MAXQUADS 24*32*4
int nquads;
static quadtype quads[MAXQUADS];

inline int clamp(int x,int min,int max) {
    const int t = x < min ? min : x;
    return t > max ? max : t;
}

inline float clampf(float x,float min,float max) {
    const float t = x < min ? min : x;
    return t > max ? max : t;
}

inline vec2 point3d_to_xy(vec3f p) {
    return (vec2){clamp((int)p.x,0,display_width-1),
                  clamp((int)p.y,0,display_height-1)};
}

void draw_line_3d(vec3f p0, vec3f p1, uint16_t colour) {
    vec2 a,b;
    a=point3d_to_xy(p0);
    b=point3d_to_xy(p1);
    draw_line(a.x,a.y,b.x,b.y, colour);
}

void draw_triangle_3d(vec3f p0, vec3f p1, vec3f p2, uint16_t colour) {
    vec2 a,b,c;
    a=point3d_to_xy(p0);
    b=point3d_to_xy(p1);
    c=point3d_to_xy(p2);
    draw_triangle(a.x,a.y,b.x,b.y,c.x,c.y, colour);
}


void maketrotationmatrix(vec3f rotation, vec2f pos, float size) {
    position=pos;
    float ca=cosf(rotation.x);
    float cb=cosf(rotation.y);
    float cc=cosf(rotation.z);
    float sa=sinf(rotation.x);
    float sb=sinf(rotation.y);
    float sc=sinf(rotation.z);

    rmx[0][0]=cc*cb*size;
    rmx[0][1]=(cc*sb*sa-sc*ca)*size;
    rmx[0][2]=(cc*sb*ca+sc*sa)*size;
    rmx[1][0]=sc*cb*size;
    rmx[1][1]=(sc*sb*sa+cc*ca)*size;
    rmx[1][2]=(sc*sb*ca-cc*sa)*size;
    rmx[2][0]=-sb*size;
    rmx[2][1]=cb*sa*size;
    rmx[2][2]=cb*ca*size;
}

void add_quad(vec3f p0, vec3f p1, vec3f p2, vec3f p3) {
    if(p0.x<0 && p1.x<0 && p2.x<0 && p3.x<0) return;
    if(p0.y<0 && p1.y<0 && p2.y<0 && p3.y<0) return;
    if(p0.x>display_width && p1.x>display_width && p2.x>display_width && p3.x>display_width) return;
    if(p0.y>display_height && p1.y>display_height && p2.y>display_height && p3.y>display_height) return;
    vec3f normal=cross3d(sub3d(p2,p0),sub3d(p3,p0));
    if(normal.z<=0) return;
    normal=normalise(normal);
    if(nquads>=MAXQUADS) return;
    float dp=dot(normal,lightdir);
    float diff=clampf(dp,0,1.0);
    vec3f diffuse=mul3df(diff,material_colour);
    float spec=clampf(2.0f*dp*normal.z-lightdir.z,0,2);
    spec=spec*spec;
    spec=spec*spec;
    spec=spec*spec;
    spec=specularstrength*spec;
    vec3f specular=mul3df(spec,light_colour);
    vec3f res=add3d(ambiant_colour,add3d(diffuse,specular));
    uint16_t colour=rgbToColour(clampf(res.x,0,255),clampf(res.y,0,255),clampf(res.z,0,255));

    quads[nquads++]=(quadtype){{p0.x,p0.y,p1.x,p1.y,p2.x,p2.y,p3.x,p3.y},
                        colour,(int16_t)((p0.z+p1.z+p2.z+p3.z)*64)};
}

int cmpquad(const void * a, const void * b) {
   return ( ((quadtype *)a)->z - ((quadtype *)b)->z );
}

void sort_quads() {
    qsort(quads, nquads, sizeof(quadtype), cmpquad);    
}

void draw_all_quads() {
    sort_quads();
    for(int i=0;i<nquads;i++) {
        quadtype q=quads[i];
        draw_triangle(q.p[0],q.p[1],q.p[2],q.p[3],q.p[4],q.p[5],q.col);
        draw_triangle(q.p[4],q.p[5],q.p[6],q.p[7],q.p[0],q.p[1],q.col);
    }
}

vec3f vrotate(vec3f v) {
    v.z=(v.z-2.0);
    return (vec3f){ (rmx[0][0]*v.x+rmx[0][1]*v.y+rmx[0][2]*v.z)+position.x,
                    (rmx[1][0]*v.x+rmx[1][1]*v.y+rmx[1][2]*v.z)+position.y,
                    (rmx[2][0]*v.x+rmx[2][1]*v.y+rmx[2][2]*v.z)};
}
void eval_bezier(const uint32_t divs, const vec3f p0, const vec3f p1, const vec3f p2, const vec3f p3, vec3f out[]) { 
    float h = 1.f / divs;
    vec3f b0 = p0;
    vec3f fph = mul3df(3*h,sub3d(p1 , p0));
    vec3f fpphh = mul3df(h*h,(add3d(sub3d(mul3df(6 , p0) , mul3df(12 , p1)) , mul3df(6 , p2))));
    vec3f fppphhh = mul3df(h*h*h,add3d(sub3d(add3d(mul3df(-6 , p0) , mul3df(18 , p1)) , mul3df(18 , p2)) , mul3df(6 , p3)));
    out[0] = b0;
    for (uint32_t i = 1; i <= divs; ++i) {
        out[i] = add3d(add3d(add3d(out[i - 1] , fph) , mul3df(0.5f,fpphh)) , mul3df(0.1666f,fppphhh));
        fph = add3d(add3d(fph , fpphh) , mul3df(0.5f,fppphhh));
        fpphh = add3d(fpphh , fppphhh);
    }
}
//static inline int max(int a, int b) {return a>b?a:b;}
//static inline int min(int a, int b) {return a<b?a:b;}
static inline float maxf(float a, float b) {return a>b?a:b;}
static inline float minf(float a, float b) {return a<b?a:b;}
void add_bezier_patch(vec3f const p[4][4]) {
    int PX=4;

    float d1=mag3d(sub3d(p[0][0],p[0][3]));
    float d2=mag3d(sub3d(p[0][0],p[3][0]));
    float d3=mag3d(sub3d(p[3][3],p[0][3]));
    float d4=mag3d(sub3d(p[3][3],p[3][0]));
    
    float maxd=sqrtf(maxf(maxf(maxf(d1,d2),d3),d4));

    int divs=maxd/PX;

    if(divs<4) divs=4;
    if(divs>13) divs=13;
    vec3f py[4][divs+1];
    for (int i=0; i<4; i++) { 
        eval_bezier(divs, p[i][0], p[i][1], p[i][2], p[i][3], py[i]); 
    }
    vec3f np[2][divs+1];
    for (int i=0; i<=divs; i++) {
        eval_bezier(divs, py[0][i], py[1][i], py[2][i], py[3][i], np[i%2]); 
        if(i>0) {
            int j1=i%2;
            int j0=1-j1;
            for (int k=0; k<divs; k++) {
                add_quad(np[j0][k],np[j1][k],np[j1][k+1],np[j0][k+1]);
            }
        }
    } 
}

void draw_teapot(vec2f pos, float size, vec3f rot, colourtype col) {

    
    material_colour=(vec3f){col.r,col.g,col.b};
    maketrotationmatrix(rot,pos,size);

    // the teapot is made from 32 patches as follows:
    // 28-32=base
    // 20-27=lid
    // 16-19=spout
    // 12-15=handle
    // 8-11=bottom body
    // 0-7=top body

    nquads=0;
    for(int ii=0;ii<32;ii++) {
        vec3f p[4][4];
        for(int j=0;j<4;j++) {
            for(int k=0;k<4; k++) {
                p[j][k]=vrotate(teapotVertices[teapotPatches[ii][j*4+k]-1]);
            }
        }
        add_bezier_patch(p);
    }
    draw_all_quads();
}

const uint8_t cubeQuads[][4] = { 
	{0,2,3,1}, 
	{1,3,7,5}, 
	{3,2,6,7}, 
	{0,1,5,4}, 
	{0,4,6,2}, 
	{5,7,6,4}, 
};

void draw_cube(vec2f pos, float size, vec3f rot) {
    colourtype col;
    nquads=0;
    maketrotationmatrix(rot,pos,size);
    for(int q=0;q<6;q++) {
        vec3f quad[4];
        for(int i=0;i<4;i++) {
            int v=cubeQuads[q][i];
            quad[i].x=(v&4)?1.0:-1.0;
            quad[i].y=(v&2)?1.0:-1.0;
            quad[i].z=(v&1)?3.0:1.0;
            quad[i]=vrotate(quad[i]);
        }
        col.r=((q+1)&1)*255;
        col.g=(((q+1)>>1)&1)*255;
        col.b=(((q+1)>>2)&1)*255;
        material_colour=(vec3f){col.r,col.g,col.b};
        add_quad(quad[0],quad[1],quad[2],quad[3]);
    }
    draw_all_quads();
}