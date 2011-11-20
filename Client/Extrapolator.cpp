#include "Extrapolator.h"
#include <cassert>

template<int count, typename user_type>
CExtrapolator<count, user_type>::CExtrapolator()
{
	user_type pos[count];
	memset(pos, 0, sizeof(pos));
	Reset(0, 0, pos);
}

template<int count, typename user_type>
CExtrapolator<count, user_type>::~CExtrapolator()
{
}

template<int count, typename user_type>
bool CExtrapolator<count, user_type>::AddSample(double packetTime, double current_time, 
												user_type const pos[count])
{
	//  The best guess I can make for velocity is the difference between 
	//  this sample and the last registered sample.
	user_type vel[count];
	
	if (fabs(packetTime - m_lastPacketTime) > 1e-4) 
	{
		double delta = 1.0 / (packetTime - m_lastPacketTime);
		for (int i = 0; i < count; ++i) 
			vel[i] = (user_type)((pos[i] - m_lastPacketPos[i]) * delta);
	}
	else 
		memset(vel, 0, sizeof(vel));

	return AddSample(packetTime, current_time, pos, vel);
}

template<int count, typename user_type>
bool CExtrapolator<count, user_type>::AddSample(double packetTime, double current_time, 
										   user_type const pos[count], user_type const vel[count])
{
	if (!Estimates(packetTime, current_time)) 
		return false;

	memcpy(m_lastPacketPos, pos, sizeof(m_lastPacketPos));

	m_lastPacketTime = packetTime;
	ReadPosition(current_time, m_snapPosition);

	m_aimTime = current_time + m_updateTime;
	double dt = m_aimTime - packetTime;
	m_snapTime = current_time;

	for (int i = 0; i < count; ++i) 
		m_aimPos[i] = (user_type)(pos[i] + vel[i] * dt);
  
	/*
		Calculate velocity using the two sets of positions and times.
	*/
	if (fabs(m_aimTime - m_snapTime) < 1e-4) 
		for (int i = 0; i < count; ++i) 
			m_snapVelocity[i] = vel[i];
	else 
	{
		double delta = 1.0 / (m_aimTime - m_snapTime);
  
		for (int i = 0; i < count; ++i) 
		m_snapVelocity[i] = (user_type)((m_aimPos[i] - m_snapPosition[i]) * delta);
	}

	return true;
}

template<int count, typename user_type>
void CExtrapolator<count, user_type>::Reset(double packetTime, double current_time, 
											user_type const pos[count])
{
	user_type vel[count];
	memset(vel, 0, sizeof(vel));
	Reset(packetTime, current_time, pos, vel);
}

template<int count, typename user_type>
void CExtrapolator<count, user_type>::Reset(double packetTime, double curTime, 
									   user_type const pos[count], user_type const vel[count])
{
	assert(packetTime <= curTime);
	
	m_lastPacketTime = packetTime;
	memcpy(m_lastPacketPos, pos, sizeof(m_lastPacketPos));
	
	m_snapTime = curTime;
	memcpy(m_snapPosition, pos, sizeof(m_snapPosition));
	
	m_updateTime = curTime - packetTime;
	m_latency = m_updateTime;
	m_aimTime = curTime + m_updateTime;
	memcpy(m_snapVelocity, vel, sizeof(m_snapVelocity));
	
	for (int i = 0; i < count; ++i) 
		m_aimPos[i] = (user_type)(m_snapPosition[i] + m_snapVelocity[i] * m_updateTime);
}

template<int count, typename user_type>
bool CExtrapolator<count, user_type>::ReadPosition(double forTime, user_type pos[count]) const
{
	user_type vel[count];
	return ReadPosition(forTime, pos, vel);
}

template<int count, typename user_type>
bool CExtrapolator<count, user_type>::ReadPosition(double forTime, user_type pos[count], user_type vel[count]) const
{
	bool ok = true;

	if (forTime < m_snapTime) 
	{
		forTime = m_snapTime;
		ok = false;
	}

	double maxRange = m_aimTime + m_updateTime;
	if (forTime > maxRange) 
	{
		forTime = maxRange;
		ok = false;
	}

	//  calculate the interpolated position
	for (int i = 0; i < count; ++i) 
	{
		vel[i] = m_snapVelocity[i];
		pos[i] = (user_type)(m_snapPosition[i] + vel[i] * (forTime - m_snapTime));
	}

	if (!ok) 
		memset(vel, 0, sizeof(vel));
	
	return ok;
}

template<int count, typename user_type>
double CExtrapolator<count, user_type>::EstimateLatency() const
{
	return m_latency;
}

template<int count, typename user_type>
double CExtrapolator<count, user_type>::EstimateUpdateTime() const
{
	return m_updateTime;
}

template<int count, typename user_type>
bool CExtrapolator<count, user_type>::Estimates(double packet, double current)
{
	if (packet <= m_lastPacketTime) 
		return false;

	double lat = current - packet;
	if (lat < 0) 
		lat = 0;
	
	if (lat > m_latency) 
		m_latency = (m_latency + lat) * 0.5;
	else 
		m_latency = (m_latency * 7 + lat) * 0.125;
	

	double tick = packet - m_lastPacketTime;
	
	if (tick > m_updateTime) 
		m_updateTime = (m_updateTime + tick) * 0.5;
	else 
		m_updateTime = (m_updateTime * 7 + tick) * 0.125;
	
  
  return true;
}

