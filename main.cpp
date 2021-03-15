
#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include<math.h>
#include<vector>
#include<iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <tchar.h>
#include<stdio.h>
//#include "NewFrame.h"
//#include<graphics.h>
#include<stdlib.h>
#include<conio.h>
using namespace std;;
static vector<int> savedpoints;
static vector<float> clip;
static vector<string> savedstate;
static vector<float> points;
static int clippfirst,clippend;
//static int draw;
//static vector<string> state;
void DrawLine2(HDC hdc, double x1, double y1, double x2, double y2);
void save()
{
    ofstream out;
    out.open("text.txt");
    for(int i = 0; i<savedpoints.size(); i++)
        out << savedpoints[i] <<endl;

    out.close();
}

void storesteps(float x,float y)
{
      points.push_back(x);
      points.push_back(y);
      //points.push_back(draw);
}

void storestates(string state)
{
    savedstate.push_back(state);
}

void savesteps()
{
ofstream out;
out.open("steps.txt");
    for(int i = 0,j=0; i<points.size();j++ ,i+=2)
       {
        out <<"x = "<<points[i]<<"  y= " <<points[i+1]<<" "<<savedstate[j]<<endl;
       }
out.close();}



//start of convex filling
struct Point
{
    int x,y;
    Point(int x,int y):x(x),y(y) {}
    Point() {}
};

struct Entry
{
    double xleft=10000,xright=-10000;
    Entry(double xleft, double xright):xleft(xleft),xright(xright) {}
    Entry() {}
};

int counter = 0;
Point p1,p2,p3,p4,p5;
vector <Point> vertcies;
Entry Entry_table [1000];


void scanEdge()
{
    int n = vertcies.size();
    Point v1=vertcies[n-1];
    for( int i=0; i<n ; i++)
    {
        Point v2= vertcies[i];


        if(v1.y == v2.y)
            return;  //slope = zero: horizontal line
        if(v1.y > v2.y)
        {
            swap(v1.x,v2.x);
            swap(v1.y,v2.y);
        }

        double x = v1.x;
        int y = v1.y;
        double m =(double)(v2.x-v1.x)/(double)(v2.y-v1.y);


        while(y < v2.y)
        {
            if (x < Entry_table[y].xleft)
                Entry_table[y].xleft = x;


            if (x > Entry_table[y].xright)
                Entry_table[y].xright = x;

            y++;
            x+=m;
        }
        v1=vertcies[i];

    }
}


void scanLines(HDC hdc, COLORREF c)
{
    for( int i=0 ; i< 1000; i++)
    {
        if(Entry_table[i].xleft < Entry_table[i].xright)
        {
            for(int j=(int)ceil(Entry_table[i].xleft); j<=(int)floor(Entry_table[i].xright) ; j++)
               {

                SetPixel(hdc,j,i,c);

               }

        }
    }
}


void mainAlgorithm(HDC hdc, COLORREF c)
{
    scanEdge();
    scanLines(hdc, c);
}

//end of convex filling

// start clipping
static double xc,yc,R;
int Round(double x)
{
    return (int)x+0.5;
}

union outCode
{
    unsigned all:4;
    struct
    {
        unsigned left:1,right:1,bot:1,top:1;
    };
};
outCode GetCode(double x,double y,double xleft,double xRight,double yBot,double yTop)
{
    outCode out;
    out.all=0;
    if(x<xleft)
        out.left=1;
    else if(x>xRight)
        out.right=1;
    else if(y<yBot)
        out.bot=1;
    else if (y > yTop)
        out.top=1;
    return out;
}

void VIntersect(double x1,double y1,double x2,double y2,double xEdge,double &xi,double &yi )
{
    xi=xEdge;
    yi=y1+(xEdge-x1)*(y2-y1)/(x2-x1);
}

void HIntersect(double x1,double y1,double x2,double y2,double yEdge,double &xi,double &yi )
{
    yi=yEdge;
    xi=x1+(yEdge-y1)*(x2-x1)/(y2-y1);
}



bool IsInside(int xc, int yc, int R, int x1, int y1)
{
	int z = pow(x1 - xc, 2) + pow(y1 - yc, 2) - pow(R, 2);
	return  z > -50;
}

