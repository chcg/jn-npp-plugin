/*
This file is part of jN, a plugin for Notepad++
Copyright (C)2013 Eugen Kremer <eugen DOT kremer AT gmail DOT com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <ActivScp.h>
#include <dispex.h>
#include <shlwapi.h>

#include <map>

#include "ActiveScriptSiteDebug.h"
#include "combase.h"
#include "SysStr.h"
class ScriptObj;

class MyActiveSite : public CComBase<IActiveScriptSite>
{
private:
	LocRef<ActiveScriptSiteDebug> m_Assd;
	SysStr GetFilePathFromContext(DWORD_PTR context);
public:
	struct cmp_str
	{
		bool operator()(TCHAR const *a, TCHAR const *b) const
		{
			return StrCmp(a, b) < 0;
		}
	};

	typedef std::map<TCHAR*, IUnknown*, cmp_str> ScriptedObjects;

	ScriptedObjects m_ScriptObjects;

	IActiveScript* m_ActiveScript; 
	IActiveScriptParse* m_ActiveScriptParse;

	MyActiveSite();
	static MyActiveSite* getInstance();

	void CloseScript();
	void Connect();

	void ReleaseScriptElement(IDispatchEx** elem);
	static void Throw(TCHAR* errmsg, REFGUID refguid);

	~MyActiveSite(void);

	void addNamedItem(TCHAR* name, IUnknown* item, bool global = true);
	static IDispatchEx* queryDispatchEx(IDispatch* obj);
	IDispatchEx* createObj(TCHAR* ctorName);

	static DISPID* getDISPID(TCHAR* name, IDispatchEx* obj);


	static BOOL callMethod(TCHAR* method, IDispatchEx* obj);

	static BOOL callMethod(TCHAR* method, IDispatchEx* obj, int value);

	static BOOL callMethod(TCHAR* method, IDispatchEx* obj, BSTR value);
	// cleans arg at the end
	static BOOL callMethod(TCHAR* method, IDispatchEx* obj,VARIANTARG* arg, int count, VARIANT* res = NULL);
	static void pushValueInToArr(IDispatchEx* arr, TCHAR* value);

	ScriptObj* WrapScriptObj(IDispatch* obj);

	HRESULT runScript(const BSTR script, const BSTR name = NULL);

	virtual HRESULT __stdcall QueryInterface(REFIID riid, LPVOID *ppv);

	#pragma region IActiveScriptSite...
	virtual HRESULT __stdcall GetLCID(LCID *plcid);

	virtual HRESULT __stdcall GetItemInfo(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown **ppunkItem, ITypeInfo **ppti) ;
	virtual HRESULT __stdcall GetDocVersionString(BSTR *pbstrVersion);
	virtual HRESULT __stdcall OnScriptTerminate(const VARIANT *pvarResult, const EXCEPINFO *pexcepInfo);

	virtual HRESULT __stdcall OnStateChange(SCRIPTSTATE ssScriptState);

	virtual HRESULT __stdcall OnScriptError(IActiveScriptError *pscriptError);

	virtual HRESULT __stdcall OnEnterScript(void);
	virtual HRESULT __stdcall OnLeaveScript(void);
	#pragma endregion
};

class ScriptObj{
private:
	LocRef<IActiveScript, true> m_ActiveScript;
	LocRef<IDispatchEx, true> m_Obj;

	DISPID* getDISPID(TCHAR* name);

	bool IsCallable();
public:
	ScriptObj(ScriptObj& src);

	ScriptObj(IActiveScript* activeScript, IDispatch* obj);
	~ScriptObj();

	VARIANT* getProperty(TCHAR* name, int as);

	BOOL callMethod(TCHAR* method);

	BOOL callMethod(TCHAR* method, int value);

	BOOL callMethod(TCHAR* method, BSTR value);
	// cleans arg at the end
	BOOL callMethod(TCHAR* method,VARIANTARG* arg, int count, VARIANT* res = NULL);
};
