#include "Songs.h"
#include "DxLib.h"
#include <Windows.h>
#include <fstream>
#include <assert.h>
#include "define.h"

static const std::string DIRECTORY = "Resource/Songs/";
static const std::string EXTENTION = "tja";

Songs::Songs( ) {
	loadSongInfoList( );
}


Songs::~Songs( ) {
}

std::vector< Songs::SONG_INFO > Songs::getSongInfoList( ) const {
	return _song_info_list;
}

Songs::SONG_DATA Songs::getSongData( int idx, DIFF diff ) const {
	SONG_DATA song_data = SONG_DATA( );
	std::string filename = _song_info_list[ idx ].filename;
	std::string tmp;
	std::ifstream ifs( filename );
	if ( ifs.fail( ) ) {
		return song_data;
	}

	while ( getline( ifs, tmp ) ) {
		if ( strstr( tmp.c_str( ), "BPM:" ) != NULL ) {
			tmp.erase( 0, 4 );
			song_data.bpm = std::stod( tmp, 0 );
			continue;
		}
		if ( strstr( tmp.c_str( ), "OFFSET:" ) != NULL ) {
			bool minus = false;
			tmp.erase( 0, 7 );
			if ( strstr( tmp.c_str( ), "-" ) != NULL ) {
				tmp.erase( 0, 1 );
				minus = true;
			}
			song_data.offset = std::stod( tmp, 0 );
			
			if ( minus ) {
				song_data.offset *= -1;
			}
			continue;
		}

		if ( song_data.offset != 0 &&
			 song_data.bpm != 0 ) {
			break;
		}
	}
	setCode( &song_data, filename, diff, song_data.bpm );

	if ( song_data.bpm == 0 ) {
		song_data.bpm = 100.0;
	}
	song_data.offset *= 1000;
	return song_data;
}

Songs::SONG_INFO Songs::getInfo( int idx ) const {
	return _song_info_list[ idx ];
}

int Songs::getLevel( std::string filename, DIFF diff ) const {
	int level = 0;
	std::string level_str;
	std::ifstream ifs( filename );
	if ( ifs.fail( ) ) {
		return 0;
	}
	std::string sarch0 = "COURSE:";
	std::string sarch1 = "COURSE:";
	std::string sarch2 = "COURSE:";
	switch ( diff ) {
	case DIFF::HARD:
		sarch0 += "hard";
		sarch1 += "Hard";
		sarch2 += "2";
		break;
	case DIFF::NORMAL:
		sarch0 += "normal";
		sarch1 += "Normal";
		sarch2 += "1";
		break;
	case DIFF::EASY:
		sarch0 += "easy";
		sarch1 += "Easy";
		sarch2 += "0";
		break;
	}
	if ( diff != DIFF::ONI ) {
		while ( std::getline( ifs, level_str ) ) {
			if ( std::strstr( level_str.c_str( ), sarch0.c_str( ) ) != NULL ) {
				break;
			}
			if ( std::strstr( level_str.c_str( ), sarch1.c_str( ) ) != NULL ) {
				break;
			}
			if ( std::strstr( level_str.c_str( ), sarch2.c_str( ) ) != NULL ) {
				break;
			}
		}
	}
	while ( getline( ifs, level_str ) ) {
		if ( strstr( level_str.c_str( ), "LEVEL:" ) != NULL ) {
			level_str.erase( 0, 6 );
			level = std::atoi( level_str.c_str( ) );
			break;
		}
	}
	return level;
}

