#pragma once
#include "Chart.h"
class VBorder
{
public:
	double value;
	int color;
	float thinck;
public:
  Chart &chart;
  VBorder(Chart &chart);
  void Draw();
  void SetCoordinate(POINTS &);
};

class HBorder
{
public:
	double &value;
	int color;	
public:
  Chart &chart;
  HBorder(Chart &chart, double &value);
  void Draw();
};