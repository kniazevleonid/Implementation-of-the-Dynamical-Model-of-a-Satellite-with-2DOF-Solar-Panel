#include <stdio.h>
//#include "stdafx.h"
#include <math.h>
#include <iostream>
#include <cmath>
#include <stdlib.h>


mtl::dense2D<double> I2(3,3);

mtl::dense2D<double> I1(3,3);

mtl::dense_vector <double> a2_(3);

mtl::dense_vector <double> e1(3);

mtl::dense_vector <double> c(3);

mtl::dense_vector<double> f3(double *y);

mtl::dense_vector <double> a1(3);

void ff(double x, double * y, double * f, int flag);

#define M 1
#define R 1
using namespace std;
/*
double fw1(double x, double * y);
double fw2(double x, double * y);
double fw3(double x, double * y);
double fq0(double x, double * y);
double fq1(double x, double * y);
double fq2(double x, double * y);
double fq3(double x, double * y);
double fpsi1(double x, double * y);
double fpsi2(double x, double * y);
*/
/*
double f(int i, double x, double * y){
	switch (i){
		case 0: return y[3];break;
		case 1: return y[4];break;
		case 2: return y[5];break;
		case 3: return (-M*y[0])/pow(pow(y[0],2)+pow(y[1],2)+pow(y[2],2),3);break;
		case 4: return (-M*y[1])/pow(pow(y[0],2)+pow(y[1],2)+pow(y[2],2),3);break;
		case 5: return (-M*y[2])/pow(pow(y[0],2)+pow(y[1],2)+pow(y[2],2),3);break;
		default : break;
	}
}
*/
/*

double ff(int i, double x, double * y){
        switch (i){
                case 0: return fw1(x, y);break;
                case 1: return fw2(x, y);break;
                case 2: return fw3(x, y);break;
		case 3: return fq0(x, y);break;
                case 4: return fq1(x, y);break;
                case 5: return fq2(x, y);break;
		case 6: return fq3(x, y);break;
                case 7: return y[9];break;
                case 8: return y[10];break;
                case 9: return fpsi1(x, y);break;
                case 10: return fpsi2(x, y);break;
                default : break;
        }
}
*/

/*************************************************************************
  Один шаг метода Рунге-Кутта четвертого порядка для решения
  системы дифферециальных уравнений.

  procedure SystemRungeKuttStep(
  const   x   :   Real;
  const   h   :   Real;
  const   n   :   Integer;
  var     y   :   array of Real);

  Алгоритм совершает один шаг метода для системы
  диффуров y[i]'=F(i,x,y) для i=1..n

  Начальная точка имеет кординаты (x,y[1], ..., y[n])

  После выполнения алгоритма в переменной y содержится состояние
  системы в точке x+h
 *************************************************************************/
void step(int n,double x,double h,double * y, int flag)
{
	int i=0;
	double * yt = new double[n];
	double * k1 = new double[n];
	double * k2 = new double[n];
	double * k3 = new double[n];
	double * k4 = new double[n];

    double * f  = new double[n];

    ff(x,y,f,flag);

	for(i = 0; i < n; i++)
	{
//		k1[i] = h*f(i, x, y);
        k1[i] = h*f[i];
    }
	for(i = 0; i < n; i++)
	{
		yt[i] = y[i]+0.5*k1[i];
	}

    ff(x+h*0.5, yt,f,flag);

	for(i = 0; i < n; i++)
    {
//		k2[i] = h*f(i, x+h*0.5, yt);
        k2[i] = h*f[i];
    }

	for(i = 0; i < n; i++)
	{
		yt[i] = y[i]+0.5*k2[i];
	}

    ff(x+h*0.5, yt,f,flag);

	for(i = 0; i < n; i++)
	{
//		k3[i] = h*f(i, x+h*0.5, yt);
        k3[i] = h*f[i];
	}
	for(i = 0; i < n; i++)
	{
		yt[i] = y[i]+k3[i];
	}

    ff(x+h, yt,f,flag);

	for(i = 0; i < n; i++)
    {
//		k4[i] = h*f(i, x+h, yt);
        k4[i] = h*f[i];
	}
	for(i = 0; i < n; i++)
	{
		y[i] = y[i]+(k1[i]+2.0*k2[i]+2.0*k3[i]+k4[i])/6;
	}
    if (flag == 1)
	{
		double modul = sqrt(y[3]*y[3]+y[4]*y[4]+y[5]*y[5]+y[6]*y[6]);
        if (modul != 0)
        {
            for(i = 3; i < 7; i++)
            {
                y[i] = y[i]/modul;
            }
        }
	}

    delete[] yt;
    delete[] k1;
    delete[] k2;
    delete[] k3;
    delete[] k4;
    delete[] f;

}
/*************************************************************************
  Алгоритм решает систему диффуров y[i]'=F(i,x,y) для i=1..n
  методом Рунге-Кутта 4 порядка.

  Начальная точка имеет кординаты (x,y[1], ..., y[n])

  До конечной точки мы добираемся через n промежуточных
  с постоянным шагом h=(x1-x)/m

  Результат помещается в переменную result[4]
 *************************************************************************/