void DrawLine(HDC hdc, int xc, int yc, int R, double x1, double y1, double x2, double y2)
{
    double dt = 0.001;
    for (double t = 0; t < 1; t += dt)
    {
        double x = x1 + t * (x2 - x1);
        double y = y1 + t * (y2 - y1);
        if (!IsInside(xc, yc, R, x, y))
      {
        SetPixel(hdc, x, y, RGB(255, 0, 0));
        clip.push_back(x);
        clip.push_back(y);
    }
    }
    if(clippfirst==1){
        storesteps(clip[0],clip[1]);
        storestates(" by clipping first point ");
        clippfirst=0;
    }
    if(clippend==1){
        storesteps(clip[clip.size()-2],clip[clip.size()-1]);
        storestates(" by clipping end point");
        clippend=0;
    }
    storesteps(clip[0],clip[1]);
    storestates("finally the first point of the line became ");
    storesteps(clip[clip.size()-2],clip[clip.size()-1]);
    storestates("and the end point of the line became \n");
}
void DrawLine2(HDC hdc, double x1, double y1, double x2, double y2)
{
    double dt = 0.001;
    for (double t = 0; t < 1; t += dt)
    {
        double x = x1 + t * (x2 - x1);
        double y = y1 + t * (y2 - y1);
        SetPixel(hdc,x,y,RGB(255,0,0));
        //storesteps(x,y,draw);
        }}

bool Clipping(double &x1,double &y1,double &x2,double &y2,double xLeft,double xRight,double yTop,double yBot,HDC hdc)
{
    outCode out1=GetCode(x1,y1,xLeft,xRight,yBot,yTop);
    outCode out2=GetCode(x2,y2,xLeft,xRight,yBot,yTop);
    while(true)
    {
        if(out1.all==0 && out2.all==0)
        {
            storesteps(x1,y1);
            storestates("no clipping it's first point ");
            storesteps(x2,y2);
            storestates(" it's end point \n");
             DrawLine2(hdc,x1,y1,x2,y2);
             savesteps();
             //cout<<"here";
            return false;}
        if(out1.all!=0 || out2.all!=0)
        {
            if(out1.all !=0)
        {
            clippfirst=1;
            storesteps(x1,y1);
            storestates(" your first point is outside the window ");
            double xi,yi;
            if(out1.left)
               { VIntersect(x1,y1,x2,y2,xLeft,xi,yi);

               }
            else if(out1.right)
                {VIntersect(x1,y1,x2,y2,xRight,xi,yi);}
            else if(out1.top)
                {HIntersect(x1,y1,x2,y2,yTop,xi,yi);}
            else if(out1.bot)
               {
                   HIntersect(x1,y1,x2,y2,yBot,xi,yi);

               }
                x1=xi;
                y1=yi;
                out1=GetCode(xi,yi, xLeft, xRight, yTop, yBot);
        }
        if(out2.all !=0)
        {
            clippend=1;
            storesteps(x2,y2);
            storestates("your end point is outside the window");
            double xi,yi;
            if(out2.left)
                {VIntersect(x1,y1,x2,y2,xLeft,xi,yi);}
            else if(out2.right)
                {VIntersect(x1,y1,x2,y2,xRight,xi,yi);}
            else if(out2.top)
                {HIntersect(x1,y1,x2,y2,yTop,xi,yi);}
            else if(out2.bot)
                {HIntersect(x1,y1,x2,y2,yBot,xi,yi);}
                x2=xi;
                y2=yi;
                out2=GetCode(xi,yi, xLeft, xRight, yTop, yBot);
        }
//        DrawLine(x1,y1,x2,y2,hdc);

        break;
        }

    }
DrawLine(hdc,xc,yc,R,x1,y1,x2,y2);
         savesteps();
}

void DrawCircle(HDC hdc ,int xc,float yc,int R,COLORREF c)
{
	double dt = 1.0 / R;
	for (double theta = 0; theta <= 6.28; theta += dt)
	{
		double x = xc + R * cos(theta);
		double y = yc + R * sin(theta);
		SetPixel(hdc, Round(x), Round(y), c);
	}
}

///// end clipping

//start line codes

void Swap(int&x1,int&y1,int&x2,int&y2)
{
    int tmp=x1;
    x1=y1;
    y1=tmp;

    int tmp2=x2;
    x2=y2;
    y2=tmp2;
}

