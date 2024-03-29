#include "StdAfx.h"
#include "utilities.h"

Acad::ErrorStatus util::createLayer(const TCHAR* layerName, AcDbObjectId& layerId) {

	Acad::ErrorStatus es;
	AcDbDatabase *pCurrentDb = acdbHostApplicationServices()->workingDatabase();
	AcDbLayerTable *pLayerTable = nullptr;

	if ((es = pCurrentDb->getLayerTable(pLayerTable, AcDb::kForWrite)) == Acad::eOk) {

		if ((es = pLayerTable->getAt(layerName, layerId, AcDb::kForWrite)) != Acad::eOk) {
			auto pLayerTableRecord = new AcDbLayerTableRecord();

			pLayerTableRecord->setName(layerName);
			if ((es = pLayerTable->add(layerId, pLayerTableRecord)) != Acad::eOk) {
				acutPrintf(_T("\ncannot add block table record at util::createLayer()"));
			}
			pLayerTableRecord->close();
		}
	}
	pLayerTable->close();

	return es;
}

Acad::ErrorStatus util::createBlockRecord(const TCHAR* name) {

	Acad::ErrorStatus es;
	AcDbDatabase* pCurrentDb = acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable* pBlockTable = nullptr;

	if ((es = pCurrentDb->getBlockTable(pBlockTable, AcDb::kForWrite)) == Acad::eOk) {
		if (pBlockTable->has(name)) {

			acutPrintf(_T("\nthis record already exist"));
			pBlockTable->close();

			return Acad::eDuplicateKey;
		}

		else {
			auto pBlockTableRecord = new AcDbBlockTableRecord();

			pBlockTableRecord->setName(name);
			pBlockTableRecord->setOrigin(AcGePoint3d::kOrigin);

			if ((es = pBlockTable->add(pBlockTableRecord)) == Acad::eOk) { pBlockTable->close(); }
			else {
				delete pBlockTableRecord;
				pBlockTable->close();
				return es;
			}

			std::vector<AcDbEntity*> entities;
			AcGeVector3d nrm(0.0, 0.0, 1.0);
			const double PI = 3.141592654;

			AcDbCircle* pFace = new AcDbCircle(AcGePoint3d::kOrigin, nrm, 1.0);
			pFace->setColorIndex(2);

			AcDbCircle* pLeftEye = new AcDbCircle(AcGePoint3d(0.33, 0.25, 0.0), nrm, 0.1);
			pLeftEye->setColorIndex(5);

			AcDbCircle* pRightEye = new AcDbCircle(AcGePoint3d(-0.33, 0.25, 0.0), nrm, 0.1);
			pRightEye->setColorIndex(5);
			
			AcDbArc* pMouth = new AcDbArc(AcGePoint3d(0, 0.5, 0), 1.0, PI + (PI * 0.3), PI + (PI * 0.7));
			pMouth->setColorIndex(1);

			entities.push_back(pFace);
			entities.push_back(pLeftEye);
			entities.push_back(pRightEye);
			entities.push_back(pMouth);

			for (auto& ent : entities) {
				if ((es = pBlockTableRecord->appendAcDbEntity(ent)) != Acad::eOk) {
					ent->close();
					delete ent;
				}
				else { ent->close(); }
			}
			pBlockTableRecord->close();
		}
	}
	return es;
}