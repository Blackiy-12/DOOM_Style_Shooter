#include <Logger.h>

int main(int argc, char** argv)
{
	Log::Logger::init();

	ENGINE_INFO("ENGINE INFO");
	ENGINE_ERROR("ENGINE ERROR");

	SHOOTER_INFO("SHOOTER INFO");
	SHOOTER_ERROR("SHOOTER ERROR");

	return 0;
}