void solvesystemrungekutta(int n,double x,double x1,int steps,double * result, int flag){

	for(int i = 0; i < steps; i++)
	{
        step(n, x+i*(x1-x)/steps, (x1-x)/steps, result, flag);
	}
}

double dist_pl(double sunvec[3], double result[6]){
	return sqrt(pow((sunvec[1]*result[0]-sunvec[0]*result[1]),2)+pow((sunvec[2]*result[1]-sunvec[1]*result[2]),2)+pow((sunvec[0]*result[2]-sunvec[2]*result[0]),2))/sqrt(sunvec[2]*sunvec[2]+sunvec[1]*sunvec[1]+sunvec[0]*sunvec[0]);
};

mtl::dense2D<double> K(mtl::dense_vector <double> a, mtl::dense_vector <double> b)
{
     using namespace mtl;
     dense2D<double> A(3, 3);
     A(0,0)= a[1]*b[1]+a[2]*b[2];
     A(0,1)= - a[1]*b[0];
     A(0,2)= - a[2]*b[0];

     A(1,0)= - a[0]*b[1];
     A(1,1)= a[0]*b[0]+a[2]*b[2];
     A(1,2)= - a[2]*b[1];

     A(2,0)= - a[0]*b[2];
     A(2,1)= - a[1]*b[2];
     A(2,2)= a[0]*b[0]+a[1]*b[1];

     return A;
}

mtl::dense2D<double> OMEGA(double *y)
{
    using namespace mtl;
    dense2D<double> A(4, 4);
    A(0,0)= 0;
    A(0,1)= y[2];
    A(0,2)= -y[1];
    A(0,3)= y[0];

    A(1,0)= -y[2];
    A(1,1)= 0;
    A(1,2)= y[0];
    A(1,3)= y[1];

    A(2,0)= y[1];
    A(2,1)= -y[0];
    A(2,2)= 0;
    A(2,3)= y[2];

    A(3,0)= -y[0];
    A(3,1)= -y[1];
    A(3,2)= -y[2];
    A(3,3)= 0;
    return A;
}

mtl::dense_vector<double> q(double *y)
{
    using namespace mtl;
    dense_vector <double> v(4);

    v=0.0;

    int i;
    for (i=0;i<4;i++)
    {
        v(i)=y[i+3];
    }

    return v;
}

mtl::dense_vector<double> dq(double *y)
{
    using namespace mtl;
    dense_vector <double> v(4);

    v=0.0;

    v=0.5*OMEGA(y)*q(y);

    return v;
}

mtl::dense2D<double> B3(double *y)
{
    using namespace mtl;
    dense2D<double> A(3, 3);

    A(0,0)= 1.0;
    A(0,1)= 0.0;
    A(0,2)= 0.0;

    A(1,0)= 0.0;
    A(1,1)= cos(y[8]);
    A(1,2)= sin(y[8]);

    A(2,0)= 0.0;
    A(2,1)= -sin(y[8]);
    A(2,2)= cos(y[8]);

    return A;
}

mtl::dense2D<double> B1(double *y)
{
    using namespace mtl;
    dense2D<double> A(3, 3);

    A(0,0)= cos(y[7]);
    A(0,1)= 0.0;
    A(0,2)= -sin(y[7]);

    A(1,0)= 0.0;
    A(1,1)= 1.0;
    A(1,2)= 0.0;

    A(2,0)= sin(y[7]);
    A(2,1)= 0.0;
    A(2,2)= cos(y[7]);

    return A;
}

