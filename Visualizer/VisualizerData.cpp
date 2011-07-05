#include "VisualizerData.h"
#include <QMutexLocker>
VisualizerData::VisualizerData(void)
: m_style(LINE), m_visible(true), m_transparent(true)
{
	m_color[0]=m_color[1]=m_color[2]=0.f;
}

VisualizerData::VisualizerData(const VisualizerData& c) 
	: m_style(c.m_style), m_name(c.m_name), m_description(c.m_description),
	  m_unit(c.m_unit), m_visible(c.m_visible), m_transparent(c.m_transparent)
{
	m_color[0] = c.m_color[0];
	m_color[1] = c.m_color[1];
	m_color[2] = c.m_color[2];
}

VisualizerData::~VisualizerData(void)
{
}


void VisualizerData::setName(string name){
	QMutexLocker locker(&m_mutex);
	m_name = name;
}

string VisualizerData::getName(){
	QMutexLocker locker(&m_mutex);
	return m_name;
}

void VisualizerData::setDescription(string description){
	QMutexLocker locker(&m_mutex);
	m_description = description;
}

string VisualizerData::getDescription(){
	QMutexLocker locker(&m_mutex);
	return m_description;
}

void VisualizerData::setColor(float red, float green, float blue){
	QMutexLocker locker(&m_mutex);
	m_color[0] = red;
	m_color[1] = green;
	m_color[2] = blue;
}

void VisualizerData::setColorInt(unsigned int red, unsigned int green, unsigned int blue){
	QMutexLocker locker(&m_mutex);
	m_color[0] = red/255.f;
	m_color[1] = green/255.f;
	m_color[2] = blue/255.f;
}

float VisualizerData::getColorRed(){
	return m_color[0];
}

float VisualizerData::getColorGreen(){
	return m_color[1];
}

float VisualizerData::getColorBlue(){
	return m_color[2];
}

void VisualizerData::setStyle(Style style){
	m_style = style;
}

VisualizerData::Style VisualizerData::getStyle(){
	return m_style;
}

void VisualizerData::setUnit(string unit){
	m_unit = unit;
}

string VisualizerData::getUnit(){
	return m_unit;
}

bool VisualizerData::isVisible(){
	return m_visible;
}

void VisualizerData::setVisible(bool v){
	m_visible = v;
}

bool VisualizerData::isTransparent(){
	return m_transparent;
}

void VisualizerData::setTransparent(bool v){
	m_transparent = v;
}