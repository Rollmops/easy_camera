#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "easy_camera.hpp"

int  quit = 0;
std::string aircraftName("undefined");
HANDLE  hSimConnect = NULL;
ConfigLoader::ConfigType cameraConfig;
ConfigLoader configLoader("camera_definitions.cfg");
int currentSaveCamera = 0;
SIMCONNECT_RECV_CAMERA_6DOF currentCameraPosition;
const float cameraMovementSpeed = 0.02f;
const float cameraTiltSpeed = 1.0f;
bool save = false;


void CALLBACK MyDispatchProcCC(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{
	switch (pData->dwID)
	{
	case SIMCONNECT_RECV_ID_EVENT:
	{
		SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;

		switch (evt->uEventID)
		{
		case EVENT_SIM_START:
			SimConnect_RequestDataOnSimObjectType(hSimConnect, REQUEST_1, DEFINITION_1, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
			break;
		case EVENT_AIRCRAFT_CHANGED:
			SimConnect_RequestDataOnSimObjectType(hSimConnect, REQUEST_1, DEFINITION_1, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
			break;
		case EVENT_LEFT:
			currentCameraPosition.fDeltaXMeters -= cameraMovementSpeed;
			SimConnect_CameraSetRelative6DOF(hSimConnect, currentCameraPosition.fDeltaXMeters, currentCameraPosition.fDeltaYMeters, currentCameraPosition.fDeltaZMeters, currentCameraPosition.fPitchDeg, currentCameraPosition.fBankDeg, currentCameraPosition.fHeadingDeg);
			break;
		case EVENT_RIGHT:
			currentCameraPosition.fDeltaXMeters += cameraMovementSpeed;
			SimConnect_CameraSetRelative6DOF(hSimConnect, currentCameraPosition.fDeltaXMeters, currentCameraPosition.fDeltaYMeters, currentCameraPosition.fDeltaZMeters, currentCameraPosition.fPitchDeg, currentCameraPosition.fBankDeg, currentCameraPosition.fHeadingDeg);
			break;
		case EVENT_UP:
			currentCameraPosition.fDeltaZMeters += cameraMovementSpeed;
			SimConnect_CameraSetRelative6DOF(hSimConnect, currentCameraPosition.fDeltaXMeters, currentCameraPosition.fDeltaYMeters, currentCameraPosition.fDeltaZMeters, currentCameraPosition.fPitchDeg, currentCameraPosition.fBankDeg, currentCameraPosition.fHeadingDeg);
			break;
		case EVENT_DOWN:
			currentCameraPosition.fDeltaZMeters -= cameraMovementSpeed;
			SimConnect_CameraSetRelative6DOF(hSimConnect, currentCameraPosition.fDeltaXMeters, currentCameraPosition.fDeltaYMeters, currentCameraPosition.fDeltaZMeters, currentCameraPosition.fPitchDeg, currentCameraPosition.fBankDeg, currentCameraPosition.fHeadingDeg);
			break;
		case EVENT_FORWARD:
			currentCameraPosition.fDeltaYMeters += cameraMovementSpeed;
			SimConnect_CameraSetRelative6DOF(hSimConnect, currentCameraPosition.fDeltaXMeters, currentCameraPosition.fDeltaYMeters, currentCameraPosition.fDeltaZMeters, currentCameraPosition.fPitchDeg, currentCameraPosition.fBankDeg, currentCameraPosition.fHeadingDeg);
			break;
		case EVENT_BACK:
			currentCameraPosition.fDeltaYMeters -= cameraMovementSpeed;
			SimConnect_CameraSetRelative6DOF(hSimConnect, currentCameraPosition.fDeltaXMeters, currentCameraPosition.fDeltaYMeters, currentCameraPosition.fDeltaZMeters, currentCameraPosition.fPitchDeg, currentCameraPosition.fBankDeg, currentCameraPosition.fHeadingDeg);
			break;
		case EVENT_CENTER:
			currentCameraPosition.fDeltaXMeters = 0;
			currentCameraPosition.fDeltaYMeters = 0;
			currentCameraPosition.fDeltaZMeters = 0;
			currentCameraPosition.fPitchDeg = 0.0f;
			currentCameraPosition.fHeadingDeg = 0.0f;
			currentCameraPosition.fBankDeg = 0.0f;
			SimConnect_CameraSetRelative6DOF(hSimConnect, currentCameraPosition.fDeltaXMeters, currentCameraPosition.fDeltaYMeters, currentCameraPosition.fDeltaZMeters, currentCameraPosition.fPitchDeg, currentCameraPosition.fBankDeg, currentCameraPosition.fHeadingDeg);
			break;
		case EVENT_PITCH_DOWN:
			currentCameraPosition.fPitchDeg += cameraTiltSpeed;
			SimConnect_CameraSetRelative6DOF(hSimConnect, currentCameraPosition.fDeltaXMeters, currentCameraPosition.fDeltaYMeters, currentCameraPosition.fDeltaZMeters, currentCameraPosition.fPitchDeg, currentCameraPosition.fBankDeg, currentCameraPosition.fHeadingDeg);
			break;
		case EVENT_PITCH_UP:
			currentCameraPosition.fPitchDeg -= cameraTiltSpeed;
			SimConnect_CameraSetRelative6DOF(hSimConnect, currentCameraPosition.fDeltaXMeters, currentCameraPosition.fDeltaYMeters, currentCameraPosition.fDeltaZMeters, currentCameraPosition.fPitchDeg, currentCameraPosition.fBankDeg, currentCameraPosition.fHeadingDeg);
			break;
		case EVENT_HEADING_LEFT:
			currentCameraPosition.fHeadingDeg -= cameraTiltSpeed;
			SimConnect_CameraSetRelative6DOF(hSimConnect, currentCameraPosition.fDeltaXMeters, currentCameraPosition.fDeltaYMeters, currentCameraPosition.fDeltaZMeters, currentCameraPosition.fPitchDeg, currentCameraPosition.fBankDeg, currentCameraPosition.fHeadingDeg);
			break;
		case EVENT_HEADING_RIGHT:
			currentCameraPosition.fHeadingDeg += cameraTiltSpeed;
			SimConnect_CameraSetRelative6DOF(hSimConnect, currentCameraPosition.fDeltaXMeters, currentCameraPosition.fDeltaYMeters, currentCameraPosition.fDeltaZMeters, currentCameraPosition.fPitchDeg, currentCameraPosition.fBankDeg, currentCameraPosition.fHeadingDeg);
			break;

		case EVENT_CAMERA_CHOOSE_1:
		case EVENT_CAMERA_CHOOSE_2:
		case EVENT_CAMERA_CHOOSE_3:
		case EVENT_CAMERA_CHOOSE_4:
		case EVENT_CAMERA_CHOOSE_5:
		case EVENT_CAMERA_CHOOSE_6:
		case EVENT_CAMERA_CHOOSE_7:
		case EVENT_CAMERA_CHOOSE_8:
		case EVENT_CAMERA_CHOOSE_9:
		{
			ConfigLoader::ConfigType::const_iterator vehicleIterator = cameraConfig.find(aircraftName);
			if (vehicleIterator != cameraConfig.end()) {
				std::map<unsigned int, SIMCONNECT_RECV_CAMERA_6DOF >::const_iterator cameraIterator = vehicleIterator->second.find(evt->uEventID);
				if (cameraIterator != vehicleIterator->second.end()) {
					SIMCONNECT_RECV_CAMERA_6DOF cameraPosition = cameraIterator->second;
					SimConnect_CameraSmoothRelative6DOF(hSimConnect, cameraPosition.fDeltaXMeters, cameraPosition.fDeltaYMeters, cameraPosition.fDeltaZMeters, cameraPosition.fPitchDeg, cameraPosition.fBankDeg, cameraPosition.fHeadingDeg, 0.3f);
					currentCameraPosition = cameraPosition;
				}
			}
			break;
		}
		case EVENT_SAVE_CAMERA_1:
		case EVENT_SAVE_CAMERA_2:
		case EVENT_SAVE_CAMERA_3:
		case EVENT_SAVE_CAMERA_4:
		case EVENT_SAVE_CAMERA_5:
		case EVENT_SAVE_CAMERA_6:
		case EVENT_SAVE_CAMERA_7:
		case EVENT_SAVE_CAMERA_8:
		case EVENT_SAVE_CAMERA_9:
			currentSaveCamera = evt->uEventID - 9;
			save = true;
			SimConnect_RequestCameraRelative6DOF(hSimConnect, REQUEST_CAMERA_POSITION);
			break;

		default:
			break;
		}
		break;
	}
	case SIMCONNECT_RECV_ID_SIMOBJECT_DATA_BYTYPE:
	{
		SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE*)pData;

		switch (pObjData->dwRequestID)
		{
		case REQUEST_1:
		{
			StructVS *pS = (StructVS*)&pObjData->dwData;
			DWORD cbtitle;
			char *aircraftNameCStr;
			SimConnect_RetrieveString(pData, cbData, &pS->strings, &aircraftNameCStr, &cbtitle);
			aircraftName = std::string(aircraftNameCStr);
			std::replace(aircraftName.begin(), aircraftName.end(), ' ', '_');
			break;
		}
		break;
		}
		break;
	}
	case SIMCONNECT_RECV_ID_CAMERA_6DOF:
	{
		currentCameraPosition = *(SIMCONNECT_RECV_CAMERA_6DOF*)pData;

		if (save) {
			cameraConfig[aircraftName][currentSaveCamera] = currentCameraPosition;
			configLoader.save(cameraConfig);
			save = false;
		}
		break;
	}

	case SIMCONNECT_RECV_ID_QUIT:
		quit = 1;
		break;
	default:
		break;
	}
}



void testCockpitCamera()
{
	if (SUCCEEDED(SimConnect_Open(&hSimConnect, "Cockpit Camera", NULL, 0, 0, 0)))
	{
		cameraConfig = configLoader.load();

		SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "title", NULL, SIMCONNECT_DATATYPE_STRINGV);

		SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");
		SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_AIRCRAFT_CHANGED, "AircraftLoaded");

		SimConnect_SetNotificationGroupPriority(hSimConnect, GROUP0, SIMCONNECT_GROUP_PRIORITY_STANDARD);

		REGISTER_CAMERA_BUTTON(1, "Num_1", "ctrl+Num_1");
		REGISTER_CAMERA_BUTTON(2, "Num_2", "ctrl+Num_2");
		REGISTER_CAMERA_BUTTON(3, "Num_3", "ctrl+Num_3");
		REGISTER_CAMERA_BUTTON(4, "Num_4", "ctrl+Num_4");
		REGISTER_CAMERA_BUTTON(5, "Num_5", "ctrl+Num_5");
		REGISTER_CAMERA_BUTTON(6, "Num_6", "ctrl+Num_6");
		REGISTER_CAMERA_BUTTON(7, "Num_7", "ctrl+Num_7");
		REGISTER_CAMERA_BUTTON(8, "Num_8", "ctrl+Num_8");
		REGISTER_CAMERA_BUTTON(9, "Num_9", "ctrl+Num_9");

		SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_LEFT);
		SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_RIGHT);
		SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_UP);
		SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_DOWN);
		SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_FORWARD);
		SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_BACK);
		SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_HEADING_LEFT);
		SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_HEADING_RIGHT);
		SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CENTER);
		SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_PITCH_UP);
		SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_PITCH_DOWN);

		SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_LEFT);
		SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_RIGHT);
		SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_UP);
		SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_DOWN);
		SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_FORWARD);
		SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_BACK);
		SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_HEADING_LEFT);
		SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_HEADING_RIGHT);
		SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_CENTER);
		SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_PITCH_UP);
		SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_PITCH_DOWN);

		SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+Num_4", EVENT_LEFT);
		SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+Num_6", EVENT_RIGHT);
		SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+Num_8", EVENT_UP);
		SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+Num_2", EVENT_DOWN);
		SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+Num_7", EVENT_FORWARD);
		SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+Num_1", EVENT_BACK);
		SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+Num_0", EVENT_HEADING_LEFT);
		SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+Num_Del", EVENT_HEADING_RIGHT);
		SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+Num_5", EVENT_CENTER);
		SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+Num_9", EVENT_PITCH_UP);
		SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+Num_3", EVENT_PITCH_DOWN);

		SimConnect_SetInputGroupState(hSimConnect, INPUT0, SIMCONNECT_STATE_ON);

		while (0 == quit)
		{
			SimConnect_CallDispatch(hSimConnect, MyDispatchProcCC, NULL);
			Sleep(1);
		}

		SimConnect_Close(hSimConnect);
	}

}

int main(int argc, _TCHAR* argv[])
{
	testCockpitCamera();

	return 0;
}