mtl::dense2D<double> J2(double *y)
{
    using namespace mtl;
    dense2D<double> A(3, 3);

    A=0.0;

    A=trans(B1(y))*trans(B3(y)) * I2 * B3(y) * B1(y);

    return A;
}

mtl::dense_vector<double> alpha2(double *y)
{
    using namespace mtl;
    dense_vector <double> v(3);

    v=0.0;

    v=dense_vector <double>(trans(B1(y)) * dense_vector <double>(trans(B3(y))*a2_));

    return v;
}

mtl::dense_vector<double> omega1(double *y)
{
    using namespace mtl;
    dense_vector <double> v(3);

    v=0.0;

    int i;
    for (i=0;i<3;i++)
    {
        v(i)=y[i];
    }

    return v;
}

mtl::dense_vector<double> f1(double *y)
{
    using namespace mtl;
    dense_vector <double> v(3);

    v=0.0;

    dense_vector <double> v1(3);
    v1= I1*omega1(y);

    v=cross(omega1(y), v1);

    return v;
}

mtl::dense_vector<double> e3(double *y)
{
    using namespace mtl;
    dense_vector <double> v(3);

    v=0.0;

    dense_vector <double> v1(3);

    v1=0.0;
    v1(0)=1.0;

    v=trans(B3(y))*v1;

    return v;
}

mtl::dense_vector<double> w2(double *y)
{
    using namespace mtl;
    dense_vector <double> v(3);

    v=0.0;
    v=omega1(y) + e1 * y[9] + dense_vector <double>(trans(B1(y)) * dense_vector <double>(e3(y) * y[10]));

    return v;
}

mtl::dense_vector<double> f2(double *y)
{
    using namespace mtl;
    dense_vector <double> v(3);

    v=0.0;

    dense_vector <double> v1(3);
    v1= J2(y)*w2(y);

    v=cross(w2(y), v1) + J2(y)*f3(y);

    return v;
}

mtl::dense_vector<double> f3(double *y)
{
    using namespace mtl;
    dense_vector <double> v(3);

    v=0.0;

    dense_vector <double> v1(3);

    v1=0.0;
    v1=e1*y[9];

    dense_vector <double> v2(3);

    v2=0.0;
    v2=omega1(y)+e1*y[9];

    dense_vector <double> v3(3);

    v3=0.0;
    v3=dense_vector <double>(trans(B1(y)) * dense_vector <double>(e3(y)*y[10]));

    v=cross(omega1(y),v1) + cross(v2, v3);

    return v;
}

mtl::dense_vector<double> f4(double *y)
{
    using namespace mtl;
    dense_vector <double> v(3);

    v=0.0;

    dense_vector <double> v1(3);

    v1=0.0;
    v1=cross(omega1(y), a1);

    dense_vector <double> v2(3);

    v2=0.0;
    v2=cross(w2(y), alpha2(y));

    v=cross(f3(y), alpha2(y))+ cross(omega1(y),v1) + cross(w2(y), v2);

    return v;
}

mtl::dense_vector <double> v(double *y)
{
    using namespace mtl;
    dense_vector <double> v(5);

    v=0.0;

    dense_vector <double> v1(3);

    v1=0.0;
    v1=-f1(y)-f2(y)-cross(a1, f4(y))-cross(alpha2(y), f4(y));

    double v2=-dot(f2(y), e1)-dot(cross(alpha2(y), f4(y)), e1);

    double v3=-dot(f2(y), e3(y))-dot(cross(alpha2(y), f4(y)), dense_vector <double> (trans(B1(y))*e3(y)));

    int i;
    for(i=0;i<3;i++)
    {
        v(i)=v1(i);
    }

    v(3)=v2;
    v(4)=v3;

    return v;
}

