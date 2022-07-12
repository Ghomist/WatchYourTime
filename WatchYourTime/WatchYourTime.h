#pragma once

#include <BML/BMLAll.h>
// #include <format>
#include <fstream>

#define MOD_VERSION "1" "." "4"

extern "C" {
	__declspec(dllexport) IMod* BMLEntry(IBML* bml);
}

class WatchYourTime : public IMod {
public:
	WatchYourTime(IBML* bml) : IMod(bml) {}

	virtual CKSTRING GetID() override { return "WatchYourTime"; }
	virtual CKSTRING GetVersion() override { return MOD_VERSION; }
	virtual CKSTRING GetName() override { return "Watch Your Time"; }
	virtual CKSTRING GetAuthor() override { return "Ghomist"; }
	virtual CKSTRING GetDescription() override;
	DECLARE_BML_VERSION;
	virtual void OnExitGame();
	virtual void OnLoad();
	void OnLoadObject(CKSTRING filename, BOOL isMap, CKSTRING masterName, CK_CLASSID filterClass, BOOL addtoscene, BOOL reuseMeshes, BOOL reuseMaterials, BOOL dynamic, XObjectArray* objArray, CKObject* masterObj) override;
	virtual void OnStartLevel();
	virtual void OnPostLoadLevel();
	virtual void OnPostExitLevel();
	virtual void OnPostNextLevel();
	//virtual void OnProcess();
	//virtual void OnModifyConfig(CKSTRING category, CKSTRING key, IProperty* prop);
	//virtual void OnLoadObject(CKSTRING filename, BOOL isMap, CKSTRING masterName,
	//	CK_CLASSID filterClass, BOOL addtoscene, BOOL reuseMeshes, BOOL reuseMaterials,
	//	BOOL dynamic, XObjectArray* objArray, CKObject* masterObj);
	//virtual void OnExitGame();

private:
	void FormatTime(char* buf, const char* prefix, time_t h, time_t m, time_t s);
	int GetLevel();

	std::string desc = "";
	bool is_bml_custom_map = false;
	//char c_desc[512] = "";
};