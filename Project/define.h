#pragma once

#define V_FONT "@HGçsèëëÃ"
#define H_FONT "HGçsèëëÃ"

const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 720;
const int FONT_SIZE = 40;
const int SPEED = 7;
const int JUDGE_X = 220;

enum GRAPH {
	GRAPH_TITLE,
	GRAPH_SELECT_BG,
	GRAPH_MENU_SELECT,
	GRAPH_SONG_SELECT,
	GRAPH_MAX,
};

enum DIFF {
	EASY,
	NORMAL,
	HARD,
	ONI,
	MAX_GENRE
};