void Parametric(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
    double dt = 1.0 / max(abs(x2 - x1), abs(y2 - y1));
    for (double t = 0; t <= 1; t += dt)
    {
        double x = x1 + t * (x2 - x1);
        double y = y1 + t * (y2 - y1);
        SetPixel(hdc, Round(x), Round(y), c);
    }
}

void MidPoint(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
int  dx=x2-x1, dy=y2-y1;
    if(abs(dx)>=abs(dy))
    {
        if(dx<0)
        {
            swap(x1,x2);
            swap(y1,y2);

            dx = -dx;
            dy = -dy;
        }
        int x=x1, y=y1;
        SetPixel(hdc, x, y, c);
        if(dy>=0)
        {

            int d = dx - 2 * dy;
            int d1 = 2*dx - 2*dy;
            int d2 = -2*dy;

            while(x<x2)
            {
                if(d<=0)
                {
                    d+=d1;
                    x++;
                    y++;
                }
                else
                {
                    d+=d2;
                    x++;
                }

                SetPixel(hdc, x, y, c);

            }
        }
        else
        {
            int d = dx + 2 * dy;
            int d1 = 2*dx + 2*dy;
            int d2 = 2*dy;

            while(x<x2)
            {
                if(d<=0)
                {
                    d+=d1;
                    x++;
                    y--;
                }
                else
                {
                    d+=d2;
                    x++;
                }

                SetPixel(hdc, x, y, c);

            }
        }

    }
    else
    {
        if(dy<0)
        {
            swap(x1,x2);
            swap(y1,y2);

            dx = -dx;
            dy = -dy;
        }
        int x=x1, y=y1;
        SetPixel(hdc, x, y, c);
        if(dx>=0)
        {
            double d = -2*dx + dy;
            int d1 = -2*dx + 2*dy;
            int d2 = -2*dx;

            while(y<y2)
            {
                if(d<=0)
                {
                    d+=d1;
                    x++;
                    y++;
                }
                else
                {
                    d+=d2;
                    y++;
                }

                SetPixel(hdc, x, y, c);

            }
        }
        else
        {
            double d = 2*dx + dy;
            int d1 = 2*dx + 2*dy;
            int d2 = 2*dx;

            while(y<y2)
            {
                if(d<=0)
                {
                    d+=d1;
                    x--;
                    y++;
                }
                else
                {
                    d+=d2;
                    y++;
                }

                SetPixel(hdc, x, y, c);
            }
        }

    }


}

void DDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{

    int dx=(x2-x1),dy=(y2-y1);

    if(abs(dx)>=abs(dy))
    {
        if(x1 > x2)
        {
            swap(x1,x2);
            swap(y1,y2);
        }

        double slope = (double)dy/dx;

        for(double x=x1, y=y1; x<x2; x++)
        {
            y += slope;
            SetPixel(hdc, x, Round(y), c);
        }

    }
    else
    {
        if(y1 > y2)
        {
            swap(x1,x2);
            swap(y1,y2);
        }

        SetPixel(hdc, x1, y1, c);

        double slope = (double)dx/dy;

        for(double x=x1, y=y1; y<y2; y++)
        {
            x += slope;
            SetPixel(hdc, Round(x), y , c);
        }
    }
}

