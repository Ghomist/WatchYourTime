#include "WatchYourTime.h"

#define _LOG_ "WATCH-UR-TIME"

#define ToSecond(t) t%60
#define ToMin(t) t%3600/60
#define ToHour(t) t/3600

int current_level = 0;
time_t game_start = 0;
time_t level_begin = 0;

void addTime(int level, time_t t, bool increase_count = false) {
// 0 for total
	time_t times[14] = { 0 };
	size_t counts[14] = { 0 };

	std::ifstream in(_LOG_);
	for (int i = 0; i <= 13; ++i)
		in >> times[i];
	for (int i = 0; i <= 13; ++i)
		in >> counts[i];
	in.close();

	times[level] += t;
	counts[level] += increase_count;

	std::ofstream out(_LOG_);
	for (int i = 0; i <= 13; ++i)
		out << times[i] << std::endl;
	for (int i = 0; i <= 13; ++i)
		out << counts[i] << std::endl;
	out.close();
}

void getTime(time_t times[], size_t counts[]) {
	std::ifstream in(_LOG_);
	for (int i = 0; i <= 13; ++i)
		in >> times[i];
	for (int i = 0; i <= 13; ++i)
		in >> counts[i];
	in.close();
}

bool data_saved = false;

void saveIngameTime() {
	if (data_saved) return;
	addTime(0, time(0) - game_start, true);
	data_saved = true;
}

IMod* BMLEntry(IBML* bml) {
	return new WatchYourTime(bml);
}

void WatchYourTime::OnExitGame() {
	saveIngameTime();
}

void WatchYourTime::OnLoad() {
	game_start = time(0);
	atexit(saveIngameTime);
	at_quick_exit(saveIngameTime);
}

void WatchYourTime::OnLoadObject(CKSTRING filename, BOOL isMap, CKSTRING masterName, CK_CLASSID filterClass, BOOL addtoscene, BOOL reuseMeshes, BOOL reuseMaterials, BOOL dynamic, XObjectArray* objArray, CKObject* masterObj) {
	if (isMap) {
		current_level = GetLevel();
		is_bml_custom_map = ('3' != filename[0]);
		// 3D Entities\\Level
	}
}

// Level Start
void WatchYourTime::OnStartLevel() {
	if (is_bml_custom_map) return;
	addTime(current_level, time(0) - level_begin, true);
	level_begin = time(0);
}

void WatchYourTime::OnPostLoadLevel() {
	level_begin = time(0);
}

// Level End
void WatchYourTime::OnPostExitLevel() {
	if (is_bml_custom_map) return;
	addTime(current_level, time(0) - level_begin);
}

// Level End and Next Level Start
void WatchYourTime::OnPostNextLevel() {
	if (is_bml_custom_map) return;
	addTime(current_level, time(0) - level_begin);
}

CKSTRING WatchYourTime::GetDescription() {
	desc = "Have you ever checked your time spent on playing Ballance?\n\n";
	time_t times[14] = { 0 };
	size_t counts[14] = { 0 };
	getTime(times, counts);
	char buf[25] = "";
	time_t h, m, s;
	h = ToHour(times[0]);
	m = ToMin(times[0]);
	s = ToSecond(times[0]);
	FormatTime(buf, "In game", h, m, s);
	desc += buf;
	desc += ", " + std::to_string(counts[0]) + " time" + (counts[0] == 1 ? "" : "s") + "\n";

	for (int i = 1; i <= 13; ++i) {
		h = ToHour(times[i]);
		m = ToMin(times[i]);
		s = ToSecond(times[i]);
		char level[9] = "";
		sprintf(level, "Level_%02d", i);
		FormatTime(buf, level, h, m, s);
		desc += buf;
		desc += ", " + std::to_string(counts[i]) + " time" + (counts[i] == 1 ? "" : "s") + "\n";
	}

	desc +=
		"\n<Auto-recorded statistics>"
		"\n<Restart Ballance to update your data>";

	return desc.c_str();
}

void WatchYourTime::FormatTime(char* buf, const char* prefix, time_t h, time_t m, time_t s) {
	if (h == 0 && m == 0)
		sprintf(buf, "%s: %llds", prefix, s);
	else if (h == 0 && m != 0)
		sprintf(buf, "%s: %lldm %02llds", prefix, m, s);
	else
		sprintf(buf, "%s: %lldh %02lldm %02llds", prefix, h, m, s);
}

int WatchYourTime::GetLevel() {
	int level = 0;
	m_bml->GetArrayByName("CurrentLevel")->GetElementValue(0, 0, &level);
	return level;
}
