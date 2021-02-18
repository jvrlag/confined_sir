// Simple motion model
// Graphical output
// 200901
#include"Matrix.h"
#include"EX.h"

List Pal(5);


void Map(const Table &H, const Table &X, const List &S, const Vector &R, long L)
{
     static long Ncolors=10;
     static List Colors=EX_Palette(0,0,0,0.5,0.5,0,Ncolors);
     static long xsize=EX_CW->width;

     long N=S.N;
     long dx=ceil(xsize/(double)L);     
	  
     for (long ix=1;ix<=L;ix++)
	  for (long iy=1;iy<=L;iy++)
	  {
	       long n=0;
	       for (long ip=1;ip<=N;ip++)
	       {
		    double vx=fabs(ix-H(ip,1));
		    double vy=fabs(iy-H(ip,2));
		    if (vx>L/2.) vx-=L;
		    if (vy>L/2.) vy-=L;
		    double dist2=Sqr(vx)+Sqr(vy);
		    if (dist2<Sqr(R(ip))) n++;
	       }
	       if (n>Ncolors-1) n=Ncolors-1;
	       EX_Set_Color(Colors(n)+1);
	       EX_Fill_Rectangle((ix-1)*dx,xsize-(iy)*dx,dx,dx);
	  }
     for (long ip=1;ip<=N;ip++)
     {
	  double ix=(X(ip,1)-0.5)*dx;
	  double iy=xsize-(X(ip,2)-0.5)*dx;

	  EX_Set_Color(Pal(S(ip)+1));
	  EX_Fill_Circle(ix,iy,5);

     }
     EX_Flush();

}

void Plot(const Table &X, const Table &H, const List &S, long L, double r0)
{
     static long xsize=EX_CW->width;
     EX_Clear();
     long N=S.N;
     double dx=xsize/(double)L;


     for (long ip=1;ip<=N;ip++)
     {
	  double ix=(X(ip,1)-0.5)*dx;
	  double iy=xsize-(X(ip,2)-0.5)*dx;

	  double hx=(H(ip,1)-0.5)*xsize/(double)L;
	  double hy=xsize-(H(ip,2)-0.5)*xsize/(double)L;
	  EX_Set_Color(Pal(5));
	  if (abs(hx-ix)<xsize/2 && abs(hy-iy)<xsize/2)
	       EX_Line(hx,hy,ix,iy);
	  EX_Set_Color(Pal(4));
	  EX_Circle(hx,hy,r0*dx);
     }


     for (long ip=1;ip<=N;ip++)
     {
	  double ix=(X(ip,1)-0.5)*dx;
	  double iy=xsize-(X(ip,2)-0.5)*dx;

	  EX_Set_Color(Pal(S(ip)+1));
	  EX_Fill_Circle(ix,iy,5);

     }
     EX_Flush();
	  
}

int main(int argc, char *argv[])
{

     long N=160;
     double L=40; // box is LxL
     double r0=L; // L/sqrt(N) is the average distance between homes
     double pinf=1.0; // probability per unit time
     double prec=0.01;
     Input(N,"-N",argc,argv);
     Input(L,"-L",argc,argv);
     Input(r0,"-r0",argc,argv);
     Input(pinf,"-pinf",argc,argv);
     Input(prec,"-prec",argc,argv);

     long xsize=800;
     EX_Start(1000,100,xsize,xsize);
     EX_Enable_Buffer();
     Pal(1)=EX_Alloc_Named_Color("white");
     Pal(2)=EX_Alloc_Named_Color("red");
     Pal(3)=EX_Alloc_Named_Color("cyan");
     Pal(4)=EX_Alloc_RGB_Color(0.4,0.4,0.4);
     Pal(5)=EX_Alloc_RGB_Color(0,0.5,0);
     
     
     Rand_Open(time(0));
     
     Table H(N,2); // coordinates of homes
     Table X(N,2); // coordinates of people

     H(1,1)=L/2;
     H(1,2)=L/2;
     for (long i=2;i<=N;i++)
     {
	  H(i,1)=Rand_I(1,L);
	  H(i,2)=Rand_I(1,L);
     }
//     printf("Homes: \n");
//     H.Write();

     Copy(X,H); // start at home
     Vector R(N); // radii
     R.Set(r0);
     
     List S(N); // 0: susceptible, 1: infected, 2: recovered
     S(1)=1;

     long Nsteps=200000;


     for (long is=1;is<=Nsteps;is++)
     {
	  if (is%N==1)
	  {
	       Delay(0.05);
//	       Plot(X,H,S,L,r0);
	       Map(H,X,S,R,L);
	       if (is==1) EX_Read_Key();
	       printf("%ld %ld %ld %ld\n",is,S.Count(0),S.Count(1),S.Count(2));
	       if (S.Count(1)==0)
	       {
		    EX_Read_Key();
		    exit(1);
	       }

	  }

	  // printf("Time: %ld\n",is);
	  // X.Write();
	  // S.Write();
	  
	  // now, move!!
	  long ip=Rand_I(1,N);
	  long x=X(ip,1);
	  long y=X(ip,2);
	  x+=Rand_I(-1,1);
	  y+=Rand_I(-1,1);
	  if (x<1) x+=L;
	  if (x>L) x-=L;
	  if (y<1) y+=L;
	  if (y>L) y-=L;
	  double vx=fabs(x-H(ip,1));
	  double vy=fabs(y-H(ip,2));
	  if (vx>L/2.) vx-=L;
	  if (vy>L/2.) vy-=L;
	  double dist2=Sqr(vx)+Sqr(vy);
	  if (dist2<=Sqr(R(ip)))  // accept movement
	  {
	       X(ip,1)=x;
	       X(ip,2)=y;
	  }
     
	  
	  for (long ip=1;ip<=N;ip++)
	  {
	       // Check for recovery
	       if (S(ip)==1)
		    if (Rand()<prec/(double)N) S(ip)=2;
	       
	       // Check for infection
	       if (S(ip)==0)
		    for (long jp=1;jp<=N;jp++)
			 if (jp!=ip && S(jp)==1 &&
			     X(ip,1)==X(jp,1) && X(ip,2)==X(jp,2)
			     && Rand()<pinf)
			      S(ip)=1;
	  }

     }
     
	  

}
