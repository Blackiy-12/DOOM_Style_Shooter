#include <EntryPoint.h>

class Shooter : public Application
{
public:
	Shooter();

	~Shooter();

};

Shooter::Shooter()
	: Application()
{

}


Shooter::~Shooter()
{

}

Application* createApplication()
{
	return new Shooter();
}