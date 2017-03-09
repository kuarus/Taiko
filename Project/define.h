#pragma once

#define V_FONT "@HGçsèëëÃ"
#define H_FONT "HGçsèëëÃ"

const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 720;
const int FONT_SIZE = 32;
const int SPEED = 5;
const int JUDGE_X = 220;
const int MAX_CODE = 192;
const int JUDGE_GREAT_RANGE = 3;
const int JUDGE_GOOD_RANGE = 10;
const int JUDGE_BAD_RANGE = 15;

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
