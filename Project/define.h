#pragma once

#define V_FONT "@HG�s����"
#define H_FONT "HG�s����"

const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 720;
const int FONT_SIZE = 32;
const int SPEED = 5;
const int JUDGE_X = 220;
const int MAX_CODE = 192;

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
