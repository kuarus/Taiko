#pragma once

#define V_FONT "@HG行書体";
#define H_FONT "HG行書体";

const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 720;
const int FONT_SIZE = 26;
const double SPEED = 1.8;
const int JUDGE_X = 220;
const int MAX_CODE = 512;
const int JUDGE_GREAT_RANGE = 8;
const int JUDGE_GOOD_RANGE = 24;
const int JUDGE_BAD_RANGE = 30;

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
