#include "easy_camera.hpp"

#include <fstream>
#include <sstream>
#include <string>

ConfigLoader::ConfigLoader(const std::string &configFilename)
	: _configFilename(configFilename)
{
}


ConfigLoader::~ConfigLoader()
{
}


ConfigLoader::ConfigType ConfigLoader::load()
{
	std::string token(";");
	ConfigType retStructure;
	std::ifstream fileObject(_configFilename.c_str());
	if (fileObject.good()) {

		std::string vehicleName;
		unsigned int numpadKey;
		float x, y, z, pitch, heading, bank;
		std::string line;

		while (std::getline(fileObject, line))
		{
			std::istringstream iss(line);
			iss >> vehicleName >> numpadKey >> x >> y >> z >> pitch >> heading >> bank;

			SIMCONNECT_RECV_CAMERA_6DOF cameraDefinition;
			cameraDefinition.fDeltaXMeters = x;
			cameraDefinition.fDeltaYMeters = y;
			cameraDefinition.fDeltaZMeters = z;
			cameraDefinition.fPitchDeg = pitch;
			cameraDefinition.fHeadingDeg = heading;
			cameraDefinition.fBankDeg = bank;

			retStructure[vehicleName][numpadKey] = cameraDefinition;
		}
		fileObject.close();
	}
	return retStructure;
}

void ConfigLoader::save(ConfigLoader::ConfigType &cameraConfiguration)
{
	std::ofstream fileObject(_configFilename.c_str(), std::ofstream::out);

	for (ConfigType::const_iterator vehicleIterator = cameraConfiguration.begin(); vehicleIterator != cameraConfiguration.end(); vehicleIterator++) {
		for (std::map<unsigned int, SIMCONNECT_RECV_CAMERA_6DOF >::const_iterator cameraIterator = vehicleIterator->second.begin(); cameraIterator != vehicleIterator->second.end(); cameraIterator++) {
			std::ostringstream oss;
			oss << vehicleIterator->first << " "
				<< cameraIterator->first << " "
				<< cameraIterator->second.fDeltaXMeters << " "
				<< cameraIterator->second.fDeltaYMeters << " "
				<< cameraIterator->second.fDeltaZMeters << " "
				<< cameraIterator->second.fPitchDeg << " "
				<< cameraIterator->second.fHeadingDeg << " "
				<< cameraIterator->second.fBankDeg;

			fileObject << oss.str() << std::endl;
		}
	}

	fileObject.close();
}
