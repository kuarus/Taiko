#pragma once
#include <string>
#include <vector>

class Songs {
public:
	enum DIFF {
		ONI,
		HARD,
		NORMAL,
		EASY,
		MAX_DIFF
	};
	struct GENRE {
		std::string name;
		std::string color_code;
		std::string font_color;
	};
	struct SONG_INFO {
		GENRE genre;
		std::string title;
		std::string music;
		std::string filename;
		int level[ DIFF::MAX_DIFF ];
		int demo_pos;
		SONG_INFO( ) {
			this->demo_pos = 0;
			for ( int i = 0; i < DIFF::MAX_DIFF; i++ ) {
				this->level[ i ] = 0;
			}
		}
	};
	struct CODE_INFO {
		int idx;
		char type;
		double bpm;
		double measure;
		double scroll;
		bool go_go_time;
		CODE_INFO( ) {
			this->idx = 0;
			this->bpm = 0;
			this->measure = 0;
			this->scroll = 0;
			this->go_go_time = false;
		}
	};
	struct BPM_INFO {
		int idx;
		double bpm;
		BPM_INFO( ) {
			this->idx = 0;
			this->bpm = 0;
		}
	};
	struct SONG_DATA {
		double offset;
		double bpm;
		std::vector< CODE_INFO > codes;
		std::vector< CODE_INFO > bars;
		std::vector< BPM_INFO > bpms;
		SONG_DATA( ) {
			this->offset = 0;
			this->bpm = 0;
		}
	};
public:
	Songs( );
	virtual ~Songs( );
public:
	std::vector< SONG_INFO > getSongInfoList( ) const;
	SONG_DATA getSongData( int idx, DIFF diff ) const;
	SONG_INFO getInfo( int idx ) const;
private:
	int getLevel( std::string filename, DIFF diff ) const;
	void loadSongInfoList( );
	std::vector< std::string > getDirectoryList( ) const;
	GENRE getGenre( std::string directory ) const;
	SONG_INFO getInfo( std::string filename, GENRE genre, std::string directory ) const;
	void setCode( SONG_DATA* song_info, std::string filename, DIFF diff, double bpm ) const;
	double calcString( double num, std::string str ) const;
private:
	std::vector< SONG_INFO > _song_info_list;
	std::vector< std::string > _file_list;
};