void Songs::loadSongInfoList( ) {
	std::vector< std::string > file_names;

	//各フォルダ内検索
	std::vector< std::string > directory_list = getDirectoryList( );
	std::vector< std::string >::const_iterator dir_ite = directory_list.begin( );
	while ( dir_ite != directory_list.end( ) ) {
		std::string directory = DIRECTORY + (*dir_ite) + "/";
		GENRE genre = getGenre( directory );

		WIN32_FIND_DATA win32fd;
		std::string sarch = DIRECTORY + (*dir_ite) + "\\*." + EXTENTION;
		HANDLE handle = FindFirstFile( sarch.c_str( ), &win32fd );
		do {
			if ( handle == INVALID_HANDLE_VALUE ) {
				continue;
			}
			if ( !( win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ) {
				//ファイル名追加
				std::string file_name = win32fd.cFileName;
				SONG_INFO song_info = getInfo( file_name, genre, directory ); 
				_song_info_list.push_back( song_info );
			}
		} while ( FindNextFile( handle, &win32fd ) );
		
		dir_ite++;
	}
}

Songs::SONG_INFO Songs::getInfo( std::string filename, Songs::GENRE genre, std::string directory ) const {
	SONG_INFO song_info = SONG_INFO( );
	song_info.genre = genre;
	song_info.filename = directory + filename;
	std::ifstream ifs( song_info.filename );
	std::string tmp;

	if ( ifs.fail( ) ) {
		return song_info;
	}

	while ( getline( ifs, tmp ) ) {
		if ( strstr( tmp.c_str( ), "SUBTITLE:" ) != NULL ) {
			continue;
		}
		if ( strstr( tmp.c_str( ), "TITLE:" ) != NULL ) {
			tmp.erase( 0, 6 );
			song_info.title = tmp;
			continue;
		}
		if ( strstr( tmp.c_str( ), "WAVE:" ) != NULL ) {
			tmp.erase( 0, 5 );
			song_info.music = directory + tmp;
			continue;
		}
		if ( strstr( tmp.c_str( ), "DEMOSTART:" ) != NULL ) {
			tmp.erase( 0, 10 );
			if ( tmp.size( ) != 0 ) {
				double d_demo = std::stoi( tmp, 0 );
				song_info.demo_pos = (int)( d_demo * 1000 + 0.5 );
			}
			continue;
		}
		if ( song_info.title.size( ) != 0 &&
			 song_info.music.size( ) != 0 &&
			 song_info.demo_pos != 0 ) {
			break;
		}
	}
	for ( int i = 0; i < DIFF::MAX_DIFF; i++ ) {
		song_info.level[ i ] = getLevel( song_info.filename, (DIFF)i );
	}
 	return song_info;
}

std::vector< std::string > Songs::getDirectoryList( ) const {
	std::vector< std::string > directory_list;
	WIN32_FIND_DATA win32fd;
	std::string sarch = DIRECTORY + "\\*";
	HANDLE handle = FindFirstFile( sarch.c_str( ), &win32fd );
	do {
		if( strcmp( win32fd.cFileName, "." ) && strcmp( win32fd.cFileName, ".." ) ) {
			if ( handle == INVALID_HANDLE_VALUE ) {
				continue;
			}
			if ( win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
				//フォルダ名追加
				directory_list.push_back( win32fd.cFileName );
			}
		}
	} while ( FindNextFile( handle, &win32fd ) );
	return directory_list;
}

Songs::GENRE Songs::getGenre( std::string directory ) const {
	std::string tmp = "";
	GENRE genre;

	std::string filename = directory + "/_info.ini";
	std::ifstream ifs( filename );
	if ( ifs.fail( ) ) {
		return genre;
	}

	while ( getline( ifs, tmp ) ) {
		if ( strstr( tmp.c_str( ), "GenreName=" ) != NULL ) {
			tmp.erase( 0, 10 );
			genre.name = tmp;
			continue;
		}
		if ( strstr( tmp.c_str( ), "GenreColor=" ) != NULL ) {
			tmp.erase( 0, 11 );
			genre.color_code = tmp;
			continue;
		}
		if ( strstr( tmp.c_str( ), "FontColor=" ) != NULL ) {
			tmp.erase( 0, 10 );
			genre.font_color = tmp;
			continue;
		}
		if ( genre.name.size( ) != 0 && genre.color_code.size( ) != 0 ) {
			break;
		}
	}
	return genre;
}

void Songs::setCode( SONG_DATA* song_data, std::string filename, DIFF diff, double bpm ) const {
	std::string tmp_str;
	std::ifstream ifs( filename );
	if ( ifs.fail( ) ) {
		return;
	}
	bool start = false;
	bool through = false;
	std::string sarch0 = "COURSE:";
	std::string sarch1 = "COURSE:";
	std::string sarch2 = "COURSE:";
	switch ( diff ) {
	case DIFF::ONI:
		sarch0 += "oni";
		sarch1 += "Oni";
		sarch2 += "3";
		break;
	case DIFF::HARD:
		sarch0 += "hard";
		sarch1 += "Hard";
		sarch2 += "2";
		break;
	case DIFF::NORMAL:
		sarch0 += "normal";
		sarch1 += "Normal";
		sarch2 += "1";
		break;
	case DIFF::EASY:
		sarch0 += "easy";
		sarch1 += "Easy";
		sarch2 += "0";
		break;
	}
	while ( std::getline( ifs, tmp_str ) ) {
		if ( std::strstr( tmp_str.c_str( ), sarch0.c_str( ) ) != NULL ) {
			break;
		}
		if ( std::strstr( tmp_str.c_str( ), sarch1.c_str( ) ) != NULL ) {
			break;
		}
		if ( std::strstr( tmp_str.c_str( ), sarch2.c_str( ) ) != NULL ) {
			break;
		}
	}
	bool go_go_time = false;
	std::string measure_str;
	double tmp_bpm = bpm;
	double tmp_scroll = 1.0;
	bool next = false;
	int measure_idx = 0;
	std::vector< CODE_INFO > tmp_codes;


	int idx = 0;
	while ( std::getline( ifs, tmp_str ) ) {
		if ( !next ) {
			idx = 0;
		}
		if ( !start ) {
			if ( std::strstr( tmp_str.c_str( ), "#START" ) != NULL ) {
				start = true;
				continue;
			}
		}
		if ( tmp_str.size( ) == 0 ) {
			continue;
		}
		if ( start ) {
			if ( std::strstr( tmp_str.c_str( ), "#END" ) != NULL ) {
				break;
			}
			if ( std::strstr( tmp_str.c_str( ), "#GOGOSTART" ) != NULL ) {
				go_go_time = true;
				continue;
			}
			if ( std::strstr( tmp_str.c_str( ), "#GOGOEND" ) != NULL ) {
				go_go_time = false;
				continue;
			}
			if ( std::strstr( tmp_str.c_str( ), "#MEASURE " ) != NULL ) {
				tmp_str.erase( 0, 9 );
				measure_str = tmp_str;
				continue;
			}
			if ( std::strstr( tmp_str.c_str( ), "#BPMCHANGE " ) != NULL ) {
				tmp_str.erase( 0, 11 );
				tmp_bpm = std::stod( tmp_str, 0 );
				BPM_INFO bpm_info = BPM_INFO( );
				bpm_info.bpm = measure_idx * MAX_CODE + idx;
				bpm_info.idx = idx;
				song_data->bpms.push_back( bpm_info );
				continue;
			}
			if ( std::strstr( tmp_str.c_str( ), "#SCROLL " ) != NULL ) {
				tmp_str.erase( 0, 8 );
				tmp_scroll = std::stod( tmp_str, 0 );
				continue;
			}
			if ( std::strstr( tmp_str.c_str( ), "#N" ) != NULL ) {
				//普通譜面
				through = true;
				continue;
			}
			if ( std::strstr( tmp_str.c_str( ), "#E" ) != NULL ) {
				//玄人譜面
				through = true;
				continue;
			}
			if ( std::strstr( tmp_str.c_str( ), "#M" ) != NULL ) {
				//達人譜面
				through = false;
				continue;
			}
			if ( std::strstr( tmp_str.c_str( ), "#" ) != NULL ) {
				continue;
			}
			if ( !through ) {
				int line_size = tmp_str.size( ) - 1;
				if ( !next ) {
					CODE_INFO bar = CODE_INFO( );
					bar.bpm = tmp_bpm;
					bar.idx = measure_idx * MAX_CODE;
					bar.type = 0;
					bar.measure = calcString( 4.0, measure_str );
					bar.scroll = tmp_scroll;
					song_data->bars.push_back( bar );
				}
				std::string::const_iterator ite = tmp_str.begin( );
				char code_str = tmp_str.at( tmp_str.size( ) - 1 );
				if ( code_str != ',' ) {
					//カンマがないため長さがわからずidxが決まらない
					next = true;
				}
				while ( ite != tmp_str.end( ) ) {
					char str = (*ite);
					if ( str == ',' ) {
						if ( next ) {
							//２行以上のコードからidxを計算する
							int size = tmp_codes.size( );
							while ( tmp_codes.size( ) != 0 ) {
								CODE_INFO tmp_code = tmp_codes.front( );
								tmp_code.idx = measure_idx * MAX_CODE + idx;
								if ( tmp_code.type != 0 ) {
									song_data->codes.push_back( tmp_code );
								}
								idx += MAX_CODE / size;
								tmp_codes.erase( tmp_codes.begin( ) );
							}
							next = false;
						}
						measure_idx++;
						break;
					}
					if ( str < '0' || str > '9' ) {
						break;
					}
					CODE_INFO code = CODE_INFO( );
					code.type = atoi( &str );
					code.bpm = tmp_bpm;
					code.idx = measure_idx * MAX_CODE + idx;
					code.go_go_time = go_go_time;
					code.measure = calcString( 4.0, measure_str );
					code.scroll = tmp_scroll;
					if ( next ) {
						tmp_codes.push_back( code );
					}
					if ( !next ) {
						if ( code.type != 0 ) {
							song_data->codes.push_back( code );
						}
						idx += MAX_CODE / line_size;
					}
					ite++;
				}
			}
		}
	}
}

double Songs::calcString( double num, std::string str ) const {
	double result = num;
	if ( std::strstr( str.c_str( ), "/" ) != NULL ) {
		int pos = str.find_first_of( "/" );
		std::string num_str0 = str.substr( 0, pos );
		std::string num_str1 = str.substr( pos + 1, str.size( ) );
		double num0 = std::stoi( num_str0, 0 );
		double num1 = std::stoi( num_str1, 0 );
		if ( num0 == num1 ) {
			return result;
		}
	result = ( num * num0 ) / num1;
	}
	return result;
};