mtl::dense2D<double> S(double *y)
{
    using namespace mtl;
    dense2D<double> A(5, 5);

    A=0.0;

    dense2D<double> B(3, 3);
    B=0.0;

    B=I1+J2(y)+K(dense_vector<double>(a1+alpha2(y)),dense_vector<double>(a1+alpha2(y)));

    dense_vector<double> v1(3);
    v1=(dense2D <double>(J2(y)+K(dense_vector<double>(a1+alpha2(y)),alpha2(y))))*e1;

    dense_vector<double> v2(3);
    v2=(dense2D <double>(J2(y)+K(dense_vector<double>(a1+alpha2(y)),alpha2(y))))*dense_vector <double>(trans(B1(y))*e3(y));

    dense_vector<double> v3(3);
    v3=trans((dense2D <double>(J2(y)+K(alpha2(y), dense_vector<double>(a1+alpha2(y))))))*e1;

    dense_vector<double> v4(3);
    v4=trans((dense2D <double>(J2(y)+K(alpha2(y), dense_vector<double>(a1+alpha2(y))))))*dense_vector <double>(trans(B1(y))*e3(y));

    double v5;

    v5=dot(e1, dense_vector <double>(J2(y)*e1)) + pow(two_norm(cross(e1, alpha2(y))), 2);

    double v6;

    v6=dot(e1, dense_vector <double>(J2(y)*dense_vector <double>(trans(B1(y))*e3(y)))) - dot(e1, alpha2(y)) * dot (dense_vector <double>(trans(B1(y))*e3(y)), alpha2(y));

    double v7;

    v7=dot(dense_vector <double>(J2(y)*e1), dense_vector <double>(trans(B1(y))*e3(y))) - dot(e1, alpha2(y)) * dot (dense_vector <double>(trans(B1(y))*e3(y)), alpha2(y));

    double v8;

    v8=dot(dense_vector <double>(trans(B1(y))*e3(y)), dense_vector <double>(J2(y)*dense_vector <double>(trans(B1(y))*e3(y)))) + pow(two_norm(cross(dense_vector <double>(trans(B1(y))*e3(y)), alpha2(y))), 2);

    int i, j;

    for (i=0;i<3;i++)
    {
        for (j=0;j<3;j++)
            A(i,j)=B(i,j);
    }

    for (i=0;i<3;i++)
    {
        A(3,i)=v3(i);
        A(4,i)=v4(i);
        A(i,3)=v1(i);
        A(i,4)=v2(i);
    }

    A(3,3) = v5;
    A(3,4) = v6;
    A(4,3) = v7;
    A(4,4) = v8;

    return A;
}


void ff(double x, double * y, double * f, int flag)
{
    if (flag == 1)
    {
        mtl::dense_vector <double> omegapsi(5);
        omegapsi = mtl::mat::inv(S(y))*v(y);

        int i;
        for (i=0;i<3;i++)
        {
            f[i]=omegapsi(i);
        }

        for (i=3;i<7;i++)
        {
            f[i]=dq(y)[i-3];
        }

        f[7] = y[9];
        f[8] = y[10];

        f[9] = omegapsi(3);
        f[10] = omegapsi(4);
        return;
    }

    int i;
    for (i=0;i<3;i++)
    {
        f[i] = y[i+3];
        f[i+3] = (-M*y[i])/pow(pow(y[0],2)+pow(y[1],2)+pow(y[2],2),3);
    }
    return;
}

mtl::dense2D<double> Qmatrix(mtl::dense_vector <double> q)
{
    mtl::dense2D<double> B(3, 3);

    B(0,0)=1.0 - 2.0 * q[2]*q[2] - 2.0 * q[3] * q[3];
    B(0,1)=2.0 * (q[1]*q[2]+q[0]*q[3]);
    B(0,2)=2.0 * (q[1]*q[3]-q[0]*q[2]);

    B(1,0)=2.0 * (q[1]*q[2]-q[0]*q[3]);
    B(1,1)=1.0 - 2.0 * q[1]*q[1] - 2.0 * q[3] * q[3];
    B(1,2)=2.0 * (q[2]*q[3]+q[0]*q[1]);

    B(2,0)=2.0 * (q[1]*q[3]+q[0]*q[2]);
    B(2,1)=2.0 * (q[2]*q[3]-q[0]*q[1]);
    B(2,2)=1.0 - 2.0 * q[1]*q[1] - 2.0 * q[2] * q[2];

    return B;
}

mtl::dense_vector <double> Ansi(mtl::dense_vector <double> ai, double * y)
{
    mtl::dense_vector <double> xi(3);
    xi=mtl::dense_vector <double> (trans(B1(y))*mtl::dense_vector <double> (trans(B3(y))*(a2_+ai)));

    mtl::dense_vector <double> Ansi(3);

    Ansi = mtl::dense_vector <double> (a1 + mtl::dense_vector <double> (xi - c));

    double temp = Ansi[1];
    Ansi[1]=Ansi[2];
    Ansi[2]=-temp;

    return Ansi;
}
