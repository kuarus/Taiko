#pragma once

#define V_FONT "@HG行書体"
#define H_FONT "HG行書体"

const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 720;
const int FONT_SIZE = 40;

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