////end draw line
////start ellipse
void polar_ellipse(HDC hdc,int xc,int yc,int r,int r2,COLORREF c)
{
    double dtheta=1.0/r;
    for(double theta=0.0; theta<6.28; theta+=dtheta)
    {
        double x=xc+r2*cos(theta);
        double y=yc+r*sin(theta);
        SetPixel(hdc,Round(x),Round(y),c);
    }
}
void Draw4Points(HDC hdc, int x, int y, int xc,int yc, COLORREF c)
{


    SetPixel(hdc, xc+x, yc+y, c);
    SetPixel(hdc, xc-x, yc+y, c);
    SetPixel(hdc, xc-x, yc-y, c);
    SetPixel(hdc, xc+x, yc-y, c);

}
void direct(HDC hdc,int xc,int yc,int r,int r2,COLORREF c)
{

       double x=r;
       int y=0;
      Draw4Points( hdc,  r,  0,  xc, yc,  c);
       while(x>=0)
       {
         x-=0.01;
       y=abs((int)(r2*sqrt(1-(pow(x,2)/pow(r,2)))));
        Draw4Points( hdc,  x,  y,  xc, yc,  c);

       }
}
void midpoint_ellipse(HDC hdc, int xc, int yc, float rx, float  ry,COLORREF c)
{
    float x=0;
    float y=ry;
    float dx=0;
    float dy=(pow(rx,2)*2*ry);
    float d=(int)(pow(ry,2)-(pow(rx,2)*ry)+(0.25*pow(rx,2))+0.5);
     Draw4Points(hdc,x,y,xc,yc,c);
     while(dx<dy)
      {
         x++;
         dx+=pow(ry,2)*2;
         if(d<0)
        d+=(dx+pow(ry,2));
         else
        {
           y--;
           dy-=pow(rx,2)*2;
           d+=(dx+pow(ry,2)-dy);
        }
    Draw4Points(hdc,x,y,xc,yc,c);
      }
  d=(int) ((pow(ry,2)*(x+0.5)*(x+0.5))+(pow(rx,2)*pow((y-1),2))-(pow(rx,2)*pow(ry,2)+0.5));
   while(y>0)
      {
          y--;
         dy-=pow(rx,2)*2;
         if(d>=0)
        d+=(pow(rx,2)-dy);
         else
        {
           x++;
           dx+=2*pow(ry,2);
           d+=(dx+pow(rx,2)-dy);
        }
Draw4Points(hdc,x,y,xc,yc,c);}}

///////end ellipse

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hloadInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are stored */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Code::Blocks Template Windows App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               700,                 /* The programs width */
               700,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );
    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}
HWND hEdit;
addwindow(HWND hwnd){
//CreateWindowW(L"Button",L"show",WS_VISIBLE | WS_CHILD,10,10,150,30,hwnd,(HMENU)12,NULL,NULL);
hEdit = CreateWindowW(L"Edit",L"",WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER | ES_AUTOVSCROLL ,0,0,300,620,hwnd,NULL,NULL,NULL);
}

