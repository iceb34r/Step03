#pragma once
namespace util {
	Acad::ErrorStatus createLayer(const TCHAR* layerName, AcDbObjectId& layerId);
	Acad::ErrorStatus createBlockRecord(const TCHAR* name);
}