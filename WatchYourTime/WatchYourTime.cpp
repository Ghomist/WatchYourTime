#include "WatchYourTime.h"

#define _LOG_ "WATCH-UR-TIME"

#define ToSecond(t) t%60
#define ToMin(t) t%3600/60
#define ToHour(t) t/3600

int current_level = 0;
time_t game_start = 0;
time_t level_begin = 0;

void addTime(int level, time_t t) {
	time_t times[14] = { 0 };

	std::ifstream in;
	in.open(_LOG_);
	for (int i = 0; i <= 13; ++i)
		in >> times[i];
	in.close();

	times[level] += t;

	std::ofstream out;
	out.open(_LOG_);
	for (int i = 0; i <= 13; ++i)
		out << times[i] << std::endl;
	out.close();
}

void getTime(time_t times[]) {
	std::ifstream in;
	in.open(_LOG_);
	for (int i = 0; i <= 13; ++i)
		in >> times[i];
	in.close();
}

void saveIngameTime() {
	addTime(0, time(0) - game_start);
}

IMod* BMLEntry(IBML* bml) {
	return new WatchYourTime(bml);
}

void WatchYourTime::OnLoad() {
	game_start = time(0);
	atexit(saveIngameTime);
}

// Level Start
void WatchYourTime::OnStartLevel() {
	if (current_level == GetLevel())
		addTime(current_level, time(0) - level_begin);
	else
		current_level = GetLevel();
	level_begin = time(0);
}

// Level End
void WatchYourTime::OnPostExitLevel() {
	addTime(current_level, time(0) - level_begin);
}

// Level End and Next Level Start
void WatchYourTime::OnPostNextLevel() {
	addTime(current_level, time(0) - level_begin);
}

CKSTRING WatchYourTime::GetDescription() {
	desc = "Have you ever checked your time spent on playing Ballance?\n\n";
	time_t times[14] = { 0 };
	getTime(times);
	char buf[25] = "";
	time_t h, m, s;
	h = ToHour(times[0]);
	m = ToMin(times[0]);
	s = ToSecond(times[0]);
	FormatTime(buf, "In game", h, m, s);
	desc += buf;

	for (int i = 1; i <= 13; ++i) {
		h = ToHour(times[i]);
		m = ToMin(times[i]);
		s = ToSecond(times[i]);
		char level[9] = "";
		sprintf(level, "Level_%02d", i);
		FormatTime(buf, level, h, m, s);
		desc += buf;
	}

	desc +=
		"\n<Auto-record>"
		"\n<Reopen Ballance to update data>";

	return desc.c_str();
}

void WatchYourTime::FormatTime(char* buf, const char* prefix, time_t h, time_t m, time_t s) {
	if (h == 0 && m == 0)
		sprintf(buf, "%s: %llds\n", prefix, s);
	else if (h == 0 && m != 0)
		sprintf(buf, "%s: %lldm %02llds\n", prefix, m, s);
	else
		sprintf(buf, "%s: %lldh %02lldm %02llds\n", prefix, h, m, s);
}

int WatchYourTime::GetLevel() {
	int level = 0;
	m_bml->GetArrayByName("CurrentLevel")->GetElementValue(0, 0, (void*)&level);
	return level;
}