void displayfile(){
FILE *file;
file=fopen("steps.txt","rb");

fseek(file,0,SEEK_END);

int sizee=ftell(file);

rewind(file);
char *data =new char(sizee+1);

fread(data,sizee,1,file);
//data[sizee]='\0';
SetWindowText(hEdit,data);
}
/*  This function is called by the Windows function DispatchMessage()  */
HMENU hmenu;
void addmenu(HWND hwnd){
    hmenu=CreateMenu();
    HMENU submenu=CreateMenu();
    HMENU submenu2=CreateMenu();
    HMENU submenu3=CreateMenu();
    //HMENU submenu4=CreateMenu();
    AppendMenu(submenu,MF_STRING,1,"convex filling");
    AppendMenu(submenu,MF_POPUP,(UINT_PTR)submenu2,"line");
    AppendMenu(submenu2,MF_STRING,2,"DDA");
    AppendMenu(submenu2,MF_STRING,3,"midpoint");
    AppendMenu(submenu2,MF_STRING,4,"parametric");
    AppendMenu(submenu,MF_POPUP,(UINT_PTR)submenu3,"ellipse");
    AppendMenu(submenu3,MF_STRING,5,"Direct");
    AppendMenu(submenu3,MF_STRING,6,"polar");
    AppendMenu(submenu3,MF_STRING,7,"midpoint");
    AppendMenu(submenu,MF_STRING,8,"clipping");
    AppendMenu(submenu,MF_STRING,12,"steps");
    AppendMenu(hmenu,MF_POPUP,(UINT_PTR)submenu,"Draw");
    AppendMenu(hmenu,MF_STRING,9,"Save");
    AppendMenu(hmenu,MF_STRING,10,"Load");
    AppendMenu(hmenu,MF_STRING,11,"Exit");
    SetMenu(hwnd,hmenu);
}
static int test;
static int xr,yr;
double X1,Y1,X2,Y2,RE,RE2;
int alpha=180.0;

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int line;
    string stat;
    ifstream in,in2;
    HDC hdc;
    hdc=GetDC(hwnd);
    switch (message)                  /* handle the messages */
    {
    case WM_COMMAND:
        switch(wParam){
        case 1:
            test=1;
            cout<<"Convex filling"<<endl;
            cout<<"select 5 points using left button"<<endl;
            break;
        case 2:
            test=2;
            cout<<"DDA for draw line"<<endl;
            cout<<"use LBUTTONDOWN to record start point"<<endl;
            cout<<"use RBUTTONDOWN to record end point"<<endl;

            break;
        case 3:
            test=3;
            cout<<"midpoint for draw line"<<endl;
            cout<<"use LBUTTONDOWN to record start point"<<endl;
            cout<<"use RBUTTONDOWN to record end point"<<endl;
            break;
        case 4:
            test=4;
            cout<<"parametric for draw line"<<endl;
            cout<<"use LBUTTONDOWN to record start point"<<endl;
            cout<<"use RBUTTONDOWN to record end point"<<endl;
            break;
        case 5:
            test=5;
            cout<<"Direct for draw ellipse"<<endl;
            cout<<"use LBUTTONDOWN to record "<<endl;
            cout<<"use RBUTTONDOWN to record "<<endl;
            break;
        case 6:
            test=6;
            cout<<"polar for draw ellipse"<<endl;
            cout<<"use LBUTTONDOWN to record "<<endl;
            cout<<"use RBUTTONDOWN to record "<<endl;
            break;
        case 7:
            test=7;
            cout<<"midpoint for draw ellipse"<<endl;
            cout<<"use LBUTTONDOWN to record "<<endl;
            cout<<"use RBUTTONDOWN to record "<<endl;
            break;
        case 8:
            test=8;
            cout<<"clipping"<<endl;
            cout<<"swap left button to draw circle window"<<endl;
            cout<<"swap right button to draw line window"<<endl;
            break;
        case 9:
           {
             test=9;
       ofstream out,out2;
       out.open("text.txt");
       for(int i = 0; i<savedpoints.size(); i++)
        {out << savedpoints[i] <<endl;}
    out.close();

    out2.open("text2.txt");
       for(int i = 0; i<savedstate.size(); i++)
        {out2 << savedstate[i] <<endl;}
         out2.close();

           }
            break;
        case 10:
            {test=10;
            in.open("text.txt");
            while(in>>line){
            savedpoints.push_back(line);}
            in.close();

            in2.open("text2.txt");
            while(in2>>stat){
            savedstate.push_back(stat);}
            in2.close();
            int n=0;
            for(int i=0;i<savedstate.size();i++){
            if(savedstate[i]=="convex"){
                    int j=0;
                    p1.x=savedpoints[j];
                    p1.y=savedpoints[j+1];
                    vertcies.push_back(p1);
                    p2.x=savedpoints[j+2];
                    p2.y=savedpoints[j+3];
                    vertcies.push_back(p2);
                    p3.x=savedpoints[j+4];
                    p3.y=savedpoints[j+5];
                    vertcies.push_back(p3);
                    p4.x=savedpoints[j+6];
                    p4.y=savedpoints[j+7];
                    vertcies.push_back(p4);
                    p5.x=savedpoints[j+8];
                    p5.y=savedpoints[j+9];
                    vertcies.push_back(p5);
                    mainAlgorithm(hdc,RGB(255,0,0));
                    n=+10;

            }
            else if(savedstate[i]=="DDA")
             {int X1,Y1,X2,Y2;
             X1=savedpoints[n];
             Y1=savedpoints[n+1];
             X2=savedpoints[n+2];
             Y2=savedpoints[n+3];
        DDA(hdc,X1,Y1,X2,Y2,RGB(255,0,0));

                 }
            }}
            break;
        case 11:
            DestroyWindow(hwnd);
            break;
        case 12:
            //openfile(hwnd);
            test=12;
            displayfile();
            break;
            }
break;
    case WM_CREATE:
         addmenu(hwnd);
        addwindow(hwnd);
         break;
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    case WM_LBUTTONDOWN:

        if(test==1){

        if(counter==0)
        {
            p1.x=LOWORD(lParam);
            p1.y=HIWORD(lParam);
            counter++;
            vertcies.push_back(p1);
            savedpoints.push_back(p1.x);
            savedpoints.push_back(p1.y);
        }
        else  if(counter==1)
        {
            p2.x=LOWORD(lParam);
            p2.y=HIWORD(lParam);
            counter++;
            vertcies.push_back(p2);
            savedpoints.push_back(p2.x);
            savedpoints.push_back(p2.y);
        }
        else if(counter==2)
        {
            p3.x=LOWORD(lParam);
            p3.y=HIWORD(lParam);
            counter++;
            vertcies.push_back(p3);
            savedpoints.push_back(p3.x);
            savedpoints.push_back(p3.y);

        }
        else if(counter==3)
        {
            p4.x=LOWORD(lParam);
            p4.y=HIWORD(lParam);
            counter++;
            vertcies.push_back(p4);
            savedpoints.push_back(p4.x);
            savedpoints.push_back(p4.y);
        }
        else if(counter==4)
        {
            p5.x=LOWORD(lParam);
            p5.y=HIWORD(lParam);
            counter++;
            vertcies.push_back(p5);
            savedpoints.push_back(p5.x);
            savedpoints.push_back(p5.y);
            savedstate.push_back("convex");
            mainAlgorithm(hdc,RGB(255,0,0));
        }}
        else if(test==8){
        xc=LOWORD(lParam);
        yc=HIWORD(lParam);
        savedpoints.push_back(xc);
        savedpoints.push_back(yc);
        }
        else if(test==2||test==3||test==4||test==5||test==6||test==7){
        X1=LOWORD(lParam);
        Y1=HIWORD(lParam);
        savedpoints.push_back(X1);
        savedpoints.push_back(Y1);
        }
        break;
	case WM_LBUTTONUP:
	    if(test==8){
		xr=LOWORD(lParam);
        yr=HIWORD(lParam);
        R = sqrt((pow(xr-xc,2) + pow(yr-yc,2))) ;
        DrawCircle(hdc,xc,yc,R,RGB(255,0,0));

		}
		else if(test==5||test==6||test==7){
        X2=LOWORD(lParam);
        Y2=HIWORD(lParam);
        RE = Round(sqrt(pow(X2-X1,2.0)+pow(Y2-Y1,2.0)));
        }
		break;

	case WM_RBUTTONDOWN:
	    if(test==8){
		X1=LOWORD(lParam);
        Y1=HIWORD(lParam);}
		else if(test==2){
        X2=LOWORD(lParam);
        Y2=HIWORD(lParam);
        savedpoints.push_back(X2);
        savedpoints.push_back(Y2);
        savedstate.push_back("DDA");
        DDA(hdc,X1,Y1,X2,Y2,RGB(255,0,0));
		}
		else if(test==3){
        X2=LOWORD(lParam);
        Y2=HIWORD(lParam);
        MidPoint(hdc,X1,Y1,X2,Y2,RGB(255,0,0));
		}
		else if(test==4){
        X2=LOWORD(lParam);
        Y2=HIWORD(lParam);
        Parametric(hdc,X1,Y1,X2,Y2,RGB(255,0,0));
		}
		else if(test==5){
        xr = LOWORD(lParam);
        yr = HIWORD(lParam);
        RE2 = Round(sqrt(pow(xr-X1,2.0)+pow(yr-Y1,2.0)));
        direct(hdc, X1, Y1, RE,RE2 ,RGB(255,0,0));
		}
		else if(test==6){
        xr= LOWORD(lParam);
        yr= HIWORD(lParam);
        RE2 = Round(sqrt(pow(xr-X1,2.0)+pow(yr-Y1,2.0)));
        polar_ellipse(hdc, X1,Y1, RE,RE2, RGB(255,0,0));
		}
		else if(test==7){
        xr= LOWORD(lParam);
        yr= HIWORD(lParam);
        RE2 = Round(sqrt(pow(xr-X1,2.0)+pow(yr-Y1,2.0)));
        midpoint_ellipse(hdc,X1,Y1,RE,RE2,RGB(255,0,0));
		}
		break;
	case WM_RBUTTONUP:
	    if(test==8){
		X2=LOWORD(lParam);
        Y2=HIWORD(lParam);
        Clipping(X1,Y1,X2,Y2,xc-R,xc+R,yc+R,yc-R,hdc);}
		break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }
ReleaseDC(hwnd,hdc);
    return 0;
}
