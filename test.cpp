#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <exception>

#include "/home/odin/zhangqianqiankevin/code/third-64/libconfig/include/libconfig.h++"
using namespace std;

using namespace libconfig;

template <typename T>
const Setting& readConfig(const Setting& setting, T t)  {
    return setting[t];
}

template <typename T1, typename ... Tn>
const Setting& readConfig(const Setting& setting, T1 t, Tn...args) {
    return readConfig(setting[t], args...);
}
template <typename ...Types>
void test(string config_file, Types ...item_list) {

    Config my_config;
    try
    {
        my_config.readFile(config_file.c_str());
        const Setting& root = my_config.getRoot();
        const Setting& server_list_setting = readConfig(root, item_list...);
        set<string> server_list;
        for(int i = 0; i < server_list_setting.getLength(); i++){
            server_list.insert(server_list_setting[i]);
            //cout<<"item "<<i<<" is"<<(string)server_list_setting[i]<<endl;
        }
        for(auto it = server_list.begin(); it!=server_list.end();it++) {
            cout<<"element:"<<*it<<endl;
        }
    }
    catch (const libconfig::FileIOException &fioex)
    {
        cout<<"file io expection"<<endl;
    }
    catch (const libconfig::ParseException &pex)
    {
        cout<<"parse expection"<<pex.getLine()<<endl;
    }
    catch (const libconfig::SettingNotFoundException &sex)
    {
        cout<<"SettingNotFoundException "<<sex.getPath()<<endl;
    }
    catch(exception& e)
    {
        cout<<"exception "<<e.what()<<endl;
    }
}

libconfig::Setting& getConfig(libconfig::Setting& setting, const std::vector<std::pair<int8_t, std::string>>& vec, int32_t index, bool& success) {
    if(index >= vec.size()) {
        success = true;
        return setting;
    }
    const std::pair<int8_t, std::string>& item = vec[index];
    if(item.first == 0) {
        return getConfig(setting[item.second], vec, index + 1, success);
    }else if (item.first == 1) {
        int32_t item_index = std::stoi(item.second);
        if(item_index < 0){
            return setting;
        }
        return getConfig(setting[item_index], vec, index + 1, success);
    }else {
        return setting;
    }
}
void test2(string config_file, std::vector<std::pair<int8_t, std::string>> vec) {
using namespace libconfig;
    Config my_config;
    try
    {
        my_config.readFile(config_file.c_str());
        Setting& root = my_config.getRoot();
        bool success = false;
        const Setting& server_list_setting = getConfig(root, vec, 0, success);
        if(!success) {
            cout<<"invalid parameter"<<endl;
            return;
        }
        set<string> server_list;
        for(int i = 0; i < server_list_setting.getLength(); i++){
            server_list.insert(server_list_setting[i]);
            //cout<<"item "<<i<<" is"<<(string)server_list_setting[i]<<endl;
        }
        for(auto it = server_list.begin(); it!=server_list.end();it++) {
            cout<<"element:"<<*it<<endl;
        }
    }
    catch (const libconfig::FileIOException &fioex)
    {
        cout<<"file io expection"<<endl;
    }
    catch (const libconfig::ParseException &pex)
    {
        cout<<"parse expection"<<pex.getLine()<<endl;
    }
    catch (const libconfig::SettingNotFoundException &sex)
    {
        cout<<"SettingNotFoundException "<<sex.getPath()<<endl;
    }
    catch(exception& e)
    {
        cout<<"exception "<<e.what()<<endl;
    }
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        cout<<"useage: you should give me the config file path";
        return -1;
    }

    string config_file = argv[1];
    test("./deep_eta_online_traffic_data.conf", "traffic_data_online", "instances", 1, "server_list");
    vector<pair<int8_t, string>> vec;
    vec.push_back(make_pair(0, "traffic_data_online"));
    vec.push_back(make_pair(0, "instances"));
    vec.push_back(make_pair(1, "1"));
    vec.push_back(make_pair(0, "server_list"));
    test2("./deep_eta_online_traffic_data.conf", vec);
}
