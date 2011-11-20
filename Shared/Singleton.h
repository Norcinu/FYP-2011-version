#ifndef __CSINGLETON__
#define __CSINGLETON__

template<class T>
class Singleton
{
public:
	static T* CreateInstance();
	static T& GetInstance();
	static void DeleteInstance();
protected:
	Singleton(void);
	~Singleton(void);
private:
	static T *m_instance;
};

template<class T>
Singleton<T>::Singleton( void )
{
}

template<class T>
Singleton<T>::~Singleton( void )
{
}

template<class T>
T* Singleton<T>::CreateInstance()
{	
	if (m_instance == 0)
		m_instance = new T;
	return m_instance;
}

template<class T>
void Singleton<T>::DeleteInstance()
{
	delete m_instance;
	m_instance = 0;
}

template<class T>
T& Singleton<T>::GetInstance()
{
	//if ( m_instance == 0 )
	//	m_instance = new T;
	return *m_instance;
}


//#define SINGLETON Singleton<T>::GetInstance()

#endif
