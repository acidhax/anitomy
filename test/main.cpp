#include <iostream>
#include "anitomy.h"
#include <dirent.h>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <boost/lexical_cast.hpp>

#include <boost/algorithm/string/join.hpp>

#include <iostream>
#include <vector>
#include <map>

using std::string;
std::vector<string> tab {".a52", ".aac", ".aif", ".aiff", ".alac", ".ape", ".au", ".dts", ".flac", ".m1s", ".m2a", ".m4a", ".mp2", ".mp3", ".mpa", ".oga", ".ogg", ".opus", ".ra", ".spx", ".wav", ".wma", ".261", ".264", ".265", ".3g2", ".3gp", ".avi", ".bik", ".divx", ".dv", ".flv", ".gifv", ".h261", ".h263", ".h264", ".h265", ".ivf", ".m1v", ".m2s", ".m2v", ".m4v", ".mj2", ".mk3d", ".mka", ".mks", ".mkv", ".mov", ".mp2v", ".mp4", ".mp4v", ".mpe", ".mpeg", ".mpg", ".mpg2", ".mpg4", ".mpgv", ".mpv2", ".mts", ".nsv", ".ogm", ".ogv", ".qt", ".rm", ".rmvb", ".rv", ".ts", ".tts", ".vob", ".vp3", ".vp6", ".vp7", ".vp8", ".webm", ".wmv", ".wtv", ".xvid", ".yuv", ".ass", ".sami", ".smi", ".srt", ".ssa", ".sub", ".vtt", ".webvtt", ".part", ".crdownload", ".partial", ".utpart", ".!ut", ".bc!"};

bool in_array(const string &value, const std::vector<string> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}

std::map<anitomy::ElementCategory, std::string> Categories = {
    { anitomy::ElementCategory::kElementAnimeSeason, "Season" },
    { anitomy::ElementCategory::kElementAnimeSeasonPrefix, "SeasonPrefix" },
    { anitomy::ElementCategory::kElementAnimeTitle, "Title" },
    { anitomy::ElementCategory::kElementAnimeType, "Type" },
    { anitomy::ElementCategory::kElementAnimeYear, "Year" },
    { anitomy::ElementCategory::kElementAudioTerm, "Audio" },
    { anitomy::ElementCategory::kElementDeviceCompatibility, "Device" },
    { anitomy::ElementCategory::kElementEpisodeNumber, "EpisodeNumber"},
    { anitomy::ElementCategory::kElementEpisodePrefix, "EpisodePrefix"},
    { anitomy::ElementCategory::kElementEpisodeTitle, "EpisodeTitle" },
    { anitomy::ElementCategory::kElementFileChecksum, "Checksum"},
    { anitomy::ElementCategory::kElementFileExtension, "Extension"},
    { anitomy::ElementCategory::kElementFileName, "Filename"},
    { anitomy::ElementCategory::kElementLanguage, "Language"},
    { anitomy::ElementCategory::kElementOther, "Other"},
    { anitomy::ElementCategory::kElementReleaseGroup, "ReleaseGroup"},
    { anitomy::ElementCategory::kElementReleaseInformation, "ReleaseInformation"},
    { anitomy::ElementCategory::kElementReleaseVersion, "Version"},
    { anitomy::ElementCategory::kElementSource, "Source"},
    { anitomy::ElementCategory::kElementSubtitles, "Subtitles"},
    { anitomy::ElementCategory::kElementUnknown, "Unknown"},
    { anitomy::ElementCategory::kElementVideoResolution, "Resolution" },
    { anitomy::ElementCategory::kElementVideoTerm, "VideoTerm"}
};

int main(int argc, const char * argv[]) {
    
    boost::filesystem::path path;
    
    if (argc > 1) {
        path = argv[1];
    } else {
        path = "/Users/mat/Downloads/";
    }
    bool isDirectory = boost::filesystem::is_directory(path);
    
    if (isDirectory == true) {
        /* print all the files and directories within directory */
        boost::filesystem::recursive_directory_iterator entry(path);
        
        while(entry != boost::filesystem::recursive_directory_iterator()) {
            string file = entry->path().filename().c_str();
            boost::filesystem::path fe = entry->path().extension();
            string file_extension  = boost::filesystem::extension(entry->path());
            
            boost::replace_all(file, "-", ".");
            if (file.rfind("-") > file.rfind(".")) {
                boost::replace_last(file, "-", "[");
            }
            boost::algorithm::to_lower(file_extension);
            if (in_array(file_extension, tab)) {
                anitomy::Anitomy anitomy;
                std::wstring file_wstring;
                for(int i = 0; i < file.length(); ++i)
                    file_wstring += wchar_t( file[i] );
                
                anitomy.Parse(file_wstring);
                
                auto& elements = anitomy.elements();
                
                // Elements are iterable, where each element is a category-value pair
                boost::property_tree::ptree pt;
                bool first = true;
                std::vector<std::string> json;
                
                std::ostringstream oss;
                
                json.push_back("{");
                size_t countSeason = elements.count(anitomy::ElementCategory::kElementAnimeSeason);
                size_t countEpisode = elements.count(anitomy::ElementCategory::kElementEpisodeNumber);
                
                if (countSeason == 0 && countEpisode > 0) {
                    anitomy::string_t epNum = elements.get(anitomy::ElementCategory::kElementEpisodeNumber);
                    if (std::stoi( epNum ) > 100) {
                        first = false;
                        json.push_back("\"");
                        json.push_back(Categories[anitomy::ElementCategory::kElementAnimeSeason].c_str());
                        json.push_back("\" : \"");
                        json.push_back(boost::lexical_cast<std::string>(stoi(epNum)));
                        json.push_back("\"");
                    }
                }
                for (auto& element : elements) {
                    if (first) {
                        first = false;
                    } else {
                        json.push_back(", ");
                    }
                    json.push_back("\"");
                    json.push_back(Categories[element.first].c_str());
                    json.push_back("\" : \"");
                    string str(element.second.begin(), element.second.end());
                    json.push_back(str);
                    json.push_back("\"");
                }
                json.push_back(", \"path");
                json.push_back("\":\"");
                json.push_back(entry->path().c_str());
                json.push_back("\"");
                json.push_back("}");
                std::string joined = boost::algorithm::join(json,"");
                std::cout << joined << std::endl;
            }
            ++entry;
        }
    } else {
        /* could not open directory */
        perror ("");
        return EXIT_FAILURE;
    }
    return 0;
}
