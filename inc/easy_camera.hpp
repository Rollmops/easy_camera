#ifndef _CAMERA_SYSTEM_HPP__
#define _CAMERA_SYSTEM_HPP__

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <algorithm>

#include "SimConnect.h"

#define REGISTER_CAMERA_BUTTON(NUM, KEY, SAVE_KEY) \
	SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CAMERA_CHOOSE_ ## NUM); \
	SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_SAVE_CAMERA_ ## NUM); \
	SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_CAMERA_CHOOSE_ ## NUM); \
	SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_SAVE_CAMERA_ ## NUM); \
	SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, KEY, EVENT_CAMERA_CHOOSE_ ## NUM); \
	SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, SAVE_KEY, EVENT_SAVE_CAMERA_ ## NUM);



class ConfigLoader
{
public:
	typedef std::map<std::string, std::map<unsigned int, SIMCONNECT_RECV_CAMERA_6DOF > > ConfigType;

	ConfigLoader(const std::string &configFilename);
	~ConfigLoader();

	ConfigType load();
	void save(ConfigType&);
private:
	std::string _configFilename;
};

static enum INPUT_ID {
	INPUT0,
	INPUT1
};

static enum DATA_DEFINE_ID {
	DEFINITION_1
};

static enum GROUP_ID {
	GROUP0,
};

static enum EVENT_ID {
	EVENT_SIM_START,
	EVENT_CAMERA_CHOOSE_1,
	EVENT_CAMERA_CHOOSE_2,
	EVENT_CAMERA_CHOOSE_3,
	EVENT_CAMERA_CHOOSE_4,
	EVENT_CAMERA_CHOOSE_5,
	EVENT_CAMERA_CHOOSE_6,
	EVENT_CAMERA_CHOOSE_7,
	EVENT_CAMERA_CHOOSE_8,
	EVENT_CAMERA_CHOOSE_9,
	EVENT_SAVE_CAMERA_1,
	EVENT_SAVE_CAMERA_2,
	EVENT_SAVE_CAMERA_3,
	EVENT_SAVE_CAMERA_4,
	EVENT_SAVE_CAMERA_5,
	EVENT_SAVE_CAMERA_6,
	EVENT_SAVE_CAMERA_7,
	EVENT_SAVE_CAMERA_8,
	EVENT_SAVE_CAMERA_9,

	EVENT_LEFT,
	EVENT_RIGHT,
	EVENT_UP,
	EVENT_DOWN,
	EVENT_FORWARD,
	EVENT_BACK,
	EVENT_HEADING_RIGHT,
	EVENT_HEADING_LEFT,
	EVENT_CENTER,
	EVENT_PITCH_UP,
	EVENT_PITCH_DOWN,

	EVENT_AIRCRAFT_CHANGED

};

static enum DATA_REQUEST_ID {
	REQUEST_1,
	REQUEST_CAMERA_POSITION
};

struct StructVS
{
	char    strings[1];   // variable-length strings
};

#endif
