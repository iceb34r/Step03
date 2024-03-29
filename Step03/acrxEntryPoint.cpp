// (C) Copyright 2002-2012 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "utilities.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("ADSK")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep03App : public AcRxArxApp {

public:
	CStep03App () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
	
	static void ADSKStep03_createLayer () {
		Acad::ErrorStatus es;
		AcDbObjectId objId;

		if ((es = util::createLayer(_T("USER"), objId)) != Acad::eOk) {
			acutPrintf(_T("\ncannot create layer"));
		}

		if ((es = util::createBlockRecord(_T("EMPLOYEE"))) != Acad::eOk) {
			acutPrintf(_T("\ncannot create block record"));
		}

		applyCurDwgLayerTableChanges();
	}

	static void ADSKStep03_setLayer() {
		Acad::ErrorStatus es;
		AcDbDatabase* pCurrentDb = acdbHostApplicationServices()->workingDatabase();
		AcDbBlockTable* pBlockTable = nullptr;
		AcDbBlockTableRecord* pBlockTableRecord = nullptr;
		AcDbEntity* pEntity = nullptr;

		if ((es = pCurrentDb->getBlockTable(pBlockTable, AcDb::kForWrite)) == Acad::eOk) {
			if ((es = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite)) != Acad::eOk) {
				acutPrintf(_T("\ncannot get block table record at ADSKStep03_setLayer()"));
			}

			AcDbBlockTableRecordIterator* pBlockTableRecordIt;
			pBlockTableRecord->newIterator(pBlockTableRecordIt);
			AcDbObjectId objId;
			AcDbBlockTableRecord* pCurrRecord = nullptr;
			const TCHAR* pName = _T("EMPLOYEE");

			

			for (pBlockTableRecordIt->start(); !pBlockTableRecordIt->done(); pBlockTableRecordIt->step()) {

				pBlockTableRecordIt->getEntity(pEntity, AcDb::kForWrite);

				if (pEntity->isA() != AcDbBlockReference::desc()) {
					pEntity->close();
					continue;
				}

				objId = AcDbBlockReference::cast(pEntity)->blockTableRecord();
				
				if ((es = acdbOpenObject(pCurrRecord, objId, AcDb::kForWrite)) != Acad::eOk) {
					acutPrintf(_T("cannot open object at ADSKStep03_setLayer()"));
					continue;
				}

				if ((pCurrRecord->getName((pName)) != Acad::eOk)) {
					pCurrRecord->close();
					continue;
				}
				pEntity->setLayer(_T("USER"));

				pCurrRecord->close();
				pEntity->close();
			}
			delete pBlockTableRecordIt;
		}
		pBlockTableRecord->close();
		pBlockTable->close();
	}
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep03App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep03App, ADSKStep03, _createLayer, CREATE, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep03App, ADSKStep03, _setLayer, SETLAYER, ACRX_CMD_MODAL, NULL)


