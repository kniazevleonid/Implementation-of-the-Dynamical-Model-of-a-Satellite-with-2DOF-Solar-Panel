#include <stdio.h>
//#include "stdafx.h"
#include <math.h>
#include <iostream>
#include <cmath>
#include <stdlib.h>

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
                case 7: return y[11];break;
                case 8: return y[11];break;
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
void step(int n,double x,double h,double * y, double(f)(int i, double x, double * y), int flag)
{
	int i=0;
	double * yt = new double[n];
	double * k1 = new double[n];
	double * k2 = new double[n];
	double * k3 = new double[n];
	double * k4 = new double[n];

	for(i = 0; i < n; i++)
	{
		k1[i] = h*f(i, x, y);
	}
	for(i = 0; i < n; i++)
	{
		yt[i] = y[i]+0.5*k1[i];
	}
	for(i = 0; i < n; i++)
	{
		k2[i] = h*f(i, x+h*0.5, yt);
	}
	for(i = 0; i < n; i++)
	{
		yt[i] = y[i]+0.5*k2[i];
	}
	for(i = 0; i < n; i++)
	{
		k3[i] = h*f(i, x+h*0.5, yt);
	}
	for(i = 0; i < n; i++)
	{
		yt[i] = y[i]+k3[i];
	}
	for(i = 0; i < n; i++)
	{
		k4[i] = h*f(i, x+h, yt);
	}
	for(i = 0; i < n; i++)
	{
		y[i] = y[i]+(k1[i]+2.0*k2[i]+2.0*k3[i]+k4[i])/6;
	}
	if (flag = 1)
	{
		double modul = sqrt(y[3]*y[3]+y[4]*y[4]+y[5]*y[5]+y[6]*y[6]);
		for(i = 3; i < 7; i++)
		{
			y[i] = y[i]/modul;
		}
	}
}
/*************************************************************************
  Алгоритм решает систему диффуров y[i]'=F(i,x,y) для i=1..n
  методом Рунге-Кутта 4 порядка.

  Начальная точка имеет кординаты (x,y[1], ..., y[n])

  До конечной точки мы добираемся через n промежуточных
  с постоянным шагом h=(x1-x)/m

  Результат помещается в переменную result[4]
 *************************************************************************/
void solvesystemrungekutta(int n,double x,double x1,int steps,double * result, double (f)(int x, double x, double * y)){

	for(int i = 0; i < steps; i++)
	{
		step(n, x+i*(x1-x)/steps, (x1-x)/steps, result, f, 0);
	}
}

double dist_pl(double sunvec[3], double result[6]){
	return sqrt(pow((sunvec[1]*result[0]-sunvec[0]*result[1]),2)+pow((sunvec[2]*result[1]-sunvec[1]*result[2]),2)+pow((sunvec[0]*result[2]-sunvec[2]*result[0]),2))/sqrt(sunvec[2]*sunvec[2]+sunvec[1]*sunvec[1]+sunvec[0]*sunvec[0]);
};

int main(){
	double result[6];
	double sunvec[3];
	
//Задаём направление на Солнце
	sunvec[0] = 0;
	sunvec[1] = -1;
	sunvec[2] = 0;

	result[0] = 0;
	result[1] = 1;
	result[2] = 0;
	result[3] = 1;
	result[4] = 0;
	result[5] = 0;
	solvesystemrungekutta(6,0,3.1415,10000,result, f);
	for(int i = 0; i < 6; i++){
		cout<<result[i]<<endl;
	}

	if((dist_pl(sunvec, result) < R)&&((sunvec[0]*result[0]+sunvec[1]*result[1]+sunvec[2]*result[2]) > 0))
		cout<<"SHADOW"<<endl;
	else
		cout<<"BRIGHT SIDE"<<endl;
	return 0;
}

