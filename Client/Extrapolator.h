#ifndef CEXTRAPOLATOR_H
#define CEXTRAPOLATOR_H


template<int count = 2, typename user_type = float>
class CExtrapolator 
{
public:
	CExtrapolator();
    ~CExtrapolator();

    bool AddSample(double packetTime, double curTime, user_type const pos[count]);
    bool AddSample(double packetTime, double curTime, user_type const pos[count], user_type const vel[count]);
    
	void Reset(double packetTime, double curTime, user_type const pos[count]);
    void Reset(double packetTime, double curTime, user_type const pos[count], user_type const vel[count]);
   
	bool ReadPosition(double forTime, user_type oPos[count]) const;
    bool ReadPosition(double forTime, user_type oPos[count], user_type oVel[count]) const;
   
	double EstimateLatency() const;
    double EstimateUpdateTime() const;
  
private:
	double m_snapTime;            
    double m_aimTime;               
    double m_lastPacketTime;         
    double m_latency;
    double m_updateTime; 

    user_type m_snapPosition[count];
    user_type m_snapVelocity[count];
    user_type m_aimPos[count];
    user_type m_lastPacketPos[count];     

	bool Estimates(double packet, double cur);
};


#endif
