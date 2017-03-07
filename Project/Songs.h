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
	};
	struct SONG_INFO {
		GENRE genre;
		std::string title;
		std::string music;
		std::string filename;
		int demo_pos;
		SONG_INFO( ) {
			this->demo_pos = 0;
		}
	};
	struct MEASURE {
		double bpm;
		double measure;
		std::vector< char > codes;
		bool go_go_time; 
		MEASURE( ) {
			this->bpm = 0;
			this->measure = 0;
			this->go_go_time = false;
		}
	};
	struct SONG_DATA {
		double offset;
		double bpm;
		std::vector< MEASURE > measures;
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
	int getLevel( int idx, DIFF diff ) const;
private:
	void loadSongInfoList( );
	std::vector< std::string > getDirectoryList( ) const;
	GENRE getGenre( std::string directory ) const;
	SONG_INFO getInfo( std::string filename, GENRE genre, std::string directory ) const;
	std::vector< MEASURE> getCode( std::string filename, DIFF diff, double bpm ) const;
	double calcString( double num, std::string str ) const;
private:
	std::vector< SONG_INFO > _song_info_list;
	std::vector< std::string > _file_list